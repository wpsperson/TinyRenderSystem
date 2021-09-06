#include "TRSCamera.h"
#include <iostream>
#include "glfw/glfw3.h"
#include "TRSConst.h"
#include "TRSCallBackFunc.h"
#include "TRSConfig.h"
#include "TRSMathUtil.h"

TRSCamera::TRSCamera(GLFWwindow* pWindow)
    : m_pWindow(pWindow)
{
    m_pos = s_DefaultCameraPos;
    m_front = TRSVec3(0, 0, -1);
    m_right = TRSVec3(1, 0, 0);
    m_up = TRSVec3(0, 1, 0);

    m_width = DefaultWindowWidth;
    m_height = DefaultWindowHeight;
    m_near = DefaultNearDistance;
    m_far = DefaultFarDistance;
    m_fFov = 45.0f;
    m_parallelMode = false;
}


TRSCamera::~TRSCamera()
{
}

TRSMatrix TRSCamera::getViewMatrix()
{
    TRSMatrix matrix;
    matrix.makeLookat(m_pos, m_front, m_up);
    return matrix;
}

TRSMatrix TRSCamera::getProjectMatrix()
{
    TRSMatrix matrix;
    if (m_parallelMode)
    {
        matrix.makeOtho(-m_width / 2, m_width / 2, -m_height / 2, m_height / 2, m_near, m_far);
    }
    else
    {
        matrix.makePerspective(toRadian(m_fFov), m_width / m_height, m_near, m_far);
    }
    return matrix;
}

TRSVec3 TRSCamera::getCameraPos() const
{
    return m_pos;
}

TRSVec3 TRSCamera::getCameraFront() const
{
    return m_front;
}

void TRSCamera::setCameraMode(bool parallelMode)
{
    m_parallelMode = parallelMode;
}

void TRSCamera::setAspect(double aspect)
{
    m_width = aspect * m_height;
}

void TRSCamera::keyboardCallBack(GLFWwindow* pWindow)
{
    float MoveSpeed = 0.05f;
    if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, true);
    }
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_pos += m_front * MoveSpeed;
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_pos -= m_front * MoveSpeed;
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_pos -= m_right * MoveSpeed;
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_pos += m_right * MoveSpeed;
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_pos -= m_up * MoveSpeed;
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_pos += m_up * MoveSpeed;
    }
}

void TRSCamera::mouseMoveCallBack(GLFWwindow* pWindow, double xpos, double ypos)
{

}

void TRSCamera::mouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll)
{

}

void TRSCamera::mouseButtonCallBack(GLFWwindow* pWindow, int button, int action, int mods)
{

}