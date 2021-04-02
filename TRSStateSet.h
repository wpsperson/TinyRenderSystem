/*!
*@brief    渲染状态集 状态集管理器
*@author   wangps
*@date     2019年5月5日
*/

#pragma once
#include "TRSExport.h"
#include <vector>
#include <map>
#include <memory>

class TRSNode;
class TRSShader;
class TRSTexture;
class TRSStateSetManager;

class TRS_EXPORT TRSStateSet
{
public:
    TRSStateSet(TRSStateSetManager* pManager);
    ~TRSStateSet();

    int id();

    void free();

    TRSShader* getShader();

    TRSTexture* getTexture();


protected:
    void setID(int id);


protected:
    int m_nID;
    std::vector<TRSNode*> m_pNodes;//共享此状态集的显示节点
    TRSStateSetManager* m_pManager;
    TRSShader* m_pShader;
    TRSTexture* m_pTexture;
};


class TRS_EXPORT TRSStateSetManager
{
public:
    static TRSStateSetManager* instance();

    std::shared_ptr<TRSStateSet> createStateSet();

    int allocateID();

    std::shared_ptr<TRSStateSet> findStateSet(int id);
    bool removeStateSet(int id);



protected:
    TRSStateSetManager();
    ~TRSStateSetManager();

protected:
    static TRSStateSetManager* m_pInstance;
    int m_nNewID;
    std::map<int, std::shared_ptr<TRSStateSet>> m_StateSets;
    std::shared_ptr<TRSStateSet> m_pDefaultStateSet;


};
