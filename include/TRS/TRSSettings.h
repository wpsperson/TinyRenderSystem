#pragma once

#include "TRS/TRSExport.h"
#include "TRS/TRSVector.h"
#include "TRS/TRSDefEnum.h"

class TRS_EXPORT TRSSettings
{
public:
    void setShaded(bool flag);

    bool displayShaded() const;

    void setWireframe(bool flag);

    bool displayWireframe() const;

    void setPoints(bool flag);

    bool displayPoints() const;

    const TRSColor& backgroundColor() const;
    void setBGColor(const TRSColor& color);

    ProjectionMode projMode() const;
    void setProjMode(ProjectionMode mode);

    bool displayAxis() const;
    void setDisplayAxis(bool disAxis);

private:
    TRSColor m_bgColor = { 0.2f, 0.2f, 0.4f };
    ProjectionMode m_projMode = ProjectionMode::Perspective;
    bool m_displayAxis = false;
    bool m_displayShaded = true;
    bool m_displayWireframe = true;
    bool m_displayPoints = true;
};

