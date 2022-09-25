- [Import URDF](#import-urdf)
  * [URDF preparation](#urdf-preparation)
  * [Importing URDF through Isaac sim URDF Importer](#importing-urdf-through-isaac-sim-urdf-importer)
- [Do the test motion of the UR5e with Omnigraph](#do-the-test-motion-of-the-ur5e-with-omnigraph)
  * [Build the graph](#build-the-graph)
- [Add sensors: StereoCam, 2D/3D Lidar, etc](#add-sensors--stereocam--2d-3d-lidar--etc)
  * [Add camera and attach to robot](#add-camera-and-attach-to-robot)
  * [Add LiDAR](#add-lidar)
  * [Add IMU](#add-imu)
  * [Add ultrasonic sensor](#add-ultrasonic-sensor)
- [Transform tree for tracking pose of the attached sensors](#transform-tree-for-tracking-pose-of-the-attached-sensors)
  * [TF Tree Publisher](#tf-tree-publisher)
- [Teleport service](#teleport-service)
- [Joint Control: Extension Python Scripting](#joint-control--extension-python-scripting)
- [MoveIt Motion Planning Framework](#moveit-motion-planning-framework)
- [Custom Message](#custom-message)
    + [Standalone Python](#standalone-python)
    + [Extension Scripting](#extension-scripting)
    + [Receiving ROS Messages](#receiving-ros-messages)
- [ROS Bridge in Standalone Workflow](#ros-bridge-in-standalone-workflow)
- [April Tags](#april-tags)
- - [April Tags](#april-tags)

# Import URDF

  

## URDF preparation

  

- universal_robot repository의 ur_description 패키지가 없다면 아래 명령어를 통해 받는다.

  

`git clone -b calibration_devel https://github.com/fmauch/universal_robot.git`

  

- 받은 패키지를 컴파일한다.

  

`catkin_make` or `catkin build`

  

- 다음의 명령어를 통해 `universal_robot/ur_description/urdf` 에 위치한 UR5e.xacro 파일을 `.xacro` 에서 `.urdf` 로 변환한다.

  

`rosrun xacro xacro -o ur5e.urdf ur5e.xacro`

  

## Importing URDF through Isaac sim URDF Importer

  

1. viewport 아래의 _Content_ 탭에서 _Isaac/Environments/Simple_Room/simple_room.usd_ 파일의 위치로 들어간다.

  

2. _simple_room.usd_ 파일을 Viewport 탭으로 드래그&드롭 하고, Transform Property의 _Translate_ component를 모두 0으로 채워넣어 origin의 영점을 맞춘다.

  

3. 상단의 탭에서 _Isaac Utils > Workflows > URDF Importer_ 를 실행한다.

  

4. 실행된 창의 _Import Option_ 에서, _clean stage_ 는 체크 해제, _Fix Base Link_ 는 체크하고 , _Joint Drive Type_ 을 Position으로 설정한다.

  

5. 위에서 만들었던 URDF 파일을 _Import_ 의 _Input File_ 로 지정하고 Import 버튼이 활성화 되었음을 확인한다.

  

6. Stage 탭의 _/World_ 를 선택한 상태에서 _Import_ 버튼을 누른다.

  

7. 로봇이 원점인 테이블 위 가운데로 spawn 되었을 것이다. Transform Property를 적절히 변경하여 원하는 Pose로 변경한다.

  

  

# Do the test motion of the UR5e with Omnigraph

  

## Build the graph

  

  

1. 상단 탭에서 _Window > Visual Scripting > Action Graph_ 를 실행하고 아래쪽 section에 Action Graph 탭이 생성된 것을 확인한다.

  

  

2. Action Graph window에서 _New Action Graph_ 아이콘을 클릭하면, Action Graph window의 좌측에 OG(omnigraph) 노드들이 리스트되어 있으며, 검색 창에 원하는 노드를 검색하거나 Isaac Ros 카테고리에서 직접 ROS관련 노드들을 찾아볼 수 있다.

  

  

3. 아래의 노드들을 찾아 오른쪽 graph window 공간으로 드래그&드롭 한다.

  

- Isaac Read Simulation Time

  

- On Playback Tick

  

- ROS1 Publish Joint State

  

- ROS1 Subscribe Joint State

  

- Articulation Controller

  

  

4. 아래 그림과 같이 노드의 point를 클릭 후 드래그하여 노드들을 연결한다.

  

![ROS1 Joint State Action Graph](https://github.com/IROL-SSU/isaac_sim_tutorial/blob/main/pictures/OG_joint.png)

  

5. controller 노드와 Publish 노드를 각각 클릭하여 우측의 Property 탭에서 _RelationShips - TargetPrim_ 에 ur5e_robot을 추가하고, controller 노드에서 usepath의 체크를 해제한다.

  

  

6. 터미널 창에서 `roslaunch ur_description load_ur5e.launch` 를 실행하여 URDF를 robot description parameter로 load한다.

  

  

7. Isaac sim으로 돌아와 왼쪽 바의 _Play_ 버튼을 클릭하여 시뮬레이션을 시작한다.

  

  

8. 다른 터미널 창을 열어 `rostopic echo /joint_states` 를 실행하고 현재 로봇의 각 Joint angular position 정보들이 출력됨을 확인한다.

  

9.  `rosrun joint_state_publisher_gui joint_state_publisher_gui joint_states:=joint_command` 를 실행하여 각 joint들의 각도를 수동으로 변경하면 Isaac sim에서 움직이는 모습을 확인한다.

  

# Add sensors: StereoCam, 2D/3D Lidar, etc

  

## Add camera and attach to robot

  

1. 상단바의 _Create > Camera_ 를 클릭하여 카메라를 추가한다

  

2.  *ur5e_robot* prim을 expand하여 tool0 prim이 표시됨을 확인하고 위에서 추가한 카메라를 prim 위로 드래그&드롭하여 tool0 하위에 Camera가 위치하도록 한다.

3. Camera의 property중 Translate는 모두 0으로, orient의 x와 z 값은 180으로 조절한다.

4. Action Graph를 아래 그림과 같이 설정한다.

![ROS camera RGB](https://github.com/IROL-SSU/isaac_sim_tutorial/blob/main/pictures/OG_cam.png)

  



|Node|Input Field|Value|
|--|--|--|
|Constant Int|Value|1|
|Get Prim Path|inputs:Prim|/World/ur5e_robot/tool0/Camera|
|ROS1 Camera Helper|type|rgb|
||topicName|rbg|
|Constant String|Value|cam|

  
  

5. rqt, rviz를 통해 데이터가 수신됨을 확인한다.

  

## Add LiDAR

  

1. 상단바의 _Create -> Isaac -> Sensors -> Lidar -> Rotating_ 를 클릭하여 LiDAR를 추가한다.

2. 카메라와 같은 방법으로 LiDAR를 Prim의 하위로 이동시켜 원하는 로봇의 링크에 부착하고 *property*의 *Transform*을 이용해 원하는 Pose로 설정한다.

  

3. LiDAR의 *property*에서 _RawUSDProperties_ 를 확장시켜 drawLines와 drawPoints를 체크한다.

  

4. Action Graph를 아래 그림과 같이 설정한다.

![ROS LiDAR](https://github.com/IROL-SSU/isaac_sim_tutorial/blob/main/pictures/OG_LiDAR.png)

  

5.  *Isaac Read Lidar Point Clout Node*와 *Isaac Read Lidar Beams Node* 의 *Property* 에서 *LiDAR prim* 을 앞서 추가한 LiDAR로 지정한다.

  

6. Play 버튼을 눌러 시뮬레이션을 실행한다.

  

7. Rviz를 통해 LiDAR의 2D laserscan 데이터가 수신됨을 확인한다.

  

8. Stop을 눌러 시뮬레이션을 정지시킨다.

  

9. LiDAR의 *property*에서 _RawUSDProperties_ 를 확장시켜 *highLod*를 체크해 vertical channel을 추가한다.

  

10. Rviz를 통해 LiDAR의 3D pointcloud 데이터가 수신됨을 확인한다.

  

## Add IMU

  

1. 상단바의 _Create -> Isaac -> Sensors -> imu Sensor_ 를 클릭하여 IMU를 추가한다.

2. 카메라와 같은 방법으로 IMU를 Prim의 하위로 이동시켜 원하는 로봇의 링크에 부착하고 *property*의 *Transform*을 이용해 원하는 Pose로 설정한다.

  

3. Action Graph를 아래 그림과 같이 설정한다.

![ROS IMU](https://github.com/IROL-SSU/isaac_sim_tutorial/blob/main/pictures/OG_imu.png)

  

4.  `rostopic echo /imu` 또는 rviz로 IMU데이터가 수신됨을 확인한다.

  

## Add ultrasonic sensor

  

- 상단바의 _Create -> Isaac -> Sensors -> ultrasonic_ 를 클릭하여 초음파 센서를 추가할 수 있으며, LiDAR와 같은 방법으로 사용할 수 있다.

  

# Transform tree for tracking pose of the attached sensors

  

## TF Tree Publisher

  

1. Action Graph를 아래 그림과 같이 설정한다.

![ROS TFtree](https://github.com/IROL-SSU/isaac_sim_tutorial/blob/main/pictures/OG_tf.png)

  

2.  *Publish Transform Tree* 노드에서 *Property*의 *TargetPrims*에 ROS tf로 publish하고자 하는 prim인 ur5e, cam, LiDAR, imu등을 선택, 추가한다.

  

3.  *parentPrims*에는 tf tree의 root 역할을 하는 reference frame을 지정한다. 지정하지 않을 경우 defalutPrim인 world 로 사용된다.

  

4. Play 버튼을 눌러 시뮬레이션을 실행한다.

  

5. rviz에서 visualize된 tf를 확인할 수 있으며, rqt를 실행하여 상단의 *plugins -> visualization -> TF tree* 를 load하여 Tf tree 구조 또한 확인해 볼 수 있다.

  

# Teleport service

  

1. 진행중인 stage를 저장하고, 상단바의 _Isaac Examples > ROS > Teleport_를 클릭하여 예제를 불러온다.

2.  _Edit Action Graph_ 아이콘을 클릭하고 _On Playback Tick_ 과 _ROS1 Teleport Service_ 노드만이 있는것을 확인한다.

3.  `rosrun isaac_tutorials ros_service_client.py` 명령어를 터미널에 입력한다. 실행한 python script가 teleport service를 call하여 cube 와 cone 이 다른 위치로 teleport 하는 것을 확인할 수 있다.

  
  

# Joint Control: Extension Python Scripting
***test motion의 내용과 일부 겹침, 재배치 필요.***

# MoveIt Motion Planning Framework

  

*추후 별도의 repository에서 [수정된 package](https://github.com/IROL-SSU/isaac_sim_tutorial/tree/main/isaac_moveit_ur) 공개 및 다운로드 과정 추가 필요*

  

1. 위에서 사용하였던 Action Graph에 아래 그림과 같이 Publish clock을 추가한다.

![ROS clock](https://github.com/IROL-SSU/isaac_sim_tutorial/blob/main/pictures/OG_clock.png)

  

2. Play 버튼을 눌러 시뮬레이션을 시작한다.

  

3. 터미널에 `roslaunch isaac_moveit_ur5e ur_isaac_execution.launch`명령어를 입력하고 실행한다.

  

4. 실행된 rviz에서 원하는 위치로 tool frame을 옮기고 plan을 눌러 경로계획 결과를 확인한다.

  

5. 경로계획 결과가 정상적이면 Execute를 눌러 로봇에 동작 명령을 보낸다.

  

6. isaac sim에서 로봇의 움직임을 확인한다.

  

# Custom Message

- `isaac_tutorials` 패키지에서 정의된 custom message인 `ContactSensor`를 publish 하는 과정을 보일 것이다.
- 해당 메시지의 정의 파일은 `ros_workspace/src/isaac_tutorials/msg/ContactSensor.msg`에 있다.

### Standalone Python
 - `roscore` 실행 후, isaac의 설치 경로에 다음 명령어를 입력하여 예제파일을 실행한다.
	`./python.sh standalone_examples/api/omni.isaac.ros_bridge/contact.py`
	
 - headless mode로 시뮬레이션이 시작되며, contact 센서와 관련된 값들이 터미널로 출력 및 topic으로 publish된다.

### Extension Scripting
1.  아래의 코드를 상단 탭의 `Window -> Script Editor`를 실행하여 입력한다.

		import asyncio
		import rospy
		
		try:
		    rospy.init_node("hello", anonymous=True, disable_signals=True, log_level=rospy.ERROR)
		except rospy.exceptions.ROSException as e:
		    print("Node has already been initialized, do nothing")

		async def my_task():
		    from std_msgs.msg import String
		    pub = rospy.Publisher("/hello_topic", String, queue_size=10)
	    
	    for frame in range(10):
	        pub.publish("hello world " + str(frame))
	        await asyncio.sleep(1.0)
	    pub.unregister()
	    pub = None
	    
		asyncio.ensure_future(my_task())
	
2. 위 코드를 통해, Extension Scripting에 ROS의 standard msg인 `String`을 사용하였다.

3. 터미널에 `rostopic echo /hello_topic` 명령어를 입력하여 실행한다. 

4. 1에서 입력한 코드를 실행하고, 3의 터미널을 통해 publish 되는 데이터를 확인한다.

5. Custom message를 사용할 때는 아래 스니펫을 이용한다.
		
		import sys

		sys.path.append('<PATH_TO_ROS_WORKSPACE>/devel/lib/python3/dist-packages/isaac_tutorials/msg')

		from _ContactSensor import ContactSensor

		contact_msg = ContactSensor()

### Receiving ROS Messages
1. `roscore` 실행 후, 다음 명령어를 통해 subscribe 예제파일을 실행한다.
`./python.sh standalone_examples/api/omni.isaac.ros_bridge/subscriber.py`

2. 터미널에 아래 명령어를 입력해 빈 message를 publish한다.
`rostopic pub -r 1 /move_cube std_msgs/Empty`

3. publish 될 때마다 cube가 이동하는 것을 확인한다.


# ROS Bridge in Standalone Workflow
 - *ROS1 Publish Clock node*를 Standalone Python으로 정교하게 제어하는 예제:

		import omni.graph.core as og
		# Create a new graph with the path /ActionGraph
		og.Controller.edit(
		    {"graph_path": "/ActionGraph", "evaluator_name": "execution"},
		    {
		        og.Controller.Keys.CREATE_NODES: [
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
		        og.Controller.Keys.SET_VALUES: [
		            # Assigning topic name to clock publisher
		            ("PublishClock.inputs:topicName", "/clock"),
		        ],
		    },
		)

- 다음 line을 원하는 곳에 추가하여 clock publisher를 1회 동작하게 하는 tick을 발생시키는 impulse event를 생성할 수 있다.

	   og.Controller.set(og.Controller.attribute("/ActionGraph/OnImpulseEvent.state:enableImpulse"), True)

- 아래의 샘플 코드 파일은 ROS component로 어떻게 action graph를 만들고, 각각의 component를 다른 주기로 ticking 하는방법을 보여준다.

		./python.sh standalone_examples/api/omni.isaac.ros_bridge/clock.py
		
- 아래의 샘플 코드 파일은 주기적으로 매 n번째 렌더링된 프레임마다 image를 publish하는 방법을 보여준다.

    	./python.sh standalone_examples/api/omni.isaac.ros_bridge/camera_periodic.py

- 아래의 샘플 코드 파일은 비주기적으로 Branch node가 enabled되었을 때, image를 publish하는 방법을 보여준다.

		./python.sh standalone_examples/api/omni.isaac.ros_bridge/camera_manual.py

- 위 image 관련 코드들의 실행 결과의 visualization은 rivz를 아래 명령어를 실행하여 이뤄진다.

		rviz -d ros_workspace/src/isaac_tutorials/rviz/camera_manual.rviz

***내용 추가예정***

# April Tags

1. 상단의 탭에서 `Isaac Examples -> ROS -> April Tag`을 실행한다.

2. *Play* 버튼을 눌러 데이터를 publish 한다. 

3. 아래 명령어를 터미널에 입력하여 april_tag의 detection 노드를 실행한다.
`roslaunch  isaac_tutorials  apriltag_continuous_detection.launch`

4. 새로운 터미널에 아래 명령어를 입력하여 rviz를 실행한다.
`rviz  -d  ros_workspace/src/isaac_tutorials/rviz/apriltag_config.rviz`

5. `rostopic  echo  tag_detections` 명령어를 실행하여 detection 결과를 확인한다.
<!--stackedit_data:
eyJoaXN0b3J5IjpbNjQzMTc5NTAwXX0=
-->