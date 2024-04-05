/*!  
 *@brief    Geometry Base
 *@author   wangps
 *@date     2022-6-28
 */

#pragma once
#include "TRS/TRSExport.h"

class TRSMesh;

class TRS_EXPORT TRSGeometry
{
public:
    TRSGeometry();

    virtual ~TRSGeometry();

    virtual void tessellation();

    TRSMesh* getMesh();

    void modified();

protected:
    TRSMesh* m_mesh = nullptr;
    bool m_dirty = true;
};
