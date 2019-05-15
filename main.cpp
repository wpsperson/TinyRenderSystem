#include "TRSUtils.h"
#include "TRSConfig.h"
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
#include "TRSStateset.h"
TRSCamera* g_pCamera;





//#include "glad\glad.h"
//#include "glfw\glfw3.h"
//#include "TRSCallBackFunc.h"
//int main()
//{
//    GLFWwindow* window(nullptr);
//    if (!TRSConfig::initGlfwWindowAndGLAD(DefaultWindowWidth, DefaultWindowHeight, &window))
//    {
//        return -1;
//    }
//    //渲染组件：shader vao texture camera
//    TRSShader* pShader = new TRSShader;
//    pShader->createProgram("vertex.glsl", "fragment.glsl");
//    TRSVAO* pVAO = new TRSVAO;
//    pVAO->createVAO(BoxVertices, sizeof(BoxVertices) / sizeof(float), EnVertexTextureColor);
//    TRSTexture* pTexture = new TRSTexture(2, std::string("container.jpg"), std::string("awesomeface.png"));
//    TRSCamera* pCamera = new TRSCamera;
//    g_pCamera = pCamera;
//    TRSConfig::registerUserInputFunc(window);//reg user input callback
//    //渲染前默认设置
//    glEnable(GL_DEPTH_TEST);
//    while (true)
//    {
//        if (glfwWindowShouldClose(window))
//        {
//            break;
//        }
//        glClearColor(s_DefaultBGColor.r, s_DefaultBGColor.g, s_DefaultBGColor.b, s_DefaultBGColor.a);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        TRSKeyboardCallBack(window);
//
//        pVAO->bind();
//        pTexture->activeAllTextures();
//        pShader->use();
//        pShader->setUniformi("texture0", 0);
//        pShader->setUniformi("texture1", 1);
//        glm::mat4 viewMatrix = pCamera->getViewMatrix();
//        glm::mat4 projectMatrix = pCamera->getProjectMatrix();
//        pShader->setUniformMatrix4("model", glm::mat4());
//        pShader->setUniformMatrix4("view", viewMatrix);
//        pShader->setUniformMatrix4("projection", projectMatrix);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//    return 0;
//}

int main()
{
    glm::vec3 lightPos = glm::vec3(1.2f, 0.0f, 0.0f);


    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGroup> pGroup = std::make_shared<TRSGroup>();
    std::shared_ptr<TRSGeode> pGeode = std::make_shared<TRSGeode>();
    pGeode->readFromVertex(BoxVerticesAndNorm, sizeof(BoxVerticesAndNorm) / sizeof(float), EnVertexNormal);
    std::shared_ptr<TRSStateSet> pGeodeStateSet = pGeode->getOrCreateStateSet();
    pGeodeStateSet->getShader()->createProgram("LightVertex.glsl", "LightFragment.glsl");
    pGeodeStateSet->getShader()->addUniform3v("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    pGeodeStateSet->getShader()->addUniform3v("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    pGeodeStateSet->getShader()->addUniform3v("lightPos", lightPos);


    std::shared_ptr<TRSGeode> pLightNode = std::make_shared<TRSGeode>(* pGeode.get() );
    glm::mat4 lightMat = glm::translate(glm::mat4(), lightPos);
    lightMat = glm::scale(lightMat, glm::vec3(0.2f));
    pLightNode->setMatrix(lightMat);
    std::shared_ptr<TRSStateSet> pLightStateSet = pLightNode->getOrCreateStateSet();
    pLightStateSet->getShader()->createProgram("LightNodeVertex.glsl", "LightNodeFragment.glsl");

    //std::shared_ptr<TRSGeode> pThird = std::make_shared<TRSGeode>();
    //pThird->readFromVertex(BoxVertices, sizeof(BoxVertices) / sizeof(float), EnVertexTextureColor);
    //pThird->setMatrix(glm::translate(glm::mat4(), glm::vec3(0, 1.5f, 0)));
    //std::shared_ptr<TRSStateSet> pSS = pThird->getOrCreateStateSet();
    //pSS->getTexture()->createTexture("container.jpg");
    //pSS->getTexture()->createTexture("awesomeface.png");
    //pSS->getShader()->createProgram("vertex.glsl", "fragment.glsl");

    pGroup->addChild(pGeode);
    pGroup->addChild(pLightNode);
    //pGroup->addChild(pThird);
    viewer->setSecenNode(pGroup);
    viewer->run();
    return 0;
}