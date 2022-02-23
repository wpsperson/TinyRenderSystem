/*!  
 *@brief    显示节点基类
 *@author   wangps
 *@date     2019年5月5日
 */

#pragma once
#include "TRSExport.h"
#include "TRSConst.h"
#include "TRSMatrix.h"
#include <memory>
#include <string>
#include <functional>

class NodeVisitor;
class TRSStateSet;
class TRSVAO;
class TRSNode;

typedef std::tr1::function<void(TRSNode*)> NodeUpdateFunc;

class TRS_EXPORT TRSNode
{
public:
    TRSNode();
    ~TRSNode();

    TRSMatrix getMatrix();

    void setMatrix(const TRSMatrix& mat);

    virtual std::shared_ptr<TRSVAO> getVAO() const;

    virtual void draw();

    virtual void traverse(NodeVisitor& visitor);

    TRSVec4 getColor() const;

    void setColor(const TRSVec4& color);

    std::shared_ptr<TRSStateSet> getStateSet() const;

    std::shared_ptr<TRSStateSet> getOrCreateStateSet();

    void setStateSet(std::shared_ptr<TRSStateSet> pStateSet);

    void setUpdateCallBack(NodeUpdateFunc);

    NodeUpdateFunc getUpdateCallBack();

    virtual std::string debugInfo() = 0;
protected:
    TRSMatrix m_mat;
    TRSVec4 m_matColor;
    std::shared_ptr<TRSStateSet> m_pStateSet;
    NodeUpdateFunc m_UpdateFunc;
};


