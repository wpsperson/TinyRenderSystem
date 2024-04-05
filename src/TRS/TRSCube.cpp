#include "TRS/TRSCube.h"
#include "TRS/TRSMesh.h"
#include <vector>


TRSCube::TRSCube()
{

}

TRSCube::~TRSCube()
{

}

void TRSCube::tessellation()
{
    TRSGeometry::tessellation();
    std::vector<TRSPoint> vertices;
    std::vector<TRSVec2> texCoords;
    std::vector<TRSVec3> normals;
    TRSPoint pt0(m_x0, m_y0, m_z0);
    TRSPoint pt1(m_x1, m_y0, m_z0);
    TRSPoint pt2(m_x1, m_y1, m_z0);
    TRSPoint pt3(m_x0, m_y1, m_z0);
    TRSPoint pt4(m_x0, m_y0, m_z1);
    TRSPoint pt5(m_x1, m_y0, m_z1);
    TRSPoint pt6(m_x1, m_y1, m_z1);
    TRSPoint pt7(m_x0, m_y1, m_z1);
    TRSVec3 normBottom(0, 0, -1);
    TRSVec3 normTop(0, 0, 1);
    TRSVec3 normFront(0, -1, 0);
    TRSVec3 normBack(0, 1, 0);
    TRSVec3 normLeft(-1, 0, 0);
    TRSVec3 normRight(1, 0, 0);
    // there are six face:
    // bottom face: 0 1 2 3 (3 2 1 0)
    vertices.push_back(pt3); texCoords.push_back(TRSVec2(0, 0)); normals.push_back(normBottom);
    vertices.push_back(pt2); texCoords.push_back(TRSVec2(1, 0)); normals.push_back(normBottom);
    vertices.push_back(pt0); texCoords.push_back(TRSVec2(0, 1)); normals.push_back(normBottom);
    vertices.push_back(pt0); texCoords.push_back(TRSVec2(0, 1)); normals.push_back(normBottom);
    vertices.push_back(pt2); texCoords.push_back(TRSVec2(1, 0)); normals.push_back(normBottom);
    vertices.push_back(pt1); texCoords.push_back(TRSVec2(1, 1)); normals.push_back(normBottom);
    // top face: 4 5 6 7
    vertices.push_back(pt4); texCoords.push_back(TRSVec2(0, 0)); normals.push_back(normTop);
    vertices.push_back(pt5); texCoords.push_back(TRSVec2(1, 0)); normals.push_back(normTop);
    vertices.push_back(pt7); texCoords.push_back(TRSVec2(0, 1)); normals.push_back(normTop);
    vertices.push_back(pt7); texCoords.push_back(TRSVec2(0, 1)); normals.push_back(normTop);
    vertices.push_back(pt5); texCoords.push_back(TRSVec2(1, 0)); normals.push_back(normTop);
    vertices.push_back(pt6); texCoords.push_back(TRSVec2(1, 1)); normals.push_back(normTop);
    // front face: 0 1 5 4
    vertices.push_back(pt0); texCoords.push_back(TRSVec2(0, 0)); normals.push_back(normFront);
    vertices.push_back(pt1); texCoords.push_back(TRSVec2(1, 0)); normals.push_back(normFront);
    vertices.push_back(pt4); texCoords.push_back(TRSVec2(0, 1)); normals.push_back(normFront);
    vertices.push_back(pt4); texCoords.push_back(TRSVec2(0, 1)); normals.push_back(normFront);
    vertices.push_back(pt1); texCoords.push_back(TRSVec2(1, 0)); normals.push_back(normFront);
    vertices.push_back(pt5); texCoords.push_back(TRSVec2(1, 1)); normals.push_back(normFront);

    // back face: 2 3 7 6
    vertices.push_back(pt2); texCoords.push_back(TRSVec2(0, 0)); normals.push_back(normBack);
    vertices.push_back(pt3); texCoords.push_back(TRSVec2(1, 0)); normals.push_back(normBack);
    vertices.push_back(pt6); texCoords.push_back(TRSVec2(0, 1)); normals.push_back(normBack);
    vertices.push_back(pt6); texCoords.push_back(TRSVec2(0, 1)); normals.push_back(normBack);
    vertices.push_back(pt3); texCoords.push_back(TRSVec2(1, 0)); normals.push_back(normBack);
    vertices.push_back(pt7); texCoords.push_back(TRSVec2(1, 1)); normals.push_back(normBack);

    // left face: 3 0 4 7
    vertices.push_back(pt3); texCoords.push_back(TRSVec2(0, 0)); normals.push_back(normLeft);
    vertices.push_back(pt0); texCoords.push_back(TRSVec2(1, 0)); normals.push_back(normLeft);
    vertices.push_back(pt7); texCoords.push_back(TRSVec2(0, 1)); normals.push_back(normLeft);
    vertices.push_back(pt7); texCoords.push_back(TRSVec2(0, 1)); normals.push_back(normLeft);
    vertices.push_back(pt0); texCoords.push_back(TRSVec2(1, 0)); normals.push_back(normLeft);
    vertices.push_back(pt4); texCoords.push_back(TRSVec2(1, 1)); normals.push_back(normLeft);

    // right face: 1 2 6 5
    vertices.push_back(pt1); texCoords.push_back(TRSVec2(0, 0)); normals.push_back(normRight);
    vertices.push_back(pt2); texCoords.push_back(TRSVec2(1, 0)); normals.push_back(normRight);
    vertices.push_back(pt5); texCoords.push_back(TRSVec2(0, 1)); normals.push_back(normRight);
    vertices.push_back(pt5); texCoords.push_back(TRSVec2(0, 1)); normals.push_back(normRight);
    vertices.push_back(pt2); texCoords.push_back(TRSVec2(1, 0)); normals.push_back(normRight);
    vertices.push_back(pt6); texCoords.push_back(TRSVec2(1, 1)); normals.push_back(normRight);

    m_mesh->setVertex(vertices);
    m_mesh->setUV(texCoords);
    m_mesh->setNormal(normals);
}

void TRSCube::setTwoPos(const TRSVec3& first, const TRSVec3& second)
{
    m_x0 = std::min(first[0], second[0]);
    m_x1 = std::max(first[0], second[0]);
    m_y0 = std::min(first[1], second[1]);
    m_y1 = std::max(first[1], second[1]);
    m_z0 = std::min(first[2], second[2]);
    m_z1 = std::max(first[2], second[2]);
    modified();
}

void TRSCube::setStartDim(const TRSVec3& start, float length, float width, float height)
{
    m_x0 = start[0];
    m_y0 = start[1];
    m_z0 = start[2];
    m_x1 = m_x0 + length;
    m_y1 = m_y0 + width;
    m_z1 = m_z0 + height;
    modified();
}

void TRSCube::setCenterDim(const TRSVec3& center, float length, float width, float height)
{
    m_x0 = center[0] - length / 2;
    m_y0 = center[1] - width / 2;
    m_z0 = center[2] - height / 2;
    m_x1 = center[0] + length / 2;
    m_y1 = center[1] + width / 2;
    m_z1 = center[2] + height / 2;
    modified();
}

