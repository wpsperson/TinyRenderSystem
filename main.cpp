﻿#include "TRSUtils.h"
#include "TRSWindowConfig.h"
#include "TRSShader.h"
#include "AllDemoCodes.h"
#include "CaseStencilTest.h"
#include "CaseTessellationOnShader.h"
#include "CasePNTriangles.h"

int main(int argn, char** argc)
{

    //CaseFirstTriangle();
    //CaseElementArray();
    //CaseColorAnimation();
    //CaseEachVertexColor();
    //CaseTextureColorBasic();
    //CaseMultiTexture();

    //CaseTextureColorArray();
    //CaseManyFunnyBoxRotate();
    //CaseMaterial_AmbientDiffuseSpecular();
    //CaseMaterial_DiffuseNormal();
    //CaseDirectionLight();
    //CasePointAttenuationLight();
    //CaseSpotlight();
    //CaseMultiLightSource();
    //CaseElementArrayTRS();
    //CaseLoadModelByAssimp();
    //CaseStencilTest();
    //CaseTessellationOnShader();
    //CaseTessHermiteCurve(argn, argc);
    //CaseTessBezierCurve(argn, argc);
    //CaseTessBezierSurface(argn, argc);
    //CaseTraditionalBSplineCurve(argn, argc);
    //CaseTraditionalBSplineSurface(argn, argc);
    //CaseTessBSplineCurve(argn, argc);
    //CaseTessBSplineSurface(argn, argc);
    //CaseTraditional3DModel(argn, argc);
    CasePNTriangles(argn, argc);
    return 0;
}