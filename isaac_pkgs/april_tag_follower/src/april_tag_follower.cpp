 #include <ros/ros.h>
 #include <moveit/planning_scene_interface/planning_scene_interface.h>
 #include <moveit/move_group_interface/move_group_interface.h>
 #include <cmath>
 #include <string>
 #include <iostream>
 #include <memory>
 #include <std_msgs/Bool.h>
 #include <std_msgs/Empty.h>
 #include <gazebo_msgs/ModelStates.h>
 #include <geometry_msgs/Point.h>
 #include <apriltag_ros/AprilTagDetectionArray.h>
 #include <tf/transform_listener.h>

class PickAndPlaceClass
{
    public:
        void main();
        PickAndPlaceClass();
        ~PickAndPlaceClass();

        geometry_msgs::Pose getTargetPose();
        void callbackGetModelStateData(const apriltag_ros::AprilTagDetectionArrayConstPtr &msg);

    protected:
        ros::NodeHandle n;
        geometry_msgs::Point boxPosition_;
        const std::string PLANNING_GROUP_ARM_ = "manipulator";
        const std::string PLANNING_GROUP_GRIPPER_ = "gripper";
        geometry_msgs::Pose target_tag_pose_;
        std::vector<geometry_msgs::Pose> waypoints_;

        ros::Publisher pub_spawn_signal_;
        ros::Subscriber sub_ModelState_;

        std_msgs::Empty empty_;

        std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface_arm_;
        std::shared_ptr<moveit::planning_interface::MoveGroupInterface> move_group_interface_gripper_;
        std::vector<apriltag_ros::AprilTagDetection, std::allocator<apriltag_ros::AprilTagDetection>>  recieved_tag_pose_;
        ros::NodeHandle nh_;

        moveit::planning_interface::MoveGroupInterface::Plan my_plan_arm_;

        ros::AsyncSpinner spinner_;
};

PickAndPlaceClass::PickAndPlaceClass() :spinner_(1)
{
    move_group_interface_arm_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(PLANNING_GROUP_ARM_);
    move_group_interface_gripper_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(PLANNING_GROUP_GRIPPER_);
    pub_spawn_signal_ = nh_.advertise<std_msgs::Empty>("trans", 1);
    sub_ModelState_ = nh_.subscribe("/tag_detections", 1, &PickAndPlaceClass::callbackGetModelStateData, this);

    spinner_.start();
}
PickAndPlaceClass::~PickAndPlaceClass()
{
    ros::shutdown();
    spinner_.stop();
}

void PickAndPlaceClass::callbackGetModelStateData(const apriltag_ros::AprilTagDetectionArrayConstPtr &msg)
{  
    recieved_tag_pose_ = msg->detections;
}

geometry_msgs::Pose PickAndPlaceClass::getTargetPose()
{   
    uint8_t detected_tag_num = recieved_tag_pose_.size();
    while (detected_tag_num == 0){
        ROS_INFO_NAMED("tutorial", "tag not detected! waiting for tag detection.");
        ros::Duration(1.0).sleep();
        detected_tag_num = recieved_tag_pose_.size();
    }
    return recieved_tag_pose_[0].pose.pose.pose;
}

