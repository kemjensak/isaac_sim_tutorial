 #include <ros/ros.h>
 #include <moveit/planning_scene_interface/planning_scene_interface.h>
 #include <moveit/move_group_interface/move_group_interface.h>
 #include <cmath>
 #include <string>
 #include <iostream>
 #include <memory>
 #include <std_msgs/Bool.h>
 #include <gazebo_msgs/ModelStates.h>
 #include <geometry_msgs/Point.h>


 class PickAndPlaceClass
 {
     public:
         void main();

     protected:
         ros::NodeHandle n;
         geometry_msgs::Point boxPosition_;
         void callbackGetModelStateData(const gazebo_msgs::ModelStatesConstPtr &msg);
         const std::string PLANNING_GROUP_ARM_ = "manipulator";
         const std::string PLANNING_GROUP_GRIPPER_ = "gripper";
    
 };

 void PickAndPlaceClass::callbackGetModelStateData(const gazebo_msgs::ModelStatesConstPtr &msg)
 {
     std::string target_model= "red_box";
     std::vector<std::string> names = msg->name;
     std::vector<std::string>::iterator itr = std::find(names.begin(), names.end(), target_model);

     if (itr != names.cend()){
         int index = std::distance(names.begin(), itr);
         boxPosition_= msg->pose[index].position;
     }
     else {
         ROS_WARN_NAMED("tutorial", "Model state of red box is not detected");
     }
 }
  
  void PickAndPlaceClass::main()
 {

     moveit::planning_interface::MoveGroupInterface move_group_interface_arm(PLANNING_GROUP_ARM_);
     moveit::planning_interface::MoveGroupInterface move_group_interface_gripper(PLANNING_GROUP_GRIPPER_);
     ros::NodeHandle n;

     ros::AsyncSpinner spinner(1);
     spinner.start();


     moveit::planning_interface::MoveGroupInterface::Plan my_plan_arm;
    
     // 1. Move to predifined position
     std::vector<double> joint_group_positions(6);
     joint_group_positions[0] = 0;
     joint_group_positions[1] = -1.57;
     joint_group_positions[2] = -1.82;
     joint_group_positions[3] = -1.32;
     joint_group_positions[4] = 1.57;
     joint_group_positions[5] = 0;
     
     move_group_interface_arm.setJointValueTarget(joint_group_positions);

     // move_group_interface_arm.setJointValueTarget(move_group_interface_arm.getNamedTargetValues("up"));


     bool success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

     move_group_interface_arm.move();

     // 1.2 Get red box's pose
     ros::Subscriber sub_ModelState = n.subscribe("gazebo/model_states", 1, &PickAndPlaceClass::callbackGetModelStateData, this);

     // 2. Place the TCP (Tool Center Point, the tip of the robot) above the blue box
     geometry_msgs::PoseStamped current_pose;
     current_pose = move_group_interface_arm.getCurrentPose("tool0");

     geometry_msgs::Pose target_pose1;
     target_pose1.orientation = current_pose.pose.orientation;
     target_pose1.position.x = boxPosition_.x;
     target_pose1.position.y = boxPosition_.y;
     target_pose1.position.z = boxPosition_.z + 0.10;
     move_group_interface_arm.setPoseTarget(target_pose1);

     success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

     move_group_interface_arm.move();


     moveit::planning_interface::MoveGroupInterface::Plan my_plan_gripper;

     // 3. Open the gripper
     move_group_interface_gripper.setJointValueTarget(move_group_interface_gripper.getNamedTargetValues("open"));

     success = (move_group_interface_gripper.plan(my_plan_gripper) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

     move_group_interface_gripper.move();

     // 4. Move the TCP close to the object
     target_pose1.position.z = target_pose1.position.z - 0.19;
     move_group_interface_arm.setPoseTarget(target_pose1);

     success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

     move_group_interface_arm.move();

     // 5. Close the  gripper
     move_group_interface_gripper.setJointValueTarget(move_group_interface_gripper.getNamedTargetValues("close"));

     success = (move_group_interface_gripper.plan(my_plan_gripper) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

     move_group_interface_gripper.move();

     // Move the TCP above the plate
     target_pose1.position.z = target_pose1.position.z + 0.15;
     move_group_interface_arm.setPoseTarget(target_pose1);

     success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

     move_group_interface_arm.move();

     // 6. Move the TCP above the plate
     target_pose1.position.y = 0.0;
     target_pose1.position.x = - 0.5;
     move_group_interface_arm.setPoseTarget(target_pose1);

     success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

     move_group_interface_arm.move();

     // 7. Lower the TCP above the plate
     target_pose1.position.z = target_pose1.position.z - 0.14;
     move_group_interface_arm.setPoseTarget(target_pose1);

     success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

     move_group_interface_arm.move();

     // 8. Open the gripper
     move_group_interface_gripper.setJointValueTarget(move_group_interface_gripper.getNamedTargetValues("open"));

     success = (move_group_interface_gripper.plan(my_plan_gripper) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

     move_group_interface_gripper.move();

     ros::shutdown();
 }

  int main(int argc, char** argv){
     ros::init(argc, argv, "move_group_interface_tutorial");
     PickAndPlaceClass PPC;
     PPC.main();
     return 0;
  }