#include "TRSUtils.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
char* getTextFromFile(const char* fileName)
{
    FILE* pFile = fopen(fileName, "rt");//�������һ��PE�ļ�  
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