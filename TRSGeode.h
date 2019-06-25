/*!
*@brief    显示节点叶节点
*@author   wangps
*@date     2019年5月5日
*/

#pragma once
#include "TRSExport.h"
#include "TRSNode.h"
#include <glm/glm.hpp>

class TRS_EXPORT TRSGeode : public TRSNode
{
public:
    TRSGeode();

    //拷贝构造函数就是共享VBO
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

