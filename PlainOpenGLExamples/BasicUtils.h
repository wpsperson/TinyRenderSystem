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

int createProgramUtils(const char* vertFile, const char* fragFile);

void vecAssign(float src[3], float dst[3]);
void vecScale(float src[3], float scale, float dst[3]);
void vecAdd(float v1[3], float v2[3], float output[3]);
