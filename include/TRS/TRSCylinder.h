#pragma once
#include "TRS/TRSExport.h"
#include "TRS/TRSGeometry.h"
#include "TRS/TRSVector.h"

class TRS_EXPORT TRSCylinder : public TRSGeometry
{
public:
    TRSCylinder();

    ~TRSCylinder();

    // direction is upright
    void setCentRadius(const TRSVec3& btmCenter, float height, float radius);

    void setTwoCentRadius(const TRSVec3& cent0, const TRSVec3& cent1, float radius);

    void  setResolution(int reso);

    void tessellation() override;

    static void computeLocalXY(const TRSVec3& localz, TRSVec3& localx, TRSVec3& localy);

protected:
    TRSVec3 m_cent0;
    TRSVec3 m_cent1;
    float m_radius = 1.0f;
    int m_resolution = 20;
};
