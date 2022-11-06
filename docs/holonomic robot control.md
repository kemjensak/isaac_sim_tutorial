
# Controlling Holonomic Robot

1. viewport 아래의 _Content_ 탭에서 _Isaac/Environments/Simple_Warehouse/full_warehouse.usd_ 파일의 위치로 들어간다.

2.  _full_warehouse.usd_ 파일을 Viewport 또는 stage 탭으로 드래드&드롭 한다.

3.  Property 탭에서 원점을 맞추고, warehouse 내부의 시점으로 적절히 camera를 이동한다.

4.  _Isaac/Environments/robots/full_warehouse.usd_robots/O3dyn/o3dyn.usd_ 를 Viewport 또는 stage 탭으로 드래드&드롭 한다.

5. robot이 ground plane을 관통하지 않도록 property의 transform 중 z translate를 0.102 로 조절한다.

6.  Action graph를 아래 그림과 같이 설정한다.
![ROS holonomic](https://github.com/IROL-SSU/isaac_sim_tutorial/blob/main/pictures/OG_holonomic.png)

7. Node 내 prim을 다음과 같이 설정한다.

|Node|Input Field|Value|
|--|--|--|
|UDS setup Holonomic Robot|comPrim|/World/o3dyn/base_link|
||robotPrim|/World/o3dyn|
|Articulation Controller|robotPath|/World/o3dyn|
||usePath|True(checked)|

8. 우측 Stage 탭에 우클릭을 하고 *Create -> Physics -> Physics Scene*을 추가한다.

9. 추가한 *PhysicsScene*의 property에서 *Enable GPU Dynamics*의 체크를 해제한다.

10. roscore를 실행한다.

11. Isaac Sim의 Play 버튼을 눌러 시뮬레이션을 실행한다.

12. `rosrun teleop_twist_keyboard teleop_twist_keyboard.py`를 실행하여  twist message를 키보드를 통해 publish한다.
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTM0OTQ0NTIyNl19
-->