#include "CasePNTriangles.h"
#include "TRSTexture.h"
#include "TRSVAO.h"
#include "TRSResource.h"
#include "TRSConst.h"
#include "TRSCamera.h"
#include "TRSViewer.h"
#include "TRSGeode.h"
#include "TRSGroup.h"
#include "TRSShader.h"
#include "TRSAssimpLoader.h"
#include "TRSStateset.h"
#include "glfw\glfw3.h"

#include <iostream>
#include "TRSUtils.h"
#include "stb_image.h"


int CaseTraditional3DModel(int argn, char** argc)
{
    std::string strFile = std::string("resources/objects/bunny.obj");
    if (argn == 2)
    {
        strFile = std::string(argc[1]);
    }
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    TRSAssimpLoader* pLoader = new TRSAssimpLoader();
    TRSGroup* pGroup = pLoader->loadByAssimp(strFile);
    std::string strDebugInfo = pGroup->debugInfo();
    std::shared_ptr<TRSGroup> pSharedGroup = std::shared_ptr<TRSGroup>(pGroup);
    viewer->setSecenNode(pSharedGroup);
    viewer->run();
    return 0;
}

int CasePNTriangles(int argn, char** argc)
{
    float vertex[] = {
        -1.0f, 0.0f, 0.0f, -0.3536f, -0.3536f, 0.866f,
        1.0f, 0.0f, 0.0f, 0.3536f, -0.3536f, 0.866f,
        0.0f, 2.0f, 0.0f, 0.0f, 0.5f, 0.866f,
    };

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> PNTriangleGeode = std::make_shared<TRSGeode>();
    PNTriangleGeode->readFromVertex(vertex, sizeof(vertex) / sizeof(float), EnVertexNormal);
    std::shared_ptr<TRSStateSet> pSS = PNTriangleGeode->getOrCreateStateSet();
    TRSShader* shader = pSS->getShader();
    //shader->createProgram("shaders/PhongVertex.glsl", "shaders/PhongFragment.glsl");
    shader->createVertexShader("shaders/PNTriangleVertex.glsl");
    shader->createFragmentShader("shaders/PhongFragment.glsl");
    shader->createTessControlShader("shaders/PNTriangleTesc.glsl");
    shader->createTessEvaluateShader("shaders/PNTriangleTese.glsl");
    shader->createProgram();

    PNTriangleGeode->getVAO()->setDrawType(GL_TRIANGLES);
    PNTriangleGeode->setColor(glm::vec4(1.0, 1.0, 0.5, 1));
    PNTriangleGeode->getVAO()->setDrawType(GL_PATCHES);
    PNTriangleGeode->getVAO()->setDrawParam(3);
    PNTriangleGeode->setPolygonMode(GL_LINE);

    viewer->setSecenNode(PNTriangleGeode);
    viewer->run();
    return 0;
}
