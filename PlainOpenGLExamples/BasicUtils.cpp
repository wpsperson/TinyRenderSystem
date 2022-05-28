#include "BasicUtils.h"
#include <fstream>
#include <sstream>
#include "glad\glad.h"
#include "glfw\glfw3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


char* readTextFile(const std::string& strFileName)
{
    std::ifstream file;
    file.open(strFileName.c_str());
    std::stringstream strStream;
    strStream << file.rdbuf();
    file.close();
    std::string strContent = strStream.str();

    int length = strContent.length();
    char* pChar = new char[length + 1];
    memcpy(pChar, strContent.c_str(), length);
    pChar[length] = '\0';
    return pChar;
}

void vecAssign(float src[3], float dst[3])
{
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

void vecScale(float src[3], float scale, float dst[3])
{
    dst[0] = src[0] * scale;
    dst[1] = src[1] * scale;
    dst[2] = src[2] * scale;
}

void vecAdd(float v1[3], float v2[3], float output[3])
{
    output[0] = v1[0] + v2[0];
    output[1] = v1[1] + v2[1];
    output[2] = v1[2] + v2[2];
}

