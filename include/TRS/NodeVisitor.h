#pragma once
#include "TRS/TRSExport.h"

class TRSNode;
class TRS_EXPORT NodeVisitor
{
public:
    NodeVisitor();
    ~NodeVisitor();

    virtual void visit(TRSNode* pNode);

    virtual void execute(TRSNode* pNode);

    virtual void reset();

    int getExeCount() const;
protected:
    int m_nNodeCount;
};


