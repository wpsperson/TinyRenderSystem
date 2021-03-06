﻿#include "TRSViewer.h"
#include "TRSWindowConfig.h"
#include "TRSConst.h"
#include <iostream>
#include "TRSCamera.h"
#include "glad\glad.h"
#include "glfw\glfw3.h"
#include "TRSCallBackFunc.h"
#include "TRSNode.h"
#include "TRSStateSet.h"
#include "CullVisitor.h"
#include "TRSShader.h"
#include "TRSGeode.h"
#include "TRSVAO.h"
#include "TRSTexture.h"
#include "Windows.h"
#include "TRSVisitors.h"

extern TRSCamera* g_pCamera;

TRSViewer::TRSViewer()
    :m_pWindow(nullptr)
    , m_BGColor(s_DefaultBGColor)
    , m_fLastTime(0.0f)
    , m_fCurTime(0.0f)
{
    if (!TRSWindowConfig::initGlfwWindowAndGLAD(DefaultWindowWidth, DefaultWindowHeight, &m_pWindow))
    {
        std::cerr << "TRSViewer::INIT::Failure" << std::endl;
    }
    m_pCamera = new TRSCamera(m_pWindow);
    g_pCamera = m_pCamera;
    TRSWindowConfig::registerUserInputFunc(m_pWindow);//reg user input callback
    m_polygonModeVisitor = new PolygonModeVisitor;
}

TRSViewer::~TRSViewer()
{
    delete m_polygonModeVisitor;
}

void TRSViewer::setSecenNode(std::shared_ptr<TRSNode> pSceneNode)
{
    m_pSceneNode = pSceneNode;
}

void TRSViewer::defaultSetting()
{
    m_fCurTime = m_fLastTime = glfwGetTime();
    glEnable(GL_DEPTH_TEST);
}

void TRSViewer::run()
{
    if (!m_pWindow)
    {
        return;
    }
    //渲染前默认设置
    defaultSetting();
    while (true)
    {
        if (glfwWindowShouldClose(m_pWindow))
        {
            break;
        }
        glClearColor(m_BGColor.r, m_BGColor.g, m_BGColor.b, m_BGColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        calcFrameTime();
        keyboardCallBack();
        updateScene();
        drawScene();

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }
}

void TRSViewer::updateScene()
{
    m_mapState2Node.clear();
    CullVisitor oCullVisitor;
    oCullVisitor.visit(m_pSceneNode.get());
    m_mapState2Node = oCullVisitor.getMapState2Node();
}

void TRSViewer::drawScene()
{
    std::map<int, std::vector<TRSNode*>>::iterator itr = m_mapState2Node.begin();
    for (; itr!=m_mapState2Node.end(); itr++)
    {
        std::shared_ptr<TRSStateSet> pStateSet = TRSStateSetManager::instance()->findStateSet(itr->first);

        //渲染组件：shader vao texture camera
        TRSShader* pShader = pStateSet->getShader();
        pShader->use();

        TRSTexture* pTexture = pStateSet->getTexture();
        pTexture->activeAllTextures(pShader->getProgramId());

        std::vector<TRSNode*>& nodeList = itr->second;
        for (std::vector<TRSNode*>::iterator itrNode = nodeList.begin(); itrNode!=nodeList.end(); itrNode++)
        {
            TRSGeode* pGeode = dynamic_cast<TRSGeode*>(*itrNode);
            std::shared_ptr<TRSVAO> pVao = pGeode->getVAO();
            pVao->bind();
            glm::mat4 modelMatrix = pGeode->getMatrix();
            glm::mat4 viewMatrix = m_pCamera->getViewMatrix();
            glm::mat4 projectMatrix = m_pCamera->getProjectMatrix();
            pShader->addUniformMatrix4("model", modelMatrix);
            pShader->addUniformMatrix4("view", viewMatrix);
            pShader->addUniformMatrix4("projection", projectMatrix);
            pShader->addUniform3v("viewPos", m_pCamera->getCameraPos());
            pShader->addUniform4v("baseColor", pGeode->getColor());
            pShader->addUniform3v("lightPos", s_DefaultLightPos);
            if (pGeode->getUpdateCallBack())
            {
                pGeode->getUpdateCallBack()(pGeode, m_pWindow);
            }
            pShader->applayAllStaticUniform();//Apply Uniform
            // simple draw call
            pGeode->draw();
            pVao->unBind();
        }

    }
}

void TRSViewer::calcFrameTime()
{
    m_fCurTime = glfwGetTime();
    float timeDiff = m_fCurTime - m_fLastTime;
    if (timeDiff < 15)
    {
        Sleep(15 - timeDiff);
    }
    m_fLastTime = m_fCurTime;
}


void TRSViewer::keyboardCallBack()
{
    TRSKeyboardCallBack(m_pWindow);
    if ((glfwGetKey(m_pWindow, GLFW_KEY_F1) == GLFW_PRESS))
    {
        m_polygonModeVisitor->switchPolygonMode();
        m_polygonModeVisitor->visit(m_pSceneNode.get());
        Sleep(100); // avoid frame rate so fast to execute this function twice and more.
    }
}
