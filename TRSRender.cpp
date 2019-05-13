#include "TRSRender.h"
#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "TRSShader.h"
#include "TRSTexture.h"
#include "TRSCamera.h"
#include "TRSCallBackFunc.h"


TRSRender::TRSRender(GLFWwindow * window)
    :m_pWindow(window), m_BGColor(0.2, 0.2, 0.4, 1)
{

}


TRSRender::TRSRender(GLFWwindow * window, TRSVAO* vao, TRSShader* pShader, TRSTexture* pTex)
    :m_pWindow(window), m_BGColor(0.2, 0.2, 0.4, 1)
{
    setVAO(vao);
    setShaders(pShader);
    setTexture(pTex);
}

TRSRender::~TRSRender()
{

}

void TRSRender::setTexture(TRSTexture* pTex)
{
    m_pTexture = pTex;
}


void TRSRender::setShaders(TRSShader* shader)
{
    m_pShader = shader;
}

void TRSRender::setVAO(TRSVAO* vao)
{
    m_pVAO = vao;
}

void TRSRender::setCamera(TRSCamera* pCamera)
{
    m_pCamera = pCamera;
}

void TRSRender::setWindow(GLFWwindow * window)
{
    m_pWindow = window;
}

void TRSRender::setBGColor(float r, float g, float b, float a)
{
    m_BGColor = glm::vec4(r, g, b, a);
}

void TRSRender::setBGColor(glm::vec4 color)
{
    m_BGColor = color;
}


void TRSRender::exec()
{
    if (!m_pWindow)
    {
        return;
    }
    //äÖÈ¾Ç°Ä¬ÈÏÉèÖÃ
    defaultSetting();
    while (true)
    {
        if (glfwWindowShouldClose(m_pWindow))
        {
            break;
        }
        glClearColor(m_BGColor.r, m_BGColor.g, m_BGColor.b, m_BGColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        TRSKeyboardCallBack(m_pWindow);

        m_pVAO->bind();
        m_pTexture->activeAllTextures();
        m_pShader->use();
        m_pShader->setUniformi("texture0", 0);
        m_pShader->setUniformi("texture1", 1);
        glm::mat4 viewMatrix = m_pCamera->getViewMatrix();
        glm::mat4 projectMatrix = m_pCamera->getProjectMatrix();
        m_pShader->setUniformMatrix4("model", glm::mat4());
        m_pShader->setUniformMatrix4("view", viewMatrix);
        m_pShader->setUniformMatrix4("projection", projectMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }
}

void TRSRender::defaultSetting()
{
    glEnable(GL_DEPTH_TEST);
}
