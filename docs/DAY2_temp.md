
> Written with [StackEdit](https://stackedit.io/).
> ### 출처/참고: https://docs.omniverse.nvidia.com/app_isaacsim/app_isaacsim/overview.html

# DAY-2

### 4.4. Joint Control: Extension Python - Panda 버전.

#### 4.4.0. Goal (GUI 방식 vs. Extension 방식)
- Adding a **'ROS Joint State publisher and subscriber'** via both **GUI and Extension scripting**.
- 우선, **GUI workflow 방식**: **ROS Joint State publisher/subscriber**를 추가.
- 다음, **Extension workflow 방식**: Isaac Sim의 **script editor 기반의 Omnigraph python API**를 사용하여 **ROS Joint State publisher/subscriber**을 추가하는 방식을 설명함.

- 당연히, Isaac Sim의 **extension workflow**에 대해서 이해하고 있어야 하며, 최종적으로는 **UR5e_with_2f85**를 stage에 load한 후, extension workflow를 통해 **UR5e_with_2f85 end-effector의 위치제어**를 구현.
- 따라서, Online documentation의 **[7-DOF Franka Emika Panda Robot 튜토리얼](https://docs.omniverse.nvidia.com/app_isaacsim/app_isaacsim/tutorial_ros_manipulation.html)**을 먼저 진행한 후, **UR5e_with_2f85**를 실습함. 
<br>

#### 4.4.1. Add joint states in UI

- 여기서 joint states란? 
- Viewport 하단의 Content tab에서 `Isaac/Robots/Franka/franka_alt_fingers.usd`를 Open해야 함.

![Screenshot from 2022-11-13 23-38-02](https://user-images.githubusercontent.com/29449124/201529368-5b8c00bd-6ca6-4c80-97dc-b2363e7db67e.png)

- Action graph 생성: _**Create > Visual Scripting > Action graph**_
- Open action graph를 한 후, 아래와 같이 노드들을 추가하고 연결.

	- **On Playback Tick**  node to execute other graph nodes every simulation frame.
	    
	- **Isaac Read Simulation Time**  node to retrieve current simulation time.
	    
	- **ROS1 Publish Joint State**  node to publish ROS Joint States to the  `/joint_states`  topic.
	    
	- **ROS1 Subscribe Joint State**  node to subscribe to ROS Joint States from the  `/joint_command`  topic.
	    
	- **Articulation Controller**  node to move the robot articulation according to commands received from the subscriber node.

![Screenshot from 2022-11-13 23-38-17](https://user-images.githubusercontent.com/29449124/201529367-020510d8-5df8-4c5a-8786-e7e7fb76840a.png)

![Screenshot from 2022-11-13 23-39-21](https://user-images.githubusercontent.com/29449124/201529365-b9a08f04-48c9-4610-9495-7107ff72a8a4.png)

#### <노드 설명>
- 1) **On Playback Tick**: Simulation 실행 시 모든 graph nodes들의 base clock 역활을 하는 tick/time을 출력함. 
	- 기능: Connect the Tick output of the On Playback Tick node to the Execution input of the ROS1 Publish Joint State, ROS1 Subscribe JointState and Articulation Controller nodes.
	- **On Playback Tick의 time**: 'The global playback time in seconds'이고, 
	- **Isaac Read Simulation Time의 Simulation Time**: 'Current Simulation Time in Seconds'.
	<br>
	- On Playback Tick의 _Time_은 하단부 keyframe이 표시되는 timeline에서의 **경과시간**이 출력되는 것. **Read Simulation Time과는 달리, Timeline이 지정된 frame 수 만큼 경과되면, timeline과 함께 0으로 리셋**되어 다시 시작됨.
	- Isaac Read Simulation Time의 Simulation Time 은 isaac sim 내에서 play를 눌러 simulation이 진행되는 동안 **경과된 시간이 누적되어 출력**되는 것임.
	

- 2) **Isaac Read Simulation Time**: 현재까지의 전체 simulation time을 읽어옴.
	- Connect the Simulation Time output of the Isaac Read Simulation Time node to the **Timestamp** input of the ROS1 Publish Joint State node. Setup other connections between nodes.


- 3) **ROS1 Publish Joint State**: ROS joint states 데이터를 /joint_states 토픽 메시지로 발행. 
	- Add the _/panda_ robot articulation to the targetPrim --> /franka
![Screenshot from 2022-11-13 23-40-02](https://user-images.githubusercontent.com/29449124/201529363-47605336-d45e-4550-9832-16849c8f4b20.png)

- 4) **ROS1 Subscribe Joint State**:ROS joint states를 /joint_command 토픽 메시지로 받아옴.
- 5) **Articulation Controller**: subscriber node로 부터 받은 commands에 따라 stage 위의 로봇을 움직임.
	- Add the **/panda robot articulation to the targetPrim**. 
	- Additionally make sure to uncheck usePath.
	
![Screenshot from 2022-11-13 23-39-40](https://user-images.githubusercontent.com/29449124/201529364-c3028af1-54e2-456d-a183-c5923f83983c.png)

- Isaac sim interface에서 "Play"버튼을 실행
- `rosrun isaac_tutorials ros_publisher.py`를 실행
- While the robot is moving, open a ROS-sourced terminal and check the joint state rostopic by `rostopic echo /joint_states`.  You’ll see that the position of the joints are changing following the robot.

![Screenshot from 2022-11-14 00-17-43](https://user-images.githubusercontent.com/29449124/201529362-40c83567-5f07-4f4d-841a-eaad0273eaff.png)

![Screenshot from 2022-11-14 00-18-13](https://user-images.githubusercontent.com/29449124/201529359-6642693e-6b97-4454-a070-70fae058067e.png)


#### 4.4.2. [Add Joint States in Extension](https://docs.omniverse.nvidia.com/app_isaacsim/app_isaacsim/tutorial_ros_manipulation.html#add-joint-states-in-extension)

- The same action done via UI can also be done **using python script**. 
- 화면 좌측 하단 _Content_  tab, and open `Isaac/Robots/Franka/franka_alt_fingers.usd`.
- Script Editor(_*Window -> Script Editor*_), and copy paste the following code into it.

	```
	import omni.graph.core as og
	from omni.isaac.core_nodes.scripts.utils import set_target_prims

	og.Controller.edit(
	    {"graph_path": "/panda/ActionGraph", "evaluator_name": "execution"},
	    {
	        og.Controller.Keys.CREATE_NODES: [
	            ("OnPlaybackTick", "omni.graph.action.OnPlaybackTick"),
	            ("PublishJointState", "omni.isaac.ros_bridge.ROS1PublishJointState"),
	            ("SubscribeJointState", "omni.isaac.ros_bridge.ROS1SubscribeJointState"),
	            ("ArticulationController", "omni.isaac.core_nodes.IsaacArticulationController"),
	            ("ReadSimTime", "omni.isaac.core_nodes.IsaacReadSimulationTime"),
	        ],
	        og.Controller.Keys.CONNECT: [
	            ("OnPlaybackTick.outputs:tick", "PublishJointState.inputs:execIn"),
	            ("OnPlaybackTick.outputs:tick", "SubscribeJointState.inputs:execIn"),
	            ("OnPlaybackTick.outputs:tick", "ArticulationController.inputs:execIn"),

	            ("ReadSimTime.outputs:simulationTime", "PublishJointState.inputs:timeStamp"),

	            ("SubscribeJointState.outputs:jointNames", "ArticulationController.inputs:jointNames"),
	            ("SubscribeJointState.outputs:positionCommand", "ArticulationController.inputs:positionCommand"),
	            ("SubscribeJointState.outputs:velocityCommand", "ArticulationController.inputs:velocityCommand"),
	            ("SubscribeJointState.outputs:effortCommand", "ArticulationController.inputs:effortCommand"),
	        ],
	        og.Controller.Keys.SET_VALUES: [
	            # Providing path to /panda robot to Articulation Controller node
	            # Providing the robot path is equivalent to setting the targetPrim in Articulation Controller node
	            ("ArticulationController.inputs:usePath", True),
	            ("ArticulationController.inputs:robotPath", "/panda"),
	        ],
	    },
	)

	# Setting the /panda target prim to Publish JointState node
	set_target_prims(primPath="/panda/ActionGraph/PublishJointState", targetPrimPaths=["/panda"])
	```
- Make sure  `roscore`  is running, press  _Run_  in the Script Editor and the Action Graph with all required nodes should be added. You can find the corresponding ActionGraph in the Stage Tree.
- Same as the previous section, test out the ROS bridge using the provide python script to publish joint commands to the robot. `rosrun isaac_tutorials ros_publisher.py`
- Check the joint state with  `rostopic  echo  /joint_states`  while it’s moving.

<br>

### 4.5. Joint Control: Extension Python - UR5e_with_2f85 ver.

#### 4.5.0. Goal
- `Edit > toggle visualization mode` On -> GPU resource status 모니터링이 가능해짐.
- Isaac Sim의 **GUI workflow**를 기반으로 **ROS Joint State publisher/subscriber**를 추가.
- Isaac Sim의 **script editor 기반의 Omnigraph python API**를 사용하여 **ROS Joint State publisher/subscriber**을 추가.
- 즉, this tutorial covered **adding a ROS Joint State publisher and subscriber** via both **UI and Extension scripting**.

<br>

- Isaac Sim의 **extension workflow**에 대해서 이해하고 있어야 함. 
- **UR5e_with_2f85**를 stage에 load한 후, extension workflow를 통해 **UR5e_with_2f85 end-effector의 위치제어**를 구현.
- Online documentation의 **[7-DOF Franka Emika Panda Robot 튜토리얼](https://docs.omniverse.nvidia.com/app_isaacsim/app_isaacsim/tutorial_ros_manipulation.html)**을 먼저 진행한 후, **UR5e_with_2f85**를 실습함. 
<br>

#### 4.5.1. Add joint states in UI
![enter image description here](https://user-images.githubusercontent.com/96465330/201400861-b8028aeb-f2c4-4f63-97ff-2d78075be1f4.png)

- Simple_room.usd 파일을 drag하여 open
![enter image description here](https://user-images.githubusercontent.com/96465330/201400940-9800a67c-66dd-4685-ac8c-4133df632b8d.png)

- Model.urdf 파일을 URDF Importer(_**Isaac Utiles > URDF Importer**_)를 통해 load함. 
- 경로: src/gazebo_pkgs/universal_robot/ur_description/urdf/model.urdf

![enter image description here](https://user-images.githubusercontent.com/96465330/201401000-e1632e1d-4bc9-49c5-9c17-0d16ae375b6b.png

![enter image description here](https://user-images.githubusercontent.com/96465330/201401048-65f75908-6f48-4ff9-b72c-cefd5c446a68.png)

- Action graph를 생성(_**Create > Visual Scripting > Action graph**_)하고, open action graph를 한 후, 아래와 같이 노드들을 추가하고 연결. )

![enter image description here](https://user-images.githubusercontent.com/96465330/201401127-3d3e4432-0037-4096-9e69-0964ebe663a2.png)

#### <노드 설명>
- **On Playback Tick**: Simulation 실행 시 모든 graph nodes들의 base clock 역활을 하는 tick/time을 출력함. 
	- 기능: Connect the Tick output of the On Playback Tick node to the Execution input of the ROS1 Publish Joint State, ROS1 Subscribe JointState and Articulation Controller nodes.
	- **On Playback Tick의 time**: 'The global playback time in seconds'이고, 
	- **Isaac Read Simulation Time의 Simulation Time**: 'Current Simulation Time in Seconds'.
	<br>
	- On Playback Tick의 _Time_은 하단부 keyframe이 표시되는 timeline에서의 **경과시간**이 출력되는 것. **Read Simulation Time과는 달리, Timeline이 지정된 frame 수 만큼 경과되면, timeline과 함께 0으로 리셋**되어 다시 시작됨.
	- Isaac Read Simulation Time의 Simulation Time 은 isaac sim 내에서 play를 눌러 simulation이 진행되는 동안 **경과된 시간이 누적되어 출력**되는 것임.
	
- **Isaac Read Simulation Time**: 현재까지의 전체 simulation time을 읽어옴.
	- Connect the Simulation Time output of the Isaac Read Simulation Time node to the **Timestamp** input of the ROS1 Publish Joint State node. Setup other connections between nodes.

- **ROS1 Publish Joint State**: ROS joint states 데이터를 /joint_states 토픽 메시지로 발행. 
	- Add the _/panda_ robot articulation to the targetPrim.

- **ROS1 Subscribe Joint State**:ROS joint states를 /joint_command 토픽 메시지로 받아옴.

- **Articulation Controller**: subscriber node로 부터 받은 commands에 따라 stage 위의 로봇을 움직임.
	- Add the **/panda robot articulation to the targetPrim**. 
	- Additionally make sure to uncheck usePath.

#### <Gripper Joint 설정>
- URDF로 import한 `ur5e_with_2f85` 에서  robotiq의 모든 joint들을 아래와 같이 추가/수정 필요.
- Stage tree의 search창에 robotiq 검색.

![enter image description here](https://user-images.githubusercontent.com/96465330/201523876-74902f26-bb0e-4e86-8f88-e7473b74b176.png)

![enter image description here](https://user-images.githubusercontent.com/96465330/201523930-49450ec8-2e2a-462c-aa63-6395bcf82eb7.png)

- `robotiq_85_left_knuckle_joint`, `robotiq_85_right_knuckle_joint`외 모든 joint들은 passive joint들이므로, active joint와의 구분을 위해 `Exclude From Articulation` 체크해야 함. 
- Drive의 `Max Force` 값을 0으로 설정, 위의 두 joint value는 6[????]으로 설정. (**단 robotiq_85_base_joint는 건드리지 않음.** )
- `robotiq_85_left_finger_joint`와 `robotiq_85_right_finger_joint`의 Axis 속성이 X축으로 되어 있는데 Z축으로 변경.

![enter image description here](https://user-images.githubusercontent.com/96465330/201524151-3f21afeb-ba1c-43b6-9fa2-3f3be2feb8e7.png)

![enter image description here](https://user-images.githubusercontent.com/96465330/201524166-b6122722-b981-4c51-a2b4-dd0fff8969d7.png)

- `robotiq_85_left_finger_tip_link`와 `robotiq_85_left_finger_tip_link`에 revolute joint를 추가한 후 drive 설정을 적용.
 ![Screenshot from 2022-11-11 22-33-21](https://user-images.githubusercontent.com/60342784/201351042-b08721ec-a038-4d45-9dd5-5772edcfb1ed.png)

- 추가한 revolute joint에서 `Exclude From Articulation`의 check를 해제한 후, body0의 target을 각각 `robotiq_85_right_finger_link`와`robotiq_85_left_finger_link`로 설정.
- `Local Position 0`과 `Local Position 1`을 아래 사진처럼 입력함.(R, L)

![Screenshot from 2022-11-11 22-36-03](https://user-images.githubusercontent.com/60342784/201351415-cd7bc58b-8898-49ee-9b79-a914ed7318f1.png)

![Screenshot from 2022-11-11 22-37-40](https://user-images.githubusercontent.com/60342784/201351620-6fe3e96b-d47e-47fd-ad4d-ff4f3c03641f.png)

- Joint_command를 발행하는 python node를 생성: src/Isaac_sim/isaac_tutorials/script/ur5e_publisher.py
- 다음 코드는 default_joints의 joint 각에서 +/- 0.7[rad] 만큼의 자세를 왕복으로 움직이도록 /joint_command 토픽 메시지를 보내는 코드임. 
- **중요: 라이선스 문제가 있으므로, 본 과정에서만 사용하고, 추후 절대 배포하시면 안됩니다..... 아시겠죠?**

	```
	#!/usr/bin/env python3

	# Copyright (c) 2021-2022, NVIDIA CORPORATION.  All rights reserved.
	#
	# NVIDIA CORPORATION and its licensors retain all intellectual property
	# and proprietary rights in and to this software, related documentation
	# and any modifications thereto.  Any use, reproduction, disclosure or
	# distribution of this software and related documentation without an express
	# license agreement from NVIDIA CORPORATION is strictly prohibited.

	import rospy
	from sensor_msgs.msg import JointState
	import numpy as np
	import time

	rospy.init_node("test_rosbridge", anonymous=True)

	pub = rospy.Publisher("/joint_command", JointState, queue_size=10)
	joint_state = JointState()


	joint_state.name = [
	    "shoulder_pan_joint",
	    "shoulder_lift_joint",
	    "elbow_joint",
	    "wrist_1_joint",
	    "wrist_2_joint",
	    "wrist_3_joint",
	    "robotiq_85_left_knuckle_joint",
	    "robotiq_85_right_knuckle_joint"
	]

	num_joints = len(joint_state.name)
	
	# make sure kit's editor is playing for receiving messages ##
    # joint unit [rad]
	joint_state.position = np.array([0.0] * num_joints)
	default_joints = [0.0, -1.57, 1.57, -0.0, -0.0, 1.6, 0.4, 0.4] 

	# limiting the movements to a smaller range (this is not the range of the robot, just the range of the movement
	max_joints = np.array(default_joints) + 0.7
	min_joints = np.array(default_joints) - 0.7
	max_joints[6],max_joints[7] = 0.4, 0.4
	min_joints[6],min_joints[7] = 0.4, 0.4
	# position control the robot to wiggle around each joint
	time_start = time.time()
	rate = rospy.Rate(20)
	while not rospy.is_shutdown():
	    joint_state.position = np.sin(time.time() - time_start) * (max_joints - min_joints) * 0.5 + default_joints
	    pub.publish(joint_state)  
	    rate.sleep()
	```
- Isaac sim interface에서 "Play"버튼을 실행
- `rosrun isaac_tutorials ur5e_publisher.py`를 통해 위에서 작성한 `ur5e_publisher.py` 코드를 실행
- While the robot is moving, open a ROS-sourced terminal and check the joint state rostopic by `rostopic echo /joint_states`. 
- You’ll see that the position of the joints are changing following the robot.


# 아래 PYTHON SCRIPT를 ur5e 버전에 맞도록 수정!

#### 4.5.2. [Add Joint States in Extension](https://docs.omniverse.nvidia.com/app_isaacsim/app_isaacsim/tutorial_ros_manipulation.html#add-joint-states-in-extension)

- The same action done via UI can also be done **using python script**. 
- 화면 좌측 하단 _Content_  tab, and open `Isaac/Robots/Franka/franka_alt_fingers.usd`.
- Script Editor(_*Window -> Script Editor*_), and copy paste the following code into it.

	```
	import omni.graph.core as og
	from omni.isaac.core_nodes.scripts.utils import set_target_prims

	og.Controller.edit(
	    {"graph_path": "/panda/ActionGraph", "evaluator_name": "execution"},
	    {
	        og.Controller.Keys.CREATE_NODES: [
	            ("OnPlaybackTick", "omni.graph.action.OnPlaybackTick"),
	            ("PublishJointState", "omni.isaac.ros_bridge.ROS1PublishJointState"),
	            ("SubscribeJointState", "omni.isaac.ros_bridge.ROS1SubscribeJointState"),
	            ("ArticulationController", "omni.isaac.core_nodes.IsaacArticulationController"),
	            ("ReadSimTime", "omni.isaac.core_nodes.IsaacReadSimulationTime"),
	        ],
	        og.Controller.Keys.CONNECT: [
	            ("OnPlaybackTick.outputs:tick", "PublishJointState.inputs:execIn"),
	            ("OnPlaybackTick.outputs:tick", "SubscribeJointState.inputs:execIn"),
	            ("OnPlaybackTick.outputs:tick", "ArticulationController.inputs:execIn"),

	            ("ReadSimTime.outputs:simulationTime", "PublishJointState.inputs:timeStamp"),

	            ("SubscribeJointState.outputs:jointNames", "ArticulationController.inputs:jointNames"),
	            ("SubscribeJointState.outputs:positionCommand", "ArticulationController.inputs:positionCommand"),
	            ("SubscribeJointState.outputs:velocityCommand", "ArticulationController.inputs:velocityCommand"),
	            ("SubscribeJointState.outputs:effortCommand", "ArticulationController.inputs:effortCommand"),
	        ],
	        og.Controller.Keys.SET_VALUES: [
	            # Providing path to /panda robot to Articulation Controller node
	            # Providing the robot path is equivalent to setting the targetPrim in Articulation Controller node
	            ("ArticulationController.inputs:usePath", True),
	            ("ArticulationController.inputs:robotPath", "/panda"),
	        ],
	    },
	)

	# Setting the /panda target prim to Publish JointState node
	set_target_prims(primPath="/panda/ActionGraph/PublishJointState", targetPrimPaths=["/panda"])
	```
- Make sure  `roscore`  is running, press  _Run_  in the Script Editor and the Action Graph with all required nodes should be added. You can find the corresponding ActionGraph in the Stage Tree.
- Same as the previous section, test out the ROS bridge using the provide python script to publish joint commands to the robot. `rosrun isaac_tutorials ros_publisher.py`
- Check the joint state with  `rostopic  echo  /joint_states`  while it’s moving.


<br>

### 4.6. Custom Message
- Isaac sim의 python환경은 ROS Noetic의 rospy 기반의 ROS 환경이 내장되어 있음.이에 따라 주요 ROS package와 메시지를 사용할 수 있으며, custom message를 생성해서 발행 및 구독(Publish & Subscribe)을 할 수 있음. 다음 튜토리얼은 접촉 센서의 데이터에 대한 custom ROS message를 발행하는 과정을 나타냄.
- 앞서 Isaac sim을 설치하면서 생성된 ROS 패키지에 ContactSensor.msg 라는 이름으로 custom message가 존재.
(src/isaac_tutorials/msg/ContactSensor.msg)

	```
	Header header
	float64 time
	float64 value
	bool in_contact
	```

- msg파일을 생성했으면, Package.xml 파일과 CMakeLists.txt 파일에 등록해야 함.
메시지 생성에 대한 의존성을 추가하기 위해 Package.xml 파일과 CMakeLists.txt 파일을 수정.


##### CMakeLists.txt

- message_generation을 추가해 메시지 생성에 대한 의존성을 추가함.
![enter image description here](https://user-images.githubusercontent.com/96465330/201332157-8018ea40-4f5d-4ca6-8a77-ff8a37a8dd83.png)

- 생성한 msg 파일을 등록
![enter image description here](https://user-images.githubusercontent.com/96465330/201332317-93f950db-e318-4064-a07d-d02645a41503.png)


##### Package.xml

- message_generation에 대한 의존성과 message_runtime에 대한 의존성을 추가
- 수정을 마치면, catkin_make을 통해 workspace를 빌드
![enter image description here](https://user-images.githubusercontent.com/96465330/201332824-c5a3d71b-7c2e-453a-852f-d796ed7dac56.png)


![](https://user-images.githubusercontent.com/96465330/201333162-a90be238-746a-4587-adb5-9eba2d1a7f1e.png)



#### Publishing Custom Messages in Extension Scripting

- Custom message를 사용할 때는 아래 스니펫을 이용한다.
- <PATH_TO_ROS_WORKSPACE>는 catkin_ws directory 주소로 대치하고, <PYTHON_VERSION>은 python3로 변경.

	```
	import sys

	sys.path.append('<PATH_TO_ROS_WORKSPACE>/devel/lib/<PYTHON_VERSION>/dist-packages/isaac_tutorials/msg')

	from _ContactSensor import ContactSensor

	contact_msg = ContactSensor()
	```


![enter image description here](https://user-images.githubusercontent.com/96465330/201481384-6f3dab36-d09f-493f-8a29-89848f7b1f62.png)


### 4.7. ROS Bridge in Standalone Workflow

- Standalone workflow 방식은 크게 두 가지로 나뉨.
	- 우선 링크 클릭해서 보여주며 설명. https://docs.omniverse.nvidia.com/app_isaacsim/app_isaacsim/tutorial_intro_workflows.html#standalone-application
	- 1. Python API를 기반으로 독립 실행형(standalone type) 코드를 **한방에 작성/실행**하면, IsaacSim이 켜지며 미리 정의된 시뮬레이션이 작동함.
	- 2. IsaacSim을 headless mode(websocket?)로 미리 켜둔 상태에서, Jupyter notebook을 기반으로 **line-by-line으로 작성/실행**하며 프로그램을 개발하는 방식임. 이 때, Live sync가 On이 되어 있는 것을 확인할 수 있고, Jupyter하고 IsaacSim은 비동기 방식으로 연동됨. 

#### Standalone 방식으로 구동하는 예시

```py
import time
import carb
from omni.isaac.kit import SimulationApp


# Example ROS bridge sample showing rospy and rosclock interaction
simulation_app = SimulationApp({"renderer": "RayTracedLighting", "headless": True})
import omni
from omni.isaac.core.utils.extensions import enable_extension
from omni.isaac.core import SimulationContext

import omni.graph.core as og

# enable ROS bridge extension
enable_extension("omni.isaac.ros_bridge")

simulation_app.update()

# check if rosmaster node is running
# this is to prevent this sample from waiting indefinetly if roscore is not running
# can be removed in regular usage
import rosgraph

if not rosgraph.is_master_online():
    carb.log_error("Please run roscore before executing this script")
    simulation_app.close()
    exit()
# Note that this is not the system level rospy, but one compiled for omniverse
from rosgraph_msgs.msg import Clock
import rospy
```

- OnImpulseEvent OmniGraph 노드는 어떠한 ROS OmniGraph 노드와 연결될 수 있으며, publish 혹은 subscribe 주기를 제어할 수 있다.
- *ROS1 Publish Clock node*를 Standalone Python으로 정교하게 제어하는 예제:
 
	```py
	import omni.graph.core as og
	og.Controller.edit(
	    {"graph_path": "/ActionGraph", "evaluator_name": "execution"},
	    {        og.Controller.Keys.CREATE_NODES: [
	            ("ReadSimTime", "omni.isaac.core_nodes.IsaacReadSimulationTime"),
	            ("PublishClock", "omni.isaac.ros_bridge.ROS1PublishClock"),
	            ("OnImpulseEvent", "omni.graph.action.OnImpulseEvent"),
	        ],
	        og.Controller.Keys.CONNECT: [
	            # Connecting execution of OnImpulseEvent node to PublishClock so it will only publish when an impulse event is triggered
	           ("OnImpulseEvent.outputs:execOut", "PublishClock.inputs:execIn"),
	            # Connecting simulationTime data of ReadSimTime to the clock publisher node
	           ("ReadSimTime.outputs:simulationTime", "PublishClock.inputs:timeStamp"),
	        ],
	        og.Controller.Keys.SET_VALUES: [Isaac sim의 Extension workflow(Script Editor)를 통해 메시지를 발행하려면, "asyncio"라는 모듈을 import해야함. 이 모듈은 실행중인 rospy thread가 Omniverse Isaac Sim thread를 제한하지 않도록 하는 기능을 가짐. 

	```
	import asyncio
	import rospy

	try:
	           # Assigning topic name to clock publisher
	           ("PublishClock.inputs:topicName", "/clock"),
	        ],
	    },
	)
	```      

- 다음 line을 원하는 곳에 추가하여 clock publisher를 1회 동작하게 하는 tick을 발생시키는 impulse event를 생성할 수 있다.


	   $ og.Controller.set(og.Controller.attribute("/ActionGraph/OnImpulseEvent.state:enableImpulse"), True)


- 아래의 샘플 코드 파일은 ROS component로 어떻게 action graph를 만들고, 각각의 component를 다른 주기로 ticking 하는방법을 보여준다. (isaac directory/standalone_examples/api/omni.isaac.ros_bridge/clock.py)
![enter image description here](https://user-images.githubusercontent.com/96465330/201475369-2ee4d420-febd-4a8c-8bb6-6f2e4ff8c46f.png)

	  $ /python.sh standalone_examples/api/omni.isaac.ros_bridge/clock.py
		      
- 아래의 샘플 코드 파일은 주기적으로 매 n번째 렌더링된 프레임마다 image를 publish하는 방법을 보여준인다.

	   $ /python.sh standalone_examples/api/omni.isaac.ros_bridge/camera_periodic.py

- 아래의 샘플 코드 파일은 비주기적으로 Branch node가 enabled되었을 때, image를 publish하는 방법을 보여준인다.

	   $ ./python.sh standalone_examples/api/omni.isaac.ros_bridge/camera_manual.py

- 위 image 관련 코드들의 실행 결과의 visualization은 rivz를 아래 명령어를 실행하여 이뤄진다.

	   $ rviz -d ros_workspace/src/isaac_tutorials/rviz/camera_manual.rviz

- ROS component node를 포함하는 action graph를 가져와 설정을 변경하는 방법을 보인다.

	   $ ./python.sh standalone_examples/api/omni.isaac.ros_bridge/carter_stereo.py
      
   위 코드의 실행 결과는 다음 명령어를 통해 visualization이 가능하다.
   
	   $ rviz  -d  ros_workspace/src/isaac_tutorials/rviz/carter_stereo.rviz
      
- 여러 로봇을 동시에 ROS navigation으로 움직이는 모습을 보인다.

	   $ ./python.sh standalone_examples/api/omni.isaac.ros_bridge/carter_multiple_robot_navigation.py hospital

- 여러 USD stage를 추가하고, ROS component node를 action graph에 추가해, 수동으로 tick하는 방법을 보인다.
 
	   $ ./python.sh standalone_examples/api/omni.isaac.ros_bridge/moveit.py

`CTRL-C`를 눌러 각 실행파일을 끝낼 수 있다.

<br>


### 4.8. April Tags

- 상단의 탭에서 `Isaac Examples -> ROS -> April Tag`을 실행한다.
![enter image description here](https://user-images.githubusercontent.com/96465330/201479735-0a62379a-3fbd-4f24-a2f0-dcdfa22f48c8.png)

- Action graph는 다음과 같이 구성
![enter image description here](https://user-images.githubusercontent.com/96465330/201480457-871ffbf6-a31e-4be6-9f6f-47c7c7f85146.png)


- *Play* 버튼을 눌러 데이터를 publish 한다. 
-  아래 명령어를 터미널에 입력하여 april_tag의 detection 노드를 실행한다.
	
	   $ roslaunch  isaac_tutorials  apriltag_continuous_detection.launch

![enter image description here](https://user-images.githubusercontent.com/96465330/201480556-8148160d-a030-4685-86e6-1f5f27d17816.png)

- 새로운 터미널에 아래 명령어를 입력하여 rviz를 실행.

      $ rviz  -d  ros_workspace/src/isaac_tutorial/rviz/apriltag_config.rviz
   ![enter image description here](https://user-images.githubusercontent.com/96465330/201480436-3998dcda-8ce5-48e8-a9f4-57cce9c0e9ac.png)

 - `rostopic  echo  tag_detections` 명령어를 실행하여 detection 결과를 확인한다.
![enter image description here](https://user-images.githubusercontent.com/96465330/201480613-df0a35c3-a3e5-4fa4-a05c-e07cea655f43.png)



## 4.9. April tag recognition using camera attached on the UR5e manipulator

## 4.9.1. Add RGB camera at the end-effector of UR5e
이전 tutorial을 잘 따라오셨다면, 다음과 같은 환경이 세팅되어 있을 것임.
![enter image description here](https://user-images.githubusercontent.com/96465330/201471767-46dc40d0-b4f8-457e-8a4f-daf8035ff121.png) 
이번 튜토리얼에서는 UR5e의 end-effector에 카메라를 부착하고 카메라의 viewport를 ROS  토픽메시지를 통해 발행하는 과정을 설명하고자 함.

### Add camera
- Create > Camera를 누르면 Stage tree에 카메라가 생성됨.

![enter image description here](https://user-images.githubusercontent.com/96465330/201472007-a999595a-c591-4259-8cac-fdcbcdfb3f8d.png)
![enter image description here](https://user-images.githubusercontent.com/96465330/201472025-ded175fb-3f67-475e-a2cc-c2bddade1713.pngrospy.init_node("hello", anonymous=True, disable_signals=True, log_level=rospy.ERROR)
	except rospy.exceptions.ROSException as e:
	    print("Node has already been initialized, do nothing")

	async def my_task():
	    from std_msgs.msg import String
	    pub = rospy.Publisher("/hello_topic", String, queue_size=10)

	    for frame in range(10):
	        pub.publish("hello world " + str(frame))
	        await asyncio.sleep(1.0)
	    pub.unregister()
-	  카메라를 end-effector 상단부에 부착시킬 것이기 때문에, flange link의 하위로 옮겨야 함.
![enter image description here](https://user-images.githubusercontent.com/96465330/201472151-3b542741-3fef-41f7-bff3-7a0618f71737.png)
- 보통 카메라는 end-effector 상단부에 위치하기 때문에, property > transform에서 z축으로 0.1정도 translate 시킴. (예시는 카메라 view가 원하는 방위로 생성되었기 때문에 따로 orientation 설정은 생략)
![enter image description here](https://user-images.githubusercontent.com/96465330/201472239-5c87a7f4-f850-4be7-9af4-de2cad0798e1.png)
 ### Build the Graph for a RGB publisher
- Visual scripting을 연다: Window > Visual Scripting > Action graph
- "New Action Graph"아이콘을 눌러 새로운 Action Graph창을 생성
- 아래 그림과 같이 Action graph를 연결하고 아래 내용과 같이 내부 파라미터를 설정  pub = None

	asyncio.ensure_future(my_task())
	```
- 위의 명령을 Script Editor (Window > Script Editor)에 붙여놓고 실행.

![enter image description here](https://user-images.githubusercontent.com/96465330/201048649-a7697efc-2b11-4cb7-98a5-f469c969e1d336137-4ca2175d-1775-41f4-b29e-f4c3e274b84f.png)

- 내부 파라미터
	- Constant Int: Value = 1.0
	- Get Prim Path : Relationships >Add Targets > World >ur5e_with_2f85 > flange> Camera 	
	- Camera Helper: topicName = rgb, type=rgb
	- Constant string: Value = UR5e

- "Play" 버튼을 누르면 stage에 새로운 viewport가 생성되고 새롭게 생성한 카메라 관점으로 화면이 보일 것임.
![enter image description here](https://user-images.githubusercontent.com/96465330/201472635-23812838-a841-4ce6-a77b-a6c8a97a1214.png)

###  Verify ROS connection
- roscore를 실행시켜 ROS 환경 활성화
- Isaac sim window에서 play 버튼을 눌러 action graph를 실행
- 다음 명령어를 터미널 창에 입력해 /rgb topic 메시지를 image_view를 통해 창을 띄움
	```
	rosrun image_view image_view image:=/rgb
	```

![enter image description here](https://user-images.githubusercontent.com/96465330/201053332-1c732b3e-b1f5-4b08-add3-46ad2e4ca056.png)
![enter image description here](https://user-images.githubusercontent.com/96465330/201472834-eeba0e50-8438-4518-bc62-8a318e54365b.png)

- Viewport와 ROS토픽 메시지로 받은 rgb 이미지가 일치하는 것을 확인할 수 있음.
- 앞에서 진행했던 UR5e joint position control과 카메라 view topic publish를 결합하면 다음과 같은 결과를 얻을 수 있음.
![enter image description here](https://user-images.githubusercontent.com/96465330/201474316-1f141b7a-6436-4a64-8fa0-51fde3dbaf83.png)




## 4.9.2. How to manipulate the UR5e
## 4.9.3. April tag recognition using the RGB camera
위의 script가 실행되면 "hello_world"라는 문구를 10번 발행하고 코드를 중지하게 된다. 다음 결과물은 다음 명령어를 terminal 창에 입력하면 확인할 수 있다.
	``` rostopic echo /hello_topic```

![enter image description here](https://user-images.githubusercontent.com/96465330/201336539-c5d9a4b0-66a5-4b19-b004-8bfb06d27921.png)

#### Publishing Custom Messages in Extension Scripting
<!--stackedit_data:
eyJoaXN0b3J5IjpbMzU3OTk5MDAxLDk5MjAxODkxXX0=
-->