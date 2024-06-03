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

    if (!pNode)
    {
        return;
    }

    if (TRSGroup*pGroup = dynamic_cast<TRSGroup*>(pNode))
    {
        return;
    }

    if (!pNode->visible())
    {
        return;
    }

    if (m_useFrustumCull)
    {
        // TODO Frustum Cull.
    }
    m_renderNodes.emplace_back(pNode);
}

const std::vector<TRSNode*>& CullVisitor::toRenderNodes()
{
    return m_renderNodes;
}

void CullVisitor::clearUp()
{
    m_renderNodes.clear();
}
