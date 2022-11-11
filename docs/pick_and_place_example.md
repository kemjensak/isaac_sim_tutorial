# Pick and place example
- 하단 Content 탭에서 `Isaac/Environments/Simple_Warehouse`에 위치한 `warehouse.usd` 파일을 우측의 `stage` 탭으로 드래그한다.

- 아래 그림과 같은 환경이 load 되었음을 확인한다.
![Screenshot from 2022-11-06 18-43-39](https://user-images.githubusercontent.com/60342784/200164020-11854545-5aac-47b9-9daa-49a5e41355b9.png)

- 상단 탭에서 Window -> Extensions를 클릭하고 conveyor를 검색창에 입력한다.
- 아래 그림과 같이 토글 스위치를 클릭하여 ENABLED 로 표시되도록 한다.

![Screenshot from 2022-11-06 18-46-28](https://user-images.githubusercontent.com/60342784/200164109-5f0d6533-a215-480e-b814-5630ba2a04e9.png)

- 상단의 `Create -> Shapes -> Cube`를 클릭하여 Cube를 만들고 이름을 `conveyor`로 변경한다. Property의 `Geometry -> Mesh -> Size`를 아래와 같이 `1`로 변경한다.

![Screenshot from 2022-11-06 19-00-38](https://user-images.githubusercontent.com/60342784/200164633-2ce53aa0-548b-45dc-9c36-b3ea3e37990f.png)

- 이제 `Transform -> Scale` 값으로 Cube의 크기를 미터 단위로 조절할 수 있다.
- 아래와 같이 Transform의 값들을 조절한다.

![Screenshot from 2022-11-11 22-03-51](https://user-images.githubusercontent.com/60342784/201346137-3ff1d2db-2e50-4a78-9958-2df103f6a6dd.png)


- `Viewport`에 아래 그림과 같이 표시됨을 확인한다.(크기는 다름)

![Screenshot from 2022-11-06 19-11-53](https://user-images.githubusercontent.com/60342784/200165112-503c424e-82b8-47a9-8aed-d3e227e17358.png)

- 상단의 탭에서 `Create -> Visual Scripting -> Action Graph`를 클릭하여 Action 그래프를 추가한다.
- 아래 그림과 같이 Action Graph를 만든다.

![Screenshot from 2022-11-06 19-20-48](https://user-images.githubusercontent.com/60342784/200165438-04460a5b-7d3b-4297-8129-0330e31f7c18.png)

- `Conveyor Belt`의 `Relationships -> inputs:conveyorPrim` 을 `World/conveyor`로 지정한다. 

- `Conveyor Belt`의 input 중 `velocity`를 0.5로 입력한다.

![Screenshot from 2022-11-06 19-25-04](https://user-images.githubusercontent.com/60342784/200165603-a2e53ec8-7b00-4ff5-8306-ce31a1e385c4.png)

- Cube를 하나 더 만들기 위해 상단의 `Create -> Shapes -> Cube`를 클릭하고 이름을 `red_box`로 변경한다. Property의 `Geometry -> Mesh -> Size`를 아래와 같이 `1`로 변경한다.

- Transform은 아래와 같이 입력한다.

![Screenshot from 2022-11-11 22-05-13](https://user-images.githubusercontent.com/60342784/201346376-381c9575-3e8b-43bf-9a38-ccafdda06793.png)


- 추가한 `red_box`와 `conveyor`를 각각 우클릭하여 아래와 같이 `Physics` preset를 추가한다.

![Screenshot from 2022-11-06 19-50-23](https://user-images.githubusercontent.com/60342784/200166520-1d5cddab-33f7-4280-8864-5b7e6bf73040.png)

- 상단 탭의 `Create -> Material -> OmniPBR`을 클릭하여 추가한다.
- 추가한 `OmniPBR`의 이름을 `simple_red`로 변경한 후, `Property`의 `Base Color`를 다음과 같이 설정한다.

![Screenshot from 2022-11-06 19-59-42](https://user-images.githubusercontent.com/60342784/200166879-b5a3a29a-72e4-408b-bf3e-8a545faca986.png)

- `red_box`의 `property`중, `Materials on selected models`를 아래와 같이 변경한다.

![Screenshot from 2022-11-06 20-02-35](https://user-images.githubusercontent.com/60342784/200166997-4d30b250-1a26-4180-8444-f21d212b94d9.png)

- 좌측의 Play 버튼을 눌러 red_box가 conveyor 위에서 움직이는지 확인한다.

- 상단 탭의 `Create -> Shapes -> Cylinder`을 클릭하여 추가한다.
- 추가한 `Cylinder`의 `Property -> Geometry ->Mesh`를 다음과 같이 설정한다.

![Screenshot from 2022-11-11 22-07-47](https://user-images.githubusercontent.com/60342784/201346842-d1569bc3-0884-4eb9-b8db-482fd88981f0.png)

- 
- 앞서 제작하였던  ur5e_with_2f85.xacro 파일의 위치로 이동하여 아래 명령어를 입력한다.
	 ```
	$ rosrun xacro xacro -o ur5e_with_2f85.urdf ur5e_with_2f85.xacro
	```
- 같은 위치에 `ur5e_with_2f85.urdf` 파일이 생성되었음을 확인한다.
- 파일을 열어 left_finger_joint와 right_finger_joint의 joint type을 fixed에서 continuous로 변경.
- 버그로 인해 이동하여야 함.(내용 보완필요)
- 상단의 탭에서 상단의 탭에서  `Isaac Utils > Workflows > URDF Importer`  를 실행한다.

- `clean stage` 는 체크 해제, `Fix Base Link` 는 체크하고 , `Joint Drive Type` 을 Position으로 설정한다.

- 제작한 URDF 파일을 `Import`의 `Input File` 로 지정하고 Stage 탭의  `/World`  를 선택한 상태에서  `Import`  버튼을 누른다.
- 생성된 `ur5e_with_2f85` prim을 선택하고 `Property`의 `Transform`을 아래와 같이 설정한다.

![Screenshot from 2022-11-11 22-08-54](https://user-images.githubusercontent.com/60342784/201347038-7dc9c6c3-0125-4f5b-b502-0862348b48ca.png)

- 하단 Content 탭에서 `Isaac/Props/KLT_Bin`에 위치한 `small_KLT.usd` 파일을 우측의 `stage` 탭으로 `World` Prim의 하위로 삽입되도록 드래그한다.
- `Transform`을 아래와 같이 설정한다.
 ![Screenshot from 2022-11-11 22-21-04](https://user-images.githubusercontent.com/60342784/201348901-57256d47-2a1e-4f69-ad03-3b9585eeb17b.png)

- 아래 그림과 같은 환경이 갖추어졌음을 확인한다.

![Screenshot from 2022-11-11 22-21-43](https://user-images.githubusercontent.com/60342784/201349003-7dbe28a8-4542-4169-bc4b-cc203b0db1e0.png)


- Urdf로 import한 `ur5e_with_2f85` 에서  robotiq의 모든 joint들을 아래와 같이 추가/수정 필요.
- `robotiq_85_left_finger_joint`, `robotiq_85_right_finger_joint`외 모든 joint를 `Exclude From Articulation` 체크, `Max Force` 값 0로 설정.
- 각 joint 회전축 확인하여 올바르게 변경.
- 

<!--stackedit_data:
eyJoaXN0b3J5IjpbMTU3NzAyODE4MSwtMTc1NjQyODA5OSwtMT
U4MzM0MTM3NCwtODk5Mzg0MDkwLC00MTg5NzYyMTgsMjAxODA3
MzcyNiwtMTQ5NjQzMDgzMCw2NzgxOTc3MDQsLTc4MTQzMTU1MS
wyNzIzOTE3MjUsLTE3NTg4NzUwOTMsNDkyMDcwMjg1XX0=
-->