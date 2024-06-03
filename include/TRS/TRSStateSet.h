#pragma once

#include <vector>
#include <map>
#include <memory>

#include "TRS/TRSExport.h"

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
    std::vector<TRSNode*> m_pNodes; // nodes that share this state set.
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

    static void free();

protected:
    TRSStateSetManager();
    ~TRSStateSetManager();

protected:
    static TRSStateSetManager* m_pInstance;
    int m_nNewID;
    std::map<int, std::shared_ptr<TRSStateSet>> m_StateSets;
    std::shared_ptr<TRSStateSet> m_pDefaultStateSet;


};
