#include "TRS/TRSVisitors.h"

#include "TRS/TRSDefGL.h"
#include "TRS/TRSGeode.h"

PolygonModeVisitor::PolygonModeVisitor()
    :m_polygonMode(GL_FILL)
{

}

void PolygonModeVisitor::setTargetNode(TRSNode* node)
{
    m_targetNode = node;
}

void PolygonModeVisitor::setPolygonMode(int mode)
{
    m_polygonMode = mode;
}

int PolygonModeVisitor::getPolygonMode()
{
    return m_polygonMode;
}

void PolygonModeVisitor::execute(TRSNode* pNode)
{
    TRSGeode* geode = dynamic_cast<TRSGeode*>(pNode);
    if (!geode)
    {
        return;
    }
    geode->setPolygonMode(m_polygonMode);
}

void PolygonModeVisitor::switchPolygonMode()
{
    if (m_polygonMode == GL_FILL)
    {
        m_polygonMode = GL_LINE;
    }
    else if (m_polygonMode == GL_LINE)
    {
        m_polygonMode = GL_POINT;
    }
    else if (m_polygonMode == GL_POINT)
    {
        m_polygonMode = GL_FILL;
    }
    //recursively visit target node.
    this->visit(m_targetNode);
}
