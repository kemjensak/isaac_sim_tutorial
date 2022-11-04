
# ROS 기본
## 1. Ubuntu 20.04 LTS 설치
## 2. Nvidia driver install
 - Isaac sim 구동을 위한 요구사항 https://docs.omniverse.nvidia.com/app_isaacsim/app_isaacsim/requirements.html
 - 터미널 실행 단축키는 ctrl + alt + T
 - Ubuntu  기본 명령어
 
	 |명령어|의미|설명|
	|------|---|---|
	|:cd        |change directory|`cd ~` 홈 디렉토리로 이동,  `cd ..`  상위 디렉토리로 이동,`cd /경로`: 특정 경로로 이동|
	|:ls|테스트2|파일&디렉토리 목록 출력|
	|:mkdir|make directory|`mkdir 폴더이름` 디렉토리 생성|
	|:echo|테스트2|`echo <target>` 터미널 창에 출력|
	|:alias|별명|명령어의 별칭 등록|
	|:sudo|superuser do|사용자 권한을 root로|

- 절대경로와 상대경로 `.`은 현재 디렉토리, `..`은 바로 상위 디렉토리를 의미. 
- 현재 위치(`/catkin_ws/src`)에서 `cd ../catkin_ws/src/ur5e_config`이면, 상위 디렉토리인 `catkin_ws`로 이동한 후, 그 하위 디렉토리인 `/catkin_ws/src/ur5e_config`로 이동.

 - 그래픽카드 및 설치 가능한 드라이버 확인 
		
		$ ubuntu-drivers devices 
		$ sudo apt install nvidia-driver-<model #>

## 3. ROS noetic 버전 설치 [goto ROS wiki](http://wiki.ros.org/noetic/Installation/Ubuntu)

 - `ros-latest.list`에 ROS 저장소를 추가
 - ROS 저장소로부터 패키지를 내려받기 위해 공개키 추가
 
		$ sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
		$ sudo apt install curl # if you haven't already installed curl
	    $ curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -

 - **운영체제에서 사용 가능한 패키지들과 그 버전에 대한 정보를 업데이트**. 설치되어 있는 패키지를 최신으로 업데이트하는 것이 아닌 설치가능한 리스트를 업데이트하는 것
   
	   $ sudo apt update

- **(참고) 운영체제에 apt-get install 명령으로 설치한 패키지들을 최신 버전으로 업그레이드**. apt-get upgrade 명령을 이용하면 apt-get update로 가져온 각 패키지들의 최신 버전에 맞게 업그레이드를 한다.    

		$ sudo apt-get upgrade

### 3-1 설치(desktop-full 버전 설치)

 - **Desktop-Full Install: (Recommended)** : Everything in **Desktop** plus 2D/3D simulators and 2D/3D perception packages
 - Desktop-Full 버전의 용량은 2GB를 초과하기 때문에 상당한 시간이 소요됨.

		$ sudo apt install ros-noetic-desktop-full
	
 - 추가적인 패키지를 설치하기 위한 명령어.
	
		$ sudo apt install ros-noetic-PACKAGENAME
		e.g. $ sudo apt install ros-noetic-slam-gmapping


### 3-2 환경설정
 - ROS는 여러 shell을 실행하며 기능을 테스트하게 됨. 이 때, 터미널 창을 열 때마다 ~/.bashrc에 정의된 ROS의 환경변수들이 자동으로 쉘에 추가되도록 설정하면 편함.  새로운 쉘이 실행될 때마다 bash에 자동적으로 ROS 환경변수가 추가되도록 하기 위해 아래의 명령을 입력한다.

		$ echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc  ## bash shell이 아닌 zsh쉘이면 ~/.zshrc로 변경
		$ source ~/.bashrc

 - 현재 쉘에서만 환경변수를 바꾸고 싶다면 아래 명령 한 줄만 입력
	
	 	$ source /opt/ros/kinetic/setup.bash 

### 3-3 패키지 빌드에 필요한 의존성

 - 패키지 빌드에 필요한 의존성 설치: ROS 패키지를 빌드하기 위해 다른 의존성 패키지를 설치한다.

		$ sudo apt install python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential
	
 - rosdep 초기화: ROS 를 사용하기 전에 반드시 rosdep 을 초기화해야 한다. 
 - rosdep을 사용하면 컴파일하려는 소스에 대한 시스템 종속성을 쉽게 설치할 수 있으며 ROS에서 일부 핵심 구성 요소를 실행하는 데 필요. 
 
	 	$ sudo rosdep init
		$ rosdep update

## 4. 작업공간 생성 및 빌드

 - 작업 폴더 생성 및 초기화: ROS 에서 catkin 이라는 ROS 전용 빌드 시스템을 사용하고 있다. 이를 사용하려면 다음처럼 catkin 작업 폴더를 생성하고 초기화해야 한다. 이 설정은 작업 폴더를 새롭게 생성하지 않는 한 처음 한 번만 설정해 주면 된다.
 
	 	$ mkdir -p ~/catkin_ws/src
		$ cd ~/catkin_ws/src
		$ catkin_init_workspace  #작업공간을 만듬

 - 현재 catkin 작업 폴더에는 src 폴더와 그 안에 CMakeList.txt 파일만 있지만 시험삼아 catkin_make 명령어를 이용해 빌드해 보자.
 
		$ cd ~/catkin_ws/
		$ catkin_make #빌드
	
 - 마지막을 catkin 빌드 시스템과 관련된 환경 파일을 불러오자.

		$ source ~/catkin_ws/devel/setup.bash

 - 설치확인: roscore 구동*(굳이 turtlesim 예제를 실행하지 않아도, roscore 명령어가 위 처럼 잘 실행 된다면 ROS Noetic 설치가 완료 되었다고 봐도 될 것이다.)


## 5. ROS 작업환경 설정: bashrc 파일 편집

 - 위 명령어를 실행한 후, 아래의 코드를 bashrc 가장 끝 부분에 복붙.
	```
	$ gedit ~/.bashrc

	# ROS 환경설정 파일
	source /opt/ros/noetic/setup.bash
	source ~/catkin_ws/devel/setup.bash

	# ROS 네트워크 설정 - 노드 간 msg 통신을 위한 설정. 
	# 하나의 PC에서 모든 ROS 패키지를 실행한다면, IP 주소는 필요없고, 아래와 같이 localhost로 설정하면 됨. 
	export ROS_HOSTNAME=localhost
	export ROS_MASTER_URI=http://localhost:11311

	# ROS alias 명령어 설정
	alias cs='cd ~/catkin_ws/src'
	alias cw='cd ~/catkin_ws'
	alias cm='cd ~/catkin_ws && catkin_make'
	alias sb='source ~/.bashrc'
	```

