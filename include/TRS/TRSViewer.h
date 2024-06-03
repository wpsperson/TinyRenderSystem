#pragma once

#include <memory>
#include <map>
#include <vector>
#include <chrono>

#include "TRS/TRSExport.h"
#include "TRS/TRSVector.h"

class TRSCamera;
class TRSNode;
class CullVisitor;
class TRSStateSet;
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

protected:
    std::shared_ptr<TRSNode> m_pSceneNode;
    TRSVec4 m_BGColor;
    TRSCamera* m_pCamera;
    CullVisitor* m_cullor;
    std::shared_ptr<TRSStateSet> m_pDefaultStateSet;
    PolygonModeVisitor* m_polygonModeVisitor;
    std::chrono::steady_clock::time_point m_fLastTime;
    std::chrono::steady_clock::time_point m_fCurTime;
};
