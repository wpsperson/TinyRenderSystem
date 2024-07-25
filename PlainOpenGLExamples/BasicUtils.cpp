#include "BasicUtils.h"
#include <iostream>
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

    std::size_t length = strContent.length();
    char* pChar = new char[length + 1];
    memcpy(pChar, strContent.c_str(), length);
    pChar[length] = '\0';
    return pChar;
}


int createProgramByShaderFiles(const char* vertFile, const char* fragFile)
{
    char* vertexShaderSource = readTextFile(vertFile);
    char* fragmentShaderSource = readTextFile(fragFile);
    return createProgramByShaderContent(vertexShaderSource, fragmentShaderSource);
}

int createProgramByShaderContent(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Failed to compile vertex shader:\n" << infoLog << std::endl;
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Failed to compile fragment shader:\n" << infoLog << std::endl;
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Failed to link program:\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
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


FloatPoint::FloatPoint()
{
}

FloatPoint::FloatPoint(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
{
}

FloatPoint FloatPoint::operator+(const FloatPoint& pt) const
{
    return FloatPoint(x + pt.x, y + pt.y, z + pt.z);
}

FloatPoint FloatPoint::operator*(float scale) const
{
    return FloatPoint(x * scale, y * scale, z * scale);
}

UVCoord::UVCoord()
{
}

UVCoord::UVCoord(float _u, float _v) : u(_u), v(_v)
{
}
