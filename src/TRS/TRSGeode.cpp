#include "TRS/TRSGeode.h"

#include <iostream>

#include "TRS/TRSDefGL.h"
#include "TRS/TRSMesh.h"
#include "TRS/TRSShader.h"
#include "TRS/TRSTexture.h"


TRSGeode::TRSGeode()
    : TRSNode()
    , m_polygonMode(GL_FILL)
{
    m_shaded = new TRSMesh;
}

TRSGeode::~TRSGeode()
{
    delete m_shaded;
    if (m_wireframe)
    {
        delete m_wireframe;
    }
    if (m_points)
    {
        delete m_points;
    }
}

void TRSGeode::draw()
{
    drawInternal();
}

TRSBox TRSGeode::boundingBox() const
{
    return m_shaded->boundingBox();
}

void TRSGeode::setMesh(TRSMesh* mesh)
{
    m_shaded->copyMesh(mesh);
}

TRSMesh* TRSGeode::getMesh() const
{
    return m_shaded;
}

void TRSGeode::setTexture(std::shared_ptr<TRSTexture> texture)
{
    m_pTexture = texture;
}

TRSTexture* TRSGeode::getTexture() const
{
    return m_pTexture.get();
}

void TRSGeode::setShadedVertice(const std::vector<TRSPoint>& vertexs)
{
    m_shaded->setVertex(vertexs);
}

void TRSGeode::setShadedNormals(const std::vector<TRSVec3>& normals)
{
    m_shaded->setNormal(normals);
}

void TRSGeode::setShadedUVs(const std::vector<TRSVec2>& UVs)
{
    m_shaded->setUV(UVs);
}

void TRSGeode::setShadedIndice(const std::vector<unsigned int>& indices)
{
    m_shaded->setIndices(indices);
}

void TRSGeode::setActive()
{
    m_shaded->bindMesh();
}

void TRSGeode::setPolygonMode(int polyMode)
{
    m_polygonMode = polyMode;
}

void TRSGeode::setRenderMode(int renderMode)
{
    m_renderMode = renderMode;
}

void TRSGeode::addRenderMode(RenderMode mode)
{
    if (!hasRenderMode(mode))
    {
        m_renderMode = (m_renderMode | mode);
    }
}

void TRSGeode::removeRenderMode(RenderMode mode)
{
    if (hasRenderMode(mode))
    {
        m_renderMode = (m_renderMode ^ mode);
    }
}

bool TRSGeode::hasRenderMode(RenderMode mode)
{
    return m_renderMode & mode;
}

std::string TRSGeode::debugInfo()
{
    std::string strDebugInfo;
    if (TRSTexture* pTexture = getTexture())
    {
        strDebugInfo += pTexture->debugInfo();
    }
    return strDebugInfo;
}

void TRSGeode::preProcess()
{
    if (DrawType::PATCHES == m_shaded->getDrawType())
    {
        glPatchParameteri(GL_PATCH_VERTICES, m_shaded->getDrawParam());
    }

    if (m_polygonMode != GL_FILL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
    }
}

void TRSGeode::drawInternal()
{
    int nElementCount = m_shaded->getElementCount();
    GLenum OpenGLDrawType = static_cast<GLenum>(m_shaded->getDrawType());
    if (nElementCount > 0)
    {
        glDrawElements(OpenGLDrawType, m_shaded->getElementCount(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(OpenGLDrawType, 0, m_shaded->getDrawCount());
    }
}

void TRSGeode::postProcess()
{
    if (m_polygonMode != GL_FILL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
