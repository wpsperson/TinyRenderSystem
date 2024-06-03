#pragma once

#include <vector>

#include "TRS/TRSExport.h"
#include "TRS/NodeVisitor.h"


class TRS_EXPORT CullVisitor : public NodeVisitor
{
public:
    CullVisitor();
    ~CullVisitor();

    virtual void execute(TRSNode* pNode) override;

    const std::vector<TRSNode*>& toRenderNodes();

    void clearUp();

protected:
    bool m_useFrustumCull = false;
    std::vector<TRSNode*> m_renderNodes;
};
