#include "TRSUtils.h"
#include <iostream>
#include <sstream>
#include <fstream>


using namespace std;

char* getTextFromFile(const char* fileName)
{
    FILE* pFile = fopen(fileName, "rt");//这里加载一个PE文件  
    fseek(pFile, 0, SEEK_END);
    int len = ftell(pFile);
    char* szBuf = new char[len];
    memset(szBuf, 0, len);
    fseek(pFile, 0, SEEK_SET);
    int iRead = fread_s(szBuf, len, 1, len, pFile);
    return szBuf;
}

char* readTextFile(const std::string& strFileName)
{
    ifstream file;
    file.open(strFileName.c_str());
    stringstream strStream;
    strStream << file.rdbuf();
    file.close();
    std::string strContent = strStream.str();

    int length = strContent.length();
    char* pChar = new char[length + 1];
    memcpy(pChar, strContent.c_str(), length);
    pChar[length] = '\0';
    return pChar;
}

TRS_EXPORT float* createXYGridVertexArray(float interval, int size, int &arraySize)
{
    arraySize = 4 * size * 3;
    float* result = new float[arraySize];
    float* pt = result;
    int OneSideSize = size / 2;
    if (size%2 == 0)
    {
        // to do
    }
    else
    {
        // first draw horizon lines
        float ptToYAxis = OneSideSize * interval;
        for (int i = -OneSideSize; i<=OneSideSize; i++)
        {
            float currentY = i*interval;
            pt[0] = -ptToYAxis;
            pt[1] = currentY;
            pt[2] = 0;
            pt[3] = ptToYAxis;
            pt[4] = currentY;
            pt[5] = 0;
            pt += 6;
        }
        // then draw vertical lines
        for (int i = -OneSideSize; i <= OneSideSize; i++)
        {
            float currentX = i*interval;
            pt[0] = currentX;
            pt[1] = -ptToYAxis;
            pt[2] = 0;
            pt[3] = currentX;
            pt[4] = ptToYAxis;
            pt[5] = 0;
            pt += 6;
        }
    }
    return result;
}
