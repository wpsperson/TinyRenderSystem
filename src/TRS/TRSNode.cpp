#include "TRS/TRSNode.h"

#include "TRS/TRSDefConst.h"
#include "TRS/TRSMesh.h"



TRSNode::TRSNode()
    : m_mat()
    , m_UpdateFunc(nullptr)
    , m_matColor(s_DefaultNodeColor)
{
    m_pMesh = std::make_shared<TRSMesh>();
}

TRSNode::~TRSNode()
{

}

TRSMatrix TRSNode::getMatrix()
{
    return m_mat;
}

void TRSNode::setMatrix(const TRSMatrix& mat)
{
    m_mat = mat;
}

void TRSNode::setMesh(TRSMesh* mesh)
{
    m_pMesh = std::make_shared<TRSMesh>(*mesh);
}

TRSMesh* TRSNode::getMesh() const
{
    return m_pMesh.get();
}

void TRSNode::setTexture(std::shared_ptr<TRSTexture> texture)
{
    m_pTexture = texture;
}

TRSTexture* TRSNode::getTexture() const
{
    return m_pTexture.get();
}

void TRSNode::setActive()
{

}

TRSVec4 TRSNode::getColor() const
{
    return m_matColor;
}

void TRSNode::setColor(const TRSVec4& color)
{
    m_matColor = color;
}

void TRSNode::setVisible(bool vis)
{
    m_visible = vis;
}

bool TRSNode::visible() const
{
    return m_visible;
}

void TRSNode::setName(const std::string& name)
{
    m_name = name;
}

const std::string& TRSNode::name() const
{
    return m_name;
}

void TRSNode::preProcess()
{

}

void TRSNode::draw()
{

}

void TRSNode::postProcess()
{

}

void TRSNode::traverse(NodeVisitor& visitor)
{

}

TRSBox TRSNode::boundingBox() const
{
    return m_pMesh->boundingBox();
}

void TRSNode::setUpdateCallBack(NodeUpdateFunc func)
{
    m_UpdateFunc = func;
}

NodeUpdateFunc TRSNode::getUpdateCallBack()
{
    return m_UpdateFunc;
}
