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
class TRSMesh;
class TRSNode;

typedef std::tr1::function<void(TRSNode*)> NodeUpdateFunc;

class TRS_EXPORT TRSNode
{
public:
    TRSNode();
    ~TRSNode();

    TRSMatrix getMatrix();

    void setMatrix(const TRSMatrix& mat);

    virtual void setActive();// used to bind vao

    virtual void preProcess();

    virtual void draw();

    virtual void postProcess();

    virtual void traverse(NodeVisitor& visitor);

    TRSVec4 getColor() const;

    void setColor(const TRSVec4& color);

    void setVisible(bool vis);

    bool visible() const;

    std::shared_ptr<TRSStateSet> getStateSet() const;

    std::shared_ptr<TRSStateSet> getOrCreateStateSet();

    void setStateSet(std::shared_ptr<TRSStateSet> pStateSet);

    void setUpdateCallBack(NodeUpdateFunc);

    NodeUpdateFunc getUpdateCallBack();

    virtual std::string debugInfo() = 0;
protected:
    bool m_visible = true;
    TRSMatrix m_mat;
    TRSVec4 m_matColor;
    std::shared_ptr<TRSStateSet> m_pStateSet;
    std::shared_ptr<TRSMesh> m_pMesh;
    NodeUpdateFunc m_UpdateFunc;
};


