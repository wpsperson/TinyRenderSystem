#include "TRSWindowConfig.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include "TRSCallBackFunc.h"

TRSWindowConfig::TRSWindowConfig()
{
}


TRSWindowConfig::~TRSWindowConfig()
{
}

bool TRSWindowConfig::initGlfwWindowAndGLAD(int w, int y, GLFWwindow **window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //这里有一个坑，一定要先创建上下文ContextCurrent，然后再让glad动态加载opengl函数
    GLFWwindow* pWindow = glfwCreateWindow(w, y, "Tiny Render System", nullptr, nullptr);
    if (!pWindow)
    {
        glfwTerminate();
        std::cout << "ERROR::GLFW::CREATEWINDOW" << std::endl;
        return false;
    }
    glfwMakeContextCurrent(pWindow);
    //先创建窗体，再让glad动态加载opengl函数
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

bool TRSWindowConfig::registerUserInputFunc(GLFWwindow *window)
{
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //注册用户鼠标按钮回调
    glfwSetMouseButtonCallback(window, TRSMouseButtonCallBack);
    //注册用户鼠标位置回调
    glfwSetCursorPosCallback(window, TRSMouseMoveCallBack);
    //鼠标滚轮滚动回调
    glfwSetScrollCallback(window, TRSMouseScrollCallBack);
    //
    glfwSetFramebufferSizeCallback(window, TRSWindowChangeCallBack);
    
    return true;
}
