


> Written with [StackEdit](https://stackedit.io/).


### 카메라 부착
- franka(Panda)의 end-effector단에 붙이기 위한 camera를 생성 `Create > Camera`
- stage tree에 생성된 Camera를 우클릭 한후 rename을 통해 `sim_camera`로 설정
- sim_camera를 drag하여 franka의 panda_link8의 하위에 둠.
- stage tree의 sim_ camera를 클릭해 property를 다음과 같이 구성
	- orientation을 x: 180, y: 0, z: -45
	- translate을 z: -0
![enter image description here](https://user-images.githubusercontent.com/96465330/202136221-70811208-17b6-49c4-bcfe-0bf55a7b558c.png).![enter image description here](https://user-images.githubusercontent.com/96465330/202136342-933096ab-97a6-4e09-bb1b-02871296fb25.png)
### Action graph 구성

#### 카메라 및 TF 관련 Action graph

- 새로운  Action graph를 생성 ` Create > Visual scripts > Action graph`
- Graph node들을 다음과 같이 구성
![enter image description here](https://user-images.githubusercontent.com/96465330/202136783-00764903-ba0f-43e9-a839-e088ad9db562.png)#### 노드 설명

  - **Get Prim Path** 의 Relationships에 Add Targets를 눌러 아까 franka의 panda_link8 하위에 둔 sim_camera 선택
  - **Isaac Create Viewport**의 viewportId를 1로 설정
  - **ROS1 Camera Helper**의 frameId를 두 노드 모두 sim_camera로 설정하고, topic Name과 type을, 하나는 rgb 나머지 하나는 camera_info로 설정(토픽 주소와 메시지 타입이 서로 이름이 같음).
  - **ROS1 Publish Transform Tree**의 ParentPrim을 panda_link0로 설정하고 targetPrim을 `sim camera`, `/World/Cube`, `panda_link8`로 설정

#### Panda Robot arm joint control 관련 Action Graph
![enter image description here](https://user-images.githubusercontent.com/96465330/202138318-9c8c08b9-cdba-4838-8bea-06372c79bd2d.png)
#### 노드 설명
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
	- Add the _/panda_ robot articulation to the targetPrim --> /frank

### ROS 연동 환경 준비
- Franka(Panda) 로봇의 end-effector와 cartesian coordinate 기준으로 로봇의 goal pose를 생성하고 제어하기 위해 Moveit을 사용
- 먼저 moveit이 설치가 되어 있지 않다면, 다음 명령어를 통해 moveit 설치
`sudo apt install ros-noetic-moveit`
- Franka(panda)에 대한 moveit package를 설치하기 위해 다음과 같은 명령어 입력
	```
	cd ~/catkin_ws/src
	git clone https://github.com/ros-planning/panda_moveit_config.git -b noetic-devel
	```
- 아래 src 폴더에 panda_moveit_config 패키지가 생성된 것을 확인할 수 있음.
![enter image description here](https://user-images.githubusercontent.com/96465330/202140693-0fa175e8-5e13-4810-9fe8-565fa93e8b8b.png)
- 패키지 다운이 완료되면 다음 명령어를 통해 workspace build 실시
	```
	cd ~/catkin_ws
	catkin_make
	```

- moveit 버전이 업그레이드 되면서 package 내부 파라미터 이름이 바뀌어, Isaac_moveit 패키지의 `franka_isaac_execution.launch` 파일 내부를 수정해야 하는데 수정 요소는 다음과 같다.
- `<arg name="fake_execution" value="true">` -> `<arg  name="moveit_controller_manager"  value="fake"/>`
![enter image description here](https://user-images.githubusercontent.com/96465330/202142014-ee76b8f2-987e-44bc-83e2-d59f7238d705.png)
### MoveIt 연동 확인
- Isaac sim과 MoveIt 연동을 확인하기 위해 다음 과정을 실시
	- roscore를 킴
	- Isaac sim "Play"버튼 누름
	- `rostopic echo /joint_states` 와 `rostopic echo /rgb`를 입력해 토픽 메시지가 잘 들어오는지 확인
	- 토픽 메시지가 잘 들어오면 다음 launch 파일을 실행 `roslaunch isaac_moveit franka_isaac_execution.launch`
	![enter image description here](https://user-images.githubusercontent.com/96465330/202143373-fd43e2a9-a848-4cc4-b638-94dcd959e58b.png)- rviz 창이 나타나면, 하단의 Add 버튼을 누르고, MotionPlanning 플러그인을 추가함.
-End-effector에 manipulator의 자세를 움직일 수 있는 원형 UI가 나타나는데 drag를 통해 manipulator의 자세를 변경하고 plan&execution을 눌러 Isaac_sim과 MoveIt이 연동해서 움직이는지 확인
![enter image description here](https://user-images.githubusercontent.com/96465330/202144216-f9e562dd-d0d2-46c3-896c-8b963a169ee9.png)![enter image description here](https://user-images.githubusercontent.com/96465330/202144302-46602aa7-87b6-4649-bc63-4b0b465beef0.png)

<!--stackedit_data:
eyJoaXN0b3J5IjpbLTEyMTM3MTIxMTMsMTIwMTIxNDk4NV19
-->