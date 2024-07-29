#include "TRS/TRSBox.h"

#include <cmath>


TRSBox::TRSBox()
{
}

TRSBox::TRSBox(const TRSPoint& pta, const TRSPoint& ptb)
{
    minx = std::min(pta[0], ptb[0]);
    maxx = std::max(pta[0], ptb[0]);
    miny = std::min(pta[1], ptb[1]);
    maxy = std::max(pta[1], ptb[1]);
    minz = std::min(pta[2], ptb[2]);
    maxz = std::max(pta[2], ptb[2]);
}

TRSBox::TRSBox(float x0, float x1, float y0, float y1, float z0, float z1)
{
    minx = x0;
    maxx = x1;
    miny = y0;
    maxy = y1;
    minz = z0;
    maxz = z1;
}

TRSBox::TRSBox(const TRSPoint& center, float halfRange)
{
    minx = center[0] - halfRange;
    maxx = center[0] + halfRange;
    miny = center[1] - halfRange;
    maxy = center[1] + halfRange;
    minz = center[2] - halfRange;
    maxz = center[2] + halfRange;
}

TRSBox::~TRSBox()
{

}

void TRSBox::mergePoint(const TRSPoint& pt)
{
    if (!valid())
    {
        minx = pt[0];
        maxx = pt[0];
        miny = pt[1];
        maxy = pt[1];
        minz = pt[2];
        maxz = pt[2];
        return;
    }
    if (pt[0] < minx)
    {
        minx = pt[0];
    }
    if (pt[0] > maxx)
    {
        maxx = pt[0];
    }

    if (pt[1] < miny)
    {
        miny = pt[1];
    }
    if (pt[1] > maxy)
    {
        maxy = pt[1];
    }

    if (pt[2] < minz)
    {
        minz = pt[2];
    }
    if (pt[2] > maxz)
    {
        maxz = pt[2];
    }
}

void TRSBox::mergeBox(const TRSBox& box)
{
    if (!valid())
    {
        if (box.valid())
        {
            *this = box;
        }
        return;
    }
    mergePoint(box.minPt());
    mergePoint(box.maxPt());
}

bool TRSBox::valid() const
{
    return (minx <= maxx);
}

TRSPoint TRSBox::center() const
{
    return TRSPoint((minx + maxx) / 2, (miny + maxy) / 2, (minz + maxz) / 2);
}

TRSPoint TRSBox::minPt() const
{
    return TRSPoint(minx, miny, minz);
}

TRSPoint TRSBox::maxPt() const
{
    return TRSPoint(maxx, maxy, maxz);
}

float TRSBox::length() const
{
    return maxx - minx;
}

float TRSBox::width() const
{
    return maxy - miny;
}

float TRSBox::height() const
{
    return maxz - minz;
}

float TRSBox::outSphereRadius() const
{
    float len = maxx - minx;
    float wid = maxy - miny;
    float hgt = maxz - minz;
    return std::sqrt(len * len + wid * wid + hgt * hgt) / 2;
}

std::array<TRSPoint, 8> TRSBox::vertices()
{
    std::array<TRSPoint, 8>result;
    result[0] = TRSPoint(minx, miny, minz);
    result[1] = TRSPoint(maxx, miny, minz);
    result[2] = TRSPoint(maxx, maxy, minz);
    result[3] = TRSPoint(minx, maxy, minz);
    result[4] = TRSPoint(minx, miny, maxz);
    result[5] = TRSPoint(maxx, miny, maxz);
    result[6] = TRSPoint(maxx, maxy, maxz);
    result[7] = TRSPoint(minx, maxy, maxz);
    return result;
}

bool TRSBox::contain(const TRSPoint& pt) const
{
    if (pt[0] > maxx || pt[0] < minx)
    {
        return false;
    }
    if (pt[1] > maxy || pt[1] < miny)
    {
        return false;
    }
    if (pt[2] > maxz || pt[2] < minz)
    {
        return false;
    }
    return true;
}

bool TRSBox::contain(const TRSBox& box) const
{
    return contain(box.minPt()) && contain(box.maxPt());
}

bool TRSBox::intersect(const TRSBox& box) const
{
    if (!this->valid() || !box.valid())
    {
        return false;
    }

    bool x_overlay = (minx <= box.maxx && maxx >= box.minx);
    bool y_overlay = (miny <= box.maxy && maxy >= box.miny);
    bool z_overlay = (minz <= box.maxz && maxz >= box.minz);
    return (x_overlay && y_overlay && z_overlay);
}

TRSBox TRSBox::boolUnion(const TRSBox& box) const
{
    TRSBox result = *this;
    result.mergeBox(box);
    return result;
}

TRSBox TRSBox::boolIntersect(const TRSBox& box) const
{
    if (!this->intersect(box))
    {
        return TRSBox();
    }

    TRSBox result;
    result.minx = std::max(minx, box.minx);
    result.miny = std::max(miny, box.miny);
    result.minz = std::max(minz, box.minz);
    result.maxx = std::min(maxx, box.maxx);
    result.maxy = std::min(maxy, box.maxy);
    result.maxz = std::min(maxz, box.maxz);
    return result;
}
