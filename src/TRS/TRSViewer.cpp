#include "TRS/TRSViewer.h"

#include <array>
#include <iostream>
#include <thread>
#include <chrono>

#include "TRS/TRSDefGL.h"
#include "TRS/TRSDefConst.h"
#include "TRS/TRSSettings.h"
#include "TRS/TRSCamera.h"
#include "TRS/TRSNode.h"
#include "TRS/CullVisitor.h"
#include "TRS/TRSShader.h"
#include "TRS/TRSPrograms.h"
#include "TRS/TRSTexture.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSDynamicText.h"
#include "TRS/TRSGroup.h"
#include "TRS/TRSTexture.h"
#include "TRS/TRSVisitors.h"
#include "TRS/TRSMesh.h"
#include "TRS/TRSCharacterTexture.h"
#include "TRS/TRSFontManager.h"
#include "TRS/TRSAxis.h"


TRSViewer::TRSViewer()
{
    m_setting = new TRSSettings;
    m_programs = new TRSPrograms;
    m_camera = new TRSCamera;
    m_fontMgr = new TRSFontManager;
    m_cullor = new CullVisitor;
    m_asciiTexture = new TRSTexture;
    m_unicodeTexture = new TRSTexture;
    m_polygonModeVisitor = new PolygonModeVisitor;
    m_axis = new TRSAxis;
}

TRSViewer::~TRSViewer()
{
    delete m_axis;
    delete m_root;
    delete m_polygonModeVisitor;
    delete m_unicodeTexture;
    delete m_asciiTexture;
    delete m_cullor;
    delete m_fontMgr;
    delete m_camera;
    delete m_programs;
    delete m_setting;
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
    if (!m_fontMgr->loadAsciiCharInfo(errorMsg))
    {
        std::cout << errorMsg << std::endl;
    }

    unsigned int unicodeTexId = TRSCharacterTexture::instance()->getTextureID();
    TextureData unicodeData(unicodeTexId, "global_unicode_texture", "texture0");
    m_unicodeTexture->addSharedTexture(unicodeData);

    unsigned int asciiTexId = m_fontMgr->fontTexutre();
    TextureData asciiData(asciiTexId, "global_ascii_texture", "texture0");
    m_asciiTexture->addSharedTexture(asciiData);

    m_axis->setSizeInfo(1.0f, 0.05f, 0.3f, 0.125f);
    m_axis->initialize(this);
    TRSBox box = m_axis->boundingBox();
    m_camera->fitToBox(box);
    // m_pCameraHandler->setSceneBox(box);

    m_fCurTime = m_fLastTime = std::chrono::steady_clock::now();
    glEnable(GL_DEPTH_TEST);
}

void TRSViewer::setSecenNode(TRSNode* pSceneNode)
{
    m_root = pSceneNode;
    m_polygonModeVisitor->setTargetNode(m_root);

    // initialized all nodes.
    m_root->initialize(this);
}

void TRSViewer::frame()
{
    const TRSColor& color = m_setting->backgroundColor();
    glClearColor(color[0], color[1], color[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (m_camera->projectionMode() != m_setting->projMode())
    {
        m_camera->setProjectionMode(m_setting->projMode());
    }
    calcFrameTime();
    cullScene();
    classify();
    drawScene();

    if (m_setting->displayAxis())
    {
        overlay();
    }
}

void TRSViewer::overlay()
{
    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, 200, 200);
    m_axis->customDraw(this);
    int width = m_camera->getWindowWidth();
    int height = m_camera->getWindowHeight();
    glViewport(0, 0, width, height);
}

TRSSettings* TRSViewer::getSettings() const
{
    return m_setting;
}

TRSPrograms* TRSViewer::getPrograms() const
{
    return m_programs;
}

TRSCamera* TRSViewer::getCamera() const
{
    return m_camera;
}

TRSFontManager* TRSViewer::getFontMgr() const
{
    return m_fontMgr;
}

void TRSViewer::processTexture(unsigned int program, TRSGeode* geode)
{
    TRSTexture* texture = geode->getTexture();

    if (!texture && NodeType::ntTextNode == geode->nodeType())
    {
        texture = m_unicodeTexture;
    }
    if (!texture && NodeType::ntDynamicText == geode->nodeType())
    {
        texture = m_asciiTexture;
    }
    if (!texture)
    {
        return;
    }
    texture->activeAllTextures(program);
}

void TRSViewer::cullScene()
{
    if (!m_root)
    {
        return;
    }
    m_cullor->clearUp();
    m_cullor->visit(m_root);
}

void TRSViewer::classify()
{
    m_drawItems.clear();
    const std::vector<TRSGeode*>& nodes = m_cullor->toRenderNodes();
    std::vector<RenderMode> renderModes;
    if (m_setting->displayShaded())
    {
        renderModes.emplace_back(RenderMode::Shaded);
    }
    if (m_setting->displayWireframe())
    {
        renderModes.emplace_back(RenderMode::WireFrame);
    }
    if (m_setting->displayPoints())
    {
        renderModes.emplace_back(RenderMode::Points);
    }

    for (TRSGeode* geode : nodes)
    {
        geode->dynamicUpdate(this);

        DrawItem item;
        item.geode = geode;
        for (RenderMode mode : renderModes)
        {
            if (geode->hasRenderMode(mode))
            {
                item.mode = mode;
                ShaderConfig config = TRSPrograms::toConfig(geode, mode);
                TRSShader * shader = m_programs->getShader(config);
                m_drawItems[shader].emplace_back(item);
            }
        }
    }
}

void TRSViewer::drawScene()
{
    for (const auto& [shader, items] : m_drawItems)
    {
        shader->use();
        TRSMatrix viewMatrix = m_camera->getViewMatrix();
        TRSMatrix projectMatrix = m_camera->getProjectMatrix();
        TRSMatrix viewProj = projectMatrix * viewMatrix;
        shader->setUniformMatrix4("view_proj", viewProj);
        shader->setUniform3v("viewPos", m_camera->getPosition());
        shader->setUniform3v("lightPos", m_camera->getPosition()); // s_DefaultLightPos
        for (const DrawItem &item : items)
        {
            TRSGeode* geode = item.geode;
            RenderMode mode = item.mode;
            TRSMatrix modelMatrix = geode->getMatrix();
            shader->setUniformMatrix4("model", modelMatrix);
            shader->setUniform4v("baseColor", geode->getColor());

            processTexture(shader->getProgramId(), geode);

            if (geode->getUpdateCallBack())
            {
                geode->getUpdateCallBack()(geode);
            }

            geode->preProcess(mode);
            geode->draw(mode);
            geode->postProcess(mode);
        }
    }
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
