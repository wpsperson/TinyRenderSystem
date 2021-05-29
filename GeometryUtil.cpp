#include "GeometryUtil.h"

float distance(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float value = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1);
    return sqrt(value);
}

float distance(float* p1, float*p2)
{
    return distance(p1[0], p1[1], p1[2], p2[0], p2[1], p2[2]);
}

void divTwoPt(float* p1, float*p2, float prop, float* out)
{
    out[0] = p1[0] * (1 - prop) + p2[0] * prop;
    out[1] = p1[1] * (1 - prop) + p2[1] * prop;
    out[2] = p1[2] * (1 - prop) + p2[2] * prop;
}

void normlize(float* v)
{
    float leng = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= leng;
    v[1] /= leng;
    v[2] /= leng;
}

void add(float* p1, float* p2, float* out)
{
    out[0] = p1[0] + p2[0];
    out[1] = p1[1] + p2[1];
    out[2] = p1[2] + p2[2];
}


