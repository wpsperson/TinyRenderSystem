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

    TRSCube(bool atCenter);

    ~TRSCube();

    void tessellation() override;

    void setLength(float len);

    void setWidth(float w);

    void setHeight(float h);

    void setOriginAtCenter(bool atCenter);

protected:
    float m_length = 1.0;   // x direction
    float m_width = 1.0;    // y direction
    float m_height = 1.0;   // z direction
    TRSPoint m_origin;
    bool m_origAtCenter = false;
};
