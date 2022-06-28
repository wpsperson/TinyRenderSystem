/*!
 *@brief     节点访问器
 *@author     wangps
 *@date       2019年5月11日
 */

#pragma once
#include "Core\TRSExport.h"

class TRSNode;
class TRS_EXPORT NodeVisitor
{
public:
    NodeVisitor();
    ~NodeVisitor();
    //访问节点
    virtual void visit(TRSNode* pNode);
    //对节点的实际操作
    virtual void execute(TRSNode* pNode);
    //重置状态
    virtual void reset();

    int getExeCount() const;
protected:
    int m_nNodeCount;
};


