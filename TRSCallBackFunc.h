/*!  
 *@brief    全局用户输入回调函数
 *@author   wangps
 *@date     2019年5月5日
 */

#pragma once
#include "TRSExport.h"
class GLFWwindow;

TRS_EXPORT void TRSMouseMoveCallBack(GLFWwindow* pWindow, double xpos, double ypos);


TRS_EXPORT void TRSKeyboardCallBack(GLFWwindow* pWindow);


TRS_EXPORT void TRSMouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll);
