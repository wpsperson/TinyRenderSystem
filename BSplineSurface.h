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
    void setCtrlPts(const float* pt, int ptNumInUDir, int ptNumInVDir);

    void calcKnotsByHardleyJuddy();

    void interpolatePoint(float u, float v, float* pt);

    void interpolateNormal(float u, float v, float* norm);

protected:
    int getUIndex(float u);
    int getVIndex(float v);
    // create m_m+1 pts which arrange in V direction.
    float* genCtrlPtsInVDir(float u);

    // create m_n+1 pts which arrange in U direction.
    float* genCtrlPtsInUDir(float v);
    float* copyOriginaPtInVDir(int uIndex);

private:
    int m_n; // num of ctrlpt -1 in u;
    int m_m; // num of ctrlpt -1 in v;
    const int m_k = 3;
    float* m_uknots;
    float* m_vknots;
    float* m_ctrlPts;
};
