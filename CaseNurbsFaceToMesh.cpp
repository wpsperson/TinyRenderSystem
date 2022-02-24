#include "CaseNurbsFaceToMesh.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
#include "delaunator.hpp"
#include "TRSEventDefine.h"

static double parametricDomainTranslate = -1.5;

void print(const TRSVec3& pt, const std::string& description = "")
{
    if (!description.empty())
    {
        std::cout << description<<": ";
    }
    std::cout << pt[0] << ", " << pt[1] << ", " << pt[2] << std::endl;
}

void CaseDelauneyTriangulation()
{
    /* x0, y0, x1, y1, ... */
    std::vector<double> coords = { -1, 1, 1, 1, 1, -1, -1, -1 };

    //triangulation happens here
    delaunator::Delaunator d(coords);
    std::vector<size_t> allTriangles = d.triangles;
    size_t triangleNumber = allTriangles.size() / 3;
    for (size_t i=0; i<triangleNumber; i++)
    {
        size_t idx0 = allTriangles[i * 3];
        size_t idx1 = allTriangles[i * 3 + 1];
        size_t idx2 = allTriangles[i * 3 + 2];
        std::cout << "triangle-" << i << ": " << idx0 << ", " << idx1 << ", " << idx2 << std::endl;
    }
}

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
    // First we darw the Nurbs Surface accurately.
    BSplineSurface* bsSurface = new BSplineSurface;
    bsSurface->setCtrlPts(SurfacePts, 6, 5);
    int uResolution = 20;
    int vResolution = 20;
    float* surface = new float[uResolution * vResolution * 6];
    memset(surface, 0, uResolution * vResolution * 6 * sizeof(float));
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
    BsplineSurFace->setColor(TRSVec4(1, 0.5, 1, 1));

    InsertParametricPointHandler* handler = new InsertParametricPointHandler(bsSurface, viewer->getCamera());
    viewer->addEventHandler(handler);
    std::shared_ptr<TRSGeode> Triangles3d = handler->get3DSpaceMesh();
    std::shared_ptr<TRSGeode> Triangles2d = handler->getParametricSpaceMesh();

    std::shared_ptr<TRSGroup> rootNodes = std::make_shared<TRSGroup>();
    //rootNodes->addChild(BsplineSurFace);
    rootNodes->addChild(Triangles2d);
    rootNodes->addChild(Triangles3d);

    glPointSize(3);
    viewer->setSecenNode(rootNodes);
    viewer->run();
    return 0;
}

InsertParametricPointHandler::InsertParametricPointHandler(BSplineSurface* nurbs, TRSCamera* camera)
    : bsSurface(nurbs)
    , m_camera(camera)
{
    Triangles2d = std::make_shared<TRSGeode>();
    TRSMatrix translateMatrix;
    translateMatrix.makeTranslate(parametricDomainTranslate, 0, 0);
    Triangles2d->setMatrix(translateMatrix);
    Triangles2d->getVAO()->setDrawType(GL_TRIANGLES);
    Triangles2d->setColor(TRSVec4(0.5, 1, 1, 1));
    Triangles2d->setPolygonMode(GL_LINE);

    Triangles3d = std::make_shared<TRSGeode>();
    Triangles3d->getVAO()->setDrawType(GL_TRIANGLES);
    Triangles3d->setColor(TRSVec4(0.5, 1, 1, 1));
    Triangles3d->setPolygonMode(GL_LINE);
    std::shared_ptr<TRSStateSet> Triangles3dSS = Triangles3d->getOrCreateStateSet();
    TRSShader* shaderTriangles3d = Triangles3dSS->getShader();
    shaderTriangles3d->createProgram("shaders/PhongVertex.glsl", "shaders/PhongFragment.glsl");
    initMesh();
    updateMesh();
}

std::shared_ptr<TRSGeode> InsertParametricPointHandler::getParametricSpaceMesh()
{
    return Triangles2d;
}

std::shared_ptr<TRSGeode> InsertParametricPointHandler::get3DSpaceMesh()
{
    return Triangles3d;
}

