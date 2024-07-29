#pragma once

#include <memory>
#include <string>
#include <functional>

#include "TRS/TRSExport.h"
#include "TRS/TRSBox.h"
#include "TRS/TRSMatrix.h"
#include "TRS/TRSDefEnum.h"

class NodeVisitor;
class TRSNode;
class TRSViewer;

typedef std::function<void(TRSNode*)> NodeUpdateFunc;

class TRS_EXPORT TRSNode
{
public:
    TRSNode();
    ~TRSNode();

    virtual NodeType nodeType() const;

    virtual void initialize(TRSViewer* viewer);

    virtual TRSBox boundingBox() const;

    virtual void traverse(NodeVisitor& visitor);

    TRSMatrix getMatrix();

    void setMatrix(const TRSMatrix& mat);

    TRSVec4 getColor() const;

    void setColor(const TRSVec4& color);

    void setVisible(bool vis);

    bool visible() const;

    void setName(const std::string& name);

    const std::string& name()const;

    void setUpdateCallBack(NodeUpdateFunc);

    NodeUpdateFunc getUpdateCallBack();

    virtual std::string debugInfo() = 0;
protected:
    bool m_visible = true;
    TRSMatrix m_mat;
    TRSVec4 m_matColor;
    std::string m_name;
    NodeUpdateFunc m_UpdateFunc;
};


