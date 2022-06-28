#include "DataModel\TRSNode.h"
#include "Core\TRSStateSet.h"
#include "DataModel\TRSMesh.h"



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

std::shared_ptr<TRSStateSet> TRSNode::getStateSet() const
{
    return m_pStateSet;
}

std::shared_ptr<TRSStateSet> TRSNode::getOrCreateStateSet()
{
    if (!m_pStateSet)
    {
        m_pStateSet = TRSStateSetManager::instance()->createStateSet();
    }
    return m_pStateSet;
}

void TRSNode::setStateSet(std::shared_ptr<TRSStateSet> pStateSet)
{
    m_pStateSet = pStateSet;
}

void TRSNode::setUpdateCallBack(NodeUpdateFunc func)
{
    m_UpdateFunc = func;
}

NodeUpdateFunc TRSNode::getUpdateCallBack()
{
    return m_UpdateFunc;
}
