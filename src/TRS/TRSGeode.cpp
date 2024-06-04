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
    m_pMesh = std::make_shared<TRSMesh>();
}

TRSGeode::~TRSGeode()
{

}

void TRSGeode::draw()
{
    drawInternal();
}

TRSBox TRSGeode::boundingBox() const
{
    return m_pMesh->boundingBox();
}

void TRSGeode::setMesh(TRSMesh* mesh)
{
    m_pMesh = std::make_shared<TRSMesh>(*mesh);
}

TRSMesh* TRSGeode::getMesh() const
{
    return m_pMesh.get();
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
    m_pMesh->setVertex(vertexs);
}

void TRSGeode::setShadedNormals(const std::vector<TRSVec3>& normals)
{
    m_pMesh->setNormal(normals);
}

void TRSGeode::setShadedUVs(const std::vector<TRSVec2>& UVs)
{
    m_pMesh->setUV(UVs);
}

void TRSGeode::setShadedIndice(const std::vector<unsigned int>& indices)
{
    m_pMesh->setIndices(indices);
}

void TRSGeode::setActive()
{
    m_pMesh->bindMesh();
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
    if (DrawType::PATCHES == m_pMesh->getDrawType())
    {
        glPatchParameteri(GL_PATCH_VERTICES, m_pMesh->getDrawParam());
    }

    if (m_polygonMode != GL_FILL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
    }
}

void TRSGeode::drawInternal()
{
    int nElementCount = m_pMesh->getElementCount();
    GLenum OpenGLDrawType = static_cast<GLenum>(m_pMesh->getDrawType());
    if (nElementCount > 0)
    {
        glDrawElements(OpenGLDrawType, m_pMesh->getElementCount(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(OpenGLDrawType, 0, m_pMesh->getDrawCount());
    }
}

void TRSGeode::postProcess()
{
    if (m_polygonMode != GL_FILL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
