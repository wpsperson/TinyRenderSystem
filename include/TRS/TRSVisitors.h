#pragma once
#include "TRS/NodeVisitor.h"

class PolygonModeVisitor : public NodeVisitor
{
public:
    PolygonModeVisitor();

    void setTargetNode(TRSNode* node);

    void setPolygonMode(int mode);

    int getPolygonMode();

    void execute(TRSNode* pNode) override;

    void switchPolygonMode();

private:
    int m_polygonMode;
    TRSNode* m_targetNode = nullptr;
};



