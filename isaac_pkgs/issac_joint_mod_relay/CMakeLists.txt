cmake_minimum_required(VERSION 3.0.2)
project(isaac_joint_mod_relay)

find_package(catkin REQUIRED)

catkin_package(

)

include_directories(

)

catkin_install_python(PROGRAMS
  scripts/ur5e_with_2f85_mod_joints_publisher.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
