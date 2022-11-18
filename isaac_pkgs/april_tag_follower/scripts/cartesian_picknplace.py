#!/usr/bin/env python
import sys
from math import pi
import copy
import rospy
import moveit_commander
import moveit_msgs.msg
import time

class MoveitCartesianTest(object):
    def __init__(self):
        super(MoveitCartesianTest, self).__init__()
        moveit_commander.roscpp_initialize(sys.argv)

                                                     
    def start_pose(self):

        print('---- Go To Start Position ---')
        start_config = [0.088, -1.2618, 1.6756, -2.03133, 4.515, 0.3915] # Start config specified on the coursework
        move_group.go(start_config, wait=True) # Move the robot to the configuration
        move_group.stop()

    def pick_pose(self):  
    
        print('---- Go To Pick Position ---')  
        
        waypoints = []
  
        pos = move_group.get_current_pose().pose
 
        pos.position.z += 0.1
        waypoints.append(copy.deepcopy(pos))
  
        pos.position.y -= 0.1
        waypoints.append(copy.deepcopy(pos))

        pos.position.x -= 0.1
        waypoints.append(copy.deepcopy(pos))

        pos.position.z -= 0.1
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

        
    def place_pose(self):   
    
        print('---- Go To Place Position ---') 
        waypoints = []
  
        pos = move_group.get_current_pose().pose
 
        pos.position.z += 0.1
        waypoints.append(copy.deepcopy(pos))
  
        pos.position.y += 0.1
        waypoints.append(copy.deepcopy(pos))

        pos.position.x += 0.1
        waypoints.append(copy.deepcopy(pos))

        pos.position.z -= 0.1
        waypoints.append(copy.deepcopy(pos))
        
        plan, fraction = move_group.compute_cartesian_path(waypoints, 0.01, 0.0) # waypoints, eef_step, jump_threshold

        traj_duration = 2.0

        display_trajectory = moveit_msgs.msg.DisplayTrajectory()
        display_trajectory.trajectory_start = robot.get_current_state()
        display_trajectory.trajectory.append(plan)
         
        display_trajectory_publisher.publish(display_trajectory)
        rospy.sleep(traj_duration)
        
        print('---- Go To Place, Execute Trajectory ---')
        move_group.execute(plan, wait = True)
        move_group.stop()
        move_group.clear_pose_targets()


def main():
    test = MoveitCartesianTest()
    
    test.start_pose()
    test.pick_pose()
    test.place_pose()
    
if __name__ == '__main__':
    rospy.init_node('cartesian_picknplce', anonymous=True)        
        
    robot = moveit_commander.RobotCommander()
    scene = moveit_commander.PlanningSceneInterface()
    move_group = moveit_commander.MoveGroupCommander('manipulator')
    display_trajectory_publisher = rospy.Publisher('/move_group/display_planned_path',
                                                     moveit_msgs.msg.DisplayTrajectory,
                                                     queue_size=20)
    main()


