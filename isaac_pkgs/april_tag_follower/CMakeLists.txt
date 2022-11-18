cmake_minimum_required(VERSION 3.0.2)
project(april_follower)

find_package(catkin REQUIRED
  COMPONENTS
    moveit_core
    moveit_ros_planning
    moveit_ros_planning_interface
    std_msgs
    geometry_msgs
    apriltag_ros
)

catkin_package(
  LIBRARIES
  INCLUDE_DIRS
  CATKIN_DEPENDS
    moveit_core
    moveit_ros_planning
    moveit_ros_planning_interface
    std_msgs
    geometry_msgs
    apriltag_ros
)

find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED system filesystem date_time thread)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

link_directories(lib)

add_executable(
  april_tag_follower
  src/april_tag_follower.cpp  
)
target_link_libraries(april_tag_follower ${catkin_LIBRARIES})