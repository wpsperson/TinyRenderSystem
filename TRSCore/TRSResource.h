/*!  
 *@brief    各种资源文件
 *@author   wangps
 *@date     2019年5月3日
 */

#pragma once
#include <vector>
#include "TRSVector.h"

static float Rectangle4Vertices[] = {
    0.5f, 0.5f, -0.5f,   // 右上角
    0.5f, -0.5f, -0.5f,  // 右下角
    -0.5f, -0.5f, -0.5f, // 左下角
    -0.5f, 0.5f, -0.5f   // 左上角
};

static unsigned int Rectangle6Indices[] = { // 注意索引从0开始! 
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};

static std::vector<TRSVec3> s_EmptyVec3;
static std::vector<TRSVec2> s_EmptyUV;

static std::vector<TRSPoint> s_BoxVertexs = 
{ 
    TRSPoint(-0.5f, -0.5f, -0.5f),
    TRSPoint(0.5f, -0.5f, -0.5f),
    TRSPoint(0.5f,  0.5f, -0.5f),
    TRSPoint(0.5f,  0.5f, -0.5f),
    TRSPoint(-0.5f,  0.5f, -0.5f),
    TRSPoint(-0.5f, -0.5f, -0.5f),
    TRSPoint(-0.5f, -0.5f,  0.5f),
    TRSPoint(0.5f, -0.5f,  0.5f),
    TRSPoint(0.5f,  0.5f,  0.5f),
    TRSPoint(0.5f,  0.5f,  0.5f),
    TRSPoint(-0.5f,  0.5f,  0.5f),
    TRSPoint(-0.5f, -0.5f,  0.5f),
    TRSPoint(-0.5f,  0.5f,  0.5f),
    TRSPoint(-0.5f,  0.5f, -0.5f),
    TRSPoint(-0.5f, -0.5f, -0.5f),
    TRSPoint(-0.5f, -0.5f, -0.5f),
    TRSPoint(-0.5f, -0.5f,  0.5f),
    TRSPoint(-0.5f,  0.5f,  0.5f),
    TRSPoint(0.5f,  0.5f,  0.5f),
    TRSPoint(0.5f,  0.5f, -0.5f),
    TRSPoint(0.5f, -0.5f, -0.5f),
    TRSPoint(0.5f, -0.5f, -0.5f),
    TRSPoint(0.5f, -0.5f,  0.5f),
    TRSPoint(0.5f,  0.5f,  0.5f),
    TRSPoint(-0.5f, -0.5f, -0.5f),
    TRSPoint(0.5f, -0.5f, -0.5f),
    TRSPoint(0.5f, -0.5f,  0.5f),
    TRSPoint(0.5f, -0.5f,  0.5f),
    TRSPoint(-0.5f, -0.5f,  0.5f),
    TRSPoint(-0.5f, -0.5f, -0.5f),
    TRSPoint(-0.5f,  0.5f, -0.5f),
    TRSPoint(0.5f,  0.5f, -0.5f),
    TRSPoint(0.5f,  0.5f,  0.5f),
    TRSPoint(0.5f,  0.5f,  0.5f),
    TRSPoint(-0.5f,  0.5f,  0.5f),
    TRSPoint(-0.5f,  0.5f, -0.5f)
};

static std::vector<TRSVec2> s_BoxUVs =
{
    TRSVec2(0.0f, 0.0f),
    TRSVec2(1.0f, 0.0f),
    TRSVec2(1.0f, 1.0f),
    TRSVec2(1.0f, 1.0f),
    TRSVec2(0.0f, 1.0f),
    TRSVec2(0.0f, 0.0f),
    TRSVec2(0.0f, 0.0f),
    TRSVec2(1.0f, 0.0f),
    TRSVec2(1.0f, 1.0f),
    TRSVec2(1.0f, 1.0f),
    TRSVec2(0.0f, 1.0f),
    TRSVec2(0.0f, 0.0f),
    TRSVec2(1.0f, 0.0f),
    TRSVec2(1.0f, 1.0f),
    TRSVec2(0.0f, 1.0f),
    TRSVec2(0.0f, 1.0f),
    TRSVec2(0.0f, 0.0f),
    TRSVec2(1.0f, 0.0f),
    TRSVec2(1.0f, 0.0f),
    TRSVec2(1.0f, 1.0f),
    TRSVec2(0.0f, 1.0f),
    TRSVec2(0.0f, 1.0f),
    TRSVec2(0.0f, 0.0f),
    TRSVec2(1.0f, 0.0f),
    TRSVec2(0.0f, 1.0f),
    TRSVec2(1.0f, 1.0f),
    TRSVec2(1.0f, 0.0f),
    TRSVec2(1.0f, 0.0f),
    TRSVec2(0.0f, 0.0f),
    TRSVec2(0.0f, 1.0f),
    TRSVec2(0.0f, 1.0f),
    TRSVec2(1.0f, 1.0f),
    TRSVec2(1.0f, 0.0f),
    TRSVec2(1.0f, 0.0f),
    TRSVec2(0.0f, 0.0f),
    TRSVec2(0.0f, 1.0f),
};

