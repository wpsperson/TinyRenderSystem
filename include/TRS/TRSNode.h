#pragma once

#include <memory>
#include <string>
#include <functional>

#include "TRS/TRSExport.h"
#include "TRS/TRSBox.h"
#include "TRS/TRSMatrix.h"

class NodeVisitor;
class TRSStateSet;
class TRSMesh;
class TRSTexture;
class TRSNode;

typedef std::function<void(TRSNode*)> NodeUpdateFunc;

class TRS_EXPORT TRSNode
{
public:
    TRSNode();
    ~TRSNode();

    TRSMatrix getMatrix();

    void setMatrix(const TRSMatrix& mat);

    void setMesh(TRSMesh* mesh);

    TRSMesh* getMesh()const;

    void setTexture(std::shared_ptr<TRSTexture> texture);

    TRSTexture* getTexture() const;

    virtual void setActive();// used to bind vao

    virtual void preProcess();

    virtual void draw();

    virtual void postProcess();

    virtual void traverse(NodeVisitor& visitor);

    virtual TRSBox boundingBox() const;

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
    std::shared_ptr<TRSMesh> m_pMesh;
    std::shared_ptr<TRSTexture> m_pTexture;
    NodeUpdateFunc m_UpdateFunc;
};


