#include "TRSNode.h"
#include "TRSStateset.h"



TRSNode::TRSNode()
    : m_mat(glm::mat4(1.0f)), m_UpdateFunc(nullptr)
{

}

TRSNode::~TRSNode()
{

}

glm::mat4 TRSNode::getMatrix()
{
    return m_mat;
}

void TRSNode::setMatrix(const glm::mat4& mat)
{
    m_mat = mat;
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
