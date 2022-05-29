/*!
 *@brief     
 *@author     wangps
 *@date       2019年5月11日
 */

#pragma once
#include "TRSExport.h"
#include <memory>
#include <map>
#include <vector>
#include <chrono>
#include "TRSVector.h"

class TRSContext;
class TRSCamera;
class TRSEventDispatcher;
class TRSEventHandler;
class TRSDefaultCameraHandler;
class TRSShortcutKeyHandler;
class TRSNode;
class PolygonModeVisitor;

class TRS_EXPORT TRSViewer
{
public:
    TRSViewer();
    ~TRSViewer();

    void setSecenNode(std::shared_ptr<TRSNode> pSceneNode);
    
    void run();

    void updateScene();

    void drawScene();

    TRSCamera* getCamera() const;

    void addEventHandler(TRSEventHandler* eventHandler);

protected:
    virtual void defaultSetting();
    void calcFrameTime();
    void keyboardViewCallBack();

protected:
    std::shared_ptr<TRSNode> m_pSceneNode;
    std::shared_ptr<TRSEventDispatcher> m_pEventDispatcher;
    std::shared_ptr<TRSDefaultCameraHandler> m_pCameraHandler;
    std::shared_ptr<TRSShortcutKeyHandler> m_pShortcutHandler;
    TRSContext* m_context = nullptr;
    TRSVec4 m_BGColor;
    TRSCamera* m_pCamera;
    PolygonModeVisitor* m_polygonModeVisitor;
    std::map<int, std::vector<TRSNode*>> m_mapState2Node;
    std::chrono::steady_clock::time_point m_fLastTime;
    std::chrono::steady_clock::time_point m_fCurTime;
};
