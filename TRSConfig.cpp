#include "TRSConfig.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

TRSConfig::TRSConfig()
{
}


TRSConfig::~TRSConfig()
{
}

bool TRSConfig::initGlfwWindowAndGLAD(int w, int y, GLFWwindow **window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //������һ���ӣ�һ��Ҫ�ȴ���������ContextCurrent��Ȼ������glad��̬����opengl����
    GLFWwindow* pWindow = glfwCreateWindow(w, y, "Tiny Render System", nullptr, nullptr);
    if (!pWindow)
    {
        glfwTerminate();
        std::cout << "ERROR::GLFW::CREATEWINDOW" << std::endl;
        return false;
    }
    glfwMakeContextCurrent(pWindow);
    //�ȴ������壬����glad��̬����opengl����
    int ret = gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));
    if (!ret)
    {
        glfwTerminate();
        std::cout << "ERROR::GLAD::LOADPORC" << std::endl;
        return false;
    }
    *window = pWindow;
    return true;
}
