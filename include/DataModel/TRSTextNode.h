#pragma once
#include "Core\TRSExport.h"
#include "DataModel\TRSNode.h"
#include "Math\TRSVector.h"
#include <string>


class TRS_EXPORT TRSTextNode : public TRSNode
{
public:
    TRSTextNode();
    ~TRSTextNode();
    void setText(const std::wstring& strText);
    void setPos(const TRSVec3& textPos);
    void setDir(const TRSVec3& textDirection);
    void setUp(const TRSVec3& textUp);
    void setSize(float size);

    virtual void draw() override;

    void setActive() override;

    virtual std::string debugInfo() override;

protected:
    void preProcess() override;
    void postProcess() override;
    void generateText();
protected:
    std::wstring m_text;
    TRSVec3 pos;
    TRSVec3 right;
    TRSVec3 up;
    float m_size;
    bool m_dirty;
    int m_ptCount;
};

