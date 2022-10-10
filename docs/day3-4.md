# DAY3~4: Omniverse issac sim 기반 commercial manipulator

-   *~~UR5e, Gripper 대상: 각각의 URDF 작성, Xacro 통합, Moveit에서 연동 Kinematics 구성 (3hr)~~*
-   *~~Moveit GUI 상에서 Controller 지정한 후 compile해주면 moveit_config(or UR5e_moveit_config 이런식으로 naming) 패키지 생성~~*

-   Moveit_servo는 원래 moveit package folder 내에 존재....

-   Conveyor belt 상의 object target obejct detect & picking/placing 공정 가상화(예제 찾아볼 것) task planning(target pose, goal pose)

-   Moveit pose tracker(출력?) -> Servo_server(여기서 제어 방법을 결정했던가???, trajectory_controller? group_controller) -> Joint twist? -> 1) Isaac sim simulator, 2) Rviz, 3) Real UR5e

-   Moveit 패키지에서 Jacobian matrix -> 공정최적화 프로그래밍(Manipulability measure 가시화, Optimal arm positioning)

## Joining two URDF(xacro)
### create new package
 - ROS workspace 내의 src에서 다음 명령어를 입력하여 새로운 패키지를 생성한다.
 
	   $ catkin_create_pkg ur5e_with_2f85_description
 - 아래 명령어를 통해 생성된 패키지 내에 `urdf`와 `launch` 디렉토리를 생성하고, `urdf` 디렉토리로 이동한다.
 
	    $ mkdir ur5e_with_2f85_description/launch ur5e_with_2f85_description/urdf
	    $ cd ur5e_with_2f85_description/urdf
	    
 - 생성된 `urdf` 디렉토리 내에서 아래 명령어를 입력해 `ur5e_with_2f85.xacro`파일을 만든다.

	   $ code ur5e_with_2f85.xacro
 - 열린 VScode에 아래의 코드를 붙여넣는다.

	  <?xml version="1.0"?>
		<robot name="ur5e_with_2f85" 
		  xmlns:xacro="http://wiki.ros.org/xacro">

		    <!-- 2f-85 -->
		    <xacro:include filename="$(find robotiq_2f_85_gripper_visualization)/urdf/robotiq_arg2f_85_model_macro.xacro" />
		    <xacro:robotiq_arg2f_85 prefix=""/>
		        
		    <!-- ur5e -->
		    <xacro:include filename="$(find ur_description)/urdf/inc/ur5e_macro.xacro" />
		    <xacro:ur5e_robot prefix="" />
		    
		    <link name="world"/>
		    
		    <joint name="world2base" type="fixed">
		        <parent link="world"/>
		        <child link="base_link"/>
		        <origin xyz="0 0 0" rpy="0 0 1.57" />
		    </joint>

		    <joint name="tool0To2f85" type="fixed">
		        <parent link="tool0"/>
		        <child link="robotiq_arg2f_base_link"/>
		        <origin xyz="0 0 0" rpy="0 0 1.57" />
		    </joint>

		</robot>

 - `Ctrl-S`로 저장 후 닫는다.
 - 위에서 만든 `launch` 디렉토리 내에서 아래 명령어를 입력하여 새로운 `load_ur5e_with_2f85.launch` 파일을 만든다.

      $ code load_ur5e_with_2f85.launch
 - 열린 VScode에 아래의 코드를 붙여넣는다.

      <?xml version="1.0"?>
		<launch>
		  <param name="robot_description" command="$(find xacro)/xacro '$(find new_robot_description)/urdf/ur5e_2f-85.xacro'"/>
		  <node pkg="robot_state_publisher" type="robot_state_publisher" name="robot_state_publisher"/>
		  <node pkg="joint_state_publisher_gui" type="joint_state_publisher_gui" name="joint_state_publisher_gui"/>
		  <node name="$(anon rviz)" pkg="rviz" type="rviz" respawn="false" output="screen"/>
		</launch>
 - `Ctrl-S`로 저장 후 닫는다.
 - `catkin_make`또는 `catkin build`후  아래  명령어를 통해 위에서 만든 `load_ur5e_with_2f85.launch` 파일을 실행한다.

	   $ roslaunch ur5e_with_2f85_description load_ur5e_with_2f85.launch 

 - 실행된 `Rviz`의 *Global Options*의 *Fixed Frame*을 `world`로 변경하고, *Add* 버튼을 눌러 *RobotModel*을 추가한다.
 - UR5e manipulator와 말단부에 장착된 2f-85 gripper를 확인할 

### create new xacro file and including two other xacro

### create movit_config package with moveit setup assistant
###


<!--stackedit_data:
eyJoaXN0b3J5IjpbLTEwMjc4ODkyNjcsLTEyMTU0NjQwMTEsLT
EwOTI4OTc4NDgsLTg3NjExMjk5MywxODc2MDA4MDUxLDEyMzY2
OTM4ODUsMTU3Mzc4NTc5NiwxMDQ1NDYzMzUyLC02NTM1MzMyMz
YsOTcxNjE1MzE5LC0xMjk0OTAwNzIxLDk4OTMxOTg2LC0yMDIz
MzU0ODc5LC0yMDg4NzQ2NjEyLDczMDk5ODExNl19
-->