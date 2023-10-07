#include "Geometry/TRSGrid.h"
#include <vector>
#include "DataModel/TRSMesh.h"

TRSGrid::TRSGrid()
{
}

TRSGrid::~TRSGrid()
{
}

void TRSGrid::setGrid(float xinterval, float yinterval, int xcount, int ycount)
{
    m_xinterval = xinterval;
    m_yinterval = yinterval;
    m_xGridCount = xcount;
    m_yGridCount = ycount;
}

void TRSGrid::setOriginDir(const TRSVec3& origin, const TRSVec3& upDir)
{
    m_origin = origin;
    m_up = upDir;
}

// I just implement default origin and dir grid so far.
void TRSGrid::tessellation()
{
    TRSGeometry::tessellation();
    int ptCount = 2 * (m_xGridCount + 1) + 2 * (m_yGridCount);
    std::vector<TRSPoint> points;
    points.reserve(ptCount);

    float left = -(m_xGridCount * m_xinterval) / 2;
    float righ = (m_xGridCount * m_xinterval) / 2;
    float bttm = -(m_yGridCount * m_yinterval) / 2;
    float topp = (m_yGridCount * m_yinterval) / 2;
    // first draw horizon lines

    TRSPoint start;
    TRSPoint end;
    for (int ii = 0; ii <= m_xGridCount; ii++)
    {
        float xpos = left + ii * m_xinterval;
        start[0] = xpos;
        start[1] = bttm;
        start[2] = 0;
        end[0] = xpos;
        end[1] = topp;
        end[2] = 0;
        points.emplace_back(start);
        points.emplace_back(end);
    }
    for (int ii = 0; ii <= m_yGridCount; ii++)
    {
        float ypos = bttm + ii * m_xinterval;
        start[0] = left;
        start[1] = ypos;
        start[2] = 0;
        end[0] = righ;
        end[1] = ypos;
        end[2] = 0;
        points.emplace_back(start);
        points.emplace_back(end);
    }
    m_mesh->setVertex(points);
}
