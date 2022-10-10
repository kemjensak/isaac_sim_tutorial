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
  - 생성된 디렉토리 내에 urdf
### create new xacro file and including two other xacro
### create movit_config package with moveit setup assistant
###


<!--stackedit_data:
eyJoaXN0b3J5IjpbLTEyOTA0NzU4MjEsOTcxNjE1MzE5LC0xMj
k0OTAwNzIxLDk4OTMxOTg2LC0yMDIzMzU0ODc5LC0yMDg4NzQ2
NjEyLDczMDk5ODExNl19
-->