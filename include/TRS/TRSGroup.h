#pragma once
#include "TRS/TRSExport.h"
#include "TRS/TRSNode.h"
#include <vector>

class TRS_EXPORT TRSGroup : public TRSNode
{
public:
    TRSGroup();
    ~TRSGroup();

    NodeType nodeType() const override;

    void initialize(TRSViewer* viewer) override;

    void addChild(TRSNode* child);

    void removeChild(TRSNode* pNode);

    size_t childNum() const;

    TRSNode* child(int idx);

    virtual void traverse(NodeVisitor& visitor) override;

    TRSBox boundingBox() const override;

    virtual std::string debugInfo() override;

protected:
    void insertChild(int idx, TRSNode*child);

protected:
    std::vector<TRSNode *> m_pChildren;
};

