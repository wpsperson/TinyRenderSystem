#include "BSplineSurface.h"



BSplineSurface::BSplineSurface()
{

}

BSplineSurface::~BSplineSurface()
{
    if (m_uknots)
    {
        delete[]m_uknots;
        m_uknots = nullptr;
    }
    if (m_vknots)
    {
        delete[]m_vknots;
        m_vknots = nullptr;
    }
    if (m_ctrlPts)
    {
        delete[]m_ctrlPts;
        m_ctrlPts = nullptr;
    }
}

void BSplineSurface::setCtrlPts(const float* pt, int n, int m)
{
    if (n < m_k || m<m_k)
    {
        return;
    }
    m_n = n;
    m_m = m;
    int ptCount = (m_n + 1) * (m_m + 1);
    m_ctrlPts = new float[ptCount*3];
    memcpy(m_ctrlPts, pt, ptCount * 3 * sizeof(float));
    calcKnotsByHardleyJuddy();
}

void BSplineSurface::calcKnotsByHardleyJuddy()
{
    // Part One: u direction
    int uknotNum = m_n + m_k + 1 + 1;
    m_uknots = new float[uknotNum];
    memset(m_uknots, 0, sizeof(float)*uknotNum);
    m_uknots[m_n + 1] = m_uknots[m_n + 2] = m_uknots[m_n + 3] = m_uknots[m_n + 4] = 1.0;
    if (m_n > m_k)
    {
        // create 2 dimension array
        int segNum = m_n - m_k + 1;
        float* disArr = new float[m_n];
        float** segLengthArr = new float*[m_m + 1];
        for (int vIndex = 0; vIndex <= m_m; vIndex++)
        {
            segLengthArr[vIndex] = new float[segNum];
            float* curSegArr = segLengthArr[vIndex];
            memset(curSegArr, 0, sizeof(float)*segNum);

            memset(disArr, 0, sizeof(float)*m_n);
            for (int uIndex = 0; uIndex < m_n; uIndex++)
            {
                float* curPt = m_ctrlPts + ((vIndex*(m_n + 1)) + uIndex) * 3;
                float dist = distance(curPt, curPt + 3);
                disArr[uIndex] = dist;
            }
            for (int i = 0; i < segNum; i++)
            {
                curSegArr[i] = disArr[i] + disArr[i + 1] + disArr[i + 2];
            }
            for (int i = 0; i < segNum - 1; i++)
            {
                curSegArr[i + 1] += curSegArr[i];
            }
            for (int i = 0; i < segNum - 1; i++)
            {
                curSegArr[i] /= curSegArr[segNum - 1];
            }
        }
        // finally, calc the average.
        for (int i = 0; i < segNum - 1; i++) // do need iterate i ==segNum - 1, it keep 1.0 always.
        {
            float average = 0.0f;
            for (int vIndex = 0; vIndex <= m_m; vIndex++)
            {
                average += segLengthArr[vIndex][i];
            }
            m_uknots[i + m_k + 1] = average / (m_m + 1);
        }
        delete[] disArr;
        for (int vIndex = 0; vIndex <= m_m; vIndex++)
        {
            delete[]segLengthArr[vIndex];
        }
        delete[] segLengthArr;
    }
    // Part Two: v direction
    // Part One: u direction
    int vknotNum = m_m + m_k + 1 + 1;
    m_vknots = new float[vknotNum];
    memset(m_vknots, 0, sizeof(float)*vknotNum);
    m_vknots[m_m + 1] = m_vknots[m_m + 2] = m_vknots[m_m + 3] = m_vknots[m_m + 4] = 1.0;
    if (m_m > m_k)
    {
        // create 2 dimension array
        int segNum = m_m - m_k + 1;
        float* disArr = new float[m_m];
        float** segLengthArr = new float*[m_n + 1];
        for (int uIndex = 0; uIndex <= m_n; uIndex++)
        {
            segLengthArr[uIndex] = new float[segNum];
            float* curSegArr = segLengthArr[uIndex];
            memset(curSegArr, 0, sizeof(float)*segNum);

            memset(disArr, 0, sizeof(float)*m_m);
            for (int vIndex = 0; vIndex < m_m; vIndex++)
            {
                float* curPt = m_ctrlPts + ((vIndex*(m_n + 1)) + uIndex) * 3;
                float* nextPt = m_ctrlPts + (((vIndex + 1)*(m_n + 1)) + uIndex) * 3;
                float dist = distance(curPt, nextPt);
                disArr[vIndex] = dist;
            }
            for (int i = 0; i < segNum; i++)
            {
                curSegArr[i] = disArr[i] + disArr[i + 1] + disArr[i + 2];
            }
            for (int i = 0; i < segNum - 1; i++)
            {
                curSegArr[i + 1] += curSegArr[i];
            }
            for (int i = 0; i < segNum - 1; i++)
            {
                curSegArr[i] /= curSegArr[segNum - 1];
            }
        }
        // finally, calc the average.
        for (int i = 0; i < segNum - 1; i++) // do need iterate i ==segNum - 1, it keep 1.0 always.
        {
            float average = 0.0f;
            for (int uIndex = 0; uIndex <= m_n; uIndex++)
            {
                average += segLengthArr[uIndex][i];
            }
            m_vknots[i + m_k + 1] = average / (m_n + 1);
        }
        delete[] disArr;
        for (int uIndex = 0; uIndex <= m_m; uIndex++)
        {
            delete[]segLengthArr[uIndex];
        }
        delete[] segLengthArr;
    }
}

void BSplineSurface::interpolatePoint(float u, float v, float* pt)
{

}

void BSplineSurface::interpolateNormal(float u, float v, float* norm)
{

}

int BSplineSurface::getUIndex(float u)
{
    for (int i = m_k; i < m_n + 1; i++)
    {
        if (m_uknots[i] <= u && m_uknots[i + 1] >= u)
        {
            return i;
        }
    }
    return -1;
}

int BSplineSurface::getVIndex(float v)
{
    for (int i = m_k; i < m_m + 1; i++)
    {
        if (m_vknots[i] <= v && m_vknots[i + 1] >= v)
        {
            return i;
        }
    }
    return -1;
}
