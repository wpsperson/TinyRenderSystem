#include "TRSUtils.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "glad\glad.h"
#include "GLFW\glfw3.h"


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

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
