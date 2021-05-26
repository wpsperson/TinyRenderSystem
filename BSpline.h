/*!
 *@brief    
 *@date     2021-5-26
 */

#pragma once

class BSpline
{
public:
    BSpline();
    ~BSpline();

    void setCtrlPts(const float* pt, int n);

    void calcKnotsByHardleyJuddy();

    void interpolatePoint(float u, float* pt);

    void interpolateNormal(float u, float* norm);

protected:
    int getIndex(float u);
private:
    int m_n;// num of ctrlpt -1;
    const int m_k = 3;
    float* m_knots;
    float* m_ctrlPts;
};

float distance(float x1, float y1, float z1, float x2, float y2, float z2);
float distance(float* p1, float*p2);
void divTwoPt(float* p1, float*p2, float prop, float* out);
