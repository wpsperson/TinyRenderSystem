#include "TRS/CullVisitor.h"

#include "TRS/TRSNode.h"
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
    TRSGeode* geode = dynamic_cast<TRSGeode*>(pNode);
    if (!geode)
    {
        return;
    }

    if (!geode->visible())
    {
        return;
    }

    if (m_useFrustumCull)
    {
        // TODO Frustum Cull.
    }
    m_renderNodes.emplace_back(geode);
}

const std::vector<TRSGeode*>& CullVisitor::toRenderNodes()
{
    return m_renderNodes;
}

void CullVisitor::clearUp()
{
    m_renderNodes.clear();
}
