#include "Geometry\TRSGeometry.h"
#include "DataModel\TRSMesh.h"


TRSGeometry::TRSGeometry()
{

}

TRSGeometry::~TRSGeometry()
{
    if (m_mesh)
    {
        delete m_mesh;
    }
}

void TRSGeometry::tessellation()
{
    if (!m_mesh)
    {
        m_mesh = new TRSMesh;
    }
    // overwrite by subclass
    m_dirty = false;
}

TRSMesh* TRSGeometry::getMesh()
{
    if (m_dirty)
    {
        tessellation();
    }
    return m_mesh;
}

void TRSGeometry::modified()
{
    m_dirty = true;
}
