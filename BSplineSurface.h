/*!
 *@brief    
 *@date     2021-5-29
 */

#pragma once
#include "GeometryUtil.h"

class BSplineSurface
{
public:
    BSplineSurface();
    ~BSplineSurface();
public:
    void setCtrlPts(const float* pt, int n, int m);

    void calcKnotsByHardleyJuddy();

    void interpolatePoint(float u, float v, float* pt);

    void interpolateNormal(float u, float v, float* norm);

protected:
    int getUIndex(float u);
    int getVIndex(float v);
private:
    int m_n; // num of ctrlpt -1 in u;
    int m_m; // num of ctrlpt -1 in v;
    const int m_k = 3;
    float* m_uknots;
    float* m_vknots;
    float* m_ctrlPts;
};