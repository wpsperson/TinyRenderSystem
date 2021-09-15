#pragma once
#include "TRSExport.h"
#include "TRSNode.h"
#include "TRSVector.h"
#include <string>

class TRSVAO;

class TRS_EXPORT TRSTextNode : public TRSNode
{
public:
    TRSTextNode();
    ~TRSTextNode();
    void setText(const std::wstring& strText);
    void sePos(const TRSVec3& textPos);
    void setDir(const TRSVec3& textDirection);
    void setUp(const TRSVec3& textUp);
    void setSize(float size);

    virtual void draw() override;
protected:
    void preProcess();
protected:
    std::shared_ptr<TRSVAO> m_pVAO;
    TRSVec4 m_matColor;
    std::wstring m_text;
    TRSVec3 pos;
    TRSVec3 right;
    TRSVec3 up;
    float m_size;
    bool m_dirty;
    int m_ptCount;
};

