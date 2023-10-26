#include "GlfwApplication.h"

#include "Core\TRSConst.h"
#include "Core\TRSViewer.h"
#include "Core\TRSStateSet.h"
#include "Core\TRSTexture.h"
#include "Core\TRSShader.h"
#include "Core\TRSBox.h"
#include "DataModel\TRSGeode.h"
#include "DataModel\TRSGroup.h"
#include "Util\TRSResource.h"
#include "Math\TRSMathUtil.h"
#include "Geometry\TRSCube.h"
#include "Event\TRSEventDispatcher.h"
#include "Event\TRSShortcutKeyHandler.h"
#include "Camera\TRSDefaultCameraHandler.h"

#include <glfw/glfw3.h>



GlfwApplication::GlfwApplication()
{

}

GlfwApplication::~GlfwApplication()
{
    TRSStateSetManager::free();
    if (m_pCameraHandler)
    {
        delete m_pCameraHandler;
    }
    if (m_dispatcher)
    {
        delete m_dispatcher;
    }
    if (m_viewer)
    {
        delete m_viewer;
    }
}

bool GlfwApplication::initContext(std::string& error)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, g_OpenGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, g_OpenGLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* pWindow = glfwCreateWindow(DefaultWindowWidth, DefaultWindowHeight, "Tiny Render System", nullptr, nullptr);
    if (!pWindow)
    {
        glfwTerminate();
        error = "Failed to create window in glfw.";
        return false;
    }
    glfwMakeContextCurrent(pWindow);
    m_window = pWindow;
    return true;
}

bool GlfwApplication::onInitial()
{
    std::string error;
    if (!initContext(error))
    {
        return false;
    }
    m_viewer = new TRSViewer;
    if (!m_viewer->loadOpenGLAddress(LoadGLAddress(glfwGetProcAddress)))
    {
        glfwTerminate();
        return false;
    }
    m_viewer->initialViewer();
    m_dispatcher = new TRSEventDispatcher;
    TRSCamera* camera = m_viewer->getCamera();
    m_pCameraHandler = new TRSDefaultCameraHandler(camera);
    m_dispatcher->addEventHandler(m_pCameraHandler);

    TRSCube* cube = new TRSCube;
    TRSMesh* mesh = cube->getMesh();
    std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();
    pNode->setMesh(mesh);
    std::shared_ptr<TRSStateSet> pSS = pNode->getOrCreateStateSet();
    pSS->getTexture()->createTexture("resources/textures/opengl.png");
    pSS->getTexture()->createTexture("resources/textures/cube.png");
    pSS->getShader()->createProgram("shaders/PosColorTexMVPVertex.glsl", "shaders/MultiTextureFragment.glsl");

    std::shared_ptr<TRSGroup> root = std::make_shared<TRSGroup>();
    root->addChild(pNode);

    m_viewer->setSecenNode(root);
    m_pCameraHandler->setSceneBox(root->boundingBox());
    return true;
}

void GlfwApplication::connectEventDispatcher()
{
    glfwSetWindowUserPointer(m_window, m_dispatcher);

    glfwSetMouseButtonCallback(m_window, TRSMouseButtonCallBack);
    glfwSetCursorPosCallback(m_window, TRSMouseMoveCallBack);
    glfwSetScrollCallback(m_window, TRSMouseScrollCallBack);
    glfwSetKeyCallback(m_window, TRSKeyboardCallBack);
    glfwSetFramebufferSizeCallback(m_window, TRSWindowChangeCallBack);
}

void GlfwApplication::onPaint()
{
    m_viewer->frame();
}

void GlfwApplication::run()
{
    bool success = onInitial();
    if (!success)
    {
        return;
    }
    connectEventDispatcher();

    while (true)
    {
        if (glfwWindowShouldClose(m_window))
        {
            break;
        }
        onPaint();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void TRSWindowChangeCallBack(GLFWwindow* window, int w, int h)
{
    TRSEventDispatcher* dispatcher = static_cast<TRSEventDispatcher*>(glfwGetWindowUserPointer(window));
    dispatcher->dispatchWindowSizeChange(w, h);
}

void TRSMouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{
    double cursorx = 0, cursory = 0;
    int width, height;
    glfwGetCursorPos(window, &cursorx, &cursory);
    glfwGetWindowSize(window, &width, &height);
    int xpos = static_cast<int>(cursorx);
    int ypos = height - static_cast<int>(cursory);   // make sure, ypos is bottom_lower
    TRSEventDispatcher* dispatcher = static_cast<TRSEventDispatcher*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            dispatcher->dispatchLeftMousePress(xpos, ypos, mods);
        }
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            dispatcher->dispatchMiddleMousePress(xpos, ypos, mods);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            dispatcher->dispatchRightMousePress(xpos, ypos, mods);
        }
    }
    else if (action == GLFW_RELEASE)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            dispatcher->dispatchLeftMouseRelease(xpos, ypos, mods);
        }
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            dispatcher->dispatchMiddleMouseRelease(xpos, ypos, mods);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            dispatcher->dispatchRightMouseRelease(xpos, ypos, mods);
        }
    }
}

void TRSMouseMoveCallBack(GLFWwindow* window, double cursorx, double cursory)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    int xpos = static_cast<int>(cursorx);
    int ypos = height - static_cast<int>(cursory);   // make sure, ypos is bottom_lower
    TRSEventDispatcher* dispatcher = static_cast<TRSEventDispatcher*>(glfwGetWindowUserPointer(window));
    dispatcher->dispatchMouseMove(xpos, ypos);
}

void TRSKeyboardCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    TRSEventDispatcher* dispatcher = static_cast<TRSEventDispatcher*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS)
    {
        dispatcher->dispatchKeyPress(key, mods);
    }
    else if (action == GLFW_RELEASE)
    {
        dispatcher->dispatchKeyRelease(key, mods);
    }
}

void TRSMouseScrollCallBack(GLFWwindow* window, double xScroll, double yScroll)
{
    TRSEventDispatcher* dispatcher = static_cast<TRSEventDispatcher*>(glfwGetWindowUserPointer(window));
    dispatcher->dispatchMouseScroll(static_cast<int>(yScroll));
}

