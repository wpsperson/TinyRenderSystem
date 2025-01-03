#include "GlfwApplication.h"

#include "TRS/TRSDefConst.h"
#include "TRS/TRSViewer.h"
#include "TRS/TRSTexture.h"
#include "TRS/TRSShader.h"
#include "TRS/TRSBox.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSGroup.h"
#include "TRS/TRSMathUtil.h"
#include "TRS/TRSMesh.h"
#include "TRS/TRSCube.h"
#include "TRS/TRSCamera.h"
#include "TRS/TRSEventDispatcher.h"
#include "TRS/TRSShortcutKeyHandler.h"
#include "TRS/TRSDefaultCameraHandler.h"

#include <glfw/glfw3.h>

#include "VertexData.h"

GlfwApplication::GlfwApplication()
{

}

GlfwApplication::~GlfwApplication()
{
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

    TRSCube cube;
    TRSMesh* mesh = cube.getMesh();
    TRSGeode* pNode = new TRSGeode;
    pNode->copyShadedMesh(mesh);
    TRSTexture* texture = pNode->useTexture();
    texture->createTexture("resources/textures/opengl.png");
    texture->createTexture("resources/textures/cube.png");

    //TRSMesh* pMesh = pNode->useShadedMesh();
    //std::vector<TRSVec3> vertexs = { TRSVec3(0,0,0),TRSVec3(1,0,0),TRSVec3(1,1,0) };
    //std::vector<TRSVec3> normals = { TRSVec3(0,0,1),TRSVec3(0,0,1),TRSVec3(0,0,1) };
    //std::vector<TRSVec2> uvs = { TRSVec2(0,0), TRSVec2(1,0), TRSVec2(1,1) };
    //std::vector<TRSVec3> colors = { TRSVec3(1,0,0),TRSVec3(0,1,0),TRSVec3(0,0,1) };
    //std::vector<unsigned int> indices = { 0,1,2 };
    //pMesh->setVertex(vertexs);
    //pMesh->setNormal(normals);
    //// pMesh->setColor(colors);
    //pMesh->setUV(uvs);
    //pMesh->setIndices(indices);

    TRSGroup* root = new TRSGroup;
    root->addChild(pNode);

    m_viewer->setSecenNode(root);
    m_viewer->getCamera()->setProjectionMode(ProjectionMode::Parallel);
    m_viewer->getCamera()->fitToBox(root->boundingBox());
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

