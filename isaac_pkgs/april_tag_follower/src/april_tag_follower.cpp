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
 #include <apriltag_ros/AprilTagDetectionArray.h>


 class PickAndPlaceClass
 {
     public:
         void main();

     protected:
         ros::NodeHandle n;
         geometry_msgs::Point boxPosition_;
         void callbackGetModelStateData(const apriltag_ros::AprilTagDetectionArrayConstPtr &msg);
         const std::string PLANNING_GROUP_ARM_ = "manipulator";
         const std::string PLANNING_GROUP_GRIPPER_ = "gripper";
         geometry_msgs::Pose target_tag_pose_, tag_pose_;

    
 };

 void PickAndPlaceClass::callbackGetModelStateData(const apriltag_ros::AprilTagDetectionArrayConstPtr &msg)
 {
    tag_pose_ = msg->detections[0].pose.pose.pose;
    // msg.detection
    //  std::string target_model= "red_box";
    //  std::vector<std::string> names = msg->name;
    //  std::vector<std::string>::iterator itr = std::find(names.begin(), names.end(), target_model);

    //  if (itr != names.cend()){
    //      int index = std::distance(names.begin(), itr);
    //      boxPosition_= msg->pose[index].position;
    //  }
    //  else {
    //      ROS_WARN_NAMED("tutorial", "Model state of red box is not detected");
    //  }
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
     joint_group_positions[0] = 1.57;
     joint_group_positions[1] = -1.32;
     joint_group_positions[2] = 1.32;
     joint_group_positions[3] = -1.57;
     joint_group_positions[4] = -1.57;
     joint_group_positions[5] = 0;
     
     move_group_interface_arm.setJointValueTarget(joint_group_positions);

     // move_group_interface_arm.setJointValueTarget(move_group_interface_arm.getNamedTargetValues("up"));


     bool success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

     move_group_interface_arm.move();

     // 2. Make subscriber to get cube(april tag) pose
     ros::Subscriber sub_ModelState = n.subscribe("/tag_detections", 1, &PickAndPlaceClass::callbackGetModelStateData, this);
    target_tag_pose_ = tag_pose_;

     // 2. Place the TCP (Tool Center Point, the tip of the robot) above the box
     geometry_msgs::PoseStamped current_pose;
     current_pose = move_group_interface_arm.getCurrentPose("tool0");

     geometry_msgs::Pose target_pose1;
     target_pose1.orientation = current_pose.pose.orientation;
     target_pose1.position.x = (target_tag_pose_.position.x * 10 + current_pose.pose.position.x - 0.05);
     target_pose1.position.y = (target_tag_pose_.position.y * 10 + current_pose.pose.position.y);
     target_pose1.position.z =  0.10;
     move_group_interface_arm.setPoseTarget(target_pose1);

     success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
     move_group_interface_arm.move();


     // 3. Open the gripper
     moveit::planning_interface::MoveGroupInterface::Plan my_plan_gripper;
     move_group_interface_gripper.setJointValueTarget(move_group_interface_gripper.getNamedTargetValues("open"));
     success = (move_group_interface_gripper.plan(my_plan_gripper) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
     move_group_interface_gripper.move();


     // 4. Move the TCP close to the object
     target_pose1.position.z = target_pose1.position.z - 0.08;
     move_group_interface_arm.setPoseTarget(target_pose1);

     success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

     move_group_interface_arm.move();

     // 5. Close the  gripper
     move_group_interface_gripper.setJointValueTarget(move_group_interface_gripper.getNamedTargetValues("close"));
    move_group_interface_gripper.plan(my_plan_gripper);
    //  success = (move_group_interface_gripper.plan(my_plan_gripper) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

    //  ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

    //  move_group_interface_gripper.move();

     // 6. Move the TCP above the plate
     target_pose1.position.z = target_pose1.position.z + 0.12;
     move_group_interface_arm.setPoseTarget(target_pose1);
     success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    //  ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
     move_group_interface_arm.move();

     // 6. Move the TCP above the bin
     target_pose1.position.y = 0.60;
     target_pose1.position.x = 0.0;
     move_group_interface_arm.setPoseTarget(target_pose1);

     success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

     ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

     move_group_interface_arm.move();

     // 7. Lower the TCP above the plate
     target_pose1.position.z = target_pose1.position.z - 0.1;
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