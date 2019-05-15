/*!  
 *@brief    ��ʾ�ڵ����
 *@author   wangps
 *@date     2019��5��5��
 */

#pragma once
#include "TRSExport.h"
#include "TRSConst.h"
#include "glm/glm.hpp"
#include <memory>
#include <string>
#include <functional>

class NodeVisitor;
class TRSStateSet;
class TRSVAO;
class TRSNode;

typedef std::tr1::function<void(TRSNode*)> NodeUpdateFunc;

class TRS_EXPORT TRSNode
{
public:
    TRSNode();
    ~TRSNode();

    glm::mat4 getMatrix();

    void setMatrix(const glm::mat4& mat);


    virtual void traverse(NodeVisitor& visitor);

    std::shared_ptr<TRSStateSet> getStateSet() const;

    std::shared_ptr<TRSStateSet> getOrCreateStateSet();

    void setStateSet(std::shared_ptr<TRSStateSet> pStateSet);

    void setUpdateCallBack(NodeUpdateFunc);

    NodeUpdateFunc getUpdateCallBack();


protected:
    glm::mat4 m_mat;
    std::shared_ptr<TRSStateSet> m_pStateSet;
    NodeUpdateFunc m_UpdateFunc;
};


