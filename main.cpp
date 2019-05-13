#include "TRSUtils.h"
#include "TRSConfig.h"
#include "TRSRender.h"
#include "TRSShader.h"
#include "TRSTexture.h"
#include "TRSVAO.h"
#include "TRSResource.h"
#include "TRSConst.h"
#include "TRSCamera.h"
#include "TRSViewer.h"
#include "TRSGeode.h"
#include "TRSGroup.h"
#include "glm\gtc\matrix_transform.hpp"
TRSCamera* g_pCamera;

//int main()
//{
//    GLFWwindow* window(nullptr);
//    if (!TRSConfig::initGlfwWindowAndGLAD(DefaultWindowWidth, DefaultWindowHeight, &window))
//    {
//        return -1;
//    }
//    //äÖÈ¾×é¼þ£ºshader vao texture camera
//    TRSShader* pShader = new TRSShader;
//    pShader->createProgram("vertex.glsl", "fragment.glsl");
//    TRSVAO* pVAO = new TRSVAO;
//    pVAO->createVAO(BoxVertices, sizeof(BoxVertices) / sizeof(float), EnVertexTextureColor);
//    TRSTexture* pTexture = new TRSTexture(2, std::string("container.jpg"), std::string("awesomeface.png"));
//    TRSCamera* pCamera = new TRSCamera;
//    g_pCamera = pCamera;
//    TRSConfig::registerUserInputFunc(window);//reg user input callback
//    //äÖÈ¾Æ÷
//    TRSRender* pRender = new TRSRender(window, pVAO, pShader, pTexture);
//    pRender->setCamera(pCamera);
//    pRender->exec();
//    return 0;
//}

int main()
{
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGroup> pGroup = std::make_shared<TRSGroup>();
    std::shared_ptr<TRSGeode> pGeode = std::make_shared<TRSGeode>();
    pGeode->readFromVertex(BoxVerticesAndTex, sizeof(BoxVerticesAndTex) / sizeof(float), EnVertexTexture);

    std::shared_ptr<TRSGeode> pSecond = std::make_shared<TRSGeode>();
    pSecond->readFromVertex(BoxVerticesAndTex, sizeof(BoxVerticesAndTex) / sizeof(float), EnVertexTexture);
    pSecond->setMatrix(glm::translate(glm::mat4(), glm::vec3(-1.5f, 0, 0)));

    pGroup->addChild(pGeode);
    pGroup->addChild(pSecond);
    viewer->setSecenNode(pGroup);
    viewer->run();
    return 0;
}