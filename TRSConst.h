/*!  
 *@brief    TinyRenderSystem �ĳ���
 *@author   wangps
 *@date     2019��5��3��
 */

#pragma once
#include <glm/glm.hpp>

#define DefaultWindowWidth 800
#define DefaultWindowHeight 600
//Ĭ�ϱ�����ɫ������ɫ
static glm::vec4 s_DefaultBGColor(0.2, 0.2, 0.4, 1);

static glm::vec3 s_DefaultCameraPos(0, 0, 3.0f);

static float s_NearDistance = 0.1;  //��ƽ��
static float s_FarDistance = 100;   //Զƽ��

