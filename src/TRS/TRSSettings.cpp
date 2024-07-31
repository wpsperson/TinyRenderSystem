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

const TRSColor& TRSSettings::backgroundColor() const
{
    return m_bgColor;
}

void TRSSettings::setBGColor(const TRSColor& color)
{
    m_bgColor = color;
}

ProjectionMode TRSSettings::projMode() const
{
    return m_projMode;
}

void TRSSettings::setProjMode(ProjectionMode mode)
{
    m_projMode = mode;
}

bool TRSSettings::displayAxis() const
{
    return m_displayAxis;
}

void TRSSettings::setDisplayAxis(bool disAxis)
{
    m_displayAxis = disAxis;
}
