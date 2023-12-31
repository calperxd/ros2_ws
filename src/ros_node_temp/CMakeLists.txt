cmake_minimum_required(VERSION 3.8)
project(ros_node_temp)


if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_library(LIBSERIAL serial)
if(NOT LIBSERIAL)
  message(FATAL_ERROR "LibSerial not found")
endif()

add_executable(temp src/ros_node_temp.cpp)
ament_target_dependencies(temp rclcpp std_msgs)
target_link_libraries(temp ${LIBSERIAL})

install(TARGETS
  temp
  DESTINATION lib/${PROJECT_NAME})

ament_package()
