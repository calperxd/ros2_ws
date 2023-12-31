# Project

## Overview
This README provides instructions for setting up and running a ROS2 node in the ROS2 Iron environment. The project is specifically configured for ROS2 Iron, and the node is located in `/opt`. It is essential to source the `local_setup.sh` script from the `install` directory to properly configure the environment.

## Prerequisites
- ROS2 Iron installed in `/opt`
- Basic understanding of ROS2 concepts and command line usage

## Building and Running the Node
This section describes how to build the ROS2 node and run it.

### Building the Node
To build the project, follow these steps:
1. Ensure that ROS2 Iron is sourced properly. This can be done by running the following command in your terminal:

```bash
source /opt/ros/iron/setup.bash
```

2. Navigate to your project directory and build the project using `colcon`:
```bash
git clone https://github.com/calperxd/ros2_ws.git
```

```bash
cd ros2_ws
```

```bash
colcon build
```

### Setting Up the Environment
After building the project, you need to set up your environment to recognize the newly built packages. Run the following command:

```bash
source install/local_setup.sh
```

### Running the Node
To run the ROS2 node, execute the following command:

```bash
ros2 run ros_node_temp temp
```

## Additional Information
- Ensure that all dependencies for your ROS2 node are correctly installed and sourced.
- For troubleshooting, refer to the ROS2 Iron documentation and ensure that your environment is correctly set up.

## Error Handling
If you encounter issues during the build or runtime, check the following:
- The ROS2 environment is sourced correctly.
- All dependencies are installed and properly configured.
- The build process completed successfully without errors.

## Support
For additional support, consider visiting ROS2 community forums or checking ROS2 Iron's official documentation.
