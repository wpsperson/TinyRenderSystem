#include "Geometry\TRSCube.h"
#include "DataModel\TRSMesh.h"
#include <vector>


TRSCube::TRSCube()
{

}

TRSCube::TRSCube(bool atCenter)
    :m_origAtCenter(atCenter)
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
    float x = m_origin[0];
    float y = m_origin[1];
    float z = m_origin[2];
    float l = m_length; // in x direction
    float w = m_width; // in y direction
    float h = m_height; // in z direction
    TRSPoint pt0(x, y, z);
    TRSPoint pt1(x + l, y, z);
    TRSPoint pt2(x + l, y + w, z);
    TRSPoint pt3(x, y + w, z);
    TRSPoint pt4(x, y, z + h);
    TRSPoint pt5(x + l, y, z + h);
    TRSPoint pt6(x + l, y + w, z + h);
    TRSPoint pt7(x, y + w, z + h);
    if (m_origAtCenter)
    {
        l /= 2;
        w /= 2;
        h /= 2;
        pt0 = TRSPoint(x - l, y - w, z - h);
        pt1 = TRSPoint(x + l, y - w, z - h);
        pt2 = TRSPoint(x + l, y + w, z - h);
        pt3 = TRSPoint(x - l, y + w, z - h);
        pt4 = TRSPoint(x - l, y - w, z + h);
        pt5 = TRSPoint(x + l, y - w, z + h);
        pt6 = TRSPoint(x + l, y + w, z + h);
        pt7 = TRSPoint(x - l, y + w, z + h);
    }
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

void TRSCube::setLength(float len)
{
    if (m_length != len)
    {
        m_length = len;
        modified();
    }
}

void TRSCube::setWidth(float w)
{
    if (m_width != w)
    {
        m_width = w;
        modified();
    }
}

void TRSCube::setHeight(float h)
{
    if (m_height != h)
    {
        m_height = h;
        modified();
    }
}

void TRSCube::setOriginAtCenter(bool atCenter)
{
    if (m_origAtCenter != atCenter)
    {
        m_origAtCenter = atCenter;
        modified();
    }
}
