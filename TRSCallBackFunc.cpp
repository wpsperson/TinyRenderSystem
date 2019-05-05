#include "TRSCallBackFunc.h"
#include "TRSCamera.h"

extern TRSCamera* g_pCamera;

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
