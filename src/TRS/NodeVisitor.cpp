#include "TRS/NodeVisitor.h"

#include "TRS/TRSNode.h"


NodeVisitor::NodeVisitor()
    :m_nNodeCount(0)
{

}

NodeVisitor::~NodeVisitor()
{

}

void NodeVisitor::visit(TRSNode* pNode)
{
    execute(pNode);
    pNode->traverse(*this);
}

void NodeVisitor::execute(TRSNode* pNode)
{
    m_nNodeCount++;
}

void NodeVisitor::reset()
{
    m_nNodeCount = 0;
}

int NodeVisitor::getExeCount() const
{
    return m_nNodeCount;
}
