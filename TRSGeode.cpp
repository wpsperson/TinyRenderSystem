#include "TRSGeode.h"
#include "TRSVAO.h"



TRSGeode::TRSGeode()
    :TRSNode(), m_matColor(s_DefaultNodeColor)
{
    m_pVAO = std::make_shared<TRSVAO>();
}

TRSGeode::TRSGeode(std::wstring& strObjFile)
{
    readFromOBJ(strObjFile);
}

TRSGeode::TRSGeode(float* vertices, int verticeSize, EnVertexStruct EnVertType)
{
    readFromVertex(vertices, verticeSize, EnVertType);
}

TRSGeode::~TRSGeode()
{

}

void TRSGeode::readFromOBJ(std::wstring& strObjFile)
{

}

void TRSGeode::readFromVertex(float* vertices, int verticeSize, EnVertexStruct EnVertType)
{
    m_pVAO->createVAO(vertices, verticeSize, EnVertType);
}

std::shared_ptr<TRSVAO> TRSGeode::getVAO() const
{
    return m_pVAO;
}

glm::vec4 TRSGeode::getColor() const
{
    return m_matColor;
}
