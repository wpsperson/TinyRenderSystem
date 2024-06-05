#pragma once

#include <chrono>
#include <memory>
#include <map>
#include <vector>
#include <unordered_map>

#include "TRS/TRSExport.h"
#include "TRS/TRSDefEnum.h"
#include "TRS/TRSDefStruct.h"
#include "TRS/TRSVector.h"

class TRSSettings;
class TRSCamera;
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

    void setSecenNode(std::shared_ptr<TRSNode> pSceneNode);

    void frame();

    void cullScene();

    void classify();

    void drawScene();

    TRSCamera* getCamera() const;

protected:
    void calcFrameTime();

    TRSShader* find2Shader(TRSGeode* node, RenderMode mode);

    TRSShader* findShader(TRSGeode* node);

    TRSShader* getOrCreateShader(ShaderType type);

protected:
    TRSSettings* m_setting = nullptr;
    std::shared_ptr<TRSNode> m_pSceneNode;
    TRSVec4 m_BGColor;
    TRSCamera* m_pCamera;
    CullVisitor* m_cullor;
    std::unordered_map<TRSShader*, std::vector<DrawItem> > m_drawItems;
    std::map<ShaderType, TRSShader*> m_shaders;
    PolygonModeVisitor* m_polygonModeVisitor;
    std::chrono::steady_clock::time_point m_fLastTime;
    std::chrono::steady_clock::time_point m_fCurTime;
};
