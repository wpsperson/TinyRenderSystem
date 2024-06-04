#pragma once

#include <vector>

#include "TRS/TRSExport.h"
#include "TRS/NodeVisitor.h"

class TRSGeode;
class TRS_EXPORT CullVisitor : public NodeVisitor
{
public:
    CullVisitor();
    ~CullVisitor();

    virtual void execute(TRSNode* pNode) override;

    const std::vector<TRSGeode*>& toRenderNodes();

    void clearUp();

protected:
    bool m_useFrustumCull = false;
    std::vector<TRSGeode*> m_renderNodes;
};
