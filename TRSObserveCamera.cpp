#include "TRSObserveCamera.h"
#include "GLFW/glfw3.h"
#include "TRSConst.h"
#include "TRSMathUtil.h"

TRSObserveCamera::TRSObserveCamera(GLFWwindow* pWindow)
    : TRSCamera(pWindow)
{
    m_leftMousePressed = false;
    m_middleMousePressed = false;
    m_modifier = 0;
    m_fPitch = 0.0f;
    m_fYaw = -90.0f;
    m_fLastX = 0;
    m_fLastY = 0;
    m_fMouseSensity = 0.15f;
    m_facialPt = TRSVec3(0, 0, 0);
}

TRSObserveCamera::~TRSObserveCamera()
{

}

void TRSObserveCamera::keyboardCallBack(GLFWwindow* pWindow)
{
    TRSCamera::keyboardCallBack(pWindow);
}

void TRSObserveCamera::mouseMoveCallBack(GLFWwindow* pWindow, double xpos, double ypos)
{
    TRSCamera::mouseMoveCallBack(pWindow, xpos, ypos);
    if (m_leftMousePressed)// left mouse button pressed, do rotate
    {
        float xOffset = xpos - m_fLastX;
        float yOffset = -(ypos - m_fLastY);
        m_fLastX = xpos;
        m_fLastY = ypos;
        m_fYaw += m_fMouseSensity * xOffset;
        m_fPitch += m_fMouseSensity * yOffset;
        if (m_fPitch > 89.0f)
        {
            m_fPitch = 89.0f;
        }
        else if (m_fPitch < -89.0f)
        {
            m_fPitch = -89.0f;
        }
        float origDist = m_pos.distance(m_facialPt);

        m_front[1] = sin(toRadian(m_fPitch));
        m_front[0] = cos(toRadian(m_fPitch)) * cos(toRadian(m_fYaw));
        m_front[2] = cos(toRadian(m_fPitch)) * sin(toRadian(m_fYaw));
        m_front.normalize();
        m_pos = m_facialPt - m_front*origDist;
        m_right = m_front.cross(TRSVec3(0, 1, 0));
        m_right.normalize();
        m_up = m_right.cross(m_front);
        return;
    }
    else if (m_middleMousePressed)// middle mouse button pressed, do panning
    {
        // todo refactor
        double xOffset = xpos - m_fLastX;
        double yOffset = -(ypos - m_fLastY);
        //std::cout <<xOffset<< ", " << yOffset << std::endl;
        m_fLastX = xpos;
        m_fLastY = ypos;

        TRSVec3 delta = m_up * float(yOffset) + m_right * float(xOffset);
        delta = delta*0.001f;
        m_pos -= delta;
        m_facialPt -= delta;
        return;
    }
}

void TRSObserveCamera::mouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll)
{
    TRSCamera::mouseScrollCallBack(pWindow, xScroll, yScroll);
    if (m_fFov >= 1.0f && m_fFov <= 45.0f)
        m_fFov -= float(yScroll);//yScroll increase, fov decrease, then scene looks bigger;
    if (m_fFov <= 1.0f)
        m_fFov = 1.0f;
    if (m_fFov >= 45.0f)
        m_fFov = 45.0f;
}

void TRSObserveCamera::mouseButtonCallBack(GLFWwindow* pWindow, int button, int action, int mods)
{
    TRSCamera::mouseButtonCallBack(pWindow, button, action, mods);
    if (GLFW_MOUSE_BUTTON_LEFT == button)
    {
        if (action == GLFW_PRESS)
        {
            m_leftMousePressed = true;
            glfwGetCursorPos(m_pWindow, &m_fLastX, &m_fLastY);
        }
        else if (action == GLFW_RELEASE)
        {
            m_leftMousePressed = false;
        }
    }
    if (GLFW_MOUSE_BUTTON_MIDDLE == button)
    {
        if (action == GLFW_PRESS)
        {
            m_middleMousePressed = true;
            glfwGetCursorPos(m_pWindow, &m_fLastX, &m_fLastY);
        }
        else if (action == GLFW_RELEASE)
        {
            m_middleMousePressed = false;
        }
    }
    if (m_modifier != mods)
    {
        m_modifier = mods;
    }
}
