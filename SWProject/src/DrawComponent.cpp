#include "DrawComponent.h"
#include "gl/freeglut.h"

#include "pmp/io/io.h"
#include "pmp/algorithms//normals.h"
#include "pmp/bounding_box.h"

void DrawComponent::Init()
{
	// load model
	pmp::read(mesh, "models\\bear_bis.obj");
	pmp::vertex_normals(mesh);
	std::cout << "#f " << mesh.n_faces() << " #v " << mesh.n_vertices() << std::endl;
}

void DrawComponent::Draw()
{
	// mesh의 face를 방문
	// face의 vertex를 방문
	// vertex의 3d position을 그리기

	pmp::BoundingBox boundingBox = pmp::BoundingBox();

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

	for (auto f : mesh.faces())
	{
		for (auto v : mesh.vertices(f))
		{
			auto p = mesh.position(v);
			boundingBox += p;
		}
	}

	pmp::Point minPoint = boundingBox.min();
	pmp::Point maxPoint = boundingBox.max();

	glBegin(GL_LINES);
	glVertex3d(minPoint[0], minPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], minPoint[1], minPoint[2]);

	glVertex3d(maxPoint[0], minPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], minPoint[1], maxPoint[2]);

	glVertex3d(maxPoint[0], minPoint[1], maxPoint[2]);
	glVertex3d(minPoint[0], minPoint[1], maxPoint[2]);

	glVertex3d(minPoint[0], minPoint[1], maxPoint[2]);
	glVertex3d(minPoint[0], minPoint[1], minPoint[2]);

	// Top rectangle
	glVertex3d(minPoint[0], maxPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], maxPoint[1], minPoint[2]);

	glVertex3d(maxPoint[0], maxPoint[1], minPoint[2]);
	glVertex3d(maxPoint[0], maxPoint[1], maxPoint[2]);

	glVertex3d(maxPoint[0], maxPoint[1], maxPoint[2]);
	glVertex3d(minPoint[0], maxPoint[1], maxPoint[2]);

	glVertex3d(minPoint[0], maxPoint[1], maxPoint[2]);
	glVertex3d(minPoint[0], maxPoint[1], minPoint[2]);

	// Vertical lines
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