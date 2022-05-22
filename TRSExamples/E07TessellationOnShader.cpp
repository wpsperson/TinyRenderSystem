#include "E07TessellationOnShader.h"
#include <iostream>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "stb_image.h"
#include "TRSUtils.h"
#include "TRSResource.h"
#include "TRSCamera.h"
#include "TRSConst.h"
#include "BSpline.h"
#include "BSplineSurface.h"


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

int CaseTessHermiteCurve(int argn, char** argc)
{
    float vertices[] = {
        -0.8f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();
    pNode->readFromVertex(vertices, sizeof(vertices) / sizeof(float), EnVertex);
    std::shared_ptr<TRSStateSet> pSS = pNode->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/DefaultVertexWithoutMVP.glsl");
    shader->createFragmentShader("shaders/DefaultFragment.glsl");
    shader->createTessControlShader("shaders/HermiteTesc.glsl");
    shader->createTessEvaluateShader("shaders/HermiteTese.glsl");
    shader->createProgram();

    shader->addUniform3v("vTan0", TRSVec3(0.5f, 0.866f, 0.0f));
    shader->addUniform3v("vTan1", TRSVec3(0.5f, 0.866f, 0.0f));
    pNode->getVAO()->setDrawType(GL_PATCHES);
    pNode->getVAO()->setDrawParam(2);

    viewer->setSecenNode(pNode);
    viewer->run();
    return 0;
}

int CaseTessBezierCurve(int argn, char** argc)
{
    float vertices[] = {
        -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f
    };
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();

    std::shared_ptr<TRSGeode> bezierCurve = std::make_shared<TRSGeode>();
    bezierCurve->readFromVertex(vertices, sizeof(vertices) / sizeof(float), EnVertex);
    std::shared_ptr<TRSStateSet> pSS = bezierCurve->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/DefaultVertexWithoutMVP.glsl");
    shader->createFragmentShader("shaders/DefaultFragment.glsl");
    shader->createTessControlShader("shaders/BezierTesc.glsl");
    shader->createTessEvaluateShader("shaders/BezierTese.glsl");
    shader->createProgram();
    bezierCurve->getVAO()->setDrawType(GL_PATCHES);
    bezierCurve->getVAO()->setDrawParam(4);
    bezierCurve->setColor(TRSVec4(1, 0.5, 0.5, 1));

    int arraySize;
    float* grid = createXYGridVertexArray(0.5, 7, arraySize);
    std::shared_ptr<TRSGeode> gridLine = std::make_shared<TRSGeode>();
    gridLine->readFromVertex(grid, arraySize, EnVertex);
    gridLine->getVAO()->setDrawType(GL_LINES);
    gridLine->setColor(TRSVec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGeode> polyLine = std::make_shared<TRSGeode>();
    polyLine->readFromVertex(vertices, sizeof(vertices) / sizeof(float), EnVertex);
    polyLine->getVAO()->setDrawType(GL_LINE_STRIP);
    polyLine->setColor(TRSVec4(1, 1, 1, 1));

    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    rootNodes->addChild(gridLine);
    rootNodes->addChild(polyLine);
    rootNodes->addChild(bezierCurve);

    viewer->setSecenNode(rootNodes);
    viewer->run();
    return 0;
}

int CaseTessBezierSurface(int argn, char** argc)
{
    // 4*4 Bezier surface control pts
    float vertices[] = {
        0.0,  0.0,  0.0,
        1.0,  0.0,  -1.0,
        2.0,  0.0,  -1.0,
        3.0,  0.0,  0.0,
        0.0,  1.0,  2.0,
        1.0,  1.0,  0.0,
        2.0,  1.0,  0.0,
        3.0,  1.0,  2.0,
        0.0,  2.0,  2.0,
        1.0,  2.0,  0.0,
        2.0,  2.0,  0.0,
        3.0,  2.0,  2.0,
        0.0,  3.0,  0.0,
        1.0,  3.0,  -1.0,
        2.0,  3.0,  -1.0,
        3.0,  3.0,  0.0,
    };
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();

    std::shared_ptr<TRSGeode> bezierSurface = std::make_shared<TRSGeode>();
    bezierSurface->readFromVertex(vertices, sizeof(vertices) / sizeof(float), EnVertex);
    std::shared_ptr<TRSStateSet> pSS = bezierSurface->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/DefaultVertexWithoutMVP.glsl");
    shader->createFragmentShader("shaders/PhongFragment.glsl");
    shader->createTessControlShader("shaders/BezierSurFaceTesc.glsl");
    shader->createTessEvaluateShader("shaders/BezierSurFaceTese.glsl");
    shader->createProgram();
    bezierSurface->getVAO()->setDrawType(GL_PATCHES);
    bezierSurface->getVAO()->setDrawParam(16);
    bezierSurface->setColor(TRSVec4(1, 0.5, 0.5, 1));

    int arraySize;
    float* grid = createXYGridVertexArray(0.5, 7, arraySize);
    std::shared_ptr<TRSGeode> gridLine = std::make_shared<TRSGeode>();
    gridLine->readFromVertex(grid, arraySize, EnVertex);
    gridLine->getVAO()->setDrawType(GL_LINES);
    gridLine->setColor(TRSVec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGeode> bezierSurfaceWireframe = std::make_shared<TRSGeode>(*bezierSurface, true);
    bezierSurfaceWireframe->setPolygonMode(GL_LINE);
    bezierSurfaceWireframe->setColor(TRSVec4(0.5, 1.0, 0.5, 1));

    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    rootNodes->addChild(gridLine);
    rootNodes->addChild(bezierSurface);
    rootNodes->addChild(bezierSurfaceWireframe);

    viewer->setSecenNode(rootNodes);
    viewer->run();

    return 0;
}


int CaseTraditionalBSplineCurve(int argn, char** argc)
{
    float vertexBigDipper[] = {
        -2.4,   1.0,    0.0,
        -1.3,   1.0,    0.0,
        -0.75,  0.7,   0.0,
         0.0,   0.0,    0.0,
         0.0,  -0.75,   0.0,
         1.28, -1.08,   0.0,
         1.68, -0.27,   0.0
    };
    BSpline* bs = new BSpline;
    bs->setCtrlPts(vertexBigDipper, 7);
    int num = 100;
    float* curve = new float[3 * num + 3];
    int sampleNum = (num / 10 + 1);
    float* samplePtPair = new float[6 * sampleNum];
    float* curPair = samplePtPair;
    float* pt = curve;
    float norm[3]; //normal of any point.
    for (int i=0; i<=num; i++)
    {
        float u = float(i) / num;
        bs->interpolatePoint(u, pt);
        if (i % 10 == 0)
        {
            memcpy(curPair, pt, sizeof(float) * 3);
            curPair += 3;

            bs->interpolateTangent(u, norm);
            normlize(norm);
            float newPt[3];
            add(pt, norm, newPt);
            memcpy(curPair, newPt, sizeof(float) * 3);
            curPair += 3;
        }
        pt += 3;
    }

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> Bspline = std::make_shared<TRSGeode>();
    Bspline->readFromVertex(curve, 303, EnVertex);
    Bspline->getVAO()->setDrawType(GL_LINE_STRIP);
    Bspline->setColor(TRSVec4(0.9, 0.5, 1, 1));

    std::shared_ptr<TRSGeode> BSplineNormPair = std::make_shared<TRSGeode>();
    BSplineNormPair->readFromVertex(samplePtPair, sampleNum*6, EnVertex);
    BSplineNormPair->getVAO()->setDrawType(GL_LINES);
    BSplineNormPair->setColor(TRSVec4(0.5, 1, 1, 1));

    std::shared_ptr<TRSGeode> CtrlPolygon = std::make_shared<TRSGeode>();
    CtrlPolygon->readFromVertex(vertexBigDipper, sizeof(vertexBigDipper) / sizeof(float), EnVertex);
    CtrlPolygon->getVAO()->setDrawType(GL_LINE_STRIP);
    CtrlPolygon->setColor(TRSVec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    rootNodes->addChild(Bspline);
    rootNodes->addChild(CtrlPolygon);
    rootNodes->addChild(BSplineNormPair);

    viewer->setSecenNode(rootNodes);
    viewer->run();
    return 0;
}


int CaseTessBSplineCurve(int argn, char** argc)
{
    float vertexBigDipper[] = {
        -2.4,   1.0,    0.0,
        -1.3,   1.0,    0.0,
        -0.75,  0.7,   0.0,
        0.0,   0.0,    0.0,
        0.0,  -0.75,   0.0,
        1.28, -1.08,   0.0,
        1.68, -0.27,   0.0
    };
    BSpline* bs = new BSpline;
    bs->setCtrlPts(vertexBigDipper, 7);
    bs->convertPiecewiseBezier();
    float* newCtrlPt = bs->getCtrlPt();
    int newPtNum = bs->getNumberOfPts();
    unsigned int* elementArr = genBS2BezierElementArray(newPtNum);
    int eleCount = ((newPtNum - 1) / 3) * 4;

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> BSplineToBezierCurve = std::make_shared<TRSGeode>();
    BSplineToBezierCurve->readFromVertex(newCtrlPt, newPtNum *3, EnVertex, elementArr, eleCount);
    std::shared_ptr<TRSStateSet> pSS = BSplineToBezierCurve->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/DefaultVertexWithoutMVP.glsl");
    shader->createFragmentShader("shaders/DefaultFragment.glsl");
    shader->createTessControlShader("shaders/BezierTesc.glsl");
    shader->createTessEvaluateShader("shaders/BezierTese.glsl");
    shader->createProgram();
    BSplineToBezierCurve->getVAO()->setDrawType(GL_PATCHES);
    BSplineToBezierCurve->getVAO()->setDrawParam(4);
    BSplineToBezierCurve->setColor(TRSVec4(1, 0.5, 0.5, 1));

    std::shared_ptr<TRSGeode> CtrlPolygon = std::make_shared<TRSGeode>();
    CtrlPolygon->readFromVertex(vertexBigDipper, sizeof(vertexBigDipper) / sizeof(float), EnVertex);
    CtrlPolygon->getVAO()->setDrawType(GL_LINE_STRIP);
    CtrlPolygon->setColor(TRSVec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGeode> NewCtrlPolygon = std::make_shared<TRSGeode>();
    NewCtrlPolygon->readFromVertex(newCtrlPt, newPtNum * 3, EnVertex);
    NewCtrlPolygon->getVAO()->setDrawType(GL_LINE_STRIP);
    NewCtrlPolygon->setColor(TRSVec4(1, 1, 1, 1));

    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    rootNodes->addChild(BSplineToBezierCurve);
    rootNodes->addChild(CtrlPolygon);
    rootNodes->addChild(NewCtrlPolygon);

    viewer->setSecenNode(rootNodes);
    viewer->run();
    return 0;
}

int CaseTraditionalBSplineSurface(int argn, char** argc)
{
    float SurfacePts[]=
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
    float* surface = new float[uResolution*vResolution*6];
    memset(surface, 0, uResolution*vResolution * 6 * sizeof(float));
    BSplineSurface* bsSurface = new BSplineSurface;
    bsSurface->setCtrlPts(SurfacePts, 6, 5);
    for (int vIndex =0; vIndex<vResolution; vIndex++)
    {
        float v = float(vIndex) / (vResolution - 1);
        for (int uIndex = 0; uIndex<uResolution; uIndex++)
        {
            float u = float(uIndex) / (uResolution - 1);
            float *curPt = surface + (vIndex*uResolution+uIndex) * 6;
            bsSurface->interpolatePoint(u, v, curPt);

            float* norm = surface + (vIndex*uResolution + uIndex) * 6 + 3;
            bsSurface->interpolateNormal(u, v, norm);
        }
    }

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> BsplineSurFace = std::make_shared<TRSGeode>();
    unsigned int* surFaceEleArr = genWireFrameElementsArray(uResolution, vResolution);
    BsplineSurFace->readFromVertex(surface, uResolution*vResolution * 6, EnVertexNormal, surFaceEleArr, uResolution*vResolution * 6);
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

    // todo glPointSize(3);
    viewer->setSecenNode(rootNodes);
    viewer->run();
    return 0;
}

int CaseTessBSplineSurface(int argn, char** argc)
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

    BSplineSurface* bsSurface = new BSplineSurface;
    bsSurface->setCtrlPts(SurfacePts, 6, 5);
    bsSurface->convertPiecewiseBezierSurface();
    float* newCtrlPt = bsSurface->getAllCtrlPt();
    int uNum = bsSurface->getNumberOfPtsInUDir();
    int vNum = bsSurface->getNumberOfPtsInVDir();

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> BsplineSurFace = std::make_shared<TRSGeode>();
    unsigned int* surFaceEleArr = genBS2BezierPatchEleArr(uNum, vNum);
    int eleArrCount = ((uNum - 1) / 3)*((vNum - 1) / 3) * 16;
    BsplineSurFace->readFromVertex(newCtrlPt, uNum*vNum * 3, EnVertex, surFaceEleArr, eleArrCount);
    std::shared_ptr<TRSStateSet> pSS = BsplineSurFace->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/DefaultVertexWithoutMVP.glsl");
    shader->createFragmentShader("shaders/PhongFragment.glsl");
    shader->createTessControlShader("shaders/BezierSurFaceTesc.glsl");
    shader->createTessEvaluateShader("shaders/BezierSurFaceTese.glsl");
    shader->createProgram();
    BsplineSurFace->getVAO()->setDrawType(GL_PATCHES);
    BsplineSurFace->getVAO()->setDrawParam(16);
    BsplineSurFace->setColor(TRSVec4(1, 0.5, 0.5, 1));
    BsplineSurFace->setPolygonMode(GL_LINE);

    std::shared_ptr<TRSGeode> CtrlPolygon = std::make_shared<TRSGeode>();
    unsigned int* ctrlPolygonEleArr = genWireFrameElementsArray(6, 5);
    CtrlPolygon->readFromVertex(SurfacePts, sizeof(SurfacePts) / sizeof(float), EnVertex, ctrlPolygonEleArr, 180);
    CtrlPolygon->setPolygonMode(GL_LINE);
    CtrlPolygon->getVAO()->setDrawType(GL_TRIANGLES);
    CtrlPolygon->setColor(TRSVec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    rootNodes->addChild(BsplineSurFace);
    rootNodes->addChild(CtrlPolygon);

    //todo glPointSize(3);
    viewer->setSecenNode(rootNodes);
    viewer->run();
    return 0;
}
