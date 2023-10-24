#pragma once

#include <array>
#include "Math/TRSVector.h"

class TRSBox
{
public:
    TRSBox();

    TRSBox(const TRSPoint& pta, const TRSPoint& ptb);

    TRSBox(float x0, float x1, float y0, float y1, float z0, float z1);

    ~TRSBox();

    void mergePoint(const TRSPoint& pt);

    void mergeBox(const TRSBox& box);

    bool valid() const;

    TRSPoint center() const;

    TRSPoint minPt() const;

    TRSPoint maxPt() const;

    float length() const;

    float width() const;

    float height() const;

    float outSphereRadius() const;

    std::array<TRSPoint, 8> vertices();

    bool contain(const TRSPoint& pt) const;

    bool contain(const TRSBox& box) const;

    bool intersect(const TRSBox& box) const;

    TRSBox boolUnion(const TRSBox& box) const;

    TRSBox boolIntersect(const TRSBox& box) const;

public:
    float minx = 1.0f;
    float maxx = -1.0f;
    float miny = 1.0f;
    float maxy = -1.0f;
    float minz = 1.0f;
    float maxz = -1.0f;
};
