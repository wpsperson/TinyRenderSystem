#pragma once
#include <string>

// now we use OpenGL 4.6
static const int g_OpenGLVersionMajor = 4;
static const int g_OpenGLVersionMinor = 6;

static const int DefaultWindowWidth = 1200;
static const int DefaultWindowHeight = 800;

struct GLFWwindow;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

char* readTextFile(const std::string& strFileName);

