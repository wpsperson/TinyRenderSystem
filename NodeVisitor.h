/*!
 *@brief     �ڵ������
 *@author     wangps
 *@date       2019��5��11��
 */

#pragma once
#include "TRSExport.h"

class TRSNode;
class TRS_EXPORT NodeVisitor
{
public:
    NodeVisitor();
    ~NodeVisitor();
    //���ʽڵ�
    virtual void visit(TRSNode* pNode);
    //�Խڵ��ʵ�ʲ���
    virtual void execute(TRSNode* pNode);
    //����״̬
    virtual void reset();

    int getExeCount() const;
protected:
    int m_nNodeCount;
};


