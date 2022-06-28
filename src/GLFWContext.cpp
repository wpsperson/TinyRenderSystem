#include "GLFWContext.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include "TRSConst.h"
#include "TRSEventDispatcher.h"

TRSEventDispatcher* GLFWContext::m_eventDispatcher = nullptr;

GLFWContext::GLFWContext()
{
}


GLFWContext::~GLFWContext()
{
}

bool GLFWContext::initContext()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, g_OpenGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, g_OpenGLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //这里有一个坑，一定要先创建上下文ContextCurrent，然后再让glad动态加载opengl函数
    GLFWwindow* pWindow = glfwCreateWindow(DefaultWindowWidth, DefaultWindowHeight, "Tiny Render System", nullptr, nullptr);
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
    m_window = pWindow;
    return true;
}

void GLFWContext::connectEventDispatcher(TRSEventDispatcher* eventDispatcher)
{
    if (!m_window)
    {
        throw "connectEventDispatcher error!";
    }
    m_eventDispatcher = eventDispatcher;
    glfwSetMouseButtonCallback(m_window, TRSMouseButtonCallBack);
    glfwSetCursorPosCallback(m_window, TRSMouseMoveCallBack);
    glfwSetScrollCallback(m_window, TRSMouseScrollCallBack);
    glfwSetKeyCallback(m_window, TRSKeyboardCallBack);
    glfwSetFramebufferSizeCallback(m_window, TRSWindowChangeCallBack);
}


void GLFWContext::setShouldClose(bool close)
{
    int nShouldClose = close ? 1 : 0;
    glfwSetWindowShouldClose(m_window, nShouldClose);
}

bool GLFWContext::shouldClose()
{
    return glfwWindowShouldClose(m_window);;
}

void GLFWContext::swapBuffer()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

TRSEventDispatcher* GLFWContext::getDispatcher()
{
    return m_eventDispatcher;
}


void TRSWindowChangeCallBack(GLFWwindow* window, int w, int h)
{
    TRSEventDispatcher* dispatcher = GLFWContext::getDispatcher();
    dispatcher->dispatchWindowSizeChange(w, h);
}

void TRSMouseButtonCallBack(GLFWwindow* pWindow, int button, int action, int mods)
{
    TRSEventDispatcher* dispatcher = GLFWContext::getDispatcher();
    double xPos = 0, yPos = 0;
    glfwGetCursorPos(pWindow, &xPos, &yPos);
    if (action == GLFW_PRESS)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            dispatcher->dispatchLeftMousePress(xPos, yPos, mods);
        }
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            dispatcher->dispatchMiddleMousePress(xPos, yPos, mods);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            dispatcher->dispatchRightMousePress(xPos, yPos, mods);
        }
    }
    else if (action == GLFW_RELEASE)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            dispatcher->dispatchLeftMouseRelease(xPos, yPos, mods);
        }
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            dispatcher->dispatchMiddleMouseRelease(xPos, yPos, mods);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            dispatcher->dispatchRightMouseRelease(xPos, yPos, mods);
        }
    }
}

void TRSMouseMoveCallBack(GLFWwindow* pWindow, double xpos, double ypos)
{
    TRSEventDispatcher* dispatcher = GLFWContext::getDispatcher();
    dispatcher->dispatchMouseMove(xpos, ypos);
}

void TRSKeyboardCallBack(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    TRSEventDispatcher* dispatcher = GLFWContext::getDispatcher();
    if (action == GLFW_PRESS)
    {
        dispatcher->dispatchKeyPress(key);
    }
    else if (action == GLFW_RELEASE)
    {
        dispatcher->dispatchKeyRelease(key);
    }
}

void TRSMouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll)
{
    TRSEventDispatcher* dispatcher = GLFWContext::getDispatcher();
    dispatcher->dispatchMouseScroll(xScroll, yScroll);
}
