cmake_minimum_required(VERSION 3.0.2)
project(isaac_moveit_ur5e)

find_package(catkin REQUIRED COMPONENTS
  rospy
  sensor_msgs
  ur5e_moveit_config
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

catkin_install_python(PROGRAMS
  scripts/ur5e_with_2f85_combined_joints_publisher.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
