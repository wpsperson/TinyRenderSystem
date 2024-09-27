#pragma once

#include "TRS/TRSDefEnum.h"

class TRSGeode;
struct DrawItem
{
    TRSGeode* geode;
    RenderMode mode;
};

struct AsciiCharInfo
{
    float width = 0.0f;
    float height = 0.0f;
    float offset_x = 0.0f;
    float offset_y = 0.0f;
    float stride = 0.0f;

    float uv_min_x = 0.0f;
    float uv_max_x = 0.0f;
    float uv_min_y = 0.0f;
    float uv_max_y = 0.0f;
};

struct TriIndex
{
    unsigned int idxa;
    unsigned int idxb;
    unsigned int idxc;
};