#include "CaseNurbsFaceToMesh.h"
#include <iostream>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include "TRSViewer.h"
#include "TRSGeode.h"
#include "TRSGroup.h"
#include "TRSTexture.h"
#include "TRSVAO.h"
#include "TRSResource.h"
#include "TRSConst.h"
#include "TRSCamera.h"
#include "TRSShader.h"
#include "TRSStateset.h"
#include "TRSUtils.h"
#include "TRSAssimpLoader.h"
#include "BSpline.h"
#include "BSplineSurface.h"


int CaseNurbsFaceToMesh(int argn, char** argc)
{
    float SurfacePts[] =
    {
        0.000000, 0.000000, 0.003754,
        1.000000, 0.000000, 1.690756,
        2.000000, 0.000000, 0.579913,
        3.000000, 0.000000, 2.426222,
        4.000000, 0.000000, 1.755028,
        5.000000, 0.000000, 1.439619,
        0.000000, 1.000000, 1.050874,
        1.000000, 1.000000, 2.687887,
        2.000000, 1.000000, 2.468520,
        3.000000, 1.000000, 2.239814,
        4.000000, 1.000000, 0.522324,
        5.000000, 1.000000, 2.576830,
        0.000000, 2.000000, 2.131504,
        1.000000, 2.000000, 1.540605,
        2.000000, 2.000000, 0.911985,
        3.000000, 2.000000, 0.044954,
        4.000000, 2.000000, 0.274209,
        5.000000, 2.000000, 1.093356,
        0.000000, 3.000000, 0.441939,
        1.000000, 3.000000, 0.497696,
        2.000000, 3.000000, 2.965575,
        3.000000, 3.000000, 1.337077,
        4.000000, 3.000000, 0.357250,
        5.000000, 3.000000, 0.014008,
        0.000000, 4.000000, 0.026734,
        1.000000, 4.000000, 1.133641,
        2.000000, 4.000000, 1.594989,
        3.000000, 4.000000, 1.713553,
        4.000000, 4.000000, 1.805292,
        5.000000, 4.000000, 1.821497,
    };
    int uResolution = 20;
    int vResolution = 20;
    float* surface = new float[uResolution * vResolution * 6];
    memset(surface, 0, uResolution * vResolution * 6 * sizeof(float));
    BSplineSurface* bsSurface = new BSplineSurface;
    bsSurface->setCtrlPts(SurfacePts, 6, 5);
    for (int vIndex = 0; vIndex < vResolution; vIndex++)
    {
        float v = float(vIndex) / (vResolution - 1);
        for (int uIndex = 0; uIndex < uResolution; uIndex++)
        {
            float u = float(uIndex) / (uResolution - 1);
            float* curPt = surface + (vIndex * uResolution + uIndex) * 6;
            bsSurface->interpolatePoint(u, v, curPt);

            float* norm = surface + (vIndex * uResolution + uIndex) * 6 + 3;
            bsSurface->interpolateNormal(u, v, norm);
        }
    }

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> BsplineSurFace = std::make_shared<TRSGeode>();
    unsigned int* surFaceEleArr = genWireFrameElementsArray(uResolution, vResolution);
    BsplineSurFace->readFromVertex(surface, uResolution * vResolution * 6, EnVertexNormal, surFaceEleArr, uResolution * vResolution * 6);
    std::shared_ptr<TRSStateSet> pSS = BsplineSurFace->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createProgram("shaders/PhongVertex.glsl", "shaders/PhongFragment.glsl");
    BsplineSurFace->getVAO()->setDrawType(GL_TRIANGLES);
    BsplineSurFace->setColor(TRSVec4(0.9, 0.5, 1, 1));


    std::shared_ptr<TRSGeode> CtrlPolygon = std::make_shared<TRSGeode>();
    unsigned int* ctrlPolygonEleArr = genWireFrameElementsArray(6, 5);
    CtrlPolygon->readFromVertex(SurfacePts, sizeof(SurfacePts) / sizeof(float), EnVertex, ctrlPolygonEleArr, 180);
    CtrlPolygon->setPolygonMode(GL_LINE);
    CtrlPolygon->getVAO()->setDrawType(GL_TRIANGLES);
    CtrlPolygon->setColor(TRSVec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    rootNodes->addChild(BsplineSurFace);
    rootNodes->addChild(CtrlPolygon);

    glPointSize(3);
    viewer->setSecenNode(rootNodes);
    viewer->run();
    return 0;
}
