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
    if (m_parallelMode)
    {
        glm::mat4 projectMatrix = glm::ortho(-m_width/2, m_width/2, -m_height/2, m_height/2, m_near, m_far);
        return projectMatrix;
    }
    else
    {
        glm::mat4 projectMatrix = glm::perspective(glm::radians(m_fFov), m_width / m_height, m_near, m_far);
        return projectMatrix;
    }
}

glm::vec3 TRSCamera::getCameraPos() const
{
    return m_pos;
}

glm::vec3 TRSCamera::getCameraFront() const
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