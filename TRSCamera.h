﻿/*!  
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

    void initMember();

    glm::mat4 getViewMatrix();

    glm::mat4 getProjectMatrix();

    glm::vec3 getCameraPos() const;

    glm::vec3 getCameraFront() const;

    void keyboardCallBack(GLFWwindow* pWindow);

    void mouseMoveCallBack(GLFWwindow* pWindow, double xpos, double ypos);

    void mouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll);

    void mouseButtonCallBack(GLFWwindow* pWindow, int button, int action, int mods);

    void setWidthHeight(int w, int h);

protected:
    glm::vec3 m_pos;        //相机当前位置
    glm::vec3 m_front;      //相机的前向方向，相机当前位置指向观察中心点的向量
    glm::vec3 m_right;      //相机的右手方向，即局部坐标系的X轴向在世界坐标系中的向量
    glm::vec3 m_up;         //相机的头顶方向，即局部坐标系的Y轴向在世界坐标系中的向量

    int m_width;
    int m_height;

    float m_fMoveSpeed;     //wsad键盘移动系数
    float m_fMouseSensity;  //鼠标移动的敏感系数

    float m_fPitch;         //鼠标上下移动产生的俯仰角 -89~89
    float m_fYaw;           //鼠标上下移动产生的偏航角 无限制
    float m_fFov;          //鼠标滚轮滚动产生的fov放大缩小 45-90
    double m_fLastX;
    double m_fLastY;
    GLFWwindow* m_pWindow;
    
    int m_cameraMode;       //mouse mode, joystick or trackball.
    bool m_leftMousePressed;      // press state.
    bool m_middleMousePressed;      // press state.
    int m_modifier;

};

