#include "TRSViewer.h"
#include "GLFWContext.h"
#include "TRSConst.h"
#include <iostream>
#include "TRSCamera.h"
#include "TRSNode.h"
#include "TRSStateSet.h"
#include "CullVisitor.h"
#include "TRSShader.h"
#include "TRSGeode.h"
#include "TRSVAO.h"
#include "TRSTexture.h"
#include "Windows.h"
#include "TRSVisitors.h"
#include "TRSCharacterTexture.h"
#include "TRSEventDispatcher.h"
#include "TRSDefaultCameraHandler.h"


TRSViewer::TRSViewer()
    : m_BGColor(s_DefaultBGColor)
{
    m_pEventDispatcher = std::make_shared<TRSEventDispatcher>();
    m_context = new GLFWContext;
    m_context->initContext();
    m_pCamera = new TRSCamera;
    m_pCameraHandler = std::make_shared<TRSDefaultCameraHandler>(m_pCamera);
    m_pEventDispatcher->addEventHandler(m_pCameraHandler.get());
    m_context->connectEventDispatcher(m_pEventDispatcher.get());
    m_polygonModeVisitor = new PolygonModeVisitor;

    TRSCharacterTexture::instance()->genTexture();
    std::string errorMsg;
    bool loadSuccess = TRSCharacterTexture::instance()->loadFreeType(errorMsg);
    if (!loadSuccess)
    {
        std::cout << errorMsg << std::endl;
    }
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
    m_fCurTime = m_fLastTime = std::chrono::steady_clock::now();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TRSViewer::run()
{
    //渲染前默认设置
    defaultSetting();
    while (true)
    {
        if (m_context->shouldClose())
        {
            break;
        }
        glClearColor(m_BGColor[0], m_BGColor[1], m_BGColor[2], m_BGColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        calcFrameTime();
        updateScene();
        drawScene();
        m_context->swapBuffer();
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
            TRSNode* pNode = *itrNode;
            std::shared_ptr<TRSVAO> pVao = pNode->getVAO();
            pVao->bind();
            TRSMatrix modelMatrix = pNode->getMatrix();
            TRSMatrix viewMatrix = m_pCamera->getViewMatrix();
            TRSMatrix projectMatrix = m_pCamera->getProjectMatrix();
            pShader->addUniformMatrix4("model", modelMatrix);
            pShader->addUniformMatrix4("view", viewMatrix);
            pShader->addUniformMatrix4("projection", projectMatrix);
            pShader->addUniform3v("viewPos", m_pCamera->getPosition());
            pShader->addUniform4v("baseColor", pNode->getColor());
            pShader->addUniform3v("lightPos", s_DefaultLightPos);
            if (pNode->getUpdateCallBack())
            {
                pNode->getUpdateCallBack()(pNode);
            }
            pShader->applayAllStaticUniform();//Apply Uniform
            // simple draw call
            pNode->draw();
            pVao->unBind();
        }

    }
}

TRSCamera* TRSViewer::getCamera() const
{
    return m_pCamera;
}

void TRSViewer::calcFrameTime()
{
    m_fCurTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds result = std::chrono::duration_cast<std::chrono::milliseconds>(m_fCurTime - m_fLastTime);
    long long timeDiff = result.count();
    if (timeDiff < 15)
    {
        Sleep(15 - timeDiff);
    }
    m_fLastTime = m_fCurTime;
}


void TRSViewer::keyboardViewCallBack()
{
    // to do
    //if ((glfwGetKey(m_pWindow, GLFW_KEY_F1) == GLFW_PRESS))
    //{
    //    m_polygonModeVisitor->switchPolygonMode();
    //    m_polygonModeVisitor->visit(m_pSceneNode.get());
    //    Sleep(100); // avoid frame rate so fast to execute this function twice and more.
    //}
}
