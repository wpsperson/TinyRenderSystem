#include "TRS/CullVisitor.h"
#include "TRS/TRSNode.h"
#include "TRS/TRSStateSet.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSGroup.h"


CullVisitor::CullVisitor()
    : NodeVisitor()
{

}

CullVisitor::~CullVisitor()
{

}

void CullVisitor::execute(TRSNode* pNode)
{
    NodeVisitor::execute(pNode);

    if (TRSGroup*pGroup = dynamic_cast<TRSGroup*>(pNode))
    {
        return;
    }

    if (!pNode->visible())
    {
        return;
    }

    std::shared_ptr<TRSStateSet> pStateSet = pNode->getStateSet();
    if (!pStateSet)
    {
        std::vector<TRSNode*>& nodeList = m_mapState2Node[0];
        nodeList.push_back(pNode);
    }
    else
    {
        std::vector<TRSNode*>& nodeList = m_mapState2Node[pStateSet->id()];
        nodeList.push_back(pNode);
    }
}

std::map<int, std::vector<TRSNode*>> CullVisitor::getMapState2Node()
{
    return m_mapState2Node;
}
