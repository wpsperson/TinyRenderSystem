/*!  
 *@brief    初始配置和上下文
 *@author   wangps
 *@date     2019年5月3日
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

    static bool registerUserInputFunc(GLFWwindow *window);

};

