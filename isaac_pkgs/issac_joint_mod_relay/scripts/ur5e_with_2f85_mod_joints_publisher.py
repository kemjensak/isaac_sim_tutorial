#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import JointState

joints_dict = {}
ignore_joints_list = ["robotiq_85_left_inner_knuckle_joint", "robotiq_85_right_inner_knuckle_joint",
                    "robotiq_85_left_finger_tip_joint", "robotiq_85_right_finger_tip_joint",
                    "robotiq_85_right_knuckle_joint" ]
def joint_states_callback(message):

    joint_commands = JointState()
    joint_commands.header = message.header

    for i, name in enumerate(message.name):
        if name in ignore_joints_list:
            continue 
        joints_dict[name] = message.position[i]
        if name == "robotiq_85_left_knuckle_joint":
            joints_dict["robotiq_85_right_knuckle_joint"] = message.position[i]

    joint_commands.name = joints_dict.keys()
    joint_commands.position = joints_dict.values()
    pub.publish(joint_commands)

    return

if __name__ == "__main__":
    rospy.init_node("ur5e_with_2f85_joints_publisher")
    pub = rospy.Publisher("/joint_command", JointState, queue_size=1)
    rospy.Subscriber("/joint_command_desired", JointState, joint_states_callback, queue_size=1)
    rospy.spin()
