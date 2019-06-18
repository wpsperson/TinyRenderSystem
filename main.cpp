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
#include "glfw\glfw3.h"
TRSCamera* g_pCamera;


void CaseTextureColorArray()
{
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> pThird = std::make_shared<TRSGeode>();
    pThird->readFromVertex(BoxVertices, sizeof(BoxVertices) / sizeof(float), EnVertexTextureColor);
    pThird->setMatrix(glm::translate(glm::mat4(), glm::vec3(0, 1.5f, 0)));
    std::shared_ptr<TRSStateSet> pSS = pThird->getOrCreateStateSet();
    pSS->getTexture()->createTexture("container.jpg");
    pSS->getTexture()->createTexture("awesomeface.png");
    pSS->getShader()->createProgram("vertex.glsl", "fragment.glsl");

    viewer->setSecenNode(pThird);
    viewer->run();
}


void updateFunc(TRSNode* pNode)
{
    glm::vec3 lightColor;
    lightColor.x = sin(glfwGetTime() * 2.0f);
    lightColor.y = sin(glfwGetTime() * 0.7f);
    lightColor.z = sin(glfwGetTime() * 1.3f);

    glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // 降低影响
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响
    pNode->getStateSet()->getShader()->setUniform3v("light.ambient", ambientColor);
    pNode->getStateSet()->getShader()->setUniform3v("light.diffuse", diffuseColor);
}

void CaseMaterial_AmbientDiffuseSpecular()
{
    glm::vec3 lightPos = glm::vec3(0.8f, 0.8f, 2.0f);

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGroup> pGroup = std::make_shared<TRSGroup>();
    std::shared_ptr<TRSGeode> pGeode = std::make_shared<TRSGeode>();
    pGeode->readFromVertex(BoxVerticesAndNorm, sizeof(BoxVerticesAndNorm) / sizeof(float), EnVertexNormal);
    std::shared_ptr<TRSStateSet> pGeodeStateSet = pGeode->getOrCreateStateSet();
    pGeodeStateSet->getShader()->createProgram("MaterialVertex.glsl", "MaterialFragment.glsl");
    pGeodeStateSet->getShader()->addUniform3v("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    pGeodeStateSet->getShader()->addUniform3v("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    //pGeodeStateSet->getShader()->addUniform3v("lightPos", lightPos);

    pGeodeStateSet->getShader()->addUniform3v("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    pGeodeStateSet->getShader()->addUniform3v("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    pGeodeStateSet->getShader()->addUniform3v("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    pGeodeStateSet->getShader()->addUniformf("material.shininess", 32.0f);
    pGeodeStateSet->getShader()->addUniform3v("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    pGeodeStateSet->getShader()->addUniform3v("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
    pGeodeStateSet->getShader()->addUniform3v("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    pGeodeStateSet->getShader()->addUniform3v("light.position", lightPos);
    pGeode->setUpdateCallBack(updateFunc);

    std::shared_ptr<TRSGeode> pLightNode = std::make_shared<TRSGeode>(*pGeode.get());
    glm::mat4 lightMat = glm::translate(glm::mat4(), lightPos);
    lightMat = glm::scale(lightMat, glm::vec3(0.2f));
    pLightNode->setMatrix(lightMat);
    std::shared_ptr<TRSStateSet> pLightStateSet = pLightNode->getOrCreateStateSet();
    pLightStateSet->getShader()->createProgram("LightNodeVertex.glsl", "LightNodeFragment.glsl");
    pGroup->addChild(pGeode);
    pGroup->addChild(pLightNode);
    viewer->setSecenNode(pGroup);
    viewer->run();
}



int main()
{

    
    return 0;
}