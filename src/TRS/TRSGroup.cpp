#include "TRS/TRSGroup.h"

#include "TRS/TRSNode.h"
#include "TRS/NodeVisitor.h"


TRSGroup::TRSGroup()
    :TRSNode()
{

}

TRSGroup::~TRSGroup()
{
    for (TRSNode* child : m_pChildren)
    {
        delete child;
    }
    m_pChildren.clear();
}

NodeType TRSGroup::nodeType() const
{
    return NodeType::ntGroup;
}

void TRSGroup::initialize(TRSViewer* viewer)
{
    for (TRSNode* child : m_pChildren)
    {
        child->initialize(viewer);
    }
}

void TRSGroup::addChild(TRSNode* child)
{
    TRSGroup::insertChild(static_cast<int>(m_pChildren.size()), child);
}

void TRSGroup::removeChild(TRSNode* pNode)
{
    std::vector<TRSNode*>::iterator itr = m_pChildren.begin();
    for (; itr != m_pChildren.end();)
    {
        TRSNode* curNode = *itr;
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


TRSNode* TRSGroup::child(int idx)
{
    if (idx < 0 || idx >= m_pChildren.size())
    {
        return nullptr;
    }
    return m_pChildren[idx];
}

void TRSGroup::traverse(NodeVisitor& visitor)
{
    for (TRSNode* child : m_pChildren)
    {
        visitor.visit(child);
    }
}

TRSBox TRSGroup::boundingBox() const
{
    TRSBox box = TRSNode::boundingBox();
    for (TRSNode* node : m_pChildren)
    {
        TRSBox childBox = node->boundingBox();
        box.mergeBox(childBox);
    }
    return box;
}

std::string TRSGroup::debugInfo()
{
    std::string strDebugInfo;
    for (TRSNode* child : m_pChildren)
    {
        strDebugInfo += child->debugInfo();
    }
    return strDebugInfo;
}

void TRSGroup::insertChild(int idx, TRSNode* pChildNode)
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