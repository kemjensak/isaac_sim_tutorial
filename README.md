
## Importing URDF

- universal_robot repository의 ur_description 패키지가 없다면 아래 명령어를 통해 받는다.
    
	`git clone -b calibration_devel https://github.com/fmauch/universal_robot.git`
   
- 받은 패키지를 컴파일한다.

    `catkin_make` or `catkin build`

- 다음의 명령어를 통해 `universal_robot/ur_description/urdf` 에 위치한 UR5e.xacro 파일을  `.xacro`  에서  `.urdf`  로 변환한다.
  
   `rosrun xacro xacro -o ur5e.urdf ur5e.xacro`

1.    viewport 아래의 _Content_ 탭에서  _Isaac/Environments/Simple_Room/simple_room.usd_ 파일의 위치로 들어간다.
    
2.  _simple_room.usd_ 파일을 Viewport 탭으로 드래그&드롭 하고, Transform Property의 _Translate_  component를 모두 0으로 채워넣어 origin의 영점을 맞춘다.
    
3.  상단의 탭에서 _Isaac Utils > Workflows > URDF Importer_ 를 실행한다.
    
4.  실행된 창의  _Import Option_  에서,  _clean stage_ 는 체크 해제,  _Fix Base Link_ 는 체크하고 , _Joint Drive Type_  을  Velocity로 변경한다.
    
5.  위에서 만들었던 URDF 파일을 _Import_  의  _Input File_ 로 지정하고 Import 버튼이 활성화 되었음을 확인한다.
    
6.  Stage 탭의 _/World_ 를 선택하고 _Import_ 버튼을 누른다.
    
7.  로봇이 원점인 테이블 위 가운데로 spawn 되었을 것이다. Transform Property를 적절히 변경하여 원하는 Pose로 변경한다.
    


