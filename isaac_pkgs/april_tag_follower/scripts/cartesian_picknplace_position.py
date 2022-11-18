#!/usr/bin/env python
import sys
from math import *
import copy
import rospy
import moveit_commander
import moveit_msgs.msg
import time
from apriltag_ros.msg import AprilTagDetectionArray
import tf
from geometry_msgs.msg import PoseStamped
from sensor_msgs.msg import JointState
from std_msgs.msg import Empty

class MoveitCartesianTest(object):
    def __init__(self):
        super(MoveitCartesianTest, self).__init__()
        moveit_commander.roscpp_initialize(sys.argv)

                                                     
    def start_pose(self):

        print('---- Go To Start Position ---')
        start_config = [1.57, -1.32, 1.32, -1.57, -1.57, 0.0] # Start config specified on the coursework
        move_group.go(start_config, wait=True) # Move the robot to the configuration
        move_group.stop()

    def set_pose(self):
        print('---- Go To Set Position ---')
        start_config = [0.0, -1.6, -0.0001, -2.3568, -0.0001, 1.5666, 0.7855] # Start config specified on the coursework
        move_group.go(start_config, wait=True) # Move the robot to the configuration
        move_group.stop()

    def tag_callback(self, data):
        try:
            self.p1.header.frame_id  = data.detections[0].pose.header.frame_id
            self.p1.pose.position = data.detections[0].pose.pose.pose.position
            self.p1.pose.orientation = data.detections[0].pose.pose.pose.orientation
            
        except:
            pass


            
    def goal_pose(self):  
    
        print('---- Go To Goal Position ---')        
        waypoints = []
        
        pos = move_group.get_current_pose().pose
        self.p1 = self.listener.transformPose("panda_link0", self.p1)
        pos.position.x = pos.position.x + self.p1.pose.position.x/18.5
        pos.position.y = pos.position.y + self.p1.pose.position.y/12
        pos.position.z = 0.4

        
        pos.orientation.x = self.set.orientation.x
        pos.orientation.y = self.set.orientation.y
        pos.orientation.z = self.set.orientation.z
        pos.orientation.w = self.set.orientation.w
        
        waypoints.append(copy.deepcopy(pos))
        
        plan, fraction = move_group.compute_cartesian_path(waypoints, 0.01, 0.0) # waypoints, eef_step, jump_threshold
        
        traj_duration = 2.0

        display_trajectory = moveit_msgs.msg.DisplayTrajectory()
        display_trajectory.trajectory_start = robot.get_current_state()
        display_trajectory.trajectory.append(plan)
         
        print('---- Display Trajectory ---')
        display_trajectory_publisher.publish(display_trajectory)
        rospy.sleep(traj_duration)
       
        print('---- Go To Pick, Execute Trajectory ---')
        move_group.execute(plan, wait = True)
        move_group.stop()

    def pick_stuff(self):
        print('---- Grip ---')
        start_config = [0.001,0.001]
        move_group_gripper.go(start_config, wait=True) # Move the robot to the configuration
        move_group_gripper.stop()
    
    def open_grip(self):
        print('---- open ---')
        start_config = [0.035, 0.035]
        move_group_gripper.go(start_config, wait=True) # Move the robot to the configuration
        move_group_gripper.stop()

    def final_pose(self):
        print('---- Go To Final Position ---')
        start_config = [-0.450, 0.365, -1.56, -1.46, 0.359, 1.48, 0.588] # Start config specified on the coursework
        move_group.go(start_config, wait=True) # Move the robot to the configuration
        move_group.stop()

def main():
    test = MoveitCartesianTest()
    rospy.Subscriber('tag_detections',AprilTagDetectionArray, test.tag_callback, queue_size=10)
    test.open_grip()
    test.start_pose()

    while not rospy.is_shutdown():
        key = input("press 'r' to run or 'q' to quick ")
        if key == 'r':
            test.set_pose()
            rospy.sleep(1)
            test.goal_pose()
            test.pick_stuff()
            test.final_pose()
            test.open_grip()
            test.start_pose()
            rospy.sleep(1)
            teleport.publish()
        
        if key == 'q':
            break

    
if __name__ == '__main__':
    rospy.init_node('cartesian_picknplce_position', anonymous=True)
            
        
    robot = moveit_commander.RobotCommander()
    scene = moveit_commander.PlanningSceneInterface()
    move_group = moveit_commander.MoveGroupCommander('panda_arm')
    move_group_gripper = moveit_commander.MoveGroupCommander('panda_hand')
    display_trajectory_publisher = rospy.Publisher('/move_group/display_planned_path',
                                                     moveit_msgs.msg.DisplayTrajectory,
                                                     queue_size=20)
    teleport = rospy.Publisher('/trans', Empty, queue_size=1)
    main()