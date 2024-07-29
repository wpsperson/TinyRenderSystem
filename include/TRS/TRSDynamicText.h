#pragma once

#include <string>

#include "TRS/TRSExport.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSVector.h"

class TRSCamera;
class TRSFontManager;
// only support ascii text.
class TRS_EXPORT TRSDynamicText : public TRSGeode
{
public:
    TRSDynamicText();

    ~TRSDynamicText();

    NodeType nodeType() const override;

    void setText(const std::string& strText);

    void setOrigin(const TRSVec3& textPos);

    void setFontSize(float size);

    void initialize(TRSViewer* viewer) override;

    TRSBox boundingBox() const override;

    virtual void draw(RenderMode mode) override;

    void dynamicUpdate(TRSViewer* viewer) override;

protected:
    void preProcess(RenderMode mode) override;

    void postProcess(RenderMode mode) override;

    bool generateText(TRSCamera* camera, TRSFontManager* fontMgr);

    TRSPoint movePoint(const TRSPoint& origin, float distx,  float disty);

protected:
    std::string m_text;
    TRSVec3 m_origin = G_ORIGIN;
    TRSVec3 m_right = G_XDIR;
    TRSVec3 m_up = G_YDIR;
    float m_fontSize = 1.0f;
    bool m_needRebuild = true;
    TRSBox m_box;
};
