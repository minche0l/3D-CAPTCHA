#pragma once
#include "pmp/types.h"
#include "DrawComponent.h"
#include "pmp/bounding_box.h"
#include "pmp/algorithms/normals.h"

using namespace pmp;
using namespace std;

// 한 개의 면(face)을 AABB로 감싸는 함수
BoundingBox AABB(Face face, SurfaceMesh& mesh)
{
    BoundingBox box;
    for (auto v : mesh.vertices(face))
    {
        auto p = mesh.position(v);
        box += p;
    }
    return box;
}

// BVH 노드
class BV
{
public:
    int level;
    BoundingBox box;
    vector<BoundingBox> boxes;
    vector<Face> faces;
    BV* left_, * right_;
    BV(vector<Face> faces, int lv, SurfaceMesh& mesh);
    void SplitBV();
    bool IsLeaf();
};

// BVH 트리
class BVH
{
private:

public:
    vector<BV> roots;

    BVH(vector<Face> allFaces, SurfaceMesh& mesh);
    vector<vector<Face>> SplitFace(vector<Face> allFaces, SurfaceMesh& mesh);
};

// BV 생성자
inline BV::BV(vector<Face> fcs, int lv, SurfaceMesh& mesh) {
    faces = fcs;
    level = lv;
    left_ = nullptr;
    right_ = nullptr;

    Point minPoint = Point(FLT_MAX, FLT_MAX, FLT_MAX);
    Point maxPoint = Point(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (auto& face : faces)
    {
        BoundingBox faceBox = AABB(face, mesh);

        minPoint = pmp::min(faceBox.min(), minPoint);
        maxPoint = pmp::max(faceBox.max(), maxPoint);
        boxes.push_back(faceBox);
    }

    box = BoundingBox(minPoint, maxPoint);

    auto lengths = maxPoint - minPoint;
    int longestAxis = 0;
    if (lengths[1] > lengths[longestAxis]) longestAxis = 1;
    if (lengths[2] > lengths[longestAxis]) longestAxis = 2;

    // 분할 기준 축
    double splitValue = 0.5 * (minPoint[longestAxis] + maxPoint[longestAxis]);

    vector<Face> leftFaces, rightFaces;

    for (size_t i = 0; i < faces.size(); i++)
    {
        if (boxes[i].center()[longestAxis] < splitValue)
        {
            leftFaces.push_back(faces[i]);      // 기준 축보다 작으면 left
        }
        else
        {
            rightFaces.push_back(faces[i]);     // 기준 축보다 크면 right
        }
    }

    // Balance 유지
    if (!leftFaces.empty() && !rightFaces.empty())
    {
        if (leftFaces.size() > 2)
        {
            left_ = new BV(leftFaces, level + 1, mesh);
        }
        if (rightFaces.size() > 2) {
            right_ = new BV(rightFaces, level + 1, mesh);
        }
    }
}

// 노드(BV)가 리프인지 검사하는 함수
inline bool BV::IsLeaf()
{
    return left_ == nullptr && right_ == nullptr;
}

// BVH 생성자
inline BVH::BVH(vector<Face> allFaces, SurfaceMesh& mesh)
{
    // 분할된 faces를 저장
    vector<vector<Face>> FcsList = SplitFace(allFaces, mesh);

    // faces를 root에 push
    for (auto& faces : FcsList)
    {
        roots.push_back(BV(faces, 0, mesh));
    }
}

// 모델의 모든 face들을 분할하는 함수
inline vector<vector<Face>> BVH::SplitFace(vector<Face> allFaces, SurfaceMesh& mesh)
{
    vector<vector<Face>> fcsList;
    vector<Face> faces;
    vector<Normal> normals;
   

    auto prev_normal = face_normal(mesh, allFaces[0]);    // 이전 face의 법선 벡터
    faces.push_back(allFaces[0]);

    // 모든 face를 순회
    for (int i = 1; i < allFaces.size() - 1; i++) {
        auto normal = face_normal(mesh, allFaces[i]);   // 현재 face의 법선 벡터

        // 현재 법선 벡터와 이전 법선 벡터가 같은 경우(면이 평행한 경우) fcsList에 저장
        if (prev_normal == normal) {
            faces.push_back(allFaces[i]);
            fcsList.push_back(faces);
            faces.clear();
            faces.push_back(allFaces[i]);
        }
        else
        {
            faces.push_back(allFaces[i]);
        }
        
        // 법선 벡터 update
        prev_normal = normal;
    }

    faces.push_back(allFaces[allFaces.size() - 1]);
    fcsList.push_back(faces);
    
    // face의 x좌표에 따라 정렬
    for (auto& faces : fcsList) {
        std::sort(faces.begin(), faces.end(), [&mesh](Face a, Face b)
            {   
                return AABB(a, mesh).center()[0] < AABB(b, mesh).center()[0];
            });
    }

    return fcsList;
}
