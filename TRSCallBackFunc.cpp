#include "TRSCallBackFunc.h"
#include "TRSCamera.h"
#include "glad\glad.h"

extern TRSCamera* g_pCamera;

TRS_EXPORT void TRSWindowChangeCallBack(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

TRS_EXPORT void TRSMouseMoveCallBack(GLFWwindow* pWindow, double xpos, double ypos)
{
    g_pCamera->mouseMoveCallBack(pWindow, xpos, ypos);
}

TRS_EXPORT void TRSKeyboardCallBack(GLFWwindow* pWindow)
{
    g_pCamera->keyboardCallBack(pWindow);
}

TRS_EXPORT void TRSMouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll)
{
    g_pCamera->mouseScrollCallBack(pWindow, xScroll, yScroll);
}
