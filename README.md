# 3D CAPTCHA using Torus patch

## AABB (Axis - Aligned Bounding Box)
객체나 모델의 경계를 둘러 싸는 가장 간단한 형태의 박스
AABB 구조로 표현된 객체를 트리 구조로 관리할 수 있습니다.

## AABB Tree

## Torus
원을 삼차원 공간 상에서 원을 포함하는 평면위의 직선 축으로 회전하여 만든 회전면
쉽게 말해 도넛 모양의 곡면이라고 할 수 있습니다.

## Torus Patch
3차원 공간에서 자유 곡면을 정밀하게 근사하는 데 사용되는 기하하적 모델링 기법
토러스를 기본 단위로 사용해 자유 곡면을 표현합니다.

1. 알고리즘
   *자유 곡면에 대한 미분 기하 계산
   자유 곡면 S(u,v) 와 S<sub>u</sub>, S<sub>v</sub> (u,v에 대해 1차 미분한 값) , S<sub>uu</sub>, S<sub>uv</sub>, S<sub>vv</sub> (u,v에 대해 2차 미분한 값) 이/가 주어졌을 때 제 1차 기본 형식의 값 E,F,G,N(단위 법선 벡터), 제 2차 기본 형식의 값 L,M,N 다음과 같이 정의됩니다.
   E = <S<sub>u</sub>,S<sub>v</sub>>

