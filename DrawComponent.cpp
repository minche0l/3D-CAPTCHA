#include "DrawComponent.h"
#include "gl/freeglut.h"
#include"pmp/algorithms/normals.h"
#include"pmp/io/io.h"
#include<algorithm>

void DrawComponent::Init()
{
	//model load
	pmp::read(mesh, "models\\bear_bis.obj");

	pmp::vertex_normals(mesh);

	std::cout << "#f" << mesh.n_faces() << "#v" << mesh.n_vertices() << std::endl;

	
}

void DrawComponent::Draw()
{	
	//mesh의 face를 방문 //triangle
	//face의 vertex 방문 // 최대 3개
	//vertex의 3d position 그리기

	auto normals = mesh.vertex_property<pmp::Normal>("v:normal");

	for (auto f : mesh.faces()) {
		glBegin(GL_TRIANGLES);
		for (auto v : mesh.vertices(f)) {
			auto p = mesh.position(v);
			auto n = normals[v];
			glNormal3d(n[0], n[1], n[2]);
			glVertex3d(p[0], p[1], p[2]);
		}
		glEnd();
	}
    CalculateAABB();
    DrawAABB();

}

void DrawComponent::CalculateAABB()
{
    // Initialize min and max points with the first vertex
    pmp::Point minPoint = mesh.position(*mesh.vertices_begin());
    pmp::Point maxPoint = minPoint;

    // Iterate through all vertices to find the min and max points
    for (auto v : mesh.vertices())
    {
        const pmp::Point& position = mesh.position(v);
        // Update min and max points for each axis
        for (int i = 0; i < 3; ++i) // 3 for x, y, z
        {
            minPoint[i] = std::min(minPoint[i], position[i]);
            maxPoint[i] = std::max(maxPoint[i], position[i]);
        }
    }

    // Store the AABB
    aabbMin = minPoint;
    aabbMax = maxPoint;
}

void DrawComponent::DrawAABB()
{
    // Draw AABB
    glBegin(GL_LINES);
    // Bottom rectangle
    glVertex3d(aabbMin[0], aabbMin[1], aabbMin[2]);
    glVertex3d(aabbMax[0], aabbMin[1], aabbMin[2]);

    glVertex3d(aabbMax[0], aabbMin[1], aabbMin[2]);
    glVertex3d(aabbMax[0], aabbMax[1], aabbMin[2]);

    glVertex3d(aabbMax[0], aabbMax[1], aabbMin[2]);
    glVertex3d(aabbMin[0], aabbMax[1], aabbMin[2]);

    glVertex3d(aabbMin[0], aabbMax[1], aabbMin[2]);
    glVertex3d(aabbMin[0], aabbMin[1], aabbMin[2]);

    // Top rectangle
    glVertex3d(aabbMin[0], aabbMin[1], aabbMax[2]);
    glVertex3d(aabbMax[0], aabbMin[1], aabbMax[2]);

    glVertex3d(aabbMax[0], aabbMin[1], aabbMax[2]);
    glVertex3d(aabbMax[0], aabbMax[1], aabbMax[2]);

    glVertex3d(aabbMax[0], aabbMax[1], aabbMax[2]);
    glVertex3d(aabbMin[0], aabbMax[1], aabbMax[2]);

    glVertex3d(aabbMin[0], aabbMax[1], aabbMax[2]);
    glVertex3d(aabbMin[0], aabbMin[1], aabbMax[2]);

    // Connecting lines
    glVertex3d(aabbMin[0], aabbMin[1], aabbMin[2]);
    glVertex3d(aabbMin[0], aabbMin[1], aabbMax[2]);

    glVertex3d(aabbMax[0], aabbMin[1], aabbMin[2]);
    glVertex3d(aabbMax[0], aabbMin[1], aabbMax[2]);

    glVertex3d(aabbMax[0], aabbMax[1], aabbMin[2]);
    glVertex3d(aabbMax[0], aabbMax[1], aabbMax[2]);

    glVertex3d(aabbMin[0], aabbMax[1], aabbMin[2]);
    glVertex3d(aabbMin[0], aabbMax[1], aabbMax[2]);
    glEnd();
}
