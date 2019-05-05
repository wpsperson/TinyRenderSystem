#include "TRSCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glfw/glfw3.h"
#include "TRSConst.h"
#include "TRSCallBackFunc.h"


TRSCamera::TRSCamera()
{
    initMember();
}


TRSCamera::~TRSCamera()
{
}

void TRSCamera::initMember()
{
    m_pos = s_DefaultCameraPos;
    m_front = glm::vec3(0, 0, -1.0f);
    m_right = glm::vec3(1.0f, 0, 0);
    m_up = glm::vec3(0, 1.0f, 0);

    m_fMoveSpeed = 0.05;
    m_fMouseSensity = 0.05;

    //pitch yaw 角度初值的含义是通过满足 m_front = glm::vec3(0, 0, -1.0f)反算出来的
    m_fPitch = 0;
    m_fYaw = -90;
    m_fFov = 45;

    m_fLastX = DefaultWindowWidth / 2;
    m_fLastY = DefaultWindowHeight / 2;
}

glm::mat4 TRSCamera::getViewMatrix()
{
    glm::mat4 viewMatrix = glm::lookAt(m_pos, m_pos + m_front, m_up);
    return viewMatrix;
}

glm::mat4 TRSCamera::getProjectMatrix()
{
    glm::mat4 projectMatrix = glm::perspective(glm::radians(m_fFov), float(DefaultWindowWidth) / DefaultWindowHeight, s_NearDistance, s_FarDistance);
    return projectMatrix;
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
    static bool s_bFirst = true;
    if (s_bFirst)
    {
        s_bFirst = false;
        m_fLastX = xpos;
        m_fLastY = ypos;
        return;
    }
    float xOffset = xpos - m_fLastX;
    float yOffset = -(ypos - m_fLastY);
    m_fLastX = xpos;
    m_fLastY = ypos;
    m_fYaw += m_fMouseSensity * xOffset;
    m_fPitch += m_fMouseSensity * yOffset;
    if (m_fPitch > 89.0f)
    {
        m_fPitch = 89.0f;
    }else if (m_fPitch < -89.0f)
    {
        m_fPitch = -89.0f;
    }

    m_front.y = sin(glm::radians(m_fPitch));
    m_front.x = cos(glm::radians(m_fPitch)) * cos(glm::radians(m_fYaw));
    m_front.z = cos(glm::radians(m_fPitch)) * sin(glm::radians(m_fYaw));
    m_front = glm::normalize(m_front);
    m_right = glm::cross(m_front, glm::vec3(0, 1, 0));
    m_right = glm::normalize(m_right);
    m_up = glm::cross(m_right, m_front);
}

void TRSCamera::mouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll)
{
    if (m_fFov >= 1.0f && m_fFov <= 45.0f)
        m_fFov -= yScroll;//yScroll increase, fov decrease, then scene looks bigger;
    if (m_fFov <= 1.0f)
        m_fFov = 1.0f;
    if (m_fFov >= 45.0f)
        m_fFov = 45.0f;
}
