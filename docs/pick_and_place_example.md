# Pick and place example
- 하단 Content 탭에서 `Isaac/Environments/Simple_Warehouse`에 위치한 `warehouse.usd` 파일을 우측의 `stage` 탭으로 드래그한다.\

- 아래 그림과 같은 환경이 load 되었음을 확인한다.![Screenshot from 2022-11-06 18-43-39](https://user-images.githubusercontent.com/60342784/200164020-11854545-5aac-47b9-9daa-49a5e41355b9.png)

- 상단 탭에서 Window -> Extensions를 클릭하고 conveyor를 검색창에 입력한다.
- 아래 그림과 같이 토글 스위치를 클릭하여 ENABLED 로 표시되도록 한다.
![Screenshot from 2022-11-06 18-46-28](https://user-images.githubusercontent.com/60342784/200164109-5f0d6533-a215-480e-b814-5630ba2a04e9.png)

- 상단의 `Create -> Shapes -> Cube`를 클릭하여 Cube를 만들고 이름을 `convayor`로 변경한다. Property의 `Geometry -> Mesh -> Size`를 아래와 같이 `1`로 변경한다.
![Screenshot from 2022-11-06 19-00-38](https://user-images.githubusercontent.com/60342784/200164633-2ce53aa0-548b-45dc-9c36-b3ea3e37990f.png)

- 이제 `Transform -> Scale` 값으로 Cube의 크기를 미터 단위로 조절할 수 있다.
- 아래와 같이 Transform의 값들을 조절한다.
![Screenshot from 2022-11-06 19-11-26](https://user-images.githubusercontent.com/60342784/200165093-c9bbe853-7c74-478c-8a7d-f52f876e70ba.png)
- `Viewport`에 아래 그림과 같이 표시됨을 확인한다.
![Screenshot from 2022-11-06 19-11-53](https://user-images.githubusercontent.com/60342784/200165112-503c424e-82b8-47a9-8aed-d3e227e17358.png)

- 상단의 탭에서 `Create -> Visual Scripting -> Action Graph`를 클릭하여 Action 그래프를 추가한다.
- 아래 그림과 같이 Action Graph를 만든다.
![Screenshot from 2022-11-06 19-20-48](https://user-images.githubusercontent.com/60342784/200165438-04460a5b-7d3b-4297-8129-0330e31f7c18.png)
- `Conveyor Belt`의 `Relationships -> inputs:conveyorPrim` 을 `World/convayor`로 지정한다. 
- `Conveyor Belt`의 input 중 `velocity`를 0.5로 입력한다.
![Screenshot from 2022-11-06 19-25-04](https://user-images.githubusercontent.com/60342784/200165603-a2e53ec8-7b00-4ff5-8306-ce31a1e385c4.png)

- Cube를 하나 더 만들기 위해 상단의 `Create -> Shapes -> Cube`를 클릭하고 Property의 `Geometry -> Mesh -> Size`를 아래와 같이 `1`로 변경한다.

- Transform은 아래와 같이 입력한다.
![Screenshot from 2022-11-06 19-28-56](https://user-images.githubusercontent.com/60342784/200165709-490427e0-9983-46b9-b76b-88add55087dc.png)


<!--stackedit_data:
eyJoaXN0b3J5IjpbMjcyMzkxNzI1LC0xNzU4ODc1MDkzLDQ5Mj
A3MDI4NV19
-->