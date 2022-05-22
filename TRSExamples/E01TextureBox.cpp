#include "E01TextureBox.h"
#include <memory>
#include "glad\glad.h"
#include "TRSViewer.h"
#include "TRSGeode.h"
#include "TRSStateSet.h"
#include "TRSTexture.h"
#include "TRSResource.h"
#include "TRSGroup.h"
#include "TRSMathUtil.h"
#include "TRSShader.h"

//开心盒子 shader未开启默认摄像机系统
void E01TextureColorArray()
{
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();
    pNode->readFromVertex(BoxVerticesAndColorAndTex, sizeof(BoxVerticesAndColorAndTex) / sizeof(float), EnVertexColorTexture);
    std::shared_ptr<TRSStateSet> pSS = pNode->getOrCreateStateSet();
    pSS->getTexture()->createTexture("resources/textures/container.jpg");
    pSS->getTexture()->createTexture("resources/textures/awesomeface.png");
    pSS->getShader()->createProgram("shaders/1_5MultiTextureVertex.glsl", "shaders/1_5MultiTextureFragment.glsl");

    std::shared_ptr<TRSGeode> pWireFrame = std::make_shared<TRSGeode>(*pNode, false);
    pWireFrame->setPolygonMode(GL_LINE);
    pWireFrame->setColor(TRSVec4(1, 1, 1, 1));

    std::shared_ptr<TRSGroup> root = std::make_shared<TRSGroup>();
    root->addChild(pWireFrame);
    root->addChild(pNode);

    viewer->setSecenNode(root);
    viewer->run();
}

//多个开心盒子场景，shader未开启默认摄像机系统
void E01ManyBoxRotate()
{
    TRSVec3 cubePositions[] = {
        TRSVec3(0.0f,  0.0f,  0.0f),
        TRSVec3(2.0f,  5.0f, -15.0f),
        TRSVec3(-1.5f, -2.2f, -2.5f),
        TRSVec3(-3.8f, -2.0f, -12.3f),
        TRSVec3(2.4f, -0.4f, -3.5f),
        TRSVec3(-1.7f,  3.0f, -7.5f),
        TRSVec3(1.3f, -2.0f, -2.5f),
        TRSVec3(1.5f,  2.0f, -2.5f),
        TRSVec3(1.5f,  0.2f, -1.5f),
        TRSVec3(-1.3f,  1.0f, -1.5f)
    };

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGroup> pGroup = std::make_shared<TRSGroup>();
    std::shared_ptr<TRSGeode> pTemplateNode = std::make_shared<TRSGeode>();
    std::shared_ptr<TRSStateSet> pSS = pTemplateNode->getOrCreateStateSet();
    pSS->getTexture()->createTexture("resources/textures/container.jpg");//container.jpg
    pSS->getTexture()->createTexture("resources/textures/awesomeface.png");
    pSS->getShader()->createProgram("shaders/1_5MultiTextureVertex.glsl", "shaders/1_5MultiTextureFragment.glsl");
    pTemplateNode->readFromVertex(BoxVerticesAndColorAndTex, sizeof(BoxVerticesAndColorAndTex) / sizeof(float), EnVertexColorTexture);
    int nBoxCount = sizeof(cubePositions) / sizeof(cubePositions[0]);
    for (int i = 0; i < nBoxCount; i++)
    {
        std::shared_ptr<TRSGeode> pTemp = std::make_shared<TRSGeode>(*pTemplateNode);
        TRSVec3 vecPos = cubePositions[i];
        TRSMatrix modelMatrix;
        modelMatrix.makeTranslate(vecPos);
        float angle = 20.0f * i;
        modelMatrix.rotate(toRadian(angle), TRSVec3(1.0f, 0.3f, 0.5f));
        pTemp->setMatrix(modelMatrix);
        pGroup->addChild(pTemp);
    }

    viewer->setSecenNode(pGroup);
    viewer->run();
}



