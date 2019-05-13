/*!  
 *@brief    ��ʾ�ڵ���Ͻڵ�
 *@author   wangps
 *@date     2019��5��5��
 */

#pragma once
#include "TRSExport.h"
#include "TRSNode.h"
#include <vector>
#include <memory>

class TRS_EXPORT TRSGroup : public TRSNode
{
public:
    TRSGroup();
    ~TRSGroup();

    void addChild(std::shared_ptr<TRSNode> ptr);

    size_t childNum() const;

    std::shared_ptr<TRSNode> child(int idx);

    virtual void traverse(NodeVisitor& visitor) override;

protected:
    void insertChild(int idx, std::shared_ptr<TRSNode>);

protected:
    std::vector<std::shared_ptr<TRSNode>> m_pChildren;
};

