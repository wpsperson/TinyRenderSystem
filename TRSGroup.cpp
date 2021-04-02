#include "TRSGroup.h"
#include "TRSNode.h"
#include "NodeVisitor.h"



TRSGroup::TRSGroup()
    :TRSNode()
{

}

TRSGroup::~TRSGroup()
{

}

void TRSGroup::addChild(std::shared_ptr<TRSNode> ptr)
{
    TRSGroup::insertChild(m_pChildren.size(), ptr);
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