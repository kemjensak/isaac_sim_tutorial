
# Import URDF

## URDF 파일 준비

 - universal_robot repository의 ur_description 패키지가 없다면 아래 명령어를 통해 받는다.
    
	`git clone -b calibration_devel https://github.com/fmauch/universal_robot.git`
   
- 받은 패키지를 컴파일한다.

    `catkin_make` or `catkin build`

- 다음의 명령어를 통해 `universal_robot/ur_description/urdf` 에 위치한 UR5e.xacro 파일을  `.xacro`  에서  `.urdf`  로 변환한다.
  
   `rosrun xacro xacro -o ur5e.urdf ur5e.xacro`
   
## Isaac sim의 URDF Importer를 통한 importing

1.    viewport 아래의 _Content_ 탭에서  _Isaac/Environments/Simple_Room/simple_room.usd_ 파일의 위치로 들어간다.
    
2.  _simple_room.usd_ 파일을 Viewport 탭으로 드래그&드롭 하고, Transform Property의 _Translate_  component를 모두 0으로 채워넣어 origin의 영점을 맞춘다.
    
3.  상단의 탭에서 _Isaac Utils > Workflows > URDF Importer_ 를 실행한다.
    
4.  실행된 창의  _Import Option_  에서,  _clean stage_ 는 체크 해제,  _Fix Base Link_ 는 체크하고 , _Joint Drive Type_  을  Position으로 설정한다.
    
5.  위에서 만들었던 URDF 파일을 _Import_  의  _Input File_ 로 지정하고 Import 버튼이 활성화 되었음을 확인한다.
    
6.  Stage 탭의 _/World_ 를 선택한 상태에서 _Import_ 버튼을 누른다.
    
7.  로봇이 원점인 테이블 위 가운데로 spawn 되었을 것이다. Transform Property를 적절히 변경하여 원하는 Pose로 변경한다.

# Do the test motion of the UR5e with Omnigraph
## Build the graph
test motion은 

1.  상단 탭에서 _Window > Visual Scripting > Action Graph_ 를 실행하고 아래쪽 section에 Action Graph 탭이 생성된 것을 확인한다.

2. Action Graph window에서 _New Action Graph_ 아이콘을 클릭하면, Action Graph window의 좌측에 OG(omnigraph) 노드들이 리스트되어 있으며, 검색 창에 원하는 노드를 검색하거나 Isaac Ros 카테고리에서 직접 ROS관련 노드들을 찾아볼 수 있다. 

3. 아래의 노드들을 찾아 오른쪽 graph window 공간으로 드래그&드롭 한다.
	- Isaac Read Simulation Time
	- On Playback Tick
	- ROS1 Publish Joint State
	- ROS1 Subscribe Joint State

4. 아래 그림과 같이 노드의 point를 클릭 후 드래드하여 노드들을 연결한다.  
![OG_nodes](https://github.com/IROL-SSU/isaac_sim_tutorial/blob/main/OG_ur5e.png?raw=true)

5. Subscribe 노드와 Publish 노드를 각각 클릭하여 우측의 Property 탭에서 _RelationShips - TargetPrim_ 에 ur5e_robot을 추가한다. 

6. 터미널 창에서 `roslaunch ur_description load_ur5e.launch` 를 실행하여 URDF를 robot description parameter로 load한다.

7. Isaac sim으로 돌아와 왼쪽 바의 _Play_ 버튼을 클릭하여 시뮬레이션을 시작한다.

8. 다른 터미널 창을 열어 `rostopic echo /joint_states`  를 실행하고 현재 로봇의 각 Joint angular position 정보들이 출력됨을 확인한다.
  
9. `rosrun joint_state_publisher_gui joint_state_publisher_gui joint_states:=joint_command` 를 실행하여 각 joint들의 각도를 수동으로 변경하면 Isaac sim에서 움직이는 모습을 확인한다.