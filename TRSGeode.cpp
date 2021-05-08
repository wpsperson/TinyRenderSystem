#include "TRSGeode.h"
#include "TRSVAO.h"
#include <iostream>
#include "TRSShader.h"
#include "TRSTexture.h"
#include "TRSStateSet.h"


TRSGeode::TRSGeode()
    :TRSNode(), m_matColor(s_DefaultNodeColor)
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

glm::vec4 TRSGeode::getColor() const
{
    return m_matColor;
}

void TRSGeode::setColor(const glm::vec4& color)
{
    m_matColor = color;
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
