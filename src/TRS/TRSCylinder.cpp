#include "TRS/TRSCylinder.h"
#include "TRS/TRSMesh.h"
#include <vector>

TRSCylinder::TRSCylinder()
{
    setCentRadius(G_ORIGIN, 1.0f, 0.5f);
}

TRSCylinder::~TRSCylinder()
{
}

void TRSCylinder::setCentRadius(const TRSVec3& btmCenter, float height, float radius)
{
    m_cent0 = btmCenter;
    m_cent1 = TRSVec3(btmCenter[0], btmCenter[1], btmCenter[2] + height);
    m_radius = radius;
    modified();
}

void TRSCylinder::setTwoCentRadius(const TRSVec3& cent0, const TRSVec3& cent1, float radius)
{
    m_cent0 = cent0;
    m_cent1 = cent1;
    m_radius = radius;
    modified();
}

void TRSCylinder::setResolution(int reso)
{
    m_resolution = reso;
    modified();
}

void TRSCylinder::tessellation()
{
    TRSGeometry::tessellation();
    std::vector<TRSPoint> vertices;
    std::vector<TRSVec2> texCoords;
    std::vector<TRSVec3> normals;
    std::vector<unsigned int> indices;

    TRSVec3 localz = (m_cent1 - m_cent0);
    localz.normalize();
    TRSVec3 localx;
    TRSVec3 localy;
    computeLocalXY(localz, localx, localy);
    TRSPoint pt;
    TRSPoint norm;
    TRSVec2 uv;
    double theta = 0.0;
    double theta_step = (kMPI * 2) / m_resolution;
    float zLength = m_cent0.distance(m_cent1);
    int seg_count = static_cast<int>(m_resolution * zLength / (m_radius * kMPI * 2));
    seg_count = std::max(1, seg_count);
    double stepz = zLength / seg_count;

    int pt_count = (seg_count + 1) * (m_resolution + 1);
    vertices.reserve(pt_count);
    texCoords.reserve(pt_count);
    normals.reserve(pt_count);

    for (int jj = 0; jj <= seg_count; jj++)
    {
        TRSPoint center = m_cent0 + localz * static_cast<float>(stepz * jj);
        float vv = static_cast<float>((stepz * jj) / zLength);
        for (int ii = 0; ii <= m_resolution; ii++)
        {
            theta = ii * theta_step;
            norm = localx * static_cast<float>(cos(theta)) + localy * static_cast<float>(sin(theta));
            norm.normalize();
            pt = center + norm * m_radius;
            uv[0] = static_cast<float>(ii) / m_resolution;
            uv[1] = vv;

            vertices.emplace_back(pt);
            normals.emplace_back(norm);
            texCoords.emplace_back(uv);
        }
    }

    indices.reserve(seg_count * m_resolution * 6);
    for (int jj = 0; jj < seg_count; jj++)
    {
        int base = jj * (m_resolution + 1);
        int next = (jj + 1) * (m_resolution + 1);
        for (int ii = 0; ii < m_resolution; ii++)
        {
            indices.emplace_back(base + ii);
            indices.emplace_back(base + ii + 1);
            indices.emplace_back(next + ii);

            indices.emplace_back(base + ii + 1);
            indices.emplace_back(next + ii + 1);
            indices.emplace_back(next + ii);
        }
    }

    m_mesh->setVertex(vertices);
    m_mesh->setUV(texCoords);
    m_mesh->setNormal(normals);
    m_mesh->setIndices(indices);
}

void TRSCylinder::computeLocalXY(const TRSVec3& localz, TRSVec3& localx, TRSVec3& localy)
{
    double dotx = std::abs(localz.dot(G_XDIR));
    double doty = std::abs(localz.dot(G_YDIR));
    double dotz = std::abs(localz.dot(G_ZDIR));
    if (dotx >= doty && dotx >= dotz)
    {
        // localx -> G_YDIR; localy -> G_ZDIR
        localx = G_ZDIR.cross(localz);
        localx.normalize();
        localy = localz.cross(localx);
    }
    else if (doty >= dotx && doty >= dotz)
    {
        // localx -> G_ZDIR; localy -> G_XDIR
        localx = G_XDIR.cross(localz);
        localx.normalize();
        localy = localz.cross(localx);
    }
    else if (dotz >= dotx && dotz >= doty)
    {
        //  we try to make localx closed to G_XDIR and localy closed to G_YDIR
        localx = G_YDIR.cross(localz);
        localx.normalize();
        localy = localz.cross(localx);
    }
}
