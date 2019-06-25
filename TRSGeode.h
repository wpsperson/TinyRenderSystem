/*!
*@brief    ��ʾ�ڵ�Ҷ�ڵ�
*@author   wangps
*@date     2019��5��5��
*/

#pragma once
#include "TRSExport.h"
#include "TRSNode.h"
#include <glm/glm.hpp>

class TRS_EXPORT TRSGeode : public TRSNode
{
public:
    TRSGeode();

    //�������캯�����ǹ���VBO
    TRSGeode(const TRSGeode& refObj, bool bSameStateset = true);

    TRSGeode(std::wstring& strObjFile);

    TRSGeode(float* vertices, int verticeSize, EnVertexStruct EnVertType);

    ~TRSGeode();

    virtual void readFromOBJ(std::wstring& strObjFile);

    virtual void readFromVertex(float* vertices, int verticeSize, EnVertexStruct EnVertType);

    std::shared_ptr<TRSVAO> getVAO() const;

    glm::vec4 getColor() const;

protected:
    std::shared_ptr<TRSVAO> m_pVAO;
    glm::vec4 m_matColor;
};