static std::vector<TRSVec3> s_BoxColors =
{
    TRSVec3(1.0f, 0.0f, 0.0f),
    TRSVec3(0.0f, 1.0f, 0.0f),
    TRSVec3(0.0f, 0.0f, 1.0f),
    TRSVec3(0.0f, 0.0f, 1.0f),
    TRSVec3(1.0f, 1.0f, 1.0f),
    TRSVec3(1.0f, 0.0f, 0.0f),
    TRSVec3(1.0f, 0.0f, 0.0f),
    TRSVec3(0.0f, 1.0f, 0.0f),
    TRSVec3(0.0f, 0.0f, 1.0f),
    TRSVec3(0.0f, 0.0f, 1.0f),
    TRSVec3(1.0f, 1.0f, 1.0f),
    TRSVec3(1.0f, 0.0f, 0.0f),
    TRSVec3(0.0f, 1.0f, 0.0f),
    TRSVec3(0.0f, 0.0f, 1.0f),
    TRSVec3(1.0f, 1.0f, 1.0f),
    TRSVec3(1.0f, 1.0f, 1.0f),
    TRSVec3(1.0f, 0.0f, 0.0f),
    TRSVec3(0.0f, 1.0f, 0.0f),
    TRSVec3(0.0f, 1.0f, 0.0f),
    TRSVec3(0.0f, 0.0f, 1.0f),
    TRSVec3(1.0f, 1.0f, 1.0f),
    TRSVec3(1.0f, 1.0f, 1.0f),
    TRSVec3(1.0f, 0.0f, 0.0f),
    TRSVec3(0.0f, 1.0f, 0.0f),
    TRSVec3(1.0f, 1.0f, 1.0f),
    TRSVec3(0.0f, 0.0f, 1.0f),
    TRSVec3(0.0f, 1.0f, 0.0f),
    TRSVec3(0.0f, 1.0f, 0.0f),
    TRSVec3(1.0f, 0.0f, 0.0f),
    TRSVec3(1.0f, 1.0f, 1.0f),
    TRSVec3(1.0f, 1.0f, 1.0f),
    TRSVec3(0.0f, 0.0f, 1.0f),
    TRSVec3(0.0f, 1.0f, 0.0f),
    TRSVec3(0.0f, 1.0f, 0.0f),
    TRSVec3(1.0f, 0.0f, 0.0f),
    TRSVec3(1.0f, 1.0f, 1.0f),
};

static std::vector<TRSVec3> s_BoxNormals =
{
    TRSVec3(0.0f,  0.0f, -1.0f),
    TRSVec3(0.0f,  0.0f, -1.0f),
    TRSVec3(0.0f,  0.0f, -1.0f),
    TRSVec3(0.0f,  0.0f, -1.0f),
    TRSVec3(0.0f,  0.0f, -1.0f),
    TRSVec3(0.0f,  0.0f, -1.0f),
    TRSVec3(0.0f,  0.0f, 1.0f),
    TRSVec3(0.0f,  0.0f, 1.0f),
    TRSVec3(0.0f,  0.0f, 1.0f),
    TRSVec3(0.0f,  0.0f, 1.0f),
    TRSVec3(0.0f,  0.0f, 1.0f),
    TRSVec3(0.0f,  0.0f, 1.0f),
    TRSVec3(-1.0f, 0.0f,  0.0f),
    TRSVec3(-1.0f, 0.0f,  0.0f),
    TRSVec3(-1.0f, 0.0f,  0.0f),
    TRSVec3(-1.0f, 0.0f,  0.0f),
    TRSVec3(-1.0f, 0.0f,  0.0f),
    TRSVec3(-1.0f, 0.0f,  0.0f),
    TRSVec3(1.0f,  0.0f,  0.0f),
    TRSVec3(1.0f,  0.0f,  0.0f),
    TRSVec3(1.0f,  0.0f,  0.0f),
    TRSVec3(1.0f,  0.0f,  0.0f),
    TRSVec3(1.0f,  0.0f,  0.0f),
    TRSVec3(1.0f,  0.0f,  0.0f),
    TRSVec3(0.0f, -1.0f,  0.0f),
    TRSVec3(0.0f, -1.0f,  0.0f),
    TRSVec3(0.0f, -1.0f,  0.0f),
    TRSVec3(0.0f, -1.0f,  0.0f),
    TRSVec3(0.0f, -1.0f,  0.0f),
    TRSVec3(0.0f, -1.0f,  0.0f),
    TRSVec3(0.0f,  1.0f,  0.0f),
    TRSVec3(0.0f,  1.0f,  0.0f),
    TRSVec3(0.0f,  1.0f,  0.0f),
    TRSVec3(0.0f,  1.0f,  0.0f),
    TRSVec3(0.0f,  1.0f,  0.0f),
    TRSVec3(0.0f,  1.0f,  0.0f),
};