## 6. Terminator, IDE

### 6.1 Terminator
 - ROS를 기반으로 개발/테스트를 하기위해서는, 여러개의 쉘을 동시에 켜고 명령어를 입력/실행해야 한다.  이 기능을 편리하게 할 수 있는 프로그램 중 하나가 **터미네이터(Terminator)**이다.
 
		$ sudo apt-get install terminator

### 6.2 VS Code (https://code.visualstudio.com/)
 - 윈도우: C/C++(Visual Studio), Python(Pycharm) 
 - Ubuntu: VS Code(Visual Studio Code)
 - 통합개발환경(IDE): 코딩, 디버그, 컴파일, 배포 등 모든 작업이 가능
  
	---
	#### 다운로드 및 설치 
	
- 설치를 위해 제목의 링크를 클릭하고, 페이지 좌측 중단의  `.deb` 확장자 설치파일을 클릭하여 다운로드 한다.
	![Screenshot from 2022-11-01 21-45-09](https://user-images.githubusercontent.com/60342784/199235493-bd2af9af-cb50-4984-9e11-24d42f26471c.png)

- 다운로드된 파일을 더블클릭으로 실행하고, `install`을 눌러 설치를 진행한다.
- 설치가 완료되면 아래와 같은 화면이 보인다. ![Screenshot from 2022-11-01 21-50-03](https://user-images.githubusercontent.com/60342784/199236239-d1dd6316-161b-44ac-ad05-d99baae3ddf4.png)
- 아래 명령어를 터미널에 입력하거나, 좌측 하단의 `show application` 아이콘을 눌러 code를 입력하여 VS code를 실행한다.

	  $ code . #실행

- 아래 그림과 같이 좌측의 `Extensions`를 누르면 설치할 수 있는  vscode extension이 표시된다.
![Screenshot from 2022-11-01 21-50-03](https://user-images.githubusercontent.com/60342784/199236790-86c5faf1-61dc-44ff-a38c-e6c37d0f22db.png)
 - 본 교육과정 진행에 필요한 extension인 Python, C/C++, CMake, ROS, XML tools 를 각각 검색하고 `Install`을 눌러 설치한다.
![Screenshot from 2022-11-01 21-50-03](https://user-images.githubusercontent.com/60342784/199237853-02023bca-fa8c-4a22-854e-3be30bb7aa12.png)

	~~- vscode가 실행된 후, 본 교육과정 진행에 필요한 몇 가지 vscode extension들을 설치: Python, C/C++, CMake, ROS, ROS Snippets, XML tools,~~  
 - https://www.youtube.com/watch?v=RXyFSnjMd7M
 - ctrl+ 누른 후, setting 창에 ROS를 검색한 후, ROS distro에 'noetic'을 입력. 
 - https://pinkwink.kr/1190


---
    출처: https://rocknz.tistory.com/entry/ROS-%EB%AA%85%EB%A0%B9%EC%96%B4-%EB%AA%A8%EC%9D%8C
    출처: https://velog.io/@717lumos/ROS-ROS%EC%84%A4%EC%B9%98-Ubuntu18.04-ROS-melodic 
	출처: https://velog.io/@717lumos/ROS-Linux-ROS-%EC%A3%BC%EC%9A%94-%EB%AA%85%EB%A0%B9%EC%96%B4
	출처: https://rocknz.tistory.com/entry/ROS-%EB%AA%85%EB%A0%B9%EC%96%B4-%EB%AA%A8%EC%9D%8C
	출처: https://conceptbug.tistory.com/entry/ROS-Ubuntu-2004-LTS
---


# ROS Gazebo기반 UR5e 가상화 및 모션제어 프레임워크 구현





# DAY3~4: Omniverse issac sim 기반 commercial manipulator

-   *~~UR5e, Gripper 대상: 각각의 URDF 작성, Xacro 통합, Moveit에서 연동 Kinematics 구성 (3hr)~~*
-   *~~Moveit GUI 상에서 Controller 지정한 후 compile해주면 moveit_config(or UR5e_moveit_config 이런식으로 naming) 패키지 생성~~*

-   Moveit_servo는 원래 moveit package folder 내에 존재....

-   Conveyor belt 상의 object target obejct detect & picking/placing 공정 가상화(예제 찾아볼 것) task planning(target pose, goal pose)

-   Moveit pose tracker(출력?) -> Servo_server(여기서 제어 방법을 결정했던가???, trajectory_controller? group_controller) -> Joint twist? -> 1) Isaac sim simulator, 2) Rviz, 3) Real UR5e

-   Moveit 패키지에서 Jacobian matrix -> 공정최적화 프로그래밍(Manipulability measure 가시화, Optimal arm positioning)

- 필요한 패키지 준비

## Joining two URDF(xacro)
### create new package and xacro file and including two other xacro

- 필요한 패키지를 아래 명령어를 통해 받는다. (`catkin_ws/src`에서 입력)
	 ```
	$ git clone https://github.com/kemjensak/robotiq_85_description.git
	$ git clone -b melodic-devel-staging https://github.com/ros-industrial/universal_robot.git universal_robot
	 ```
     
 - ROS workspace 내의 src에서 다음 명령어를 입력하여 새로운 패키지를 생성한다.
 
	   $ catkin_create_pkg ur5e_with_2f85_description
	   
 - 아래 명령어를 통해 생성된 패키지 내에 `urdf`와 `launch` 디렉토리를 생성하고, `urdf` 디렉토리로 이동한다.
 
	    $ mkdir ur5e_with_2f85_description/launch ur5e_with_2f85_description/urdf
	    $ cd ur5e_with_2f85_description/urdf
	    
 - 생성된 `urdf` 디렉토리 내에서 아래 명령어를 입력해 `ur5e_with_2f85.xacro`파일을 만든다.

	   $ code ur5e_with_2f85.xacro
 - 열린 VScode에 아래의 코드를 붙여넣는다.

	```xml
		<?xml version="1.0"?>
	  <robot name="ur5e_with_2f85" 
	    xmlns:xacro="http://wiki.ros.org/xacro">

	      <!-- 2f-85 -->
	      <xacro:include filename="$(find robotiq_85_description)/urdf/robotiq_85_gripper.urdf.xacro" />
	      <xacro:robotiq_85_gripper prefix="" parent="flange">
	        <origin xyz="0.0 0.0 0.0" rpy="0 0 0" />
	      </xacro:robotiq_85_gripper>  
	          
	      <!-- ur5e -->
	      <xacro:include filename="$(find ur_description)/urdf/inc/ur5e_macro.xacro" />
	      <!-- <xacro:ur5e_robot prefix="" /> -->
	      <xacro:ur5e_robot prefix="" transmission_hw_interface="hardware_interface/EffortJointInterface" />
	      
	      <link name="world"/>

	      <link name="tcp"/>
	      
	      <joint name="world-base" type="fixed">
	          <parent link="world"/>
	          <child link="base_link"/>
	          <origin xyz="0 0 0" rpy="0 0 1.57" />
	      </joint>

	      <joint name="tool0-tcp" type="fixed">
	          <parent link="tool0"/>
	          <child link="tcp"/>
	          <origin xyz="0 0 0.162" rpy="0 0 0" />
	      </joint>
	      
	      <gazebo>
	        <plugin name="ros_control" filename="libgazebo_ros_control.so">
	          <!--robotNamespace>/</robotNamespace-->
	          <!--robotSimType>gazebo_ros_control/DefaultRobotHWSim</robotSimType-->
	        </plugin>
	      </gazebo>

	      <!-- Configure self collision properties per link -->
	      <gazebo reference="shoulder_link">
	        <selfCollide>true</selfCollide>
	      </gazebo>
	    </torsional>
	</gazebo>



	  </robot>      <gazebo reference="upper_arm_link">
	        <selfCollide>true</selfCollide>
	      </gazebo>
	      <gazebo reference="forearm_link">
	        <selfCollide>true</selfCollide>
	      </gazebo>
	      <gazebo reference="wrist_1_link">
	        <selfCollide>true</selfCollide>
	      </gazebo>
	      <gazebo reference="wrist_3_link">
	        <selfCollide>true</selfCollide>
	      </gazebo>
	      <gazebo reference="wrist_2_link">
	        <selfCollide>true</selfCollide>
	      </gazebo>



	  <gazebo reference="robotiq_85_left_finger_tip_link">
	    <mu1>100</mu1>
	    <mu2>100</mu2>
	    <kp>1000000.0</kp>
	    <kd>10.0</kd>
	    <slip1>0</slip1>
	    <slip2>0</slip2>
	    <minDepth>0.001</minDepth>
	     <torsional>
	        <ode>
	            <coefficient>1000</coefficient>
	            <slip>0</slip>
	        </ode>
	    </torsional>
	</gazebo>
	  <gazebo reference="robotiq_85_right_finger_tip_link">
	    <mu1>100</mu1>
	    <mu2>100</mu2>
	    <kp>1000000.0</kp>
	    <kd>10.0</kd>
	    <slip1>0</slip1>
	    <slip2>0</slip2>
	    <minDepth>0.001</minDepth>
	     <torsional>
	        <ode>
	            <coefficient>1000</coefficient>
	            <slip>0</slip>
	        </ode>
	    </torsional>
	</gazebo>
	  </robot>
	```
 - `Ctrl-S`로 저장 후 닫는다.
 - 위에서 만든 `launch` 디렉토리 내에서 아래 명령어를 입력하여 새로운 `load_ur5e_with_2f85_demo.launch` 파일을 만든다.

       $ code load_ur5e_with_2f85_demo.launch
      
 - 열린 VScode에 아래의 코드를 붙여넣는다.
	```xml
	<?xml version="1.0"?>
	<launch>
	  <param name="robot_description" command="$(find xacro)/xacro '$(find ur5e_with_2f85_description)/urdf/ur5e_with_2f85.xacro'"/>
	  <node pkg="robot_state_publisher" type="robot_state_publisher" name="robot_state_publisher"/>
	  <node pkg="joint_state_publisher_gui" type="joint_state_publisher_gui" name="joint_state_publisher_gui"/>
	  <node name="$(anon rviz)" pkg="rviz" type="rviz" respawn="false" output="screen"/>
	</launch>
	```
- `Ctrl-S`로 저장 후 닫는다.

- `launch` 디렉토리 내에서 아래 명령어를 입력하여 새로운 `load_ur5e_with_2f85.launch` 파일을 만든다.

       $ code load_ur5e_with_2f85.launch
      
 - 열린 VScode에 아래의 코드를 붙여넣는다.
	```xml
	<?xml version="1.0"?>
	<launch>
	  <param name="robot_description" command="$(find xacro)/xacro '$(find ur5e_with_2f85_description)/urdf/ur5e_with_2f85.xacro'"/>
	</launch>
	```
- `Ctrl-S`로 저장 후 닫는다.
- 아래 명령어를 통해 빌드에 필요한 패키지들을 설치한다.
	```
	$ cd ~/catkin_ws
	$ rosdep install --from-paths src --ignore-src -r -y
	 ```
- `catkin_make`또는 `catkin build`후  아래  명령어를 통해 위에서 만든 `load_ur5e_with_2f85_demo.launch` 파일을 실행한다.
	```
	$ source ~/.bashrc
	$ roslaunch ur5e_with_2f85_description load_ur5e_with_2f85_demo.launch 
	```
 - 실행된 `Rviz`의 *Global Options*의 *Fixed Frame*을 `world`로 변경하고, *Add* 버튼을 눌러 *RobotModel*을 추가한다.
 - UR5e manipulator와 말단부에 장착된 2f-85 gripper를 확인할 수 있으며, 함께 실행된 `joint_state_publisher_gui`의 슬라이더를 움직여 각 joint의 값을 변경해 볼 수 있다.

### create movit_config package with moveit setup assistant
- 아래 명령어를 입력하여 `moveit_setup_assistant`를 실행한다.

	  $ roslaunch moveit_setup_assistant setup_assistant.launch
- *Create New MoveIt Configuration Package* 를 클릭하여 새로운 moveit config 패키지를 제작한다.
- *Load a URDF or COLLADA Robot Model*의 *Browse*를 클릭하여 앞서 제작한 `ur5e_with_2f85.xacro` 파일을 불러온 후  Load Files를 클릭한다.
- 우측의 *Viewer* 에서 불러온 모델을 확인한 후 좌측의 *Self-Collisions*를 클릭한다.
- 우측 상단의 *Generate Collision Matrix*를 클릭하고 *linear view*와  *matrix view*를 눌러 생성된 결과를 확인한다.
- 좌측 *Planning Groups*의 *Add Group*을 클릭하여 아래와 같이 입력한다.
![edit_planning_group](https://user-images.githubusercontent.com/60342784/194872835-6fb4dee5-c101-47d8-bb20-8deec8d13386.png)

- *Add Kin. Chain*을 클릭하고 좌측 하단의 *Expand All*을 클릭하여 모든 link가 보이게 한 후, 아래 그림과 같이 *Base Link*를 `base_link`로, *Tip Link*를 `tool0`로 설정한다.
![Screenshot from 2022-10-29 21-54-17](https://user-images.githubusercontent.com/60342784/198832679-1b19689f-b3f6-49d4-89ed-c57e931adc50.png)


- *Save*를 눌러 저장한 후 다시 *Add Group*을 눌러 아래와 같이 입력한다.
![Screenshot from 2022-10-10 22-16-49](https://user-images.githubusercontent.com/60342784/194875273-da510f44-b175-4894-b60a-a9a907139166.png)

- *Add Joints*를 눌러 `finger_joint`를 선택한 후 오른쪽 화살표를 눌러 *Selected Joint*로 이동시키고, *Save*를 눌러 저장한다.
![Screenshot from 2022-10-29 21-55-21](https://user-images.githubusercontent.com/60342784/198832712-d24065cf-07c4-4e42-aabc-daf97f6d261e.png)

- 아래 그림과 같이 설정되었음을 확인한다.![Screenshot from 2022-10-29 21-55-57](https://user-images.githubusercontent.com/60342784/198832730-397d044a-ee88-4d2d-9799-7cdba13605f4.png)


- 좌측의 *Robot Poses*를 클릭하여 Robot의 predefined pose를 아래와 같이 추가한다.
- *Add Pose*를 누른 후 *Pose Name*을 `home`으로, *Planning Group*을 `manipulator`로 설정한  후 모든 joint 값이 `0`임을 확인하고 *Save*를 누른다.
![Screenshot from 2022-10-29 21-57-29](https://user-images.githubusercontent.com/60342784/198832810-b4b5dbcc-7ae6-4f04-b8bc-6faf0136df8e.png)

- *Add Pose*를 누른 후 *Pose Name*을 `up`으로, *Planning Group*을 `manipulator`로 설정한  후, `shoulder_lift_joint` 값을 `-1.5708`로 변경하고, 나머지 모든 joint의 값이 `0`임을 확인하고 *Save*를 누른다.
![Screenshot from 2022-10-29 21-57-54](https://user-images.githubusercontent.com/60342784/198832869-ef0f60f6-f9e9-49c3-9f98-c8cd8db6170f.png)

- *Add Pose*를 누른 후 *Pose Name*을 `open`으로, *Planning Group*을 `gripper`로 설정한  후, `finger_joint` 값이 `0`임을 확인하고 *Save*를 누른다.
![Screenshot from 2022-10-29 21-58-30](https://user-images.githubusercontent.com/60342784/198832912-e6119f65-3f2c-46c6-a7af-65ee5a6c6da7.png)

- *Add Pose*를 누른 후 *Pose Name*을 `close`로, *Planning Group*을 `gripper`로 설정한  후, `finger_joint` 값이 `0.8`임을 확인하고 *Save*를 누른다.
![Screenshot from 2022-10-29 21-58-49](https://user-images.githubusercontent.com/60342784/198832919-fe638550-f979-432b-9f3e-a8fb3e24164c.png)


- 총 4개의 *Pose*가 아래와 같이 추가되었음을 확인한다.
![Screenshot from 2022-10-10 22-38-10](https://user-images.githubusercontent.com/60342784/194879285-8395c47d-8f7c-4692-92a1-748e48b17d26.png)

- 좌측의 *End Effectors - Add End Effector*를 클릭하여 아래 그림과 같이 입력하고 *Save*를 눌러 저장한다.
![Screenshot from 2022-10-10 22-42-20](https://user-images.githubusercontent.com/60342784/194880048-48179c32-5361-41cd-aa00-9349f45e64e2.png)

- 좌측의 *Passive Joints*를 클릭하여 아래 그림과 같이 gripper의 나머지 조인트를 모두 Passive joint로 설정한다.
![Screenshot from 2022-10-29 21-59-34](https://user-images.githubusercontent.com/60342784/198832951-9470409f-d3e8-47e9-aad8-4deb30dc21a6.png)


- 좌측의 *Controllers - Auto Add FollowJointsTraj*를 클릭하여 생성된 Controller의 이름과 Controller Type을 아래 그림과 같이 변경한다.![Screenshot from 2022-10-29 22-00-20](https://user-images.githubusercontent.com/60342784/198832987-fdfa60af-af62-4ef8-9995-22fd64b36621.png)


- 좌측의 *Author Information* 을 클릭하여 작성자의 정보를 입력한다.
- 좌측의 *Configulation Files*로 이동하여 *Configuration Package Save Path*를 ros workspace로 지정하고, 경로의 맨 뒤에 `/ur5e_with_2f85_moveit_config`를 붙여준다.
![Screenshot from 2022-10-10 23-18-20](https://user-images.githubusercontent.com/60342784/194887782-c3da84db-e5e2-4197-9bc1-d8afa7a5d10c.png)
-  *Generate Package*를 클릭하여 `ur5e_with_2f85_moveit_config` 패키지를 생성한다.
- `catkin_make`또는 `catkin build`후 위에서 만든 패키지의 `demo.launch` 파일을 아래의 명령어로 실행하여 `rviz`의 `MoveIT` plugin을 통해 manipulator를 제어해 본다.
	```
	$ source ~/.bashrc
	$ roslaunch ur5e_with_2f85_moveit_config demo.launch
	```
- `ros_control`을 통한 제어를 위해, `ur5e_with_2f85_moveit_config/config/ros_controllers.yaml`에 아래의 내용을 붙여넣는다.

	```yaml
	# Simulation settings for using moveit_sim_controllers
	moveit_sim_hw_interface:
	  joint_model_group: manipulator
	  joint_model_group_pose: home
	# Settings for ros_control_boilerplate control loop
	generic_hw_control_loop:
	  loop_hz: 300
	  cycle_time_error_threshold: 0.01
	# Settings for ros_control hardware interface
	hardware_interface:
	  joints:
	    - shoulder_pan_joint
	    - shoulder_lift_joint
	    - elbow_joint
	    - wrist_1_joint
	    - wrist_2_joint
	    - wrist_3_joint
	    - robotiq_85_left_knuckle_joint
	  sim_control_mode: 1  # 0: position, 1: velocity
	# Publish all joint states
	# Creates the /joint_states topic necessary in ROS
	joint_state_controller:
	  type: joint_state_controller/JointStateController
	  publish_rate: 50
	controller_list:
	- name: "pos_joint_traj_controller"
	  action_ns: follow_joint_trajectory
	  type: FollowJointTrajectory
	  joints:
	    - shoulder_pan_joint
	    - shoulder_lift_joint
	    - elbow_joint
	    - wrist_1_joint
	    - wrist_2_joint
	    - wrist_3_joint
	- name: "gripper_controller"
	  action_ns: follow_joint_trajectory
	  type: FollowJointTrajectory
	  joints:
	    - robotiq_85_left_knuckle_joint
	```


## Preparing Gazebo
- ROS workspace 내의 src에서 다음 명령어를 입력하여 새로운 패키지를 생성한다.
 
	   $ catkin_create_pkg ur5e_with_2f85_gazebo
	   
- `config`와 `launch`, `urdf` 디렉토리를 만들고, 

 	   $ cd ur5e_with_2f85_gazebo
 	   $ mkdir config launch urdf
 	  
- VS code를 이용해 `config` 디렉토리 내에 `ur5e_with_2f85_controllers.yaml` 파일을 만들고, 

      $ code config/ur5e_with_2f85_controllers.yaml
    
- 내용은 아래와 같이 입력한다.
	```yaml
	joint_state_controller:
	  type: joint_state_controller/JointStateController
	  publish_rate: &loop_hz 500

	pos_joint_traj_controller:
	  type: effort_controllers/JointTrajectoryController
	  joints:
	    - shoulder_pan_joint
	    - shoulder_lift_joint
	    - elbow_joint
	    - wrist_1_joint
	    - wrist_2_joint
	    - wrist_3_joint
	  gains:
	    shoulder_pan_joint:
	      p: 1000
	      d: 50
	      i: 10
	      i_clamp: 100
	    shoulder_lift_joint:
	      p: 5000
	      d: 30
	      i: 100
	      i_clamp: 400
	    elbow_joint:
	      p: 5000
	      d: 50
	      i: 10
	      i_clamp: 100
	    wrist_1_joint:
	      p: 200
	      d: 20
	      i: 10
	      i_clamp: 100
	    wrist_2_joint:
	      p: 100
	      d: 10
	      i: 0.1
	      i_clamp: 100
	    wrist_3_joint:
	      p: 10
	      d: 1
	      i: 0.1
	      i_clamp: 100
	gripper_controller:
	  type: effort_controllers/JointTrajectoryController
	  joints:
	    - robotiq_85_left_knuckle_joint
	  max_effort: 100
	  gains:
	    robotiq_85_left_knuckle_joint:
	      p: 300
	      d: 1
	      i: 0.1
	      i_clamp: 100
	```
- 저장 후, `launch` 디렉토리 내에 `bringup.launch` 파일을 만들고, 

      $ code launch/bringup.launch
    
- 내용은 아래와 같이 입력한다.
	```xml
		<?xml version="1.0"?>
	<launch>

	  <!-- Controller configuration -->
	  <arg name="controller_config_file" default="$(find ur5e_with_2f85_gazebo)/config/ur5e_with_2f85_controllers.yaml" doc="Config file used for defining the ROS-Control controllers."/>
	  <arg name="controllers" default="joint_state_controller pos_joint_traj_controller gripper_controller" doc="Controllers that are activated by default."/>
	  <arg name="stopped_controllers" default="joint_group_pos_controller" doc="Controllers that are initally loaded, but not started."/>

	  <!-- robot_state_publisher configuration -->
	  <arg name="tf_prefix" default="" doc="tf_prefix used for the robot."/>
	  <arg name="tf_pub_rate" default="500" doc="Rate at which robot_state_publisher should publish transforms."/>

	  <!-- Gazebo parameters -->
	  <arg name="paused" default="false" doc="Starts Gazebo in paused mode" />
	  <arg name="gui" default="true" doc="Starts Gazebo gui" />

	  <!-- Load urdf on the parameter server -->
	  <include file="$(find ur5e_with_2f85_description)/launch/load_ur5e_with_2f85.launch"/>

	  <!-- Robot state publisher -->
	  <node pkg="robot_state_publisher" type="robot_state_publisher" name="robot_state_publisher">
	    <param name="publish_frequency" type="double" value="$(arg tf_pub_rate)" />
	    <param name="tf_prefix" value="$(arg tf_prefix)" />
	  </node>

	  <!-- Start the 'driver' (ie: Gazebo in this case) -->
	  <include file="$(find ur5e_with_2f85_gazebo)/launch/ur5e_with_2f85_control.launch">
	    <arg name="controller_config_file" value="$(arg controller_config_file)"/>
	    <arg name="controllers" value="$(arg controllers)"/>
	    <arg name="gui" value="$(arg gui)"/>
	    <arg name="paused" value="$(arg paused)"/>
	    <arg name="stopped_controllers" value="$(arg stopped_controllers)"/>
	  </include>
	</launch>

	```

- 저장 후, 동일한 `launch` 디렉토리 내에 `ur5e_with_2f85_control.launch` 파일을 만들고, 

      $ code launch/ur5e_with_2f85_control.launch
    
- 내용은 아래와 같이 입력한다.
	```xml
		<?xml version="1.0"?>
	<launch>
	  <!-- Parameters we share with ur_robot_driver -->
	  <arg name="controller_config_file" doc="Config file used for defining the ROS-Control controllers."/>
	  <arg name="controllers" default="joint_state_controller pos_joint_traj_controller"/>
	  <arg name="stopped_controllers" default="joint_group_pos_controller"/>

	  <!-- Gazebo parameters -->
	  <arg name="gazebo_model_name" default="robot" doc="The name to give to the model in Gazebo (after spawning it)." />
	  <arg name="gazebo_world" default="worlds/empty.world" doc="The '.world' file to load in Gazebo." />
	  <arg name="gui" default="true" doc="If true, Gazebo UI is started. If false, only start Gazebo server." />
	  <arg name="paused" default="false" doc="If true, start Gazebo in paused mode. If false, start simulation as soon as Gazebo has loaded." />
	  <arg name="robot_description_param_name" default="robot_description" doc="Name of the parameter which contains the robot description (ie: URDF) which should be spawned into Gazebo." />
	  <arg name="spawn_z" default="0.1" doc="At which height the model should be spawned. NOTE: lower values will cause the robot to collide with the ground plane." />
	  <arg name="start_gazebo" default="true" doc="If true, Gazebo will be started. If false, Gazebo will be assumed to have been started elsewhere." />

	  <!-- Load controller settings -->
	  <rosparam file="$(arg controller_config_file)" command="load"/>

	  <!-- Start Gazebo and load the empty world if requested to do so -->
	  <include file="$(find gazebo_ros)/launch/empty_world.launch" if="$(arg start_gazebo)">
	    <arg name="world_name" value="$(arg gazebo_world)"/>
	    <arg name="paused" value="$(arg paused)"/>
	    <arg name="gui" value="$(arg gui)"/>
	  </include>

	  <!-- Spawn the model loaded earlier in the simulation just started -->
	  <node name="spawn_gazebo_model" pkg="gazebo_ros" type="spawn_model"
	    args="
	      -urdf
	      -param $(arg robot_description_param_name)
	      -model $(arg gazebo_model_name)
	      -z $(arg spawn_z)"
	    output="screen" respawn="false" />

	  <!-- Load and start the controllers listed in the 'controllers' arg. -->
	  <node name="ros_control_controller_spawner" pkg="controller_manager" type="spawner"
	    args="$(arg controllers)" output="screen" respawn="false" />

	  <!-- Load other controllers, but do not start them -->
	  <node name="ros_control_stopped_spawner" pkg="controller_manager" type="spawner"
	    args="--stopped $(arg stopped_controllers)" output="screen" respawn="false" />

	</launch>

	```
- 저장 후, 동일한 `launch` 디렉토리 내에 `pick_and_place.launch` 파일을 만들고, 

      $ code launch/pick_and_place.launch
	
	```xml
	<?xml version="1.0"?>
	<launch>
	  <include file="$(find ur5e_with_2f85_gazebo)/launch/bringup.launch"/>
	  <!-- conveyor_belt -->
	  <node name="spawn_conveyor_belt" pkg="gazebo_ros" type="spawn_model" args="-file $(find ur5e_with_2f85_gazebo)/urdf/conveyor_belt.urdf -urdf -model conveyor_belt -y -0.7" />
	  <!-- bin -->
	  <node name="spawn_bin" pkg="gazebo_ros" type="spawn_model" args="-file $(find ur5e_with_2f85_gazebo)/urdf/bin.urdf -urdf -model bin -y 0.1 -x -0.5 -z 0.05" />
	  <!-- red_box -->
	  <node name="spawn_red_box" pkg="gazebo_ros" type="spawn_model" args="-file $(find ur5e_with_2f85_gazebo)/urdf/red_box.urdf -urdf -model red_box -y -0.3 -x 0.3 -z 0.4" />

	  <include file="$(find ur5e_with_2f85_moveit_config)/launch/move_group.launch"/>


	</launch>
	```
- 저장 후, `urdf` 디렉토리 내에 `bin.urdf` 파일을 만들고, 

     $ code launch/bin.launch
	```xml
	<?xml version='1.0'?>
	<robot name ='bin'>

	<link name="world"/>

	<joint name = "joint_fix" type = "fixed">
	  <parent link="world"/>
	  <child link = "bottom"/>
	</joint>


	<link name ="bottom"> <!--2-->
	  <collision>
	    <origin xyz="0 0 0" rpy="0 0 0"/>
	    <geometry>
	      <box size="0.4 0.4 0.02"/> <!--3-->
	    </geometry>
	  </collision>

	  <visual>
	    <origin xyz="0 0 0" rpy="0 0 0"/>
	    <geometry>
	      <box size="0.4 0.4 0.02"/>
	    </geometry>
	  </visual>

	  <inertial>
	    <origin xyz="0 0 0" rpy="0 0 0" />
	    <mass value="0.05" />
	    <inertia
	      ixx="0.001" ixy="0.0"  ixz="0.0"
	      iyy="0.001" iyz="0.0"
	      izz="0.001" />
	  </inertial>
	</link>

	<joint name = "left_wall" type = "fixed">
	  <parent link="bottom"/>
	  <child link = "left"/>
	</joint>


	<link name = "left">
	  <collision> <!--4-->
	    <origin xyz="-0.2 0 0.05" rpy="0 0 0"/>
	    <geometry>
	      <box size="0.02 0.4 0.2"/>
	    </geometry>
	  </collision>

	  <visual>
	    <origin xyz="-0.2 0 0.05" rpy="0 0 0"/>
	    <geometry>
	      <box size="0.02 0.4 0.2"/>
	    </geometry>
	  </visual>

	  <inertial>
	    <origin xyz="-0.2 0 0.05" rpy="0 0 0" />
	    <mass value="0.05" />
	    <inertia
	      ixx="0.001" ixy="0.0"  ixz="0.0"
	      iyy="0.001" iyz="0.0"
	      izz="0.001" />
	  </inertial>
	</link>

	<joint name = "right_wall" type = "fixed">
	  <parent link="bottom"/>
	  <child link = "right"/>
	</joint>

	<link name="right">
	  <collision>
	    <origin xyz="0.2 0 0.05" rpy="0 0 0"/>
	    <geometry>
	      <box size="0.02 0.4 0.2"/>
	    </geometry>
	  </collision>

	  <visual>
	    <origin xyz="0.2 0 0.05" rpy="0 0 0"/>
	    <geometry>
	      <box size="0.02 0.4 0.2"/>
	    </geometry>
	  </visual>

	  <inertial>
	    <origin xyz="0.2 0 0.05" rpy="0 0 0" />
	    <mass value="0.05" />
	    <inertia
	      ixx="0.001" ixy="0.0"  ixz="0.0"
	      iyy="0.001" iyz="0.0"
	      izz="0.001" />
	  </inertial>
	</link>

	<joint name = "back_wall" type = "fixed">
	  <parent link="bottom"/>
	  <child link = "back"/>
	</joint>

	<link name="back">
	  <collision>
	    <origin xyz="0 0.2 0.05" rpy="0 0 0"/>
	    <geometry>
	      <box size="0.4 0.02 0.2"/>
	    </geometry>
	  </collision>

	  <visual>
	    <origin xyz="0 0.2 0.05" rpy="0 0 0"/>
	    <geometry>
	      <box size="0.4 0.02 0.2"/>
	    </geometry>
	  </visual>

	  <inertial>
	    <origin xyz="0 0.2 0.05" rpy="0 0 0" />
	    <mass value="0.05" />
	    <inertia
	      ixx="0.001" ixy="0.0"  ixz="0.0"
	      iyy="0.001" iyz="0.0"
	      izz="0.001" />
	  </inertial>
	</link>

	<joint name = "front_wall" type = "fixed">
	  <parent link="bottom"/>
	  <child link = "front"/>
	</joint>

	<link name="front">
	  <collision>
	    <origin xyz="0 -0.2 0.05" rpy= "0 0 0"/>
	    <geometry>
	      <box size="0.4 0.02 0.2"/>
	    </geometry>
	  </collision>

	  <visual>
	    <origin xyz="0 -0.2 0.05" rpy="0 0 0"/>
	    <geometry>
	      <box size="0.4 0.02 0.2"/>
	    </geometry>
	  </visual>

	  <inertial>
	    <origin xyz="0 -0.2 0.05" rpy="0 0 0"/>
	    <mass value="0.05" />
	    <inertia
	      ixx="0.001" ixy="0.0"  ixz="0.0"
	      iyy="0.001" iyz="0.0"
	      izz="0.001" />
	  </inertial>
	</link>

	<gazebo reference="bottom">
	  <mu1>10</mu1>
	  <mu2>10</mu2>
	  <material>Gazebo/Blue</material>
	</gazebo>

	<gazebo reference="right">
	  <mu1>10</mu1>
	  <mu2>10</mu2>
	  <material>Gazebo/Blue</material>
	</gazebo>

	<gazebo reference="left">
	  <mu1>10</mu1>
	  <mu2>10</mu2>
	  <material>Gazebo/Blue</material>
	</gazebo>

	<gazebo reference="back">
	  <mu1>10</mu1>
	  <mu2>10</mu2>
	  <material>Gazebo/Blue</material>
	</gazebo>

	<gazebo reference="front">
	  <mu1>10</mu1>
	  <mu2>10</mu2>
	  <material>Gazebo/Blue</material>
	</gazebo>

	</robot>
	```
- 저장 후, `urdf` 디렉토리 내에 `conveyor_belt.urdf` 파일을 만들고, 

    $ code launch/conveyor_belt.launch
	```xml
	<?xml version="1.0"?>
	<robot name="conveyor_belt">

	<!-- conveyor belt is just a long flat box for objects to slide on it -->

	<!-- world link -->
	<link name="world"/>

	<!-- base_link and its fixed joint -->
	<joint name="joint_fix" type="fixed">
	  <parent link="world"/>
	  <child link="base_link"/>
	</joint>


	<link name="base_link">
	  <collision>
	    <origin xyz="0 0 0.1" rpy="0 0 0"/>
	    <geometry>
	      <box size="5 1 0.2"/>
	    </geometry>
	  </collision>

	  <visual>
	    <origin xyz="0 0 0.1" rpy="0 0 0"/>
	    <geometry>
	      <box size="5 1 0.2"/>
	    </geometry>
	  </visual>

	  <inertial>
	    <origin xyz="0 0 0.1" rpy="0 0 0"/>
	    <mass value="1"/>
	    <inertia
	      ixx="1.0" ixy="0.0" ixz="0.0"
	      iyy="1.0" iyz="0.0"
	      izz="1.0"/>
	  </inertial>
	</link>

	<gazebo reference="base_link">
	  <mu1>0</mu1>
	  <mu2>0</mu2>
	  <material>Gazebo/FlatBlack</material>
	</gazebo>
	</robot>
	```
- 저장 후, `urdf` 디렉토리 내에 `red_box.urdf` 파일을 만들고, 

    $ code launch/red_box.launch
	```xml
	<?xml version="1.0"?>
	<robot name="red_box">

	<link name="base_link">
	  <collision>
	    <origin xyz="0 0 0.02" rpy="0 0 0"/>
	    <geometry>
	      <box size="0.055 0.055 0.03" />
	    </geometry>
	  </collision>

	  <visual>
	    <origin xyz="0 0 0.02" rpy="0 0 0"/>
	    <geometry>
	      <box size="0.055 0.055 0.03" />
	    </geometry>
	  </visual>

	  <inertial>
	    <origin xyz="0 0 0.02" rpy="0 0 0" />
	    <mass value="0.1" />
	    <inertia
	      ixx="0.001" ixy="0.0"  ixz="0.0"
	      iyy="0.001" iyz="0.0"
	      izz="0.001" />
	  </inertial>
	  </link>

	<gazebo reference="base_link">
	  <material>Gazebo/Red</material>
	    <mu1>100</mu1>
	    <mu2>100</mu2>
	    <kp>10000.0</kp>
	    <kd>10000.0</kd>
	    <slip1>0</slip1>
	    <slip2>0</slip2>
	    <minDepth>0.001</minDepth>
	     <torsional>
	        <ode>
	            <coefficient>1000</coefficient>
	            <slip>0</slip>
	        </ode>
	    </torsional>
	</gazebo>
	</robot>
	```
- 저장 후, 아래 명령어를 통해 새로운 `ur5e_with_2f85` 패키지를 만든다.
	```bash
	$ cd ~/catkin_ws/src
	$ catkin_create_pkg ur5e_with_2f85
	```
	


- `catkin_make` 또는 `catkin build`로 패키지들을 빌드한다. 

- 아래의 명령어를 순서대로 입력하여 `Gazebo`와 `MoveIT`을 실행한다.
	```bash
	$ roslaunch ur5e_with_2f85_gazebo ur5e_with_2f85_bringup.launch 
	-> 제작한 로봇을 Gazebo에 spawn. Real Robot의 역할을 함.

	$ roslaunch ur5e_with_2f85_moveit_config move_group.launch
	-> MoveIT 실행, move group 생성

	$ roslaunch ur5e_with_2f85_moveit_config moveit_rviz.launch rviz_config:=true
	-> Rviz를 통한 조작

	$ roslaunch moveit_servo spacenav_cpp.launch
	-> 수정중(Pose_tracker)

	```
## MoveIT! Servoing
```
sequenceDiagram 
	 actor user as User
	 participant A as moveit_servo::PoseTracking tracker
	 participant B as moveit_servo::Servo servo_ 
	 participant C as ServoCalcs::ServoCalcs servo_calcs_  
	 actor D as ur_robot_driver 
	 loop
	 
	 A-->>B:create object
	 B-->>C:create object
	 user->>A: target_pose 
	 D->>A: current_ee_tf
	 D->>C: current_state_(from planning_scene_monitor)
	 A->>C: delta_twist_cmds
	 C->>D: joint_group_velocity_controller/command
	 
	 end
```


### 패키지 준비
- 필요한 패키지들을 아래 주소에서 받아 빌드한다.
    
   https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/ros-planning/moveit/tree/master/moveit_ros/moveit_servo
      
- 다음 명령어를 workspace 디렉토리에서 실행한다.

      $ rosdep install --ignore-src --from-paths src -y

- 받은 `moveit_servo` 패키지의 `CmakeLists.txt` 파일을 열어 line 92에 한 줄을 띄우고 아래의 코드를 추가한다.
	
	```cpp
	add_executable(ur_pose_tracker
	  src/ur_pose_tracker.cpp
	)
	add_dependencies(ur_pose_tracker ${catkin_EXPORTED_TARGETS})
	target_link_libraries(ur_pose_tracker
	  ${catkin_LIBRARIES}
	  ${SERVO_LIB_NAME}
	  pose_tracking
	)
	```		

- 그리고 line 149의 `spacenav_to_twist` 아래 한 줄을 띄우고 `ur_pose_tracker`를 추가한다.

- `moveit_servo`패키지의 src 디렉토리에 `ur_pose_tracker.cpp` 파일을, include 디렉토리에 `ur_pose_tracker.hpp`파일을 만든다.
- 각각의 파일에 아래의 코드를 붙여넣는다.

    *[해당 코드 완성후 추가 필요](https://github.com/IROL-SSU/isaac_sim_tutorial_pkgs/blob/master/moveit_servo/src/ur_pose_tracker.cpp)*
    
- `config` 디렉토리 내의 `ur_simulated_config.yaml`을 복사하고 이름을 `ur_isaac_config.yaml`로 변경한다.
- 파일을 열고 아래의 코드와 같이 설정 파라이터들을 수정한다.

   	 *[해당 코드 완성후 추가 필요](https://github.com/IROL-SSU/isaac_sim_tutorial_pkgs/blob/master/moveit_servo/config/ur_isaac_config.yaml)*


## Gazebo 환경 구축


## ToDo list

- [ ] isaac sim의 ros extension중 joint state publisher를 통한 ur의 joint group pos(vel) controller 구동기능 구현
	- [ ] joint state를 controller로 전송
	- [ ] command의 joint state msg를 변경하여 isaac sim으로
- [ ] `ur_pose_tracker` 코드 완성

## 문제점
![enter image description here](https://github.com/osrf/gazebo_tutorials/raw/master/ros_control/Gazebo_ros_transmission.png)

- Gazebo와 Real robot(ur_robot_driver)에는 `hardware_interface`가 작성되어 있으나, isaac sim에는 없음.

 ### 해결책
1. *~~isaac_sim에 대응하는 `hardware_interface` 작성(제어기능x, 단순 command, feedback(state) forward만)~~*

2. `hardware_interface`가 이미 작성된 gazebo/Real robot과 함께 사용하여 isaac sim은 해당 로봇의 state로만 움직이게 구현


4. ~~*isaac_sim 내 extension으로 해당 기능 구현하는 package 구현*~~
->gazebo plugin 및 self collision 추가 필요

### 시뮬레이션 실행
```bash

$ roslaunch ur_gazebo ur5.launch 
-> Real Robot의 역할로써 동작, 

$ roslaunch ur5_moveit_config ur5_moveit_planning_execution.launch sim:=true
-> ur5e_with_2f85 로 변경

$ roslaunch ur5_moveit_config moveit_rviz.launch config:=true
-> ur5e_with_2f85 로 변경

$ roslaunch moveit_servo spacenav_cpp.launch
-> isaac sim의 sensor data를 받아 계산한 target pose의 publisher로 변경

```

## Reference
- https://roscon.ros.org/2014/wp-content/uploads/2014/07/ros_control_an_overview.pdf
- https://classic.gazebosim.org/tutorials?tut=ros_control&cat=connect_ros
- http://wiki.ros.org/ros_control/Tutorials/Create%20your%20own%20hardware%20interface
- https://github.com/UniversalRobots/Universal_Robots_ROS_Driver/blob/master/ur_robot_driver/src/hardware_interface.cpp
- https://github.com/Toni-SM/semu.robotics.ros_bridge
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTg0NjQ2MDM3NywxMzM3OTczNTg1LC00Nz
A0NzEwLDEwNzExMjQ2MDIsMjMxODMyMjU4LDE0NjA3Njk0NjMs
LTc5MTQwNTEyOSw2ODkzODc4NzIsLTE3MTY4MjQ5MiwyMDIyNj
czNjM0LC04NzcyMzYwNDEsMjA0MDIxMTM1NiwtMTQ0MDY1MjU1
NiwxMzYxMDM1MDk2LDIxMDY1MjYyNzksMjExNzY1MTk5NSwtNz
c4MzY1Mjg1LC0xODY4NjgwMTcxLC0yMDQ0NTg3ODMsLTEyOTkz
MTE0MTVdfQ==
-->
<!--stackedit_data:
eyJoaXN0b3J5IjpbODU5OTg5MjI4LC0xMzEzNjYxOTI2LC0xMD
M1MTI3Mzk1LC02OTgyMjE3NDMsLTY3Mjk3NzI5MSw2MjY4MjU2
MzYsMTA3MTM5OTI2NCwtMTgzODQ3ODY2MCwtOTA5NzcwMDc5LD
czMjMyODc0NiwtMTcwNjg3MzQzLC01MTk3NjczMjcsLTc3NjE1
MDE5MCwtMTA2ODgxMjI5Nyw5MDUzNzUyMTUsMTM5Nzc4MzU2OC
wtNDY0OTI2ODQ1LDEyMjA0MDc5OTgsMTAyOTUxNjE5NCwtMTcz
NDc5ODA4XX0=
-->
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTQwODg3ODM5NCwtMTM0MjMyMjY4Myw4ND
A2NDIxNDMsNTIzMTYzNjIzLDczMTI0Njc4NSwyMjQ3ODE1MjEs
MTQxMTIxNTQ2MCwxMTAyODE2MjQzLC04NDY0NjAzNzcsMTMzNz
k3MzU4NSwtNDcwNDcxMCwxMDcxMTI0NjAyLDIzMTgzMjI1OCwx
NDYwNzY5NDYzLC03OTE0MDUxMjksNjg5Mzg3ODcyLC0xNzE2OD
I0OTIsMjAyMjY3MzYzNCwtODc3MjM2MDQxLDIwNDAyMTEzNTZd
fQ==
-->