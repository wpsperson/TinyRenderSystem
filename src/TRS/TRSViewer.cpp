#include "TRS/TRSViewer.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "TRS/TRSDefGL.h"
#include "TRS/TRSDefConst.h"
#include "TRS/TRSCamera.h"
#include "TRS/TRSNode.h"
#include "TRS/TRSStateSet.h"
#include "TRS/CullVisitor.h"
#include "TRS/TRSShader.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSTexture.h"
#include "TRS/TRSVisitors.h"
#include "TRS/TRSCharacterTexture.h"


TRSViewer::TRSViewer()
    : m_BGColor(s_DefaultBGColor)
{
    m_polygonModeVisitor = new PolygonModeVisitor;
    m_pCamera = new TRSCamera;
}

TRSViewer::~TRSViewer()
{
    delete m_polygonModeVisitor;
}

bool TRSViewer::loadOpenGLAddress(LoadGLAddress func)
{
    if (!gladLoadGLLoader(func))
    {
        return false;
    }
    return true;
}

bool TRSViewer::loadOpenGL()
{
    if (!gladLoadGL())
    {
        return false;
    }
    return true;
}

void TRSViewer::initialViewer()
{
    TRSCharacterTexture::instance()->genTexture();
    std::string errorMsg;
    bool loadSuccess = TRSCharacterTexture::instance()->loadFreeType(errorMsg);
    if (!loadSuccess)
    {
        std::cout << errorMsg << std::endl;
    }
    m_fCurTime = m_fLastTime = std::chrono::steady_clock::now();
    glEnable(GL_DEPTH_TEST);
}

void TRSViewer::setSecenNode(std::shared_ptr<TRSNode> pSceneNode)
{
    m_pSceneNode = pSceneNode;
    m_polygonModeVisitor->setTargetNode(m_pSceneNode.get());
}


void TRSViewer::frame()
{
    glClearColor(m_BGColor[0], m_BGColor[1], m_BGColor[2], m_BGColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    calcFrameTime();
    updateScene();
    drawScene();
}

void TRSViewer::updateScene()
{
    if (!m_pSceneNode)
    {
        return;
    }
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
            pNode->setActive();
            TRSMatrix modelMatrix = pNode->getMatrix();
            TRSMatrix viewMatrix = m_pCamera->getViewMatrix();
            TRSMatrix projectMatrix = m_pCamera->getProjectMatrix();
            pShader->addUniformMatrix4("model", modelMatrix);
            pShader->addUniformMatrix4("view", viewMatrix);
            pShader->addUniformMatrix4("projection", projectMatrix);
            pShader->addUniform3v("viewPos", m_pCamera->getPosition());
            pShader->addUniform4v("baseColor", pNode->getColor());
            pShader->addUniform3v("lightPos", m_pCamera->getPosition()); // s_DefaultLightPos
            if (pNode->getUpdateCallBack())
            {
                pNode->getUpdateCallBack()(pNode);
            }
            pShader->applayAllStaticUniform();//Apply Uniform
            // simple draw call
            pNode->preProcess();
            pNode->draw();
            pNode->postProcess();
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
        std::this_thread::sleep_for(std::chrono::milliseconds(15 - timeDiff));
        //Sleep(15 - timeDiff); Window API
    }
    m_fLastTime = m_fCurTime;
}

