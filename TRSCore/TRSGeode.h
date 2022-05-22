/*!
*@brief    显示节点叶节点
*@author   wangps
*@date     2019年5月5日
*/

#pragma once
#include "TRSExport.h"
#include "TRSNode.h"
#include "TRSVector.h"

class TRS_EXPORT TRSGeode : public TRSNode
{
public:
    TRSGeode();

    //拷贝构造函数就是共享VBO
    TRSGeode(const TRSGeode& refObj, bool bSameStateset = true);

    TRSGeode(float* vertices, int verticeSize, EnVertexStruct EnVertType);

    ~TRSGeode();

    virtual void draw() override;

    //vertices float point. verticeSize表示有多少个float
    virtual void readFromVertex(float* vertices, int verticeSize, EnVertexStruct EnVertType);

    virtual void readFromVertex(float* vertices, int verticeSize, EnVertexStruct EnVertType, unsigned int* indice, int indexCount);

    virtual std::shared_ptr<TRSVAO> getVAO() const override;

    void setPolygonMode(int polyMode);

    virtual std::string debugInfo() override;

protected:
    void preProcess();
    void drawInternal();
    void postProcess();

protected:
    std::shared_ptr<TRSVAO> m_pVAO;
    int m_polygonMode;
};

