



# ROS기반 디지털트윈
1. 개발환경 구축
2. ROS 기본이론
3. ROS 기반 제어 프레임워크
4. Gazebo 기반 디지털트윈 구축

![Overview_ROS_based_Digitaltwin](https://user-images.githubusercontent.com/29449124/199985410-a9130e83-aa0b-4f60-8cac-63a4bbe8ba38.jpg)


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

## 6. Terminator, 

### 6.1 Terminator
 - ROS를 기반으로 개발/테스트를 하기위해서는, 여러개의 쉘을 동시에 켜고 명령어를 입력/실행해야 한다.  이 기능을 편리하게 할 수 있는 프로그램 중 하나가 **터미네이터(Terminator)**이다.
 
		$ sudo apt-get install terminator

### 6.2 VS Code (https://code.visualstudio.com/)
 - 윈도우: C/C++(Visual Studio), Python(Pycharm) 
 - Ubuntu: VS Code(Visual Studio Code)
 - 통합개발환경(IDE): 코딩, 디버그, 컴파일, 배포 등 모든 작업이 가능

![](https://blog.kakaocdn.net/dn/T6azV/btqTow6ro5x/fQxTZFACLMY0yOtREEEyBk/img.png)

		$ code . #실행

 - vscode가 실행된 후, 본 교육과정 진행에 필요한 몇 가지 vscode extension들을 설치: Python, C/C++, CMake, ROS, ROS Snippets, XML tools,  
 - https://www.youtube.com/watch?v=RXyFSnjMd7M
 - ctrl+ 누른 후, setting 창에 ROS를 검색한 후, ROS distro에 'noetic'을 입력. 
 - Launching VSCode with a ROS env. and overview of ROS UI elements.
 - ROS commands in the vscode command pallet (ctrl+shift+p)
 - URDF editing

 - ubuntu /usr/bin/env: python: No such file or directory 문제발생 시 -> $ sudo apt install python-is-python3
	```
	$ set ROS  #ROS가 sourced 되었는지 확인
	$ cd ~/catkin_ws
	$ code . 
	```
### VSCode로 ROS 패키지 생성해서, 간단한 subscribe 노드 작성/실행(출처: https://pinkwink.kr/1190) 
- Command pallet에서 ROS:catkin_create_pkg를 실행하면, package name을 입력하라고 함. 
![](https://t1.daumcdn.net/cfile/tistory/991992415C67FEA724)
![](https://t1.daumcdn.net/cfile/tistory/99491F405C67FEA737)

- 그 후 Dependencies 입력하면, package.xml 내에 정의된 패키지들이 보임.
![](https://t1.daumcdn.net/cfile/tistory/99290C375C67FEA72E)
![](https://t1.daumcdn.net/cfile/tistory/998FCC485C67FEA704)

- basic_subsribe 패키지 폴더 내에 scripts 폴더 생성 후 listen_turtlesim.py 노드 코드를 생성한다. 
![](https://t1.daumcdn.net/cfile/tistory/9928F2415C67FEA738)

- vscode 내에서 subscribe 코드를 통해 turtle1/pose 토픽을 받아보자. roscore를 돌리고, turtlesim_node -> turtle_teteop_key를 실행. 
![](https://t1.daumcdn.net/cfile/tistory/995A8A455C67FEA70B)
![](https://t1.daumcdn.net/cfile/tistory/99269A3D5C67FEA734)

- rostopic list를 보면 아래와 같음.
![](https://t1.daumcdn.net/cfile/tistory/99DD073C5C67FEA73B)

- ctrl+shift+b 후에 catkin_make: build를 실행한 후, 터미널에서 아래의 명령어를 실행'
- 이 때, turtle_teleop_key를 실행시킨 터미널에 클릭을 한 후 방향키로 터틀을 이동시켜야 함. (not the turtlesim gui window!)
	 ```
	## 노드 파일이 있는 폴더 위치에서 아래의 명령을 실행하여 실행 권한을 python code에 전달
	$ sudo chmod +x listen_turtlesim.py
	 ```


### VSCode로 python ROS package 작성 및 디버깅 
- http://wiki.ros.org/ROS/Tutorials/WritingPublisherSubscriber%28python%29  
- ROS workspace -> Python properties 설정 -> Debug profile 생성 -> Debug session 시작

![](https://lh3.googleusercontent.com/xNqAqO5h3tYU8oJeUCRhSw6r3FPi6iBnTFh1ciQVRvG-z5dt28VXQxaEM9ISc74JTN1iSMqYaSUsCIS42RrjV_j8HEMBevMuhFlaGbTsNbOuqmXn-go2jeMniBd9v-8h149Szd0mhJVcMP42y0O3ZmdVGMsGmEp4D0tNSvXPFPogPNvxbVplZw191MzUgcLOxrEkiSAMatmg6-iV_QO699WR39nUnBFZax6Rf9SKTJFDJYYZSkCFhR-eoiOR3hRe9XMdPSTyIUU5Vd4PJS8gOhGAv9ueRAXh12jSnkjFLo3ZWEeBzufEM90amKKkHZxoD_zOX6tPj3LhSZhSjYk_IeWEXXJKkX38Wo-k1uw0n0Rhf25lMi0CqX8wumkb867Yi90zo4fVJ15Sv805F-5_5EsXm10E5lEppWYaHvjKgx2nrjxr_u8jRHVwO0H_E0UxmbV9EMkT-lu5UGI18s45Aq9MMpmlKeY1fL7RFL73mPU0LT_l1kc9TiHlphqjuTA9V1-j5uodLB38VDsJe5D1uABDwWUEDlYaNW1X_f6Eyo6FeQKPbQOQVKsn2ZHD8D7vURYLEW4JzgT4IXCMMDuySsgagjkhjOm7jo6XOJ34z9d4z2r6ybEs08fWi1zqRmUOnHQrrn1hXlHVPCkZzb5B-gQ99_07TCRrh9ZrOKfqFpF-m_swJq1uUq4T9DY18WiTY3zVozSZfDquvdVj6dnPKp8ql-2n5eYnXckOu6xWqSSaqwx5Bl1US52vQc4CUQUxOqn5vdzydYXPx55c7McrlOOUZ9ZTFi-5XsE_SO3_2vbfWmOQ9nQKZ7HW7YUjO9tm2JVpmC0jB_nsn4ivhvIJVgHndPD9l2HgT1E4j4Qno3Fq0SX40u05Q5ZTXU4FmvOCyueMYOKaCEPCwTTSNUIyjwLItMTBcpPigWsynuFNk-7YxJRb=w3017-h1691-no?authuser=0)

---
    출처: https://rocknz.tistory.com/entry/ROS-%EB%AA%85%EB%A0%B9%EC%96%B4-%EB%AA%A8%EC%9D%8C
    출처: https://velog.io/@717lumos/ROS-ROS%EC%84%A4%EC%B9%98-Ubuntu18.04-ROS-melodic 
	출처: https://velog.io/@717lumos/ROS-Linux-ROS-%EC%A3%BC%EC%9A%94-%EB%AA%85%EB%A0%B9%EC%96%B4
	출처: https://rocknz.tistory.com/entry/ROS-%EB%AA%85%EB%A0%B9%EC%96%B4-%EB%AA%A8%EC%9D%8C
	출처: https://conceptbug.tistory.com/entry/ROS-Ubuntu-2004-LTS
	출처: https://pinkwink.kr/1188?category=558361
---

## 자주 사용되는 ROS file 
![enter image description here](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/cdd78bce-9dbc-45ae-a76c-be9be9bb8955/Untitled.png?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45/20221104/us-west-2/s3/aws4_request&X-Amz-Date=20221104T062110Z&X-Amz-Expires=86400&X-Amz-Signature=4c1635dc0447d5e96297c9d00e48adb1e3c53dc3fbb66e0097efd5086ebf0e18&X-Amz-SignedHeaders=host&response-content-disposition=filename=%22Untitled.png%22&x-id=GetObject)
- Package.xml: Package에 반드시 포함되어야 하는  파일 중 하나이며, 패키지의 이름, 버전, dependency 등 패키지의 메타정보를 담고 있음. 각 패키지 마다 고유의 Package.xml 파일을 가지고 있음.
- CMakeLists.txt: Package에 반드시 포함되어야 하는 파일 중 하나이며, catkin_make을 하기 위해 참조하는 파일
- *.Yaml: 타 시스템 간에 데이터를 주고받을 때 약속된 포맷으로 정의되어 있는 파일, 보통 ROS parameter를 기재하여 이후 파일을 불러와 parameter server에 load시켜 사용함.
- *.Launch: ROS node를 여러 개 실행시킬  수 있으며, XML 형식에 맞춰 작성한 명령을 통해 package parameter, node name, node namespace등의 다양한 옵션을 변경 및 설정할 수 있음.
- *.URDF/Xacro: 로봇 모델에 대한 정보들을 명세해 놓은 규격서, XML 포맷을 사용함.
- *.SRDF: MoveIT에서 요구하는 파일 형식으로, 로봇 모델의 정보뿐만 아니라, 충돌 요소, end-effector 정보 같은 다양한 정보를 담고 있음. MoveIT setup assistant를 통해 사용자가 가지고 있는 URDF/Xacro 파일에 대한 SRDF 파일을 제작할 수 있음.
- *.so: 공유 라이브러리 파일을 뜻하며, gazebo 플러그인을 불러올 때 관련 so file을 참조하여 사용함. 

# ROS Gazebo기반 UR5e 가상화 및 모션제어 프레임워크 구현
![enter image description here](https://user-images.githubusercontent.com/96465330/199203335-35c09777-bd1f-4aae-9f2f-4cb9ff6f4fea.jpg)

## Xacro file 구성
- manipulation.xacro
```
	<?xml version="1.0"?>
	<robot xmlns:xacro="http://ros.org/wiki/xacro" name="ur_assm"> 
		<link name="world"/> 
		<link name="ur_base_link"> 
			<inertial> 
				<origin xyz="0.0 0.0 0.0" rpy="0 0 0" /> 
				<mass value="4.0" /> 
				<inertia 
				ixx="0.0016928" 
				ixy="-8.6453E-08" 
				ixz="5.6638E-07" 
				iyy="0.0012526" 
				iyz="2.3853E-05" 
				izz="0.0020813" /> 
				</inertial> 
				<visual> 
					<origin xyz="0 0 0" rpy="0 0 0" /> 
					<geometry> 
						<mesh filename="package://ur_assm/meshes/ur_base_link.STL" />
					</geometry> 
					<material name=""> 
						<color rgba="0.79216 0.81961 0.93333 1" /> 
					</material> 
				</visual> 
										'
										'
										'
</robot>
 ```
- gripper.xacro
```
<?xml version="1.0"?> 

<robot name="robotiq_85_gripper" xmlns:xacro="http://ros.org/wiki/xacro"> 		
	<xacro:property name="pi" value="3.14159"/> 
	<xacro:macro name="robotiq_85_gripper" params="prefix parent *origin"> 
	<xacro:include filename="$(find robotiq_85_description)/urdf/robotiq_85_gripper.transmission.xacro" /> 
	<xacro:robotiq_85_gripper_transmission prefix="${prefix}" /> 
	<joint name="${prefix}robotiq_85_base_joint" type="fixed"> 
	<parent link="${parent}"/> 
	<child link="${prefix}robotiq_85_base_link"/> 
	<xacro:insert_block name="origin"/> 
	</joint>
										'
										'
										'
</robot>
 ```
https://github.com/roboticsgroup/roboticsgroup_upatras_gazebo_plugins
gripper joint는 각 joint마다 hardware interface가 존재하지 않고 mimic 기능을 통해 하나의 joint로 연결되어 움직인다.  하지만 gazebo 기본 플러그인에 mimic 플러그인이 존재하지 않는 경우가 있어 관련 패키지를 첨부함.

- 전체.xacro
```
<?xml version="1.0"?>
<robot xmlns:xacro="http://ros.org/wiki/xacro" name="ur_with_gripper"> 
	<xacro:include filename="$(find ur_assm)/urdf/ur_assm.xacro"/> 
	<xacro:include filename="$(find robotiq_85_description)/urdf/robotiq_85_gripper.urdf.xacro"/> 
	<xacro:include filename="$(find ur_assm)/urdf/ur_assm_transmission.xacro"/> 
	<xacro:robotiq_85_gripper prefix="" parent="tool0"> 
		<origin xyz="0 0 0" rpy="1.57 -1.57 0"/> 
	</xacro:robotiq_85_gripper> 
	<gazebo> 
		<plugin name="gazebo_ros_control" filename="libgazebo_ros_control.so"/> 
	</gazebo> 
</robot>
 ```


## Gazebo simulation을 위한 xacro file 수정 사항

- 아래의 tag를 xacro 파일의 상단에 추가

```
	<gazebo>
		<plugin name="gazebo_ros_control" filename="libgazebo_ros_control.so"/>
	</gazebo>
 ```

- Gazebo 시뮬레이션을 위한 xacro 파일 tag 설정: 상기 gazebo simulation framework 상의 xacro 코드 상단에 위 <gazebo> ... </gazebo> 태크를 추가해야 gazebo 환경에서 simulation이 가능함. 
- 플러그인의 이름이 gazebo_ros_control이고, 해당 파일명이 libgazebo_ros_control.so임을 의미.
- 여기서, gazebo 플러그인 중 gazebo_ros_control 플러그인의 역활은 xacro(urdf 상의 transmission tag)에 정의되어 있는 hardware interface 정보를 ROS의 parameter server로부터 로드하여 gazebo(또는 real robot system)상의 해당 hardware interface와 대응되는 hardware에서 요구하는 command format으로 변환/전달해주는 역활을 하는 interface plug-in임. 

## Transmission tag in xacro file

- 다음은 manipulator에 대한 transmission.xacro 파일. 
- transmission 태그는 actuator와 joint 사이 발생하는 일을 모델링하는 urdf extenstion. 
- 다음 tag가 없으면 joint를 제어할 시, joint 제어 방법인 ros_control 패키지를 사용할 수 없음. 
- transmission 파일에서 각 joint에 상응하는 'hardware interface'를 정의할 수 있는데, hardware interface는 ROS control command를 hardware-specific command로 변환하는 역할을 한다.

```
	<?xml version="1.0"?>

	<robot xmlns:xacro="http://ros.org/wiki/xacro" name="ur_assm_transmission">
    <transmission name="Joint_1_trans">
        <type>transmission_interface/SimpleTransmission</type>
        <joint name="joint_1">
		            <hardwareInterface>hardware_interface/PositionJointInterface</hardwareInterface>
		        </joint>
		        <actuator name="motor1">
		            <hardwareInterface>hardware_interface/PositionJointInterface</hardwareInterface>
		            <mechanicalReduction>1.0</mechanicalReduction>
		        </actuator>
		    </transmission>

		    <transmission name="Joint_2_trans">
		        <type>transmission_interface/SimpleTransmission</type>
		        <joint name="joint_2">
		            <hardwareInterface>hardware_interface/PositionJointInterface</hardwareInterface>
		        </joint>
		        <actuator name="motor2">
		            <hardwareInterface>hardware_interface/PositionJointInterface</hardwareInterface>
		            <mechanicalReduction>1.0</mechanicalReduction>
		        </actuator>
		    </transmission>

		    <transmission name="Joint_3_trans">
		        <type>transmission_interface/SimpleTransmission</type>
		        <joint name="joint_3">
		            <hardwareInterface>hardware_interface/PositionJointInterface</hardwareInterface>
		        </joint>
		        <actuator name="motor3">
		            <hardwareInterface>hardware_interface/PositionJointInterface</hardwareInterface>
		            <mechanicalReduction>1.0</mechanicalReduction>
		        </actuator>
		    </transmission>

		    <transmission name="Joint_4_trans">
		        <type>transmission_interface/SimpleTransmission</type>
		        <joint name="joint_4">
		            <hardwareInterface>hardware_interface/PositionJointInterface</hardwareInterface>
		        </joint>
		        <actuator name="motor4">
		            <hardwareInterface>hardware_interface/PositionJointInterface</hardwareInterface>
		            <mechanicalReduction>1.0</mechanicalReduction>
		        </actuator>
		    </transmission>

		    <transmission name="Joint_5_trans">
		        <type>transmission_interface/SimpleTransmission</type>
		        <joint name="joint_5">
		            <hardwareInterface>hardware_interface/PositionJointInterface</hardwareInterface>
		        </joint>
		        <actuator name="motor5">
		            <hardwareInterface>hardware_interface/PositionJointInterface</hardwareInterface>
		            <mechanicalReduction>1.0</mechanicalReduction>
		        </actuator>
		    </transmission>

		    <transmission name="Joint_6_trans">
		        <type>transmission_interface/SimpleTransmission</type>
		        <joint name="joint_6">
		            <hardwareInterface>hardware_interface/PositionJointInterface</hardwareInterface>
		        </joint>
		        <actuator name="motor6">
		            <hardwareInterface>hardware_interface/PositionJointInterface</hardwareInterface>
		            <mechanicalReduction>1.0</mechanicalReduction>
		        </actuator>
		    </transmission>
	</robot>
```

## Controller.yaml

- 앞서 정의된 joint transmission definition 기반의 joint control을 위해, ros_control 패키지를 사용함.
- ros_control에서는 다음과 같은 종류의 제어방법을 제공함. (refer to: http://wiki.ros.org/ros_control)
	- joint state controller
	- joint group controller
	- joint trajectory controller.

- 사용 가능한 여러 control method 중, Position_controller/JointTrajectoryControlller를 사용하는 경우, 아래와 같이 yaml 파일에 그 specification을 정의되며, 이는 launch 파일 실행 시 parameter_server에 그 정보가 등록되어, **controller_manager**에서 handling됨. 
- (참고) 일반적으로 가장 상단에는 joint state feedback을 위한 JointStateController를 정의하고, 그 다음에 joint controller를 정의함. 

```
    // publish all joint states
	joint_state_controller:
	  type: joint_state_controller/JointStateController
	  publish_rate: 50

	//manipulator controller
	pos_joint_traj_controller:
	  type: position_controllers/JointTrajectoryController
	  joints:
	    - joint_1
	    - joint_2
	    - joint_3
	    - joint_4
	    - joint_5
	    - joint_6
	  constraints:
	    goal_time: 0.6
	    stopped_velocity_tolerance: 0.05
	    joint_1: {trajectory: 0.1, goal: 0.1}
	    joint_2: {trajectory: 0.1, goal: 0.1}
	    joint_3: {trajectory: 0.1, goal: 0.1}
	    joint_4: {trajectory: 0.1, goal: 0.1}
	    joint_5: {trajectory: 0.1, goal: 0.1}
	    joint_6: {trajectory: 0.1, goal: 0.1}
	  stop_trajectory_duration: 0.5
	  state_publish_rate: 500
	  action_monitor_rate: 10
	  
	//gripper controller
	gripper_controller:
	  type: position_controllers/JointTrajectoryController
	  joints:
	    - robotiq_85_left_knuckle_joint
	  constraints:
	    goal_time: 0.6
	    stopped_velocity_tolerance: 0.05
	    robotiq_85_left_knuckle_joint: {trajectory: 0.1, goal: 0.1}
	  stop_trajectory_duration: 0.5
	  state_publish_rate: 500
	  action_monitor_rate: 10
```

## UR5e_....._bringup.launch

```
	<launch>
	    <!-- Load joint controller configuration from YAML file to parameter server -->
	    <rosparam file="$(find 패키지명)/config/controller.yaml" command="load"/>

	    <!-- Load the controllers -->
	    <node name="controller_spawner" pkg="controller_manager" type="spawner" respawn="false"
	        output="screen" args="joint_state_controller pos_joint_traj_controller gripper_controller" />

	    <!-- convert joint states to TF transform for rviz, etc -->
	    <node name="robot_state_publisher" pkg="robot_state_publisher" type="robot_state_publisher"
	        respawn="false" output="screen" >
	        <!-- <remap from="/joint_states" to="/joint_state_controller/joint_states" /> -->
	    </node>

	    <!-- Load the URDF/xacro file into ROS Parameter server -->
	    <param name="robot_description" command="$(find xacro)/xacro '$(find ur_assm)/urdf/ur_with_gripper.urdf.xacro'"/>

	    <!-- Load empty world on gazebo -->
	    <include file="$(find gazebo_ros)/launch/empty_world.launch">
	        <arg name="world_name" value="worlds/empty.world"/>
	        <arg name="paused" value="false"/>
	        <arg name="use_sim_time" value="true"/>
	        <arg name="gui" value="true"/>
	        <arg name="recording" value="false"/>
	        <arg name="debug" value="false"/>
	    </include>
	    
	    <!-- Run a python script to the send a service call to gazebo_ros to spawn a URDF robot -->
	    <node name="spawn_model" pkg="gazebo_ros" type="spawn_model" respawn="false" output="screen" args="-urdf -model robot -param robot_description"/>
	</launch>
```

- 이 command는 위에서 작성한 controller.yaml 파일을  parameter server에 load하는 부분이다.

```
	<rosparam file="$(find ur_assm)/config/controller.yaml" command="load"/>
```

- 그다음 controller manager 노드를 실행시키는 명령으로서 parameter server에 load한 ros_controller를 불러와 controller_manager에 load한다.
```
	<node name="controller_spawner" pkg="controller_manager" type="spawner" respawn="false" output="screen" args="joint_state_controller pos_joint_traj_controller gripper_controller"/>
```

- robot_state_publisher는 robot의 state를 tf 변환을 하고 그에대한 정보를 topic 메시지로 발행하는 역할을 한다.
```
	<node name="robot_state_publisher" pkg="robot_state_publisher" 	type="robot_state_publisher" respawn="false" output="screen" >
	</node>
```

- 다음 명령은 robot 모델이 정의된 xacro 파일에 대한 정보를 parameter server에 load하는 명령이다.
```
	<param name="robot_description" command="$(find xacro)/xacro '$(find ur_assm)/urdf/ur_with_gripper.urdf.xacro'"/>
```

- 정의한 robot 모델 파일과 ros control을 실행시켰으므로 gazebo 환경을 실행시켜야 한다. 따라서 빈 공간 gazebo 환경을 load하기 위해 gazebo_ros 패키지의 empty_world.launch 파일을 불러와 실행시킨다.
```	
	<include file="$(find gazebo_ros)/launch/empty_world.launch"> 
		<arg name="world_name" value="worlds/empty.world"/> 
		<arg name="paused" value="false"/> 
		<arg name="use_sim_time" value="true"/> 
		<arg name="gui" value="true"/> 
		<arg name="recording" value="false"/> 
		<arg name="debug" value="false"/> 
	</include>
```
 
- gazebo 환경이 load되었으면 그 위에 robot을 spawn하기 위해 spawn_model 노드를 실행시킨다. 이때, args 파라미터를 통해 parameter server에 올라간 robot model 정보를 불러와 사용한다. 
 ```
	<node name="spawn_model" pkg="gazebo_ros" type="spawn_model" respawn="false" output="screen" args="-urdf -model robot -param robot_description"/>
```
##

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
	    <gazebo reference="upper_arm_link">
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
- 우측 상단의 *Generate Collision Matrix*를 클릭하고 *linear view*와  *matrix view*를 눌러 생성된 결과가 아래와 같음을 확인하고, 다를 경우 수정한다.
![Screenshot from 2022-11-05 18-13-18](https://user-images.githubusercontent.com/60342784/200112543-5da9aefc-99dd-49d7-bb20-a5409ee22857.png)

- 좌측 *Planning Groups*의 *Add Group*을 클릭하여 아래와 같이 입력한다.
![edit_planning_group](https://user-images.githubusercontent.com/60342784/194872835-6fb4dee5-c101-47d8-bb20-8deec8d13386.png)

- *Add Kin. Chain*을 클릭하고 좌측 하단의 *Expand All*을 클릭하여 모든 link가 보이게 한 후, 아래 그림과 같이 *Base Link*를 `base_link`로, *Tip Link*를 `tcp`로 설정한다.
![Screenshot from 2022-11-05 18-16-16](https://user-images.githubusercontent.com/60342784/200112675-2f7ddafc-c898-4c36-bd8c-4da801674362.png)


- *Save*를 눌러 저장한 후 다시 *Add Group*을 눌러 아래와 같이 입력한다.
![Screenshot from 2022-10-10 22-16-49](https://user-images.githubusercontent.com/60342784/194875273-da510f44-b175-4894-b60a-a9a907139166.png)

- *Add Joints*를 눌러 `robotiq_85_left_knuckle_joint`를 선택한 후 오른쪽 화살표를 눌러 *Selected Joint*로 이동시키고, *Save*를 눌러 저장한다.
![Screenshot from 2022-10-29 21-55-21](https://user-images.githubusercontent.com/60342784/198832712-d24065cf-07c4-4e42-aabc-daf97f6d261e.png)

- 아래 그림과 같이 설정되었음을 확인한다.![Screenshot from 2022-10-29 21-55-57](https://user-images.githubusercontent.com/60342784/198832730-397d044a-ee88-4d2d-9799-7cdba13605f4.png)


- 좌측의 *Robot Poses*를 클릭하여 Robot의 predefined pose를 아래와 같이 추가한다.
- *Add Pose*를 누른 후 *Pose Name*을 `home`으로, *Planning Group*을 `manipulator`로 설정한  후 모든 joint 값이 `0`임을 확인하고 *Save*를 누른다.
![Screenshot from 2022-10-29 21-57-29](https://user-images.githubusercontent.com/60342784/198832810-b4b5dbcc-7ae6-4f04-b8bc-6faf0136df8e.png)

- *Add Pose*를 누른 후 *Pose Name*을 `up`으로, *Planning Group*을 `manipulator`로 설정한  후, `shoulder_lift_joint` 값을 `-1.5708`로 변경하고, 나머지 모든 joint의 값이 `0`임을 확인하고 *Save*를 누른다.
![Screenshot from 2022-10-29 21-57-54](https://user-images.githubusercontent.com/60342784/198832869-ef0f60f6-f9e9-49c3-9f98-c8cd8db6170f.png)

- *Add Pose*를 누른 후 *Pose Name*을 `open`으로, *Planning Group*을 `gripper`로 설정한  후, `finger_joint` 값이 `0`임을 확인하고 *Save*를 누른다.
![Screenshot from 2022-10-29 21-58-30](https://user-images.githubusercontent.com/60342784/198832912-e6119f65-3f2c-46c6-a7af-65ee5a6c6da7.png)

- *Add Pose*를 누른 후 *Pose Name*을 `close`로, *Planning Group*을 `gripper`로 설정한  후, `finger_joint` 값이 `0.33`임을 확인하고 *Save*를 누른다.
![Screenshot from 2022-11-05 18-56-31](https://user-images.githubusercontent.com/60342784/200114109-3d24f8c9-3cfe-4d8f-97bd-8ce4783f53d4.png)



- 총 4개의 *Pose*가 아래와 같이 추가되었음을 확인한다.
![Screenshot from 2022-10-10 22-38-10](https://user-images.githubusercontent.com/60342784/194879285-8395c47d-8f7c-4692-92a1-748e48b17d26.png)

- ~~좌측의 *End Effectors - Add End Effector*를 클릭하여 아래 그림과 같이 입력하고 *Save*를 눌러 저장한다.~~
![Screenshot from 2022-10-10 22-42-20](https://user-images.githubusercontent.com/60342784/194880048-48179c32-5361-41cd-aa00-9349f45e64e2.png)

- 좌측의 *Passive Joints*를 클릭하여 아래 그림과 같이 gripper의 나머지 조인트를 모두 Passive joint로 설정한다.
![Screenshot from 2022-11-05 18-19-16](https://user-images.githubusercontent.com/60342784/200112790-ca3446c3-a5a8-4ec3-9882-d6682afd2565.png)


- 좌측의 *Controllers - Auto Add FollowJointsTrajectory*를 클릭하여 생성된 Controller의 이름과 Controller Type을 아래 그림과 같이 변경한다.![Screenshot from 2022-10-29 22-00-20](https://user-images.githubusercontent.com/60342784/198832987-fdfa60af-af62-4ef8-9995-22fd64b36621.png)


- 좌측의 *Author Information* 을 클릭하여 작성자의 정보를 입력한다.
- 좌측의 *Configulation Files*로 이동하여 *Configuration Package Save Path*를 ros workspace의 `src`디렉토리로 지정하고, 경로의 맨 뒤에 `/ur5e_with_2f85_moveit_config`를 붙여준다.
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
	      p: 40
	      d: 0
	      i: 0.1
	      i_clamp: 1
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
	```
	$ code urdf/bin.urdf
	```
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
	```
    $ code urdf/conveyor_belt.urdf
    ```
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
	```
    $ code urdf/red_box.urdf
   ```
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
	$ cd ur5e_with_2f85
	```
- 패키지 폴더 내 `package.xml`에 아래 내용을 붙여넣는다.
```xml
<?xml version="1.0"?>
<package format="2">
  <name>ur5e_with_2f85</name>
  <version>0.0.0</version>

  <description>The ur5e_with_2f85 package</description>

  <maintainer email="jinseok.kim970@gmail.com">Jinseok Kim</maintainer>
  <author email="jinseok.kim970@gmail.com">Jinseok Kim</author>

  <license>BSD</license>

  <buildtool_depend>catkin</buildtool_depend>

  <depend>moveit_core</depend>
  <depend>moveit_ros_planning</depend>
  <depend>moveit_ros_planning_interface</depend>
  <depend>std_msgs</depend>
  <depend>gazebo_msgs</depend>
  <depend>geometry_msgs</depend>

  <!-- <exec_depend>gazebo_ros</exec_depend> -->
  <exec_depend>ur5e_with_2f85_gazebo</exec_depend>
  <exec_depend>ur5e_with_2f85_description</exec_depend>

  <test_depend>rosunit</test_depend>

</package>
```
- `CmakeLists.txt` 에 아래 내용을 붙여넣는다.
```cpp
cmake_minimum_required(VERSION 3.0.2)
project(ur5e_with_2f85)

find_package(catkin REQUIRED
  COMPONENTS
    moveit_core
    moveit_ros_planning
    moveit_ros_planning_interface
    std_msgs
    gazebo_msgs
    geometry_msgs
)

catkin_package(
  LIBRARIES
  INCLUDE_DIRS
  CATKIN_DEPENDS
    moveit_core
    moveit_ros_planning
    moveit_ros_planning_interface
    std_msgs
    gazebo_msgs
    geometry_msgs
)

find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED system filesystem date_time thread)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

link_directories(lib)

add_executable(
  pick_and_place_gazebo_example
  src/pick_and_place_gazebo_example.cpp  
)
target_link_libraries(pick_and_place_gazebo_example ${catkin_LIBRARIES})
```
- `src` 디렉토리에 `pick_and_place_gazebo_example.cpp` 파일을 만들고 아래 내용을 붙여넣는다.
	```
	$ code pick_and_place_gazebo_example.cpp
	```
	```cpp
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


	class PickAndPlaceClass
	{
	    public:
	        void main();

	    protected:
	        ros::NodeHandle n;
	        geometry_msgs::Point boxPosition_;
	        void callbackGetModelStateData(const gazebo_msgs::ModelStatesConstPtr &msg);
	        const std::string PLANNING_GROUP_ARM_ = "manipulator";
	        const std::string PLANNING_GROUP_GRIPPER_ = "gripper";
	   
	};

	void PickAndPlaceClass::callbackGetModelStateData(const gazebo_msgs::ModelStatesConstPtr &msg)
	{
	    std::string target_model= "red_box";
	    std::vector<std::string> names = msg->name;
	    std::vector<std::string>::iterator itr = std::find(names.begin(), names.end(), target_model);

	    if (itr != names.cend()){
	        int index = std::distance(names.begin(), itr);
	        boxPosition_= msg->pose[index].position;
	    }
	    else {
	        ROS_WARN_NAMED("tutorial", "Model state of red box is not detected");
	    }
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
	    joint_group_positions[0] = 0;
	    joint_group_positions[1] = -1.57;
	    joint_group_positions[2] = -1.82;
	    joint_group_positions[3] = -1.32;
	    joint_group_positions[4] = 1.57;
	    joint_group_positions[5] = 0;
	    
	    move_group_interface_arm.setJointValueTarget(joint_group_positions);

	    // move_group_interface_arm.setJointValueTarget(move_group_interface_arm.getNamedTargetValues("up"));


	    bool success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

	    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

	    move_group_interface_arm.move();

	    // 1.2 Get red box's pose
	    ros::Subscriber sub_ModelState = n.subscribe("gazebo/model_states", 1, &PickAndPlaceClass::callbackGetModelStateData, this);

	    // 2. Place the TCP (Tool Center Point, the tip of the robot) above the blue box
	    geometry_msgs::PoseStamped current_pose;
	    current_pose = move_group_interface_arm.getCurrentPose("tool0");

	    geometry_msgs::Pose target_pose1;
	    target_pose1.orientation = current_pose.pose.orientation;
	    target_pose1.position.x = boxPosition_.x;
	    target_pose1.position.y = boxPosition_.y;
	    target_pose1.position.z = boxPosition_.z + 0.10;
	    move_group_interface_arm.setPoseTarget(target_pose1);

	    success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

	    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

	    move_group_interface_arm.move();


	    moveit::planning_interface::MoveGroupInterface::Plan my_plan_gripper;

	    // 3. Open the gripper
	    move_group_interface_gripper.setJointValueTarget(move_group_interface_gripper.getNamedTargetValues("open"));

	    success = (move_group_interface_gripper.plan(my_plan_gripper) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

	    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

	    move_group_interface_gripper.move();

	    // 4. Move the TCP close to the object
	    target_pose1.position.z = target_pose1.position.z - 0.19;
	    move_group_interface_arm.setPoseTarget(target_pose1);

	    success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

	    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

	    move_group_interface_arm.move();

	    // 5. Close the  gripper
	    move_group_interface_gripper.setJointValueTarget(move_group_interface_gripper.getNamedTargetValues("close"));

	    success = (move_group_interface_gripper.plan(my_plan_gripper) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

	    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

	    move_group_interface_gripper.move();

	    // Move the TCP above the plate
	    target_pose1.position.z = target_pose1.position.z + 0.15;
	    move_group_interface_arm.setPoseTarget(target_pose1);

	    success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

	    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

	    move_group_interface_arm.move();

	    // 6. Move the TCP above the plate
	    target_pose1.position.y = 0.0;
	    target_pose1.position.x = - 0.5;
	    move_group_interface_arm.setPoseTarget(target_pose1);

	    success = (move_group_interface_arm.plan(my_plan_arm) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

	    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

	    move_group_interface_arm.move();

	    // 7. Lower the TCP above the plate
	    target_pose1.position.z = target_pose1.position.z - 0.14;
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
	 ```



- `catkin_make` 또는 `catkin build`로 패키지들을 빌드하고 `source ~/.bashrc` 입력. 

- 아래의 명령어를 순서대로 입력하여 `Gazebo`와 `MoveIT`을 통한 Pick and place 시뮬레이션 환경을 실행한다.
	```bash
	$ roslaunch ur5e_with_2f85_gazebo pick_and_place.launch 
	-> 제작한 로봇을 Gazebo에 spawn하여 시뮬레이션 환경을 만들고 moveIT 실행.

	$ rosrun ur5e_with_2f85 pick_and_place_gazebo_example
	-> cpp 코드를 통한 Pick and place 시작
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
---
-- 이하 내용은 pose tracker 관련  내용입니다.
---
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
eyJoaXN0b3J5IjpbMTkzNjA5NTk0MCwtMTMzMTEwNDM5MiwtMT
U2MDEwOTE5MSwtNjUwNzk4NzA1LC0xMzk5NTY4NiwtMTkxMTcx
OTM5NCwtMTY3ODEwMTMyLDE0MDg4NzgzOTQsLTEzNDIzMjI2OD
MsODQwNjQyMTQzLDUyMzE2MzYyMyw3MzEyNDY3ODUsMjI0Nzgx
NTIxLDE0MTEyMTU0NjAsMTEwMjgxNjI0MywtODQ2NDYwMzc3LD
EzMzc5NzM1ODUsLTQ3MDQ3MTAsMTA3MTEyNDYwMiwyMzE4MzIy
NThdfQ==
-->