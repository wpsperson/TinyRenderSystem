#pragma once
#include <string>

// now we use OpenGL 4.6
static const int g_OpenGLVersionMajor = 4;
static const int g_OpenGLVersionMinor = 6;

static const int DefaultWindowWidth = 1200;
static const int DefaultWindowHeight = 800;

static float IdentityMatrix[16] = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };

struct GLFWwindow;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

char* readTextFile(const std::string& strFileName);

int createProgramByShaderFiles(const char* vertFile, const char* fragFile);

int createProgramByShaderContent(const char* vertShader, const char* fragShader);

void vecAssign(float src[3], float dst[3]);
void vecScale(float src[3], float scale, float dst[3]);
void vecAdd(float v1[3], float v2[3], float output[3]);

struct FloatPoint
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    FloatPoint();

    FloatPoint(float _x, float _y, float _z);

    FloatPoint operator+(const FloatPoint& pt) const;

    FloatPoint operator*(float scale) const;
};

struct UVCoord
{
    float u = 0.0f;
    float v = 0.0f;
    UVCoord();
    UVCoord(float _u, float _v);
};

