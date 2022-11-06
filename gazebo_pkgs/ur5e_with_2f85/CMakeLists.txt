cmake_minimum_required(VERSION 3.0.2)
project(ur5e_with_2f85)

find_package(catkin REQUIRED
  COMPONENTS
    moveit_core
    moveit_ros_planning
    moveit_ros_planning_interface
    std_msgs
    gazebo_msgs
    geometry_msgs
)

catkin_package(
  LIBRARIES
  INCLUDE_DIRS
  CATKIN_DEPENDS
    moveit_core
    moveit_ros_planning
    moveit_ros_planning_interface
    std_msgs
    gazebo_msgs
    geometry_msgs
)

find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED system filesystem date_time thread)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

link_directories(lib)

add_executable(
  pick_and_place_gazebo_example
  src/pick_and_place_gazebo_example.cpp  
)
target_link_libraries(pick_and_place_gazebo_example ${catkin_LIBRARIES})