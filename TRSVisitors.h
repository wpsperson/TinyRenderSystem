#pragma once
#include "NodeVisitor.h"

class PolygonModeVisitor : public NodeVisitor
{
public:
    PolygonModeVisitor();

    void setPolygonMode(int mode);

    int getPolygonMode();

    void execute(TRSNode* pNode) override;

    void switchPolygonMode();

private:
    int m_polygonMode;
};



