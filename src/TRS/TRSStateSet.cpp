#include "TRS/TRSStateSet.h"

#include "TRS/TRSShader.h"
#include "TRS/TRSTexture.h"

static int g_IDCounter = 0;

TRSStateSet::TRSStateSet()
{
    m_nID = g_IDCounter++;
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
