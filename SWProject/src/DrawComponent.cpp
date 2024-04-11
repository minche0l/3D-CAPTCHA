#include "DrawComponent.h"
#include "gl/freeglut.h"
#include "GCAABBBVH.h"

#include "pmp/io/io.h"
#include "pmp/algorithms//normals.h"
#include "pmp/bounding_box.h"

#include <queue>

using namespace std;
using namespace pmp;

BVH* bvh = nullptr;
vector<Face> faces;
int level = 10;

void DrawAABB(BoundingBox box)
{
	Point minPoint = box.min();
	Point maxPoint = box.max();

	// Draw bottom rectangle
	glBegin(GL_LINE_LOOP);
	glVertex3d(minPoint[0], minPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], minPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], minPoint[1], maxPoint[2]);
	glVertex3d(minPoint[0], minPoint[1], maxPoint[2]);
	glEnd();

	// Draw top rectangle
	glBegin(GL_LINE_LOOP);
	glVertex3d(minPoint[0], maxPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], maxPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], maxPoint[1], maxPoint[2]);
	glVertex3d(minPoint[0], maxPoint[1], maxPoint[2]);
	glEnd();

	// Draw vertical lines
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

void DrawBV(int level, BVH* bvh)
{
	std::queue<BV*> q;

	for (auto& bv : bvh->roots)
	{
		q.push(&bv);
	}

	while (!q.empty()) {
		auto& bv = q.front();

		if (bv->level == level || level < 0)
			DrawAABB((*bv).box);

		if (bv->left_ != nullptr)
			q.push(bv->left_);

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
}

void DrawComponent::Draw()
{
	// mesh의 face를 방문
	// face의 vertex를 방문
	// vertex의 3d position을 그리기

	auto normals = mesh.vertex_property<pmp::Normal>("v:normal");

	for (auto f : mesh.faces())
	{
		glBegin(GL_TRIANGLES);
		for (auto v : mesh.vertices(f))
		{
			auto p = mesh.position(v); // x y z
			auto n = normals[v];
			glNormal3d(n[0], n[1], n[2]);
			glVertex3d(p[0], p[1], p[2]);
		}
		glEnd();
	}

	DrawBV(level, bvh);
}