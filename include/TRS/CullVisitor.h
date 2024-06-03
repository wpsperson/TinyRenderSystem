#pragma once
#include "TRS/TRSExport.h"
#include "TRS/NodeVisitor.h"
#include <map>
#include <vector>


class TRS_EXPORT CullVisitor : public NodeVisitor
{
public:
    CullVisitor();
    ~CullVisitor();
    virtual void execute(TRSNode* pNode) override;
    
    std::map<int, std::vector<TRSNode*>> getMapState2Node();

protected:
    std::map<int, std::vector<TRSNode*>> m_mapState2Node;
    
};
