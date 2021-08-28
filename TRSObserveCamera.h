/*!  
 *@brief    when rotate this camera, the facial pt is keep still, the camera position change
 *@author   wangps
 *@date     2021-8-28
 */

#pragma once
#include "TRSExport.h"
#include "TRSCamera.h"

class TRS_EXPORT TRSObserveCamera : public TRSCamera
{
public:
    TRSObserveCamera(GLFWwindow* pWindow);
    ~TRSObserveCamera();

    void keyboardCallBack(GLFWwindow* pWindow);

    void mouseMoveCallBack(GLFWwindow* pWindow, double xpos, double ypos);

    void mouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll);

    void mouseButtonCallBack(GLFWwindow* pWindow, int button, int action, int mods);

protected:
    // camera control parameter
    float m_fMouseSensity;  //鼠标移动的敏感系数
    float m_fPitch;         //鼠标上下移动产生的俯仰角 -89~89
    float m_fYaw;           //鼠标上下移动产生的偏航角 无限制
    double m_fLastX;
    double m_fLastY;

    bool m_leftMousePressed;      // press state.
    bool m_middleMousePressed;      // press state.
    int m_modifier;
};
