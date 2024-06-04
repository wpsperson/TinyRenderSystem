#pragma once
#include "TRS/TRSVector.h"


// now we use OpenGL 4.6
static const int g_OpenGLVersionMajor = 4;
static const int g_OpenGLVersionMinor = 6;

static const int DefaultWindowWidth = 1200;
static const int DefaultWindowHeight = 800;

//default background color, deep blue
static TRSVec4 s_DefaultBGColor(0.2f, 0.2f, 0.4f, 1.0f);

//default node color, light white
static TRSVec4 s_DefaultNodeColor(0.5, 0.5, 0.5, 1);

static TRSVec3 s_DefaultCameraPos(0, 0, 3.0f);

static TRSVec3 s_DefaultLightPos(0.0f, 0.0f, 100.0f);

static float DefaultFov = 30.0f;
static float DefaultNearDistance = 0.1f;    // near plane
static float DefaultFarDistance = 100.0f;   // far plane

static const char* s_TextUnitPrefix = "texture";


constexpr double kMPI = 3.141592653589793;




