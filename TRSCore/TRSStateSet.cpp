#include "TRSStateSet.h"
#include "TRSShader.h"
#include "TRSTexture.h"



TRSStateSet::TRSStateSet(TRSStateSetManager* pManager)
    :m_pManager(m_pManager)
{
    m_nID = pManager->allocateID();
    m_pShader = new TRSShader();
    m_pTexture = new TRSTexture();
}

TRSStateSet::~TRSStateSet()
{
    delete m_pShader;
    delete m_pTexture;
}

TRSShader* TRSStateSet::getShader()
{
    return m_pShader;
}

TRSTexture* TRSStateSet::getTexture()
{
    return m_pTexture;
}

int TRSStateSet::id()
{
    return m_nID;
}

void TRSStateSet::free()
{
    m_pManager->removeStateSet(m_nID);
    delete this;
}

void TRSStateSet::setID(int id)
{
    m_nID = id;
}

TRSStateSetManager* TRSStateSetManager::m_pInstance = nullptr;

TRSStateSetManager::TRSStateSetManager()
    :m_nNewID(0)
{
    m_pDefaultStateSet = std::make_shared<TRSStateSet>(this);//默认状态集的id是0
    m_pDefaultStateSet->getShader()->createProgram("shaders/DefaultVertex.glsl", "shaders/DefaultFragment.glsl");
    m_StateSets.insert(std::make_pair(m_pDefaultStateSet->id(), m_pDefaultStateSet));
}

TRSStateSetManager::~TRSStateSetManager()
{

}

TRSStateSetManager* TRSStateSetManager::instance()
{
    if (!m_pInstance)
    {
        m_pInstance = new TRSStateSetManager;
    }
    return m_pInstance;
}

std::shared_ptr<TRSStateSet> TRSStateSetManager::createStateSet()
{
    std::shared_ptr<TRSStateSet> pNewStateSet = std::make_shared<TRSStateSet>(this);
    m_StateSets.insert(std::make_pair(pNewStateSet->id(), pNewStateSet));
    return pNewStateSet;
}

int TRSStateSetManager::allocateID()
{
    return m_nNewID ++;
}

std::shared_ptr<TRSStateSet> TRSStateSetManager::findStateSet(int id)
{
    if (m_StateSets.count(id))
    {
        return m_StateSets[id];
    }
    return nullptr ;
}

bool TRSStateSetManager::removeStateSet(int id)
{
    if (m_StateSets.count(id))
    {
        m_StateSets.erase(id);
        return true;
    }
    return false;
}

void TRSStateSetManager::free()
{
    delete m_pInstance;
    m_pInstance = nullptr;
}
