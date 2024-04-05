/*!
 *@brief     
 *@author     wangps
 *@date       2019年5月11日
 */

#pragma once
#include "TRS/TRSExport.h"
#include <memory>
#include <map>
#include <vector>
#include <chrono>
#include "TRS/TRSVector.h"

class TRSCamera;
class TRSEventDispatcher;
class TRSEventHandler;
class TRSDefaultCameraHandler;
class TRSShortcutKeyHandler;
class TRSNode;
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
    PolygonModeVisitor* m_polygonModeVisitor;
    std::map<int, std::vector<TRSNode*>> m_mapState2Node;
    std::chrono::steady_clock::time_point m_fLastTime;
    std::chrono::steady_clock::time_point m_fCurTime;
};
