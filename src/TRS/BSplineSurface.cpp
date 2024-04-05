#include "TRS/BSplineSurface.h"
#include "TRS/BSpline.h"


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

void BSplineSurface::setCtrlPts(const float* pt, int ptNumInUDir, int ptNumInVDir)
{
    if (ptNumInUDir-1 < m_k || ptNumInVDir-1<m_k)
    {
        return;
    }
    m_n = ptNumInUDir - 1;
    m_m = ptNumInVDir - 1;
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
    float* ptInVDir = genCtrlPtsInVDir(u);
    BSpline bs;
    bs.setCtrlPts(ptInVDir, m_m+1);
    bs.interpolatePoint(v, pt);
    delete[] ptInVDir;
}

void BSplineSurface::interpolateNormal(float u, float v, float* norm)
{
    float tangentV[3];
    float* ptInVDir = genCtrlPtsInVDir(u);
    BSpline bs;
    bs.setCtrlPts(ptInVDir, m_m+1);
    bs.interpolateTangent(v, tangentV);
    normlize(tangentV);
    delete[] ptInVDir;

    float tangentU[3];
    float* ptInUDir = genCtrlPtsInUDir(v);
    BSpline bs2;
    bs2.setCtrlPts(ptInUDir, m_n+1);
    bs2.interpolateTangent(u, tangentU);
    normlize(tangentU);
    delete[] ptInUDir;

    cross(tangentU, tangentV, norm);
}

void BSplineSurface::convertPiecewiseBezierSurface()
{
    if (m_n == m_k && m_m == m_k)
    {
        return;
    }
    int new_n = (m_n - m_k)*(m_k - 1) + m_n;
    int new_m = (m_m - m_k)*(m_k - 1) + m_m;
    float* newUKnots = nullptr;
    float* newVKnots = nullptr;
    float** root = new float*[new_m + 1];
    for (int vIndex=0;vIndex<=new_m; vIndex++)
    {
        root[vIndex] = new float[(new_n + 1) * 3];
    }
    // first u direction
    for (int vIndex=0; vIndex<=m_m; vIndex++)
    {
        float* origCtrlPtsInUDir = m_ctrlPts + 3 * vIndex*(m_n + 1);
        BSpline bs;
        bs.setCtrlPts(origCtrlPtsInUDir, m_n + 1);
        bs.convertPiecewiseBezier();
        float* newCtrlPts = bs.getCtrlPt();
        memcpy(root[vIndex], newCtrlPts, (new_n + 1) * 3 * sizeof(float));
        if (!newUKnots)
        {
            newUKnots = new float[new_n + m_k + 2];
            memcpy(newUKnots, bs.getKnots(), (new_n + m_k + 2) * sizeof(float));
        }
    }
    // then v direction
    for (int uIndex = 0; uIndex<=new_n; uIndex++)
    {
        float* temp = new float[(new_m + 1) * 3];
        // pre process:copy root to temp.
        for (int vIndex = 0; vIndex<=/*new_m*/ m_m; vIndex++)//only [0 ~ m_m] is valid.
        {
            memcpy(temp + 3 * vIndex, root[vIndex] + 3 * uIndex, 3 * sizeof(float));
        }

        BSpline bs;
        bs.setCtrlPts(temp, m_m + 1);
        bs.convertPiecewiseBezier();
        float* newCtrlPts = bs.getCtrlPt();
        memcpy(temp, newCtrlPts, (new_m + 1) * 3 * sizeof(float));
        if (!newVKnots)
        {
            newVKnots = new float[new_m + m_k + 2];
            memcpy(newVKnots, bs.getKnots(), (new_m + m_k + 2) * sizeof(float));
        }

        // post process:copy temp to root.
        for (int vIndex = 0; vIndex <=new_m; vIndex++)
        {
            memcpy(root[vIndex] + 3 * uIndex, temp + 3 * vIndex, 3 * sizeof(float));
        }
        delete[]temp;
    }

    // finally, assign member variable.
    m_n = new_n;
    m_m = new_m;
    delete[]m_ctrlPts;
    m_ctrlPts = new float[(new_m + 1) * (new_n + 1) * 3];
    for (int vIndex = 0; vIndex <= new_m; vIndex++)
    {
        memcpy(m_ctrlPts + 3 * vIndex*(new_n + 1), root[vIndex], (new_n + 1) * 3 * sizeof(float));
        delete root[vIndex];
    }
    delete[]root;
    delete[]m_uknots;
    m_uknots = newUKnots;
    delete[]m_vknots;
    m_vknots = newVKnots;
}

float* BSplineSurface::getAllCtrlPt()
{
    return m_ctrlPts;
}

int BSplineSurface::getNumberOfPtsInUDir()
{
    return m_n + 1;
}

int BSplineSurface::getNumberOfPtsInVDir()
{
    return m_m + 1;
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

float* BSplineSurface::genCtrlPtsInVDir(float u)
{
    float* ctrlPts = new float[(m_m + 1)*3];
    for (int vIndex = 0; vIndex<=m_m; vIndex++)
    {
        float* resultPt = ctrlPts+ 3*vIndex;
        float* origCtrlPts = m_ctrlPts + 3 * vIndex*(m_n + 1);
        BSpline bs;
        bs.setCtrlPts(origCtrlPts, m_n+1);
        bs.interpolatePoint(u, resultPt);
    }
    return ctrlPts;
}

float* BSplineSurface::genCtrlPtsInUDir(float v)
{
    float* ctrlPts = new float[(m_n + 1) * 3];
    for (int uIndex = 0; uIndex <= m_n; uIndex++)
    {
        float* resultPt = ctrlPts + 3 * uIndex;
        float* origCtrlPts = copyOriginaPtInVDir(uIndex);
        BSpline bs;
        bs.setCtrlPts(origCtrlPts, m_m+1);
        bs.interpolatePoint(v, resultPt);
        delete[] origCtrlPts;
    }
    return ctrlPts;
}

float* BSplineSurface::copyOriginaPtInVDir(int uIndex)
{
    float* ctrlPtsInVDirAtUIndex = new float[(m_m + 1) * 3];
    memset(ctrlPtsInVDirAtUIndex, 0, (m_m + 1) * 3 * sizeof(float));
    float* curPt = ctrlPtsInVDirAtUIndex;
    for (int vIndex = 0; vIndex <= m_m; vIndex++)
    {
        float* pt = m_ctrlPts + 3 * (vIndex*(m_n + 1) + uIndex);
        memcpy(curPt, pt, 3 * sizeof(float));
        curPt += 3;
    }
    return ctrlPtsInVDirAtUIndex;
}
