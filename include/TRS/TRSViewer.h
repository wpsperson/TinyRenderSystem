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

    void cullScene();

    void classify();

    void drawScene();

    TRSCamera* getCamera() const;

    TRSFontManager* getFontMgr() const;

protected:
    void calcFrameTime();

    TRSShader* find2Shader(TRSGeode* node, RenderMode mode);

    TRSShader* findShader(TRSGeode* node);

    TRSShader* getOrCreateShader(ShaderType type);

    void processTexture(unsigned int program, TRSGeode *node);

protected:
    TRSSettings* m_setting = nullptr;
    TRSFontManager* m_fontMgr = nullptr;
    TRSTexture* m_unicodeTexture = nullptr;
    TRSTexture* m_asciiTexture = nullptr;
    TRSNode* m_root = nullptr;
    TRSCamera* m_pCamera = nullptr;
    CullVisitor* m_cullor = nullptr;
    TRSVec4 m_BGColor;
    std::unordered_map<TRSShader*, std::vector<DrawItem> > m_drawItems;
    std::map<ShaderType, TRSShader*> m_shaders;
    PolygonModeVisitor* m_polygonModeVisitor;
    std::chrono::steady_clock::time_point m_fLastTime;
    std::chrono::steady_clock::time_point m_fCurTime;
};
