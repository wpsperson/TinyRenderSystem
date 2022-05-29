#include "E02BasicMaterial.h"
#include <memory>
#include <chrono>
#include "glad\glad.h"
#include "TRSViewer.h"
#include "TRSGeode.h"
#include "TRSStateSet.h"
#include "TRSTexture.h"
#include "TRSResource.h"
#include "TRSGroup.h"
#include "TRSMathUtil.h"
#include "TRSShader.h"

//第二章 基本光照
void updateFunc(TRSNode* pNode)
{
    static auto start = std::chrono::steady_clock::now();
    auto current = std::chrono::steady_clock::now();
    std::chrono::milliseconds result = std::chrono::duration_cast<std::chrono::milliseconds>(current - start);
    __int64 msElapsed2 = result.count();
    float fElaps = float(msElapsed2) / 1000;

    TRSVec3 lightColor;
    lightColor[0] = float(sin(fElaps * 2.0f));
    lightColor[1] = float(sin(fElaps * 0.7f));
    lightColor[2] = float(sin(fElaps * 1.3f));

    TRSVec3 diffuseColor = lightColor * 0.5f; // 降低影响
    TRSVec3 ambientColor = diffuseColor * 0.2f; // 很低的影响
    pNode->getStateSet()->getShader()->addUniform3v("light.ambient", ambientColor);
    pNode->getStateSet()->getShader()->addUniform3v("light.diffuse", diffuseColor);
}

void E02Material_AmbientDiffuseSpecular()
{
    TRSVec3 lightPos = TRSVec3(0.8f, 0.8f, 2.0f);

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGroup> pGroup = std::make_shared<TRSGroup>();
    std::shared_ptr<TRSGeode> pGeode = std::make_shared<TRSGeode>();
    pGeode->readFromVertex(BoxVerticesAndNorm, sizeof(BoxVerticesAndNorm) / sizeof(float), EnVertexNormal);
    std::shared_ptr<TRSStateSet> pGeodeStateSet = pGeode->getOrCreateStateSet();
    pGeodeStateSet->getShader()->createProgram("shaders/2_1MaterialVertex.glsl", "shaders/2_1MaterialFragment.glsl");

    pGeodeStateSet->getShader()->addUniform3v("material.ambient", TRSVec3(1.0f, 0.5f, 0.31f));
    pGeodeStateSet->getShader()->addUniform3v("material.diffuse", TRSVec3(1.0f, 0.5f, 0.31f));
    pGeodeStateSet->getShader()->addUniform3v("material.specular", TRSVec3(0.5f, 0.5f, 0.5f));
    pGeodeStateSet->getShader()->addUniformf("material.shininess", 32.0f);
    pGeodeStateSet->getShader()->addUniform3v("light.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pGeodeStateSet->getShader()->addUniform3v("light.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
    pGeodeStateSet->getShader()->addUniform3v("light.specular", TRSVec3(1.0f, 1.0f, 1.0f));
    pGeodeStateSet->getShader()->addUniform3v("light.position", lightPos);
    pGeode->setUpdateCallBack(updateFunc);

    std::shared_ptr<TRSGeode> pLightNode = std::make_shared<TRSGeode>(*pGeode.get(), false);
    TRSMatrix lightMat;
    lightMat.translate(lightPos);
    lightMat.scale(0.2f);
    pLightNode->setMatrix(lightMat);
    std::shared_ptr<TRSStateSet> pLightStateSet = pLightNode->getOrCreateStateSet();
    pLightStateSet->getShader()->createProgram("shaders/2_1LightNodeVertex.glsl", "shaders/2_1LightNodeFragment.glsl");
    pGroup->addChild(pGeode);
    pGroup->addChild(pLightNode);
    viewer->setSecenNode(pGroup);
    viewer->run();
}

//第二章 漫反射贴图和法线贴图
void E02Material_DiffuseNormal()
{
    TRSVec3 lightPos = TRSVec3(0.8f, 0.8f, 2.0f);
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGroup> pGroup = std::make_shared<TRSGroup>();
    std::shared_ptr<TRSGeode> pBoxNode = std::make_shared<TRSGeode>();
    pBoxNode->readFromVertex(BoxVerticesAndNormAndTex, sizeof(BoxVerticesAndNormAndTex) / sizeof(float), EnVertexNormTexture);
    std::shared_ptr<TRSStateSet> pBoxSS = pBoxNode->getOrCreateStateSet();
    pBoxSS->getShader()->createProgram("shaders/2_2DiffuseNormVertex.glsl", "shaders/2_2DiffuseNormFragment.glsl");
    pBoxSS->getTexture()->createTexture("resources/textures/container2.jpg", "material.diffuse");//container2.png 加载后图片显示雪花转为jpg。
    pBoxSS->getTexture()->createTexture("resources/textures/container2_specular.jpg", "material.specular");
    //pBoxSS->getShader()->addUniform3v("material.specular", TRSVec3(0.5f, 0.5f, 0.5f));
    pBoxSS->getShader()->addUniformf("material.shininess", 32.0f);
    pBoxSS->getShader()->addUniform3v("light.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pBoxSS->getShader()->addUniform3v("light.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
    pBoxSS->getShader()->addUniform3v("light.specular", TRSVec3(1.0f, 1.0f, 1.0f));
    pBoxSS->getShader()->addUniform3v("light.position", lightPos);

    std::shared_ptr<TRSGeode> pLightNode = std::make_shared<TRSGeode>(*pBoxNode.get(), false);
    TRSMatrix lightMat;
    lightMat.translate(lightPos);
    lightMat.scale(0.2f);
    pLightNode->setMatrix(lightMat);
    std::shared_ptr<TRSStateSet> pLightStateSet = pLightNode->getOrCreateStateSet();
    pLightStateSet->getShader()->createProgram("shaders/2_1LightNodeVertex.glsl", "shaders/2_1LightNodeFragment.glsl");

    pGroup->addChild(pBoxNode);
    pGroup->addChild(pLightNode);
    viewer->setSecenNode(pGroup);
    viewer->run();
}



