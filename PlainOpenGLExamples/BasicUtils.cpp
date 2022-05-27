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



