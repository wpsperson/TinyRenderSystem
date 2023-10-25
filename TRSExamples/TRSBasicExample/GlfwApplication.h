#pragma once
#include <string>

struct GLFWwindow;
class TRSViewer;
class TRSEventDispatcher;
class TRSDefaultCameraHandler;
class TRSShortcutKeyHandler;

class GlfwApplication
{
public:
    GlfwApplication();

    ~GlfwApplication();

    bool initContext(std::string &error);

    bool onInitial();

    void connectEventDispatcher();

    void onPaint();

    void run();

private:
    GLFWwindow* m_window = nullptr;
    TRSViewer* m_viewer = nullptr;
    TRSDefaultCameraHandler* m_pCameraHandler = nullptr;
};


void TRSWindowChangeCallBack(GLFWwindow* window, int w, int h);
void TRSMouseButtonCallBack(GLFWwindow* pWindow, int button, int action, int mods);
void TRSMouseMoveCallBack(GLFWwindow* pWindow, double xpos, double ypos);
void TRSKeyboardCallBack(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
void TRSMouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll);