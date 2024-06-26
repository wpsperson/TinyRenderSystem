﻿#pragma once
#include "TRS/TRSExport.h"
#include "TRS/TRSNode.h"
#include <vector>
#include <memory>

class TRS_EXPORT TRSGroup : public TRSNode
{
public:
    TRSGroup();
    ~TRSGroup();

    void addChild(std::shared_ptr<TRSNode> ptr);

    void removeChild(TRSNode* pNode);

    size_t childNum() const;

    std::shared_ptr<TRSNode> child(int idx);

    virtual void traverse(NodeVisitor& visitor) override;

    TRSBox boundingBox() const override;

    virtual std::string debugInfo() override;
protected:
    void insertChild(int idx, std::shared_ptr<TRSNode>);

protected:
    std::vector<std::shared_ptr<TRSNode>> m_pChildren;
};

