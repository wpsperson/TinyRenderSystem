/*!  
 *@brief    TinyRenderSystem 的常量
 *@author   wangps
 *@date     2019年5月3日
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
//默认背景颜色，深蓝色
static glm::vec4 s_DefaultBGColor(0.2, 0.2, 0.4, 1);

//默认节点颜色，浅白色
static glm::vec4 s_DefaultNodeColor(0.5, 0.5, 0.5, 1);

static glm::vec3 s_DefaultCameraPos(0, 0, 3.0f);

static float s_NearDistance = 0.1f;  //近平面
static float s_FarDistance = 100.0f;   //远平面

static const char* s_TextUnitPrefix = "texture";

