/*!
*@brief    显示节点叶节点
*@author   wangps
*@date     2019年5月5日
*/

#pragma once
#include "Core\TRSExport.h"
#include "DataModel\TRSNode.h"
#include "Math\TRSVector.h"
#include <vector>

class TRS_EXPORT TRSGeode : public TRSNode
{
public:
    TRSGeode();

    ~TRSGeode();

    virtual void draw() override;

    void setMeshData(const std::vector<TRSPoint>& vertexs, const std::vector<TRSVec3>& normals, const std::vector<TRSVec2>& UVs, const std::vector<TRSVec3>& colors);

    void setIndexArray(const std::vector<unsigned int>& indices);

    void setActive() override;

    void setPolygonMode(int polyMode);

    virtual std::string debugInfo() override;

protected:
    void preProcess() override;
    void postProcess() override;
    void drawInternal();

protected:
    int m_polygonMode;
};

