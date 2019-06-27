#include "TRSViewer.h"
#include "TRSConfig.h"
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

extern TRSCamera* g_pCamera;

TRSViewer::TRSViewer()
    :m_pWindow(nullptr)
    , m_BGColor(s_DefaultBGColor)
    , m_fLastTime(0.0f)
    , m_fCurTime(0.0f)
{
    if (!TRSConfig::initGlfwWindowAndGLAD(DefaultWindowWidth, DefaultWindowHeight, &m_pWindow))
    {
        std::cerr << "TRSViewer::INIT::Failure" << std::endl;
    }
    m_pCamera = new TRSCamera;
    g_pCamera = m_pCamera;
    TRSConfig::registerUserInputFunc(m_pWindow);//reg user input callback
}

TRSViewer::~TRSViewer()
{

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
        TRSKeyboardCallBack(m_pWindow);
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

        TRSTexture* pTexture = pStateSet->getTexture();
        pTexture->activeAllTextures();
        //渲染组件：shader vao texture camera
        TRSShader* pShader = pStateSet->getShader();
        pShader->use();
        pShader->setTexUniform(pTexture->getSampleNames());

        std::vector<TRSNode*>& nodeList = itr->second;
        for (std::vector<TRSNode*>::iterator itrNode = nodeList.begin(); itrNode!=nodeList.end(); itrNode++)
        {
            TRSGeode* pGeode = dynamic_cast<TRSGeode*>(*itrNode);
            std::shared_ptr<TRSVAO> pVao = pGeode->getVAO();
            pVao->bind();
            glm::mat4 modelMatrix = pGeode->getMatrix();
            glm::mat4 viewMatrix = m_pCamera->getViewMatrix();
            glm::mat4 projectMatrix = m_pCamera->getProjectMatrix();
            pShader->setUniformMatrix4("model", modelMatrix);
            pShader->setUniformMatrix4("view", viewMatrix);
            pShader->setUniformMatrix4("projection", projectMatrix);
            pShader->setUniform3v("viewPos", m_pCamera->getCameraPos());
            pShader->setUniform4v("baseColor", pGeode->getColor());
            pShader->applayAllStaticUniform();//Apply Uniform
            if (pGeode->getUpdateCallBack())
            {
                pGeode->getUpdateCallBack()(pGeode);
            }
            glDrawArrays(GL_TRIANGLES, 0, pGeode->getVAO()->getDrawCount());
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
