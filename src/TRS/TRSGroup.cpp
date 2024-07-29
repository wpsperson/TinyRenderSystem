#include "TRS/TRSGroup.h"

#include "TRS/TRSNode.h"
#include "TRS/NodeVisitor.h"


TRSGroup::TRSGroup()
    :TRSNode()
{

}

TRSGroup::~TRSGroup()
{

}

NodeType TRSGroup::nodeType() const
{
    return NodeType::ntGroup;
}

void TRSGroup::initialize(TRSViewer* viewer)
{
    for (std::shared_ptr<TRSNode> child : m_pChildren)
    {
        child->initialize(viewer);
    }
}

void TRSGroup::addChild(std::shared_ptr<TRSNode> ptr)
{
    TRSGroup::insertChild(static_cast<int>(m_pChildren.size()), ptr);
}

void TRSGroup::removeChild(TRSNode* pNode)
{
    std::vector<std::shared_ptr<TRSNode>>::iterator itr = m_pChildren.begin();
    for (; itr != m_pChildren.end();)
    {
        TRSNode* curNode = (*itr).get();
        if (curNode == pNode)
        {
            itr = m_pChildren.erase(itr);
            break;
        }
        else
        {
            itr++;
        }
    }
}

size_t TRSGroup::childNum() const
{
    return m_pChildren.size();
}


std::shared_ptr<TRSNode> TRSGroup::child(int idx)
{
    if (idx < 0 || idx >= m_pChildren.size())
    {
        return nullptr;
    }
    return m_pChildren[idx];
}

void TRSGroup::traverse(NodeVisitor& visitor)
{
    std::vector<std::shared_ptr<TRSNode>>::iterator itr = m_pChildren.begin();
    for (; itr!=m_pChildren.end(); itr++)
    {
        visitor.visit((*itr).get());
    }
}

TRSBox TRSGroup::boundingBox() const
{
    TRSBox box = TRSNode::boundingBox();
    for (const std::shared_ptr<TRSNode>& node : m_pChildren)
    {
        TRSBox childBox = node->boundingBox();
        box.mergeBox(childBox);
    }
    return box;
}

std::string TRSGroup::debugInfo()
{
    std::string strDebugInfo;
    std::vector<std::shared_ptr<TRSNode>>::iterator itr = m_pChildren.begin();
    for (; itr != m_pChildren.end(); itr++)
    {
        std::shared_ptr<TRSNode> child = *itr;
        strDebugInfo += child->debugInfo();
    }
    return strDebugInfo;
}

void TRSGroup::insertChild(int idx, std::shared_ptr<TRSNode> pChildNode)
{
    if (idx >= m_pChildren.size())
    {
        m_pChildren.push_back(pChildNode);
    }
    else
    {
        m_pChildren.insert(m_pChildren.begin() + idx, pChildNode);
    }
}