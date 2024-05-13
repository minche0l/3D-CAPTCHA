# 3D CAPTCHA using Torus Patch
## 1. AABB(Axis-Aligned Bounding Box)
* 객체나 모델의 경계를 둘러싸는 가장 간단한 형태의 박스<br><br>
* 박스를 이루는 면의 노말 벡터들이 x,y,z 축과 일치하는 모양
* 모델을 이루는 다각형의 x,y,z 좌표의 최소와 최대를 각 박스의 vertex로 생성
* 물체가 회전하면 AABB의 크기 변화
  <img src="https://github.com/SeokJinB/3D-CAPTCHA/assets/100738541/0f851ad7-dcc1-4e60-b5ee-d175399946f9" width="200" height="300">
  <img src="https://github.com/SeokJinB/3D-CAPTCHA/assets/100738541/88e16159-4bb3-4402-84f5-1558308a21b7" width="200" height="300">

## 2. BVH(Bounding Volume Hierarchy)
* 객체의 바운딩 박스를 이용해 객체 Polygon을 트리 형태로 저장한 자료구조<br><br>
* root node, internal node, leaf node 구성
* Ray Tracing, Collision detection 등에 사용
  <img src="https://github.com/SeokJinB/3D-CAPTCHA/assets/100738541/0f851ad7-dcc1-4e60-b5ee-d175399946f9" width="200" height="300">
  <img src="https://github.com/SeokJinB/3D-CAPTCHA/assets/100738541/02b96719-d5de-43f0-9679-e38ebb4fe49d" width="200" height="300">

