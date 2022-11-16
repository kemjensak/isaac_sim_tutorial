


> Written with [StackEdit](https://stackedit.io/).

### 카메라 부착
- franka(Panda)의 end-effector단에 붙이기 위한 camera를 생성 `Create > Camera`
- stage tree에 생성된 Camera를 우클릭 한후 rename을 통해 `sim_camera`로 설정
- sim_camera를 drag하여 franka의 panda_link8의 하위에 둠.
- stage tree의 sim_camera를 클릭해 property를 다음과 같이 구성
	- orientation을 x: 180, y: 0, z: -45
	- translate을 z: -0.1

	
	
### Action graph 구성

#### 카메라 및 TF 관련 Action graph

- 새로운  Action graph를 생성 ` Create > Visual scripts > Action graph`
- Graph node들을 다음과 같이 구성

<!--stackedit_data:
eyJoaXN0b3J5IjpbMTIwMTIxNDk4NV19
-->