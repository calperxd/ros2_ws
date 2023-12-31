#include "rclcpp/rclcpp.hpp"
#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>
#include <std_msgs/msg/float32.hpp>
#include <vector>
#include <cstdint>
#include <iostream>

using namespace LibSerial;
using namespace std::chrono_literals;

// Constants
#define START_OF_FRAME 0xAA
#define END_OF_FRAME   0x55
#define TEMP_DATA_TYPE 0x01
#define TEMP_DATA_SIZE 4

class MyNode : public rclcpp::Node {
public:
    MyNode() : Node("ros_node_temp"), publish_counter_(0) {
        try {
            RCLCPP_INFO(this->get_logger(), "Running");
            serial_port_.Open("/dev/ttyUSB0");
            serial_port_.SetBaudRate(BaudRate::BAUD_115200);
            serial_port_.SetCharacterSize(CharacterSize::CHAR_SIZE_8);
            serial_port_.SetStopBits(StopBits::STOP_BITS_1);
            serial_port_.SetParity(Parity::PARITY_NONE);
            serial_port_.SetFlowControl(FlowControl::FLOW_CONTROL_NONE);
            temperature_publisher_ = this->create_publisher<std_msgs::msg::Float32>("/temperature", 10);
        } catch (const OpenFailed& e) {
            RCLCPP_ERROR(this->get_logger(), "Failed to open serial port: %s", e.what());
        }

        timer_ = this->create_wall_timer(10ms, std::bind(&MyNode::timer_callback, this));
    }

    ~MyNode() {
        if (serial_port_.IsOpen()) {
            serial_port_.Close();
        }
    }

private:
    void timer_callback() {
        if (serial_port_.IsOpen()) {
            try {
                std::vector<uint8_t> buffer(1);
                while (serial_port_.IsDataAvailable()) {
                    serial_port_.read(reinterpret_cast<char*>(buffer.data()), 1);
                    process_byte(buffer[0]);
                }
            } catch (const ReadTimeout& e) {
                RCLCPP_ERROR(this->get_logger(), "Timeout while reading from serial port: %s", e.what());
            } catch (const std::exception& e) {
                RCLCPP_ERROR(this->get_logger(), "Error reading from serial port: %s", e.what());
            }
        }
    }

    void process_byte(uint8_t byte) {
        static std::vector<uint8_t> packet;
        static bool in_frame = false;

        if (byte == START_OF_FRAME && !in_frame) {
            in_frame = true;
            packet.clear();
            packet.push_back(byte);
        } else if (byte == END_OF_FRAME && in_frame) {
            in_frame = false;
            packet.push_back(byte);
            process_packet(packet);
        } else if (in_frame) {
            packet.push_back(byte);
        }
    }

    void process_packet(const std::vector<uint8_t>& packet) {
        float temperature;
        if (extract_temperature_from_packet(packet.data(), packet.size(), &temperature)) {
            RCLCPP_INFO(this->get_logger(), "Extracted Temperature: %0.2f", temperature);

            // Publish Temperature
            std_msgs::msg::Float32 msg;
            msg.data = temperature;
            temperature_publisher_->publish(msg);

            // Increment the counter and send a special character after every 20 packets
            publish_counter_++;
            if (publish_counter_ == 10) {
                publish_counter_ = 0;
                char special_char = 'U';
                serial_port_.write(&special_char, 1);  // Send a special character 'U' over serial
            }
        } else {
            RCLCPP_ERROR(this->get_logger(), "Invalid packet");
        }
    }


    bool verify_packet(const uint8_t *packet, int size) {
        if (packet[0] != START_OF_FRAME || packet[size - 1] != END_OF_FRAME) {
            return false;
        }
        if (packet[1] != TEMP_DATA_TYPE || packet[2] != TEMP_DATA_SIZE) {
            return false;
        }
        uint8_t checksum = 0;
        for (int i = 1; i < size - 2; i++) {
            checksum += packet[i];
        }
        return checksum == packet[size - 2];
    }

    bool extract_temperature_from_packet(const uint8_t *packet, int size, float *temperature) {
        if (!verify_packet(packet, size)) {
            return false;
        }
        memcpy(temperature, &packet[3], TEMP_DATA_SIZE);
        return true;
    }

    SerialStream serial_port_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr temperature_publisher_;
    int publish_counter_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MyNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
