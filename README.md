# 3D CAPTCHA using Torus patch

## AABB (Axis - Aligned Bounding Box)
* 객체나 모델의 경계를 둘러 싸는 가장 간단한 형태의 박스
* 박스를 이루는 면의 노말 벡터들이 x,y,z 축과 일치하는 모양이다.
* 모델을 이루는 다각형의 x,y,z 좌표의 최소와 최대를 각 박스의 vertex로 해서 생성한다.
* 물체가 회전함에 따라 AABB의 크기는 계속 변화한다.
* AABB 구조로 표현된 객체를 트리 구조로 관리할 수 있다.

## BVH (Bounding Volume Hiearachy)
* BVH는 기본적으로 계층 구조를 가진다.
* 보통 이진 트리 구조로 표현된다. 각 내부 노드는 두 개의 자식 노드를 가질 수 있다. 
* root node, internal node, leaf node 이렇게 3가지 노드로 구성된다.
* 각각의 노드는 가상의 경계 상자인 BV(Bounding Volume)을 가지는데, BV는 노드에 속한 객체들의 경계를 둘러싸는 최소한의 사각형이나 상자를 의미한다.
* BVH를 구성하는 기하 개체가 움직였을 때, 해당 BV를 재계산하고 여전히 부모 BV에 포함되는지 검사하고, 만약 포함되지 않으면 해당 오브젝트의 노트는 삭제하고 부모 BV는 재계산해야한다. 그리고 재계산 했던 BV 노드를 BVH root부터 시작해서 다시 삽입해야 한다.
* Ray Tracing, collision detection 등에 사용된다.

## Tools
* 





