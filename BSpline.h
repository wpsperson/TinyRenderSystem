/*!
 *@brief    
 *@date     2021-5-26
 */

#pragma once
#include "GeometryUtil.h"

class BSpline
{
public:
    BSpline();
    ~BSpline();

    void setCtrlPts(const float* pt, int n);

    void calcKnotsByHardleyJuddy();

    void interpolatePoint(float u, float* pt);

    void interpolateTangent(float u, float* norm);

protected:
    int getIndex(float u);
private:
    int m_n;// num of ctrlpt -1;
    const int m_k = 3;
    float* m_knots;
    float* m_ctrlPts;
};


