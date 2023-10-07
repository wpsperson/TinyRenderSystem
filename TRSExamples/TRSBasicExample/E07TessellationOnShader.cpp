#include "E07TessellationOnShader.h"
#include <iostream>

#include "Core/TRSConst.h"
#include "Core/TRSViewer.h"
#include "Core/TRSTexture.h"
#include "Core/TRSVAO.h"
#include "Core/TRSConst.h"
#include "Core/TRSShader.h"
#include "Core/TRSStateset.h"
#include "DataModel/TRSGeode.h"
#include "DataModel/TRSGroup.h"
#include "DataModel/TRSMesh.h"
#include "Camera/TRSCamera.h"
#include "Util/TRSUtils.h"
#include "Util/TRSResource.h"
#include "Math/BSpline.h"
#include "Math/BSplineSurface.h"
#include "Geometry/TRSGrid.h"

int CaseTessHermiteCurve(int argn, char** argc)
{
    std::vector<TRSVec3> vertices = { {-0.8f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f} };

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();
    TRSMesh* mesh = pNode->getMesh();
    mesh->setVertex(vertices);
    mesh->setDrawType(GL_PATCHES);
    mesh->setDrawParam(2);
    std::shared_ptr<TRSStateSet> pSS = pNode->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/PosVertex.glsl");
    shader->createFragmentShader("shaders/DefaultFragment.glsl");
    shader->createTessControlShader("shaders/HermiteTesc.glsl");
    shader->createTessEvaluateShader("shaders/HermiteTese.glsl");
    shader->createProgram();

    shader->addUniform3v("vTan0", TRSVec3(0.5f, 0.866f, 0.0f));
    shader->addUniform3v("vTan1", TRSVec3(0.5f, 0.866f, 0.0f));

    viewer->setSecenNode(pNode);
    viewer->run();
    return 0;
}


