#pragma once

#include <vector>
#include <map>
#include <memory>

#include "TRS/TRSExport.h"

class TRSNode;
class TRSShader;
class TRSTexture;

class TRS_EXPORT TRSStateSet
{
public:
    TRSStateSet();
    ~TRSStateSet();

    int id();

    TRSShader* getShader();

    TRSTexture* getTexture();

protected:
    int m_nID;
    TRSShader* m_pShader;
    TRSTexture* m_pTexture;
};

