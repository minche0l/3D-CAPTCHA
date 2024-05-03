#include "DrawComponent.h"
#include "gl/freeglut.h"
#include "BVH.h"

#include "pmp/io/io.h"
#include "pmp/algorithms//normals.h"
#include "pmp/bounding_box.h"

#include <queue>

using namespace std;
using namespace pmp;

BVH* bvh = nullptr;
vector<Face> faces;
int lv;

int treeHeight(BV& node)
{
	if (node.IsLeaf())
	{
		return 0;
	}
	else
	{
		// 왼쪽 서브트리와 오른쪽 서브트리의 높이 계산
		int leftHeight = 0;
		int rightHeight = 0;

		if (node.left_ != nullptr)
		{
			leftHeight = treeHeight(*node.left_);
		}

		if (node.right_ != nullptr)
		{
			rightHeight = treeHeight(*node.right_);
		}

		return 1 + max(leftHeight, rightHeight);
	}
}

int maxHeight(BVH& bvh)
{
	int maxHeight = 0;

	// 각 루트 노드마다 트리의 높이를 계산하고 가장 큰 값을 선택
	for (size_t i = 0; i < bvh.roots.size(); ++i)
	{
		int height = treeHeight(bvh.roots[i]);
		maxHeight = max(maxHeight, height);
	}

	return maxHeight;
}

// AABB Drawing
void DrawAABB(BoundingBox box)
{
	Point minPoint = box.min();
	Point maxPoint = box.max();

	// 아랫면 Drawing
	glBegin(GL_LINE_LOOP);
	glVertex3d(minPoint[0], minPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], minPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], minPoint[1], maxPoint[2]);
	glVertex3d(minPoint[0], minPoint[1], maxPoint[2]);
	glEnd();

	// 윗면 Drawing
	glBegin(GL_LINE_LOOP);
	glVertex3d(minPoint[0], maxPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], maxPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], maxPoint[1], maxPoint[2]);
	glVertex3d(minPoint[0], maxPoint[1], maxPoint[2]);
	glEnd();

	// 수직선 Drawing
	glBegin(GL_LINES);
	glVertex3d(minPoint[0], minPoint[1], minPoint[2]);
	glVertex3d(minPoint[0], maxPoint[1], minPoint[2]);

	glVertex3d(maxPoint[0], minPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], maxPoint[1], minPoint[2]);

	glVertex3d(maxPoint[0], minPoint[1], maxPoint[2]);
	glVertex3d(maxPoint[0], maxPoint[1], maxPoint[2]);

	glVertex3d(minPoint[0], minPoint[1], maxPoint[2]);
	glVertex3d(minPoint[0], maxPoint[1], maxPoint[2]);
	glEnd();
}

// BV와 BV를 감싸는 AABB 그리기
void DrawBVbyLevel(BVH* bvh, SurfaceMesh& mesh, auto& normals, int lv)
{
	std::queue<BV*> q;

	for (auto& bv : bvh->roots)
	{
		q.push(&bv);
	}

	while (!q.empty()) {
		auto& bv = q.front();

		// lv 레벨 노드를 Drawing
		if (bv->level == lv || lv < 0)
		{
			for (auto& f : bv->faces)
			{
				glBegin(GL_TRIANGLES);
				for (auto v : mesh.vertices(f))
				{
					auto& p = mesh.position(v);
					auto& n = normals[v];
					glNormal3d(n[0], n[1], n[2]);
					glVertex3d(p[0], p[1], p[2]);
				}
				glEnd();
			}

			//BV를 감싸는 AABB Drawing
			DrawAABB(bv->box);
		}

		if (bv->left_ != nullptr)
			q.push(bv->left_);
		if (bv->mid_ != nullptr)
			q.push(bv->mid_);
		if (bv->right_ != nullptr)
			q.push(bv->right_);

		q.pop();

	}
}

void DrawComponent::Init()
{
	// load the model
	pmp::read(mesh, "models\\kitten.obj");
	pmp::vertex_normals(mesh);
	std::cout << "#f " << mesh.n_faces() << " #v " << mesh.n_vertices() << std::endl;

	for (auto f : mesh.faces())
	{
		faces.push_back(f);
	}

	bvh = new BVH(faces, mesh);
	lv = maxHeight(*bvh);
	cout << lv << endl;
}

void DrawComponent::Draw()
{
	// mesh의 face를 방문
	// face의 vertex를 방문
	// vertex의 3d position을 그리기

	auto normals = mesh.vertex_property<pmp::Normal>("v:normal");

	// 전체 모델 렌더링
	//for (auto f : mesh.faces())
	//{
	//	glBegin(GL_TRIANGLES);
	//	for (auto v : mesh.vertices(f))
	//	{
	//		auto p = mesh.position(v); // x y z
	//		auto n = normals[v];
	//		glNormal3d(n[0], n[1], n[2]);
	//		glVertex3d(p[0], p[1], p[2]);
	//	}
	//	glEnd();
	//}

	DrawBVbyLevel(bvh, mesh, normals, lv * 0.8);
}