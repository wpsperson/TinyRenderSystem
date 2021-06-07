#include "TRSVisitors.h"
#include "glad/glad.h"
#include "TRSGeode.h"

PolygonModeVisitor::PolygonModeVisitor()
    :m_polygonMode(GL_FILL)
{

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
}
