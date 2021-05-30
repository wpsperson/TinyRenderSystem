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

void cross(float* p1, float*p2, float* out)
{
    out[0] = p1[1] * p2[2] - p1[2] * p2[1];
    out[1] = p1[2] * p2[0] - p1[0] * p2[2];
    out[2] = p1[0] * p2[1] - p1[1] * p2[0];
}

unsigned int* genWireFrameElementsArray(int n, int m)
{
    if (n<2 || m<2)
    {
        return nullptr;
    }
    unsigned int *elementArray = new unsigned int[n*m * 6];
    unsigned int *curEle = elementArray;
    for (unsigned int v = 1; v<m; v++)
    {
        for (unsigned int u=1; u<n; u++)
        {
            unsigned int idxRightBtm= v*n + u;
            unsigned int idxLeftBtm = v*n + u - 1;
            unsigned int idxRightTop = (v - 1)*n + u;
            unsigned int idxLeftTop = (v - 1)*n + u - 1;
            *curEle = idxRightTop;
            curEle++;
            *curEle = idxLeftTop;
            curEle++;
            *curEle = idxLeftBtm;
            curEle++;
            *curEle = idxRightTop;
            curEle++;
            *curEle = idxLeftBtm;
            curEle++;
            *curEle = idxRightBtm;
            curEle++;
        }
    }
    return elementArray;
}

