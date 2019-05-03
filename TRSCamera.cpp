#include "TRSCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glfw/glfw3.h"
#include "TRSConst.h"

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

    m_fPitch = 0;
    m_fYaw = -90;
    m_fZoom = 45;
}

glm::mat4 TRSCamera::getViewMatrix()
{
    glm::mat4 viewMatrix = glm::lookAt(m_pos, m_pos + m_front, m_up);
    return viewMatrix;
}

glm::mat4 TRSCamera::getProjectMatrix()
{
    glm::mat4 projectMatrix = glm::perspective(m_fZoom, float(DefaultWindowWidth) / DefaultWindowHeight, s_NearDistance, s_FarDistance);
    return projectMatrix;
}

void TRSCamera::keyboardInput(GLFWwindow* pWindow)
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
}
