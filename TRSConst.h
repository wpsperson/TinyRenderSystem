/*!  
 *@brief    TinyRenderSystem �ĳ���
 *@author   wangps
 *@date     2019��5��3��
 */

#pragma once
#include <glm/glm.hpp>

enum EnVertexStruct
{
    EnVertex,
    EnVertexTexture,
    EnVertexTextureColor,
    EnVertexColorTexture,
    EnVertexNormal
};


#define DefaultWindowWidth 800
#define DefaultWindowHeight 600
//Ĭ�ϱ�����ɫ������ɫ
static glm::vec4 s_DefaultBGColor(0.2, 0.2, 0.4, 1);

//Ĭ�Ͻڵ���ɫ��ǳ��ɫ
static glm::vec4 s_DefaultNodeColor(0.5, 0.5, 0.5, 1);

static glm::vec3 s_DefaultCameraPos(0, 0, 3.0f);

static float s_NearDistance = 0.1f;  //��ƽ��
static float s_FarDistance = 100.0f;   //Զƽ��

static const char* s_TextUnitPrefix = "texture";

