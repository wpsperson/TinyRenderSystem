#pragma once
#include "Core\TRSExport.h"
#include "Geometry\TRSGeometry.h"
#include "Math\TRSVector.h"

class TRS_EXPORT TRSGrid : public TRSGeometry
{
public:
    TRSGrid();

    ~TRSGrid();

    void setGrid(float xinterval, float yinterval, int xcount, int ycount);

    void setOriginDir(const TRSVec3& origin, const TRSVec3& upDir);

    void tessellation() override;

protected:
    TRSVec3 m_origin = G_ORIGIN;
    TRSVec3 m_up = G_ZDIR;
    float m_xinterval = 1.0f;
    float m_yinterval = 1.0f;
    int m_xGridCount = 20;
    int m_yGridCount = 20;
};