int CaseTessBezierCurve(int argn, char** argc)
{
    std::vector<TRSVec3> vertices = { {-1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f} , {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f} };
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> bezierCurve = std::make_shared<TRSGeode>();
    TRSMesh* mesh = bezierCurve->getMesh();
    mesh->setVertex(vertices);
    mesh->setDrawType(GL_PATCHES);
    mesh->setDrawParam(4);

    std::shared_ptr<TRSStateSet> pSS = bezierCurve->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/PosVertex.glsl");
    shader->createFragmentShader("shaders/DefaultFragment.glsl");
    shader->createTessControlShader("shaders/BezierTesc.glsl");
    shader->createTessEvaluateShader("shaders/BezierTese.glsl");
    shader->createProgram();
    bezierCurve->setColor(TRSVec4(1, 0.5, 0.5, 1));

    std::shared_ptr<TRSGeode> gridLine = std::make_shared<TRSGeode>();
    TRSGrid* meshGrid = new TRSGrid;
    meshGrid->setGrid(0.5, 0.5, 10, 10);
    meshGrid->getMesh()->setDrawType(GL_LINES);
    gridLine->setMesh(meshGrid->getMesh());
    gridLine->setColor(TRSVec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGeode> polyLine = std::make_shared<TRSGeode>();
    TRSMesh* meshPolyline = polyLine->getMesh();
    meshPolyline->setVertex(vertices);
    meshPolyline->setDrawType(GL_LINE_STRIP);
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
    std::vector<TRSVec3> vertices = {
        {0.0,  0.0,  0.0},
        {1.0,  0.0,  -1.0},
        {2.0,  0.0,  -1.0},
        {3.0,  0.0,  0.0},
        {0.0,  1.0,  2.0},
        {1.0,  1.0,  0.0},
        {2.0,  1.0,  0.0},
        {3.0,  1.0,  2.0},
        {0.0,  2.0,  2.0},
        {1.0,  2.0,  0.0},
        {2.0,  2.0,  0.0},
        {3.0,  2.0,  2.0},
        {0.0,  3.0,  0.0},
        {1.0,  3.0,  -1.0},
        {2.0,  3.0,  -1.0},
        {3.0,  3.0,  0.0},
    };
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();

    std::shared_ptr<TRSGeode> bezierSurface = std::make_shared<TRSGeode>();
    TRSMesh* mesh = bezierSurface->getMesh();
    mesh->setVertex(vertices);
    mesh->setDrawType(GL_PATCHES);
    mesh->setDrawParam(16);
    std::shared_ptr<TRSStateSet> pSS = bezierSurface->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/PosVertex.glsl");
    shader->createFragmentShader("shaders/PhongFragment.glsl");
    shader->createTessControlShader("shaders/BezierSurFaceTesc.glsl");
    shader->createTessEvaluateShader("shaders/BezierSurFaceTese.glsl");
    shader->createProgram();
    bezierSurface->setColor(TRSVec4(1, 0.5, 0.5, 1));

    std::shared_ptr<TRSGeode> gridLine = std::make_shared<TRSGeode>();
    TRSGrid* meshGrid = new TRSGrid;
    meshGrid->setGrid(0.5, 0.5, 10, 10);
    meshGrid->getMesh()->setDrawType(GL_LINES);
    gridLine->setMesh(meshGrid->getMesh());
    gridLine->setColor(TRSVec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    rootNodes->addChild(gridLine);
    rootNodes->addChild(bezierSurface);

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
    TRSMesh* mesh_bs = Bspline->getMesh();
    mesh_bs->setVertex(TRSMesh::convertToVec3Array(curve, (num + 1) * 3));
    mesh_bs->setDrawType(GL_LINE_STRIP);
    Bspline->setColor(TRSVec4(0.9, 0.5, 1, 1));

    std::shared_ptr<TRSGeode> BSplineNormPair = std::make_shared<TRSGeode>();
    TRSMesh* mesh_norm = BSplineNormPair->getMesh();
    mesh_norm->setVertex(TRSMesh::convertToVec3Array(samplePtPair, sampleNum * 6));
    mesh_norm->setDrawType(GL_LINES);
    BSplineNormPair->setColor(TRSVec4(0.5, 1, 1, 1));

    std::shared_ptr<TRSGeode> CtrlPolygon = std::make_shared<TRSGeode>();
    TRSMesh* mesh_ctrl_poly = CtrlPolygon->getMesh();
    mesh_ctrl_poly->setVertex(TRSMesh::convertToVec3Array(vertexBigDipper, sizeof(vertexBigDipper) / sizeof(float)));
    mesh_ctrl_poly->setDrawType(GL_LINE_STRIP);
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
    std::vector<unsigned int> elements(elementArr, elementArr + eleCount);
    std::shared_ptr<TRSStateSet> pSS = BSplineToBezierCurve->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/PosVertex.glsl");
    shader->createFragmentShader("shaders/DefaultFragment.glsl");
    shader->createTessControlShader("shaders/BezierTesc.glsl");
    shader->createTessEvaluateShader("shaders/BezierTese.glsl");
    shader->createProgram();
    TRSMesh* mesh = BSplineToBezierCurve->getMesh();
    mesh->setVertex(TRSMesh::convertToVec3Array(newCtrlPt, newPtNum * 3));
    mesh->setIndices(elements);
    mesh->setDrawType(GL_PATCHES);
    mesh->setDrawParam(4);
    BSplineToBezierCurve->setColor(TRSVec4(1, 0.5, 0.5, 1));

    std::shared_ptr<TRSGeode> CtrlPolygon = std::make_shared<TRSGeode>();
    TRSMesh* meshCtrlPolygon = CtrlPolygon->getMesh();
    meshCtrlPolygon->setVertex(TRSMesh::convertToVec3Array(vertexBigDipper, sizeof(vertexBigDipper) / sizeof(float)));
    meshCtrlPolygon->setDrawType(GL_LINE_STRIP);
    CtrlPolygon->setColor(TRSVec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGeode> NewCtrlPolygon = std::make_shared<TRSGeode>();
    TRSMesh* meshNewCtrl = NewCtrlPolygon->getMesh();
    meshNewCtrl->setVertex(TRSMesh::convertToVec3Array(newCtrlPt, newPtNum * 3));
    meshNewCtrl->setDrawType(GL_LINE_STRIP);
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
    float* surface = new float[uResolution*vResolution*3];
    float* surfNormals = new float[uResolution * vResolution * 3];
    memset(surface, 0, uResolution * vResolution * 3 * sizeof(float));
    memset(surfNormals, 0, uResolution*vResolution * 3 * sizeof(float));
    BSplineSurface* bsSurface = new BSplineSurface;
    bsSurface->setCtrlPts(SurfacePts, 6, 5);
    for (int vIndex =0; vIndex<vResolution; vIndex++)
    {
        float v = float(vIndex) / (vResolution - 1);
        for (int uIndex = 0; uIndex<uResolution; uIndex++)
        {
            float u = float(uIndex) / (uResolution - 1);
            float *curPt = surface + (vIndex*uResolution+uIndex) * 3;
            bsSurface->interpolatePoint(u, v, curPt);

            float* norm = surfNormals + (vIndex*uResolution + uIndex) * 3;
            bsSurface->interpolateNormal(u, v, norm);
        }
    }

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> BsplineSurFace = std::make_shared<TRSGeode>();
    unsigned int* surFaceEleArr = genWireFrameElementsArray(uResolution, vResolution);
    std::vector<unsigned int> surFaceIndices(surFaceEleArr, surFaceEleArr + uResolution * vResolution * 6);
    std::shared_ptr<TRSStateSet> pSS = BsplineSurFace->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createProgram("shaders/PhongVertex.glsl", "shaders/PhongFragment.glsl");

    TRSMesh* mesh = BsplineSurFace->getMesh();
    mesh->setVertex(TRSMesh::convertToVec3Array(surface, uResolution * vResolution * 3));
    mesh->setNormal(TRSMesh::convertToVec3Array(surfNormals, uResolution * vResolution * 3));
    mesh->setIndices(surFaceIndices);
    mesh->setDrawType(GL_TRIANGLES);
    BsplineSurFace->setColor(TRSVec4(0.9, 0.5, 1, 1));


    std::shared_ptr<TRSGeode> CtrlPolygon = std::make_shared<TRSGeode>();
    unsigned int* ctrlPolygonEleArr = genWireFrameElementsArray(6, 5);
    std::vector<unsigned int> indices(ctrlPolygonEleArr, ctrlPolygonEleArr + 180);
    TRSMesh* meshCtrlPoly = CtrlPolygon->getMesh();
    meshCtrlPoly->setVertex(TRSMesh::convertToVec3Array(SurfacePts, sizeof(SurfacePts) / sizeof(float)));
    meshCtrlPoly->setIndices(indices);
    meshCtrlPoly->setDrawType(GL_TRIANGLES);
    CtrlPolygon->setPolygonMode(GL_LINE);
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
    std::vector<unsigned int> surfaceIndices(surFaceEleArr, surFaceEleArr + eleArrCount);
    std::shared_ptr<TRSStateSet> pSS = BsplineSurFace->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    shader->createVertexShader("shaders/PosVertex.glsl");
    shader->createFragmentShader("shaders/PhongFragment.glsl");
    shader->createTessControlShader("shaders/BezierSurFaceTesc.glsl");
    shader->createTessEvaluateShader("shaders/BezierSurFaceTese.glsl");
    shader->createProgram();
    TRSMesh* mesh = BsplineSurFace->getMesh();
    mesh->setVertex(TRSMesh::convertToVec3Array(newCtrlPt, uNum * vNum * 3));
    mesh->setIndices(surfaceIndices);
    mesh->setDrawType(GL_PATCHES);
    mesh->setDrawParam(16);
    BsplineSurFace->setColor(TRSVec4(1, 0.5, 0.5, 1));
    BsplineSurFace->setPolygonMode(GL_TRIANGLES);

    std::shared_ptr<TRSGeode> CtrlPolygon = std::make_shared<TRSGeode>();
    unsigned int* ctrlPolygonEleArr = genWireFrameElementsArray(6, 5);
    std::vector<unsigned int> indices(ctrlPolygonEleArr, ctrlPolygonEleArr + 180);
    TRSMesh* meshCtrlPoly = CtrlPolygon->getMesh();
    meshCtrlPoly->setVertex(TRSMesh::convertToVec3Array(SurfacePts, sizeof(SurfacePts) / sizeof(float)));
    meshCtrlPoly->setIndices(indices);
    meshCtrlPoly->setDrawType(GL_TRIANGLES);
    CtrlPolygon->setPolygonMode(GL_LINE);
    CtrlPolygon->setColor(TRSVec4(0.5, 0.5, 1, 1));

    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    rootNodes->addChild(BsplineSurFace);
    rootNodes->addChild(CtrlPolygon);
    viewer->setSecenNode(rootNodes);
    viewer->run();
    return 0;
}