void InsertParametricPointHandler::processRightMousePress(double xpos, double ypos, int mods)
{
    double width = m_camera->getWindowWidth();
    double height = m_camera->getWindowHeight();
    TRSMatrix projectMatrix = m_camera->getProjectMatrix();
    TRSMatrix projectInverse = projectMatrix.getInverse();
    TRSMatrix viewMatrix = m_camera->getViewMatrix();
    TRSMatrix viewInverse = viewMatrix.getInverse();
    double ndcX = (xpos / width) * 2 - 1;
    double ndcY = (ypos / height) * 2 - 1;
    // in windows, y coordinate increase from top to bottom, while in OpenGL, y coordinate increase from bottom to top
    ndcY = -ndcY;
    TRSVec3 ndcNear(ndcX, ndcY, -1);
    TRSVec3 ndcFar(ndcX, ndcY, 1);
    TRSVec3 viewCoordNear = projectInverse * ndcNear;
    TRSVec3 viewCoordFar = projectInverse * ndcFar;
    TRSVec3 worldCoordNear = viewInverse * viewCoordNear;
    TRSVec3 worldCoordFar = viewInverse * viewCoordFar;
    float zFar = (worldCoordFar[2]);
    float zNear = (worldCoordNear[2]);
    bool valid = (zFar < 0 && zNear > 0);
    if (!valid)
    {
        return;
    }
    float proportion = (0 - zFar) / (zNear - zFar);
    TRSVec3 ptOnXYPlane = worldCoordFar * (1 - proportion) + worldCoordNear * proportion;
    float u = ptOnXYPlane[0] - parametricDomainTranslate;
    float v = ptOnXYPlane[1];
    print(worldCoordNear, "near");
    print(worldCoordFar, "far");
    std::cout << "v: " << u << ", v: " << v << std::endl;
    bool parameterValid = u >= 0 && u <= 1 && v >= 0 && v <= 1;
    if (!parameterValid)
    {
        return;
    }
    uvCoords.push_back(u);
    uvCoords.push_back(v);
    updateMesh();
}

void InsertParametricPointHandler::processKeyPress(int key)
{
    if (TRS_KEY_S == key)
    {
        std::string filename;
        std::cin >> filename;
        if (filename.empty())
        {
            return;
        }
        saveParameterToFile(filename);
    }
    else if (TRS_KEY_L == key)
    {
        std::string filename;
        std::cin >> filename;
        if (filename.empty())
        {
            return;
        }
        loadParameterFromFile(filename);
        updateMesh();
    }
}

void InsertParametricPointHandler::initMesh()
{
    const int initUSize = 4;
    const int initVSize = 4;
    for (int vIndex = 0; vIndex < initVSize; vIndex++)
    {
        float v = float(vIndex) / (initVSize - 1);
        for (int uIndex = 0; uIndex < initUSize; uIndex++)
        {
            float u = float(uIndex) / (initUSize - 1);
            uvCoords.push_back(u);
            uvCoords.push_back(v);
        }
    }
}

void InsertParametricPointHandler::updateMesh()
{
    // generate parametric-domain mesh points
    size_t MeshPointNumber = uvCoords.size() / 2;
    parametricSpacePoints.clear();
    for (size_t i = 0; i < MeshPointNumber; i++)
    {
        float u = uvCoords[i * 2];
        float v = uvCoords[i * 2 + 1];
        parametricSpacePoints.push_back(u);
        parametricSpacePoints.push_back(v);
        parametricSpacePoints.push_back(0.0f);
    }

    // we map those triangles from parametric-domain into 3d-space. and draw those
    parametricSpacePointsNormals.clear();
    float point[3];
    float normal[3];
    for (size_t i = 0; i < MeshPointNumber; i++)
    {
        float u = uvCoords[i * 2];
        float v = uvCoords[i * 2 + 1];
        bsSurface->interpolatePoint(u, v, point);
        bsSurface->interpolateNormal(u, v, normal);
        parametricSpacePointsNormals.push_back(point[0]);
        parametricSpacePointsNormals.push_back(point[1]);
        parametricSpacePointsNormals.push_back(point[2]);
        parametricSpacePointsNormals.push_back(normal[0]);
        parametricSpacePointsNormals.push_back(normal[1]);
        parametricSpacePointsNormals.push_back(normal[2]);
    }

    // and apply the Delaunay Triangulation to those points to generate triangles in parametric space.
    delaunator::Delaunator delaunay(uvCoords);
    std::vector<size_t> allTriangles = delaunay.triangles;
    std::vector<unsigned int> indexArray2d;
    for (size_t idx : allTriangles)
    {
        indexArray2d.push_back(static_cast<unsigned int>(idx));
    }

    Triangles2d->readFromVertex(parametricSpacePoints.data(), parametricSpacePoints.size(), EnVertex, indexArray2d.data(), indexArray2d.size());
    Triangles3d->readFromVertex(parametricSpacePointsNormals.data(), parametricSpacePointsNormals.size(), EnVertexNormal, indexArray2d.data(), indexArray2d.size());
}

void InsertParametricPointHandler::saveParameterToFile(const std::string& fileName)
{
    std::ofstream stream(fileName.c_str());
    if (stream.fail())
    {
        return;
    }
    int uvCount = uvCoords.size();
    std::string strNextLine = "\n";
    stream << uvCount << strNextLine;
    for (double value : uvCoords)
    {
        stream << value << strNextLine;
    }
    stream.close();
}

void InsertParametricPointHandler::loadParameterFromFile(const std::string& fileName)
{
    std::ifstream stream(fileName.c_str());
    if (stream.fail())
    {
        return;
    }
    int size;
    std::string strNextLine;
    getline(stream, strNextLine);
    std::stringstream ss(strNextLine);
    ss >> size;
    uvCoords.clear();
    uvCoords.reserve(size);
    for (int i=0; i<size;i++)
    {
        double value;
        getline(stream, strNextLine);
        std::stringstream ss(strNextLine);
        ss >> value;
        uvCoords.push_back(value);
    }
    stream.close();
}
