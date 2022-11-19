#!/usr/bin/env python3

import rospy
import numpy as np
from isaac_ros_messages.srv import IsaacPose
from isaac_ros_messages.srv import IsaacPoseRequest
from geometry_msgs.msg import Pose
from std_msgs.msg import Empty


def teleport_client(msg):
    rospy.wait_for_service("teleport")
    try:
        teleport = rospy.ServiceProxy("teleport", IsaacPose)
        teleport(msg)
        return
    except rospy.ServiceException as e:
        print("Service call failed: %s" % e)

def callback(data):
    cube_pose = Pose()
    cube_pose.position.x = np.random.uniform(-0.14, 0.06)
    cube_pose.position.y = np.random.uniform(-0.44, 0.18 )
    cube_pose.position.z = 0.23
    cube_pose.orientation.w = 1
    cube_pose.orientation.x = 0
    cube_pose.orientation.y = 0
    cube_pose.orientation.z = 0

    teleport_msg = IsaacPoseRequest()
    teleport_msg.names = ["/World/tag_cube"]
    teleport_msg.poses = [cube_pose]
    teleport_client(teleport_msg)
# compose teleport messages

if __name__ == '__main__':
    rospy.init_node('tag_spawner', anonymous=True)
    rospy.Subscriber('/trans',Empty,callback,queue_size=1)
    rospy.spin()