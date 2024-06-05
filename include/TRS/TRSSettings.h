#pragma once

#include "TRS/TRSExport.h"


class TRS_EXPORT TRSSettings
{
public:
    void setShaded(bool flag);

    bool displayShaded() const;

    void setWireframe(bool flag);

    bool displayWireframe() const;

    void setPoints(bool flag);

    bool displayPoints() const;

private:
    bool m_displayShaded = true;
    bool m_displayWireframe = true;
    bool m_displayPoints = true;
};

