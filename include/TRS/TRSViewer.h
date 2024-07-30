#pragma once

#include <chrono>
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
class TRSPrograms;
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

    void processTexture(unsigned int program, TRSGeode *node);

protected:
    TRSSettings* m_setting = nullptr;
    TRSPrograms* m_programs = nullptr;
    TRSCamera* m_camera = nullptr;
    TRSFontManager* m_fontMgr = nullptr;
    TRSNode* m_root = nullptr;
    CullVisitor* m_cullor = nullptr;
    TRSTexture* m_asciiTexture = nullptr;
    TRSTexture* m_unicodeTexture = nullptr;
    PolygonModeVisitor* m_polygonModeVisitor = nullptr;

    TRSVec4 m_BGColor;
    std::unordered_map<TRSShader*, std::vector<DrawItem> > m_drawItems;
    std::chrono::steady_clock::time_point m_fLastTime;
    std::chrono::steady_clock::time_point m_fCurTime;
};
