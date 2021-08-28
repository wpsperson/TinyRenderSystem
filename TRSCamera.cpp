#include "TRSCamera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "glfw/glfw3.h"
#include "TRSConst.h"
#include "TRSCallBackFunc.h"
#include "TRSConfig.h"

TRSCamera::TRSCamera(GLFWwindow* pWindow)
    : m_pWindow(pWindow)
{
    m_pos = s_DefaultCameraPos;
    m_front = glm::vec3(0, 0, -1);
    m_right = glm::vec3(1, 0, 0);
    m_up =    glm::vec3(0, 1, 0);

    m_width = DefaultWindowWidth;
    m_height = DefaultWindowHeight;
    m_near = DefaultNearDistance;
    m_far = DefaultFarDistance;
    m_fFov = 45.0f;
    m_parallelMode = false;

    m_fMoveSpeed = 0.05f;
    m_fMouseSensity = 0.05f;
}


TRSCamera::~TRSCamera()
{
}

glm::mat4 TRSCamera::getViewMatrix()
{
    glm::mat4 viewMatrix = glm::lookAt(m_pos, m_pos + m_front, m_up);
    return viewMatrix;
}

glm::mat4 TRSCamera::getProjectMatrix()
{
    glm::mat4 projectMatrix = glm::perspective(glm::radians(m_fFov), float(m_width) / m_height, m_near, m_far);
    return projectMatrix;
}

glm::vec3 TRSCamera::getCameraPos() const
{
    return m_pos;
}

glm::vec3 TRSCamera::getCameraFront() const
{
    return m_front;
}

void TRSCamera::setWidthHeight(int w, int h)
{
    m_width = w;
    m_height = h;
}

void TRSCamera::keyboardCallBack(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, true);
    }
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_pos += m_front * m_fMoveSpeed;
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_pos -= m_front * m_fMoveSpeed;
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_pos -= m_right * m_fMoveSpeed;
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_pos += m_right * m_fMoveSpeed;
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_pos -= m_up * m_fMoveSpeed;
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_pos += m_up * m_fMoveSpeed;
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