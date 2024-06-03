#include "TRS/TRSGeode.h"

#include <iostream>

#include "TRS/TRSDefGL.h"
#include "TRS/TRSMesh.h"
#include "TRS/TRSShader.h"
#include "TRS/TRSTexture.h"
#include "TRS/TRSStateSet.h"


TRSGeode::TRSGeode()
    : TRSNode()
    , m_polygonMode(GL_FILL)
{

}

TRSGeode::~TRSGeode()
{

}

void TRSGeode::draw()
{
    drawInternal();
}

void TRSGeode::setMeshData(const std::vector<TRSPoint>& vertexs, const std::vector<TRSVec3>& normals, const std::vector<TRSVec2>& UVs, const std::vector<TRSVec3>& colors)
{
    m_pMesh->setVertex(vertexs);
    m_pMesh->setNormal(normals);
    m_pMesh->setUV(UVs);
    m_pMesh->setColor(colors);
}

void TRSGeode::setIndexArray(const std::vector<unsigned int>& indices)
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

std::string TRSGeode::debugInfo()
{
    TRSShader* pShader = m_pStateSet->getShader();
    TRSTexture* pTexture = m_pStateSet->getTexture();
    std::string strDebugInfo;
    strDebugInfo += pShader->debugInfo();
    strDebugInfo += pTexture->debugInfo();
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
