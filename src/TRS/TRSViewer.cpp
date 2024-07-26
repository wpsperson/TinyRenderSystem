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
#include "TRS/TRSTexture.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSTexture.h"
#include "TRS/TRSVisitors.h"
#include "TRS/TRSMesh.h"
#include "TRS/TRSCharacterTexture.h"
#include "TRS/TRSFontManager.h"


TRSViewer::TRSViewer()
    : m_BGColor(s_DefaultBGColor)
{
    m_setting = new TRSSettings;
    m_polygonModeVisitor = new PolygonModeVisitor;
    m_pCamera = new TRSCamera;
    m_fontMgr = new TRSFontManager;
    m_cullor = new CullVisitor;
    m_unicodeTexture = new TRSTexture;
}

TRSViewer::~TRSViewer()
{
    delete m_unicodeTexture;
    delete m_cullor;
    delete m_fontMgr;
    delete m_pCamera;
    delete m_polygonModeVisitor;
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
    unsigned int unicodeTexId = TRSCharacterTexture::instance()->getTextureID();
    TextureData texData(unicodeTexId, "all_unicode_texture", "ourTexture");
    m_unicodeTexture->addSharedTexture(texData);

    if (!m_fontMgr->loadAsciiCharInfo(errorMsg))
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
    cullScene();
    classify();
    drawScene();
}

void TRSViewer::cullScene()
{
    TRSNode* root = m_pSceneNode.get();
    if (!root)
    {
        return;
    }
    m_cullor->clearUp();
    m_cullor->visit(root);
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
        geode->setActive();
        DrawItem item;
        item.geode = geode;
        for (RenderMode mode : renderModes)
        {
            if (geode->hasRenderMode(mode))
            {
                item.mode = mode;
                TRSShader * shader = find2Shader(geode, mode);
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
        TRSMatrix viewMatrix = m_pCamera->getViewMatrix();
        TRSMatrix projectMatrix = m_pCamera->getProjectMatrix();
        shader->setUniformMatrix4("view", viewMatrix);
        shader->setUniformMatrix4("projection", projectMatrix);
        shader->setUniform3v("viewPos", m_pCamera->getPosition());
        shader->setUniform3v("lightPos", m_pCamera->getPosition()); // s_DefaultLightPos
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

TRSShader* TRSViewer::find2Shader(TRSGeode* geode, RenderMode mode)
{
    TRSMesh* mesh = geode->getComponentMesh(mode);
    int meshStruct = mesh->getMeshStruct();
    bool hasNormal = (meshStruct & msNormal);
    bool hasUV = (meshStruct & msUV);

    TRSShader* shader = nullptr;
    if (RenderMode::Shaded == mode)
    {
        TRSTexture* texture = geode->getTexture();
        if (hasUV && texture && texture->count() == 2)
        {
            shader = getOrCreateShader(ShaderType::DualTexture);
        }
        else if (hasUV && texture && hasNormal)
        {
            shader = getOrCreateShader(ShaderType::PhongTexture);
        }
        else if (hasNormal)
        {
            shader = getOrCreateShader(ShaderType::Phong);
        }
        else if (hasUV)
        {
            shader = getOrCreateShader(ShaderType::FontShader);
        }
        else
        {
            shader = getOrCreateShader(ShaderType::Default);
        }
    }
    else
    {
        if (hasNormal)
        {
            shader = getOrCreateShader(ShaderType::Phong);
        }
        else
        {
            shader = getOrCreateShader(ShaderType::Default);
        }
    }
    return shader;
}

TRSShader* TRSViewer::findShader(TRSGeode* node)
{
    TRSMesh* mesh = node->useShadedMesh();
    TRSTexture* texture = nullptr;
    texture = node->getTexture();
    int meshStruct =  mesh->getMeshStruct();
    bool hasNormal = (meshStruct && msNormal);
    bool hasUV = (meshStruct && msUV);

    TRSShader* shader = nullptr;
    if (hasUV && texture && texture->count() == 2)
    {
        shader = getOrCreateShader(ShaderType::DualTexture);
    }
    else if (hasUV && texture && hasNormal)
    {
        shader = getOrCreateShader(ShaderType::PhongTexture);
    }
    else if(hasNormal)
    {
        shader = getOrCreateShader(ShaderType::Phong);
    }
    else
    {
        shader = getOrCreateShader(ShaderType::Default);
    }
    return shader;
}

TRSShader* TRSViewer::getOrCreateShader(ShaderType type)
{
    if (m_shaders.count(type))
    {
        return m_shaders.at(type);
    }

    TRSShader* shader = new TRSShader;
    if (ShaderType::Default == type)
    {
        shader->createProgram("shaders/DefaultVertex.glsl", "shaders/DefaultFragment.glsl");
    }
    else if (ShaderType::Phong == type)
    {
        shader->createProgram("shaders/PhongVertex.glsl", "shaders/PhongFragment.glsl");
    }
    else if (ShaderType::PhongTexture == type)
    {
        shader->createProgram("shaders/PosNormTexVertex.glsl", "shaders/PosNormTexFragment.glsl");
    }
    else if (ShaderType::DualTexture == type)
    {
        shader->createProgram("shaders/PosColorTexMVPVertex.glsl", "shaders/MultiTextureFragment.glsl");
    }
    else if (ShaderType::FontShader == type)
    {
        shader->createProgram("shaders/FontsVertex.glsl", "shaders/FontsFragment .glsl");
    }
    m_shaders.insert(std::make_pair(type, shader));
    return shader;
}

void TRSViewer::processTexture(unsigned int program, TRSGeode* geode)
{
    TRSTexture* texture = geode->getTexture();

    if (!texture && NodeType::ntTextNode == geode->nodeType())
    {
        texture = m_unicodeTexture;
    }
    if (!texture)
    {
        return;
    }
    texture->activeAllTextures(program);
}