void PickAndPlaceClass::main()
{

    moveit::planning_interface::MoveGroupInterface::Plan my_plan_arm;
    moveit::planning_interface::MoveGroupInterface::Plan my_plan_gripper;

    // 1. Move to predifined position
    std::vector<double> joint_group_positions(6);
    joint_group_positions[0] = 1.57;
    joint_group_positions[1] = -1.32;
    joint_group_positions[2] = 1.32;
    joint_group_positions[3] = -1.57;
    joint_group_positions[4] = -1.57;
    joint_group_positions[5] = 0;
    move_group_interface_arm_->setJointValueTarget(joint_group_positions);
    bool success = (move_group_interface_arm_->plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    move_group_interface_arm_->plan(my_plan_arm_);

    // // ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
    move_group_interface_arm_->move();


    // 2.Get cube(april tag) pose from april detector
    ros::Duration(1.0).sleep();
    target_tag_pose_ = getTargetPose();

    // 3. Place the TCP above the cube
    geometry_msgs::PoseStamped current_pose;
    current_pose = move_group_interface_arm_->getCurrentPose("tcp");

    geometry_msgs::Pose target_pose;
    target_pose.orientation = current_pose.pose.orientation;
    target_pose.position.x = (target_tag_pose_.position.y + current_pose.pose.position.x);
    target_pose.position.y = (target_tag_pose_.position.x + current_pose.pose.position.y);
    target_pose.position.z =  current_pose.pose.position.z - 0.1;
    move_group_interface_arm_->setPoseTarget(target_pose);

    success = (move_group_interface_arm_->plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
    move_group_interface_arm_->move();

    // 4. adjust TCP pose
    ros::Duration(1.0).sleep();
    target_tag_pose_ = getTargetPose();

    target_pose.position.x += target_tag_pose_.position.y;
    target_pose.position.y += target_tag_pose_.position.x;
    target_pose.position.z =  0.1;

    success = (move_group_interface_arm_->plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
    move_group_interface_arm_->move();

    ros::Duration(1.0).sleep();
    target_tag_pose_ = getTargetPose();

    target_pose.position.x += target_tag_pose_.position.y;
    target_pose.position.y += target_tag_pose_.position.x;
    

    success = (move_group_interface_arm_->plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
    move_group_interface_arm_->move();

    // 5. Open the gripper
    move_group_interface_gripper_->setJointValueTarget(move_group_interface_gripper_->getNamedTargetValues("open"));
    success = (move_group_interface_gripper_->plan(my_plan_gripper) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
    move_group_interface_gripper_->move();

    // 6. Move the TCP close to the object
    target_pose.position.z = target_pose.position.z - 0.09;
    move_group_interface_arm_->setPoseTarget(target_pose);
    success = (move_group_interface_arm_->plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
    move_group_interface_arm_->move();

    // 7. Close the  gripper
    move_group_interface_gripper_->setJointValueTarget(move_group_interface_gripper_->getNamedTargetValues("close"));
    move_group_interface_gripper_->plan(my_plan_gripper);
    success = (move_group_interface_gripper_->plan(my_plan_gripper) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
    move_group_interface_gripper_->move();

    // 8. Move the TCP above the cube
    target_pose.position.z = target_pose.position.z + 0.12;
    move_group_interface_arm_->setPoseTarget(target_pose);
    success = (move_group_interface_arm_->plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
    move_group_interface_arm_->move();

    // 9. Move the TCP above the bin
    waypoints_.clear();
    waypoints_.push_back(target_pose);

    target_pose.position.y = 0.60;
    target_pose.position.x = 0.0;
    waypoints_.push_back(target_pose);
    moveit_msgs::RobotTrajectory trajectory;
    const double jump_threshold = 0.01;
    const double eef_step = 0.0;
    double fraction = move_group_interface_arm_->computeCartesianPath(waypoints_, eef_step, jump_threshold, trajectory);
    move_group_interface_arm_->execute(trajectory);

    // 10. Lower the TCP above the plate
    target_pose.position.z = target_pose.position.z - 0.1;
    move_group_interface_arm_->setPoseTarget(target_pose);
    success = (move_group_interface_arm_->plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
    move_group_interface_arm_->move();

    // 11. Open the gripper
    move_group_interface_gripper_->setJointValueTarget(move_group_interface_gripper_->getNamedTargetValues("open"));
    success = (move_group_interface_gripper_->plan(my_plan_gripper) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
    move_group_interface_gripper_->move();

    ros::Duration(1.0).sleep();
    pub_spawn_signal_.publish(empty_);
}

int main(int argc, char** argv){
    ros::init(argc, argv, "move_group_interface_tutorial");
    PickAndPlaceClass PPC;
    while(ros::ok()){
        PPC.main();
    }
    ros::shutdown();
    return 0;
}