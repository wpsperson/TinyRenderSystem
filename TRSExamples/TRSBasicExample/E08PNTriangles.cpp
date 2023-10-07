#include "E08PNTriangles.h"
#include "Core/TRSConst.h"
#include "Core/TRSViewer.h"
#include "Core/TRSStateset.h"
#include "Core/TRSShader.h"
#include "Core/TRSTexture.h"
#include "Core/TRSVAO.h"
#include "Util/TRSResource.h"
#include "Util/TRSUtils.h"
#include "Camera/TRSCamera.h"
#include "DataModel/TRSMesh.h"
#include "DataModel/TRSGeode.h"
#include "DataModel/TRSGroup.h"
#include "DataModel/NodeVisitor.h"
#include "IO/TRSAssimpLoader.h"
#include "Math/TRSVector.h"

#include <iostream>
#include "Windows.h"


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

void LodCallBack(TRSNode* pNode)
{
    // todo
    //static int lod = 1;
    //if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
    //{
    //    lod++;
    //    if (lod > 10)
    //    {
    //        lod = 10;
    //    }
    //    pNode->getStateSet()->getShader()->addUniformi("lod", lod);
    //    Sleep(100);
    //}
    //else if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS))
    //{
    //    lod--;
    //    if (lod < 1)
    //    {
    //        lod = 1;
    //    }
    //    pNode->getStateSet()->getShader()->addUniformi("lod", lod);
    //    Sleep(100);
    //}
}

class PNNodeVisitor : public NodeVisitor
{
public:
    void execute(TRSNode* pNode)
    {
        TRSGeode* PNTriangleGeode = dynamic_cast<TRSGeode*>(pNode);
        if (PNTriangleGeode)
        {
            std::shared_ptr<TRSStateSet> pSS = PNTriangleGeode->getOrCreateStateSet();
            TRSShader* shader = pSS->getShader();
            shader->createVertexShader("shaders/PNTriangleVertex.glsl");
            shader->createFragmentShader("shaders/PhongFragment.glsl");
            shader->createTessControlShader("shaders/PNTriangleTesc.glsl");
            shader->createTessEvaluateShader("shaders/PNTriangleTese.glsl");
            shader->createProgram();
            shader->addUniformi("lod", 1);

            TRSMesh* mesh = PNTriangleGeode->getMesh();
            mesh->setDrawType(GL_PATCHES);
            mesh->setDrawParam(3);
            PNTriangleGeode->setColor(TRSVec4(1.0, 1.0, 0.5, 1));
            PNTriangleGeode->setUpdateCallBack(LodCallBack);
           // PNTriangleGeode->setPolygonMode(GL_LINE);
        }
    }
};

int CasePNTriangles(int argn, char** argc)
{
    float vertex[] = {
        -1.0f, 0.0f, 0.0f, -0.3536f, -0.3536f, 0.866f,
        1.0f, 0.0f, 0.0f, 0.3536f, -0.3536f, 0.866f,
        0.0f, 2.0f, 0.0f, 0.0f, 0.5f, 0.866f,
    };

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::string strFile = std::string("resources/objects/bunny.obj");
    if (argn == 2)
    {
        strFile = std::string(argc[1]);
    }
    TRSAssimpLoader* pLoader = new TRSAssimpLoader();
    TRSGroup* pGroup = pLoader->loadByAssimp(strFile);
    PNNodeVisitor visitor;
    visitor.visit(pGroup);

    //std::shared_ptr<TRSGeode> PNTriangleGeode = std::make_shared<TRSGeode>();
    //PNTriangleGeode->readFromVertex(vertex, sizeof(vertex) / sizeof(float), EnVertexNormal);
    //std::shared_ptr<TRSStateSet> pSS = PNTriangleGeode->getOrCreateStateSet();
    //TRSShader* shader = pSS->getShader();
    //shader->createVertexShader("shaders/PNTriangleVertex.glsl");
    //shader->createFragmentShader("shaders/PhongFragment.glsl");
    //shader->createTessControlShader("shaders/PNTriangleTesc.glsl");
    //shader->createTessEvaluateShader("shaders/PNTriangleTese.glsl");
    //shader->createProgram();

    //PNTriangleGeode->getVAO()->setDrawType(GL_TRIANGLES);
    //PNTriangleGeode->setColor(TRSVec4(1.0, 1.0, 0.5, 1));
    //PNTriangleGeode->getVAO()->setDrawType(GL_PATCHES);
    //PNTriangleGeode->getVAO()->setDrawParam(3);
    //PNTriangleGeode->setPolygonMode(GL_LINE);

    std::shared_ptr<TRSGroup> pSharedGroup = std::shared_ptr<TRSGroup>(pGroup);
    viewer->setSecenNode(pSharedGroup);
    viewer->run();
    return 0;
}
