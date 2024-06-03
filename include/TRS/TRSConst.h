#pragma once
#include "TRS/TRSVector.h"

enum EnVertexStruct
{
    EnVertex,
    EnVertexNormal,
    EnVertexTexture,
    EnVertexNormTexture,
    EnVertexTextureColor,
    EnVertexColorTexture,
};

enum VertexAttributeIndex
{
    VertexIndex = 0,
    NormalIndex = 1,
    TextureCoordIndex0 = 2,
    ColorIndex = 3,
    TextureCoordIndex1,
    TextureCoordIndex2,
};

enum MeshStruct
{
    msVertex = 1,
    msNormal = 2,
    msUV = 4,
    msColor = 8,
};

// now we use OpenGL 4.6
static const int g_OpenGLVersionMajor = 4;
static const int g_OpenGLVersionMinor = 6;

static const int DefaultWindowWidth = 1200;
static const int DefaultWindowHeight = 800;

//默认背景颜色，深蓝色
static TRSVec4 s_DefaultBGColor(0.2f, 0.2f, 0.4f, 1.0f);

//默认节点颜色，浅白色
static TRSVec4 s_DefaultNodeColor(0.5, 0.5, 0.5, 1);

static TRSVec3 s_DefaultCameraPos(0, 0, 3.0f);

static TRSVec3 s_DefaultLightPos(0.0f, 0.0f, 100.0f);

static float DefaultFov = 30.0f;
static float DefaultNearDistance = 0.1f;  //近平面
static float DefaultFarDistance = 100.0f;   //远平面

static const char* s_TextUnitPrefix = "texture";


constexpr double kMPI = 3.141592653589793;




