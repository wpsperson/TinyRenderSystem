#pragma once

#include <string>

#include "TRS/TRSExport.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSVector.h"

class TRS_EXPORT TRSTextNode : public TRSGeode
{
public:
    TRSTextNode();
    ~TRSTextNode();

    NodeType nodeType() const override;

    void setText(const std::wstring& strText);

    void setPos(const TRSVec3& textPos);

    void setDir(const TRSVec3& textDirection);

    void setUp(const TRSVec3& textUp);

    void setSize(float size);

    void initialize(TRSViewer* viewer) override;

    virtual void draw(RenderMode mode) override;

    virtual std::string debugInfo() override;

protected:
    void preProcess(RenderMode mode) override;
    void postProcess(RenderMode mode) override;
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

