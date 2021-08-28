/*!  
 *@brief    相机
 *@author   wangps
 *@date     2019年5月3日
 */

#pragma once
#include "TRSExport.h"
#include "glm/glm.hpp"
struct GLFWwindow;

class TRS_EXPORT TRSCamera
{
public:
    TRSCamera(GLFWwindow* pWindow);
    ~TRSCamera();

    glm::mat4 getViewMatrix();

    glm::mat4 getProjectMatrix();

    glm::vec3 getCameraPos() const;

    glm::vec3 getCameraFront() const;

    void setCameraMode(bool parallelMode);

    void setAspect(double aspect);

    virtual void keyboardCallBack(GLFWwindow* pWindow);

    virtual void mouseMoveCallBack(GLFWwindow* pWindow, double xpos, double ypos);

    virtual void mouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll);

    virtual void mouseButtonCallBack(GLFWwindow* pWindow, int button, int action, int mods);

protected:
    GLFWwindow* m_pWindow;
    // view matrix parameter
    glm::vec3 m_pos;        //相机当前位置
    glm::vec3 m_front;      //相机的前向方向，相机当前位置指向观察中心点的向量
    glm::vec3 m_right;      //相机的右手方向，即局部坐标系的X轴向在世界坐标系中的向量
    glm::vec3 m_up;         //相机的头顶方向，即局部坐标系的Y轴向在世界坐标系中的向量
    // project matrix parameter
    float m_width;            // keep the aspect with window change
    float m_height;           // keep the aspect with window change
    float m_near;
    float m_far;
    float m_fFov;           // vertical angle, perspective mode only
    bool m_parallelMode;    // whether it is parallel project or perspective project.

};

