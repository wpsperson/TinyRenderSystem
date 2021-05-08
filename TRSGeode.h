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

    TRSGeode(float* vertices, int verticeSize, EnVertexStruct EnVertType);

    ~TRSGeode();

    //vertices float point. verticeSize表示有多少个float
    virtual void readFromVertex(float* vertices, int verticeSize, EnVertexStruct EnVertType);

    virtual void readFromVertex(float* vertices, int verticeSize, EnVertexStruct EnVertType, unsigned int* indice, int indexCount);

    std::shared_ptr<TRSVAO> getVAO() const;

    glm::vec4 getColor() const;

    void setColor(const glm::vec4& color);

    virtual std::string debugInfo() override;

protected:
    std::shared_ptr<TRSVAO> m_pVAO;
    glm::vec4 m_matColor;
};

