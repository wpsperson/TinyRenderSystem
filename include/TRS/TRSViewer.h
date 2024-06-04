#pragma once

#include <memory>
#include <map>
#include <vector>
#include <chrono>

#include "TRS/TRSExport.h"
#include "TRS/TRSDefEnum.h"
#include "TRS/TRSVector.h"

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

    void updateScene();

    void drawScene();

    TRSCamera* getCamera() const;

protected:
    void calcFrameTime();

    TRSShader* findShader(TRSGeode* node);

    TRSShader* getOrCreateShader(ShaderType type);

protected:
    std::shared_ptr<TRSNode> m_pSceneNode;
    TRSVec4 m_BGColor;
    TRSCamera* m_pCamera;
    CullVisitor* m_cullor;
    std::map<ShaderType, TRSShader*> m_shaders;
    PolygonModeVisitor* m_polygonModeVisitor;
    std::chrono::steady_clock::time_point m_fLastTime;
    std::chrono::steady_clock::time_point m_fCurTime;
};
