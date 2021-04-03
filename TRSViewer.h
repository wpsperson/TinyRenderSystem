/*!
 *@brief     
 *@author     wangps
 *@date       2019年5月11日
 */

#pragma once
#include "TRSExport.h"
#include <memory>
#include "glm\glm.hpp"
#include <map>
#include <vector>

class TRSCamera;
class TRSNode;
struct GLFWwindow;

class TRS_EXPORT TRSViewer
{
public:
    TRSViewer();
    ~TRSViewer();

    void setSecenNode(std::shared_ptr<TRSNode> pSceneNode);
    
    void run();

    void updateScene();

    void drawScene();

protected:
    virtual void defaultSetting();
    void calcFrameTime();

protected:
    std::shared_ptr<TRSNode> m_pSceneNode;
    GLFWwindow* m_pWindow;
    glm::vec4 m_BGColor;
    TRSCamera* m_pCamera;

    std::map<int, std::vector<TRSNode*>> m_mapState2Node;
    float m_fLastTime;
    float m_fCurTime;
};
