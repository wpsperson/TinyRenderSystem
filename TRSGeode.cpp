#include "TRSGeode.h"
#include "TRSVAO.h"
#include <iostream>
#include "TRSShader.h"
#include "TRSTexture.h"
#include "TRSStateSet.h"


TRSGeode::TRSGeode()
    : TRSNode()
    , m_matColor(s_DefaultNodeColor)
    , m_polygonMode(GL_FILL)
{
    m_pVAO = std::make_shared<TRSVAO>();
}

TRSGeode::TRSGeode(const TRSGeode& refObj, bool bSameStateset/* = true*/)
    : TRSNode(), m_matColor(s_DefaultNodeColor)
{
    m_pVAO = std::make_shared<TRSVAO>();
    m_pVAO->genVAO(true);
    m_pVAO->setVBO(refObj.getVAO()->getVBO());
    m_pVAO->setBuffType(refObj.getVAO()->getBufferType());
    m_pVAO->setDrawCount(refObj.getVAO()->getDrawCount());
    m_pVAO->setDrawType(refObj.getVAO()->getDrawType());
    m_pVAO->setDrawParam(refObj.getVAO()->getDrawParam());
    m_pVAO->unBind();

    if (bSameStateset)
    {
        m_pStateSet = refObj.m_pStateSet;
        m_UpdateFunc = refObj.m_UpdateFunc;
    }
}

TRSGeode::TRSGeode(float* vertices, int verticeSize, EnVertexStruct EnVertType)
{
    readFromVertex(vertices, verticeSize, EnVertType);
}

TRSGeode::~TRSGeode()
{

}

void TRSGeode::draw()
{
    preProcess();

    drawInternal();

    postProcess();


}

void TRSGeode::readFromVertex(float* vertices, int verticeSize, EnVertexStruct EnVertType)
{
    m_pVAO->createVAO(vertices, verticeSize, EnVertType);
}

void TRSGeode::readFromVertex(float* vertices, int verticeSize, EnVertexStruct EnVertType, unsigned int* indice, int indexCount)
{
    m_pVAO->createVAO(vertices, verticeSize, EnVertType, indice, indexCount);
}

std::shared_ptr<TRSVAO> TRSGeode::getVAO() const
{
    return m_pVAO;
}

TRSVec4 TRSGeode::getColor() const
{
    return m_matColor;
}

void TRSGeode::setColor(const TRSVec4& color)
{
    m_matColor = color;
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
    strDebugInfo = "Geode VAO ID: " + std::to_string(m_pVAO->getVAO());
    strDebugInfo += pShader->debugInfo();
    strDebugInfo += pTexture->debugInfo();
    return strDebugInfo;
}

void TRSGeode::preProcess()
{
    if (GL_PATCHES == m_pVAO->getDrawType())
    {
        glPatchParameteri(GL_PATCH_VERTICES, m_pVAO->getDrawParam());
    }

    if (m_polygonMode != GL_FILL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
    }
}

// #define GL_POINTS 0x0000
// #define GL_LINES 0x0001
// #define GL_LINE_LOOP 0x0002
// #define GL_LINE_STRIP 0x0003
// #define GL_TRIANGLES 0x0004
// #define GL_TRIANGLE_STRIP 0x0005
// #define GL_TRIANGLE_FAN 0x0006
void TRSGeode::drawInternal()
{
    int nElementCount = m_pVAO->getElementCount();
    int OpenGLDrawType = m_pVAO->getDrawType();
    if (nElementCount > 0)
    {
        glDrawElements(OpenGLDrawType, m_pVAO->getElementCount(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(OpenGLDrawType, 0, m_pVAO->getDrawCount());
    }

}

void TRSGeode::postProcess()
{
    if (m_polygonMode != GL_FILL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
