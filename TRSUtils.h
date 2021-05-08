/*!  
 *@brief    工具方法和工具类
 *@author   wangps
 *@date     2019年5月3日
 */

#pragma once
#include <iostream>
#include <string>
#include "TRSExport.h"

struct GLFWwindow;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);

//deprecated
TRS_EXPORT char* getTextFromFile(const char* fileName);

//recommend
TRS_EXPORT char* readTextFile(const std::string& strFileName);

TRS_EXPORT float* createXYGridVertexArray(float interval, int size, int &arraySize);
