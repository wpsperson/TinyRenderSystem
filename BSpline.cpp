#include "BSpline.h"

BSpline::BSpline()
    : m_ctrlPts(nullptr)
    , m_knots(nullptr)
    , m_n(0)
{

}

BSpline::~BSpline()
{
    if (m_knots)
    {
        delete[]m_knots;
        m_knots = nullptr;
    }
    if (m_ctrlPts)
    {
        delete[]m_ctrlPts;
        m_ctrlPts = nullptr;
    }
}

void BSpline::setCtrlPts(const float* pt, int numberOfPts)
{
    if (numberOfPts -1 < m_k)
    {
        return;
    }
    m_n = numberOfPts - 1;
    m_ctrlPts = new float[3 * (m_n + 1)];
    memcpy(m_ctrlPts, pt, sizeof(float) * 3 * (m_n + 1));
    calcKnotsByHardleyJuddy();
}

void BSpline::calcKnotsByHardleyJuddy()
{
    int knotNum = m_n + m_k + 1 + 1;
    m_knots = new float[knotNum];
    memset(m_knots, 0, sizeof(float)*knotNum);
    m_knots[m_n + 1] = m_knots[m_n + 2] = m_knots[m_n + 3] = m_knots[m_n + 4] = 1.0;
    if (m_n == m_k)
    {
        return;
    }
    // Hardley-Juddy method
    float* disArr = new float[m_n];//distance between adjacent ctrl points.
    for (int i=0; i<m_n; i++)
    {
        float dist = distance(m_ctrlPts+ i * 3, m_ctrlPts+ 3 * i + 3);
        disArr[i] = dist;
    }
    int segNum = m_n - m_k + 1;
    float* segLengArr = new float[segNum]; // each spline segment length.
    for (int i=0; i<segNum; i++)
    {
        segLengArr[i] = disArr[i] + disArr[i + 1] + disArr[i + 2];
    }
    for (int i=1; i<segNum; i++)
    {
        segLengArr[i] += segLengArr[i - 1];
    }
    for (int i = 0; i<segNum-1; i++)
    {
        m_knots[i + m_k + 1] = segLengArr[i] / segLengArr[segNum - 1];
    }
    delete[] disArr;
    delete[] segLengArr;
}

void BSpline::interpolatePoint(float u, float* pt)
{
    int i = getIndex(u);
    float d[48];//16 pt
    // define  0->i-k, k-l-> i-l
    memcpy(d, m_ctrlPts + 3 * (i - m_k), 12 * sizeof(float)); // first line four ctrl pts. (L == 0)
    for (int L = 1; L<=m_k; L++)
    {
        for (int j=i-m_k, idx=0; j<=i-L; j++, idx++)
        {
            float* curPt = d + 3*(4*L+idx);
            float alpha = (u - m_knots[j + L]) / (m_knots[j + m_k + 1] - m_knots[j + L]);
            float* d_last_j = d + 3 * (4 *(L-1) + idx);
            float* d_last_j1 = d + 3 * (4 * (L - 1) + idx+1);
            divTwoPt(d_last_j, d_last_j1, alpha, curPt);
        }
    }
    float* ret = d + 3 * (4 * m_k);
    memcpy(pt, ret, sizeof(float) * 3);
}

void BSpline::interpolateTangent(float u, float* norm)
{
    int i = getIndex(u);
    float d[48];//16 pt
    memcpy(d, m_ctrlPts + 3 * (i - m_k), 12 * sizeof(float)); // first line four ctrl pts.
    // calc new ctrl pt
    for (int j = i - m_k, idx = 0; j <= i - 1; j++, idx++)      // second line three new ctrl pt.
    {
        float* curPt = d + 3 * (4 * 1 + idx);// First derivative
        float* d_last_j = d + 3 * (idx);
        float* d_last_j1 = d + 3 * (idx + 1);
        float alpha = m_k / (m_knots[j + m_k + 1] - m_knots[j + 1]);
        curPt[0] = alpha * (d_last_j1[0] - d_last_j[0]);
        curPt[1] = alpha * (d_last_j1[1] - d_last_j[1]);
        curPt[2] = alpha * (d_last_j1[2] - d_last_j[2]);
    }

    // then Deboor algorithm for third line and fourth line
    int new_k = m_k - 1;
    for (int L=1; L<= new_k; L++)
    {
        for (int j = i - new_k, idx = 0; j <= i - L; j++, idx++)
        {
            float* curPt = d + 3 * (4 *(L+1) + idx);
            float alpha = (u - m_knots[j + L]) / (m_knots[j + new_k + 1] - m_knots[j + L]);
            float* d_last_j = d + 3 * (4 * L + idx);
            float* d_last_j1 = d + 3 * (4 * L + idx + 1);
            divTwoPt(d_last_j, d_last_j1, alpha, curPt);
        }
    }

    float* ret = d + 3 * (4 * m_k);
    memcpy(norm, ret, sizeof(float) * 3);
}

void BSpline::convertPiecewiseBezier()
{
    if (m_n == m_k)
    {
        return;
    }
    int new_n = (m_n - m_k)*(m_k -1) + m_n;// each knot except endpoint repeat insert (m_k - 1)
    // first recalc new knots array.
    float* newKnots = new float[new_n + m_k + 1 + 1];
    memset(newKnots, 0, (new_n + m_k + 1 + 1) * sizeof(float));
    newKnots[new_n + 1] = newKnots[new_n + 2] = newKnots[new_n + 3] = newKnots[new_n + 4] = 1.0;
    float* curKnot = newKnots + m_k + 1;
    for (int i=m_k+1; i<=m_n; i++)
    {
        float value = m_knots[i];
        *(curKnot++) = value;
        *(curKnot++) = value;
        *(curKnot++) = value;
    }

    // then recalc new ctrl points
    float* newCtrlPts = new float[(new_n + 1) * 3];
    memset(newCtrlPts, 0, (new_n + 1) * 3 * sizeof(float));
    float* curNewPt = newCtrlPts + new_n * 3;
    float* curOldPt = m_ctrlPts + m_n * 3;
    memcpy(curNewPt, curOldPt, 3 * sizeof(float));
    float d1_j[3];
    float d1_j1[3];
    float d2[3];
    for (int i=m_n; i>=m_k+1; i--)
    {
        int j = i - m_k;
        // insert two repeat same knot at knots[i]

    }
    //for (int s=1; s<=2; s++)
    //{
    //    for ()
    //    {
    //    }
    //}

}

int BSpline::getIndex(float u)
{
    for (int i=m_k; i<m_n+1; i++)
    {
        if (m_knots[i] <= u && m_knots[i+1] >=u)
        {
            return i;
        }
    }
    return -1;
}
