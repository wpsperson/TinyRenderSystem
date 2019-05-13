/*!  
 *@brief    ȫ���û�����ص�����
 *@author   wangps
 *@date     2019��5��5��
 */

#pragma once
#include "TRSExport.h"
class GLFWwindow;

//����ߴ�仯�ص�
TRS_EXPORT void TRSWindowChangeCallBack(GLFWwindow* window, int w, int h);

//����ƶ��ص�
TRS_EXPORT void TRSMouseMoveCallBack(GLFWwindow* pWindow, double xpos, double ypos);

//���̰��»ص�
TRS_EXPORT void TRSKeyboardCallBack(GLFWwindow* pWindow);

//�����ֻص�
TRS_EXPORT void TRSMouseScrollCallBack(GLFWwindow* pWindow, double xScroll, double yScroll);
