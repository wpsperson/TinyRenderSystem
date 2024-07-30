#pragma once

#include <chrono>
#include <map>
#include <vector>
#include <unordered_map>

#include "TRS/TRSExport.h"
#include "TRS/TRSDefEnum.h"
#include "TRS/TRSDefStruct.h"
#include "TRS/TRSVector.h"

class TRSSettings;
class TRSCamera;
class TRSFontManager;
class TRSTexture;
class TRSNode;
class TRSGeode;
class TRSShader;
class CullVisitor;
class PolygonModeVisitor;

using LoadGLAddress = void* (*)(const char* name);

class TRS_EXPORT TRSViewer
{
public:
    TRSViewer();
    ~TRSViewer();

    bool loadOpenGLAddress(LoadGLAddress func);

    bool loadOpenGL();

    void initialViewer();

    void setSecenNode(TRSNode* pSceneNode);

    void frame();

    TRSCamera* getCamera() const;

    TRSFontManager* getFontMgr() const;

protected:
    void cullScene();

    void classify();

    void drawScene();

    void calcFrameTime();

    TRSShader* find2Shader(TRSGeode* node, RenderMode mode);

    TRSShader* findShader(TRSGeode* node);

    TRSShader* getOrCreateShader(ShaderType type);

    void processTexture(unsigned int program, TRSGeode *node);

protected:
    TRSSettings* m_setting = nullptr;
    TRSFontManager* m_fontMgr = nullptr;
    TRSCamera* m_pCamera = nullptr;
    TRSNode* m_root = nullptr;
    CullVisitor* m_cullor = nullptr;
    TRSTexture* m_unicodeTexture = nullptr;
    TRSTexture* m_asciiTexture = nullptr;

    std::map<ShaderType, TRSShader*> m_shaders;
    std::unordered_map<TRSShader*, std::vector<DrawItem> > m_drawItems;
    PolygonModeVisitor* m_polygonModeVisitor;
    TRSVec4 m_BGColor;
    std::chrono::steady_clock::time_point m_fLastTime;
    std::chrono::steady_clock::time_point m_fCurTime;
};
