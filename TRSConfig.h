/*!  
 *@brief    ��ʼ���ú�������
 *@author   wangps
 *@date     2019��5��3��
 */
 
#pragma once
#include "TRSExport.h"

class GLFWwindow;

class TRS_EXPORT TRSConfig
{
public:
    TRSConfig();
    ~TRSConfig();
    static bool initGlfwWindowAndGLAD(int w, int y, GLFWwindow **window);


};

