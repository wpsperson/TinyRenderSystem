/*!  
 *@brief    
 *@author   wangps
 *@date     2022-6-28
 */

#pragma once
#include "Core\TRSExport.h"
#include "Geometry\TRSGeometry.h"
#include "Math\TRSVector.h"

class TRS_EXPORT TRSCube : public TRSGeometry
{
public:
    TRSCube();

    ~TRSCube();

    void tessellation() override;

    void setTwoPos(const TRSVec3& first, const TRSVec3& second);

    void setStartDim(const TRSVec3& start, float length, float width, float height);

    void setCenterDim(const TRSVec3& center, float length, float width, float height);


protected:
    float m_x0 = -0.5f;
    float m_x1 = 0.5f;
    float m_y0 = -0.5f;
    float m_y1 = 0.5f;
    float m_z0 = -0.5f;
    float m_z1 = 0.5f;
};
