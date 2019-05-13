/*!  
 *@brief    全局用户输入回调函数
 *@author   wangps
 *@date     2019年5月5日
 */

#pragma once
#include "TRSExport.h"
class GLFWwindow;

//窗体尺寸变化回调
TRS_EXPORT void TRSWindowChangeCallBack(GLFWwindow* window, int w, int h);

//鼠标移动回调
TRS_EXPORT void TRSMouseMoveCallBack(GLFWwindow* pWindow, double xpos, double ypos);

//键盘按下回调
TRS_EXPORT void TRSKeyboardCallBack(GLFWwindow* pWindow);

//鼠标滚轮回调
TRS_EXPORT void TRSMouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll);
