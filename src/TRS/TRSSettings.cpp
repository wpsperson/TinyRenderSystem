#include "TRS/TRSSettings.h"

void TRSSettings::setShaded(bool flag)
{
    m_displayShaded = flag;
}

bool TRSSettings::displayShaded() const
{
    return m_displayShaded;
}

void TRSSettings::setWireframe(bool flag)
{
    m_displayWireframe = flag;
}

bool TRSSettings::displayWireframe() const
{
    return m_displayWireframe;
}

void TRSSettings::setPoints(bool flag)
{
    m_displayPoints = flag;
}

bool TRSSettings::displayPoints() const
{
    return m_displayPoints;
}
