#include "E02BasicMaterial.h"
#include <memory>
#include <chrono>
#include "glad\glad.h"
#include "Core\TRSViewer.h"
#include "Core\TRSStateSet.h"
#include "Core\TRSTexture.h"
#include "Core\TRSShader.h"
#include "DataModel\TRSGeode.h"
#include "DataModel\TRSGroup.h"
#include "Util\TRSResource.h"
#include "Math\TRSMathUtil.h"
#include "Geometry\TRSCube.h"




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
    //pNode->getStateSet()->getShader()->addUniform3v("light.ambient", ambientColor);
    pNode->getStateSet()->getShader()->addUniform3v("light.diffuse", diffuseColor);
}

void E02Material_AmbientDiffuseSpecular()
{
    TRSVec3 lightPos = TRSVec3(0.8f, 0.8f, 2.0f);

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGroup> pGroup = std::make_shared<TRSGroup>();
    std::shared_ptr<TRSGeode> pGeode = std::make_shared<TRSGeode>();
    // pGeode->readFromVertex(BoxVerticesAndNorm, sizeof(BoxVerticesAndNorm) / sizeof(float), EnVertexNormal);
    TRSCube* cube = new TRSCube(true);
    TRSMesh* mesh = cube->getMesh();
    pGeode->setMesh(mesh);
    std::shared_ptr<TRSStateSet> pGeodeStateSet = pGeode->getOrCreateStateSet();
    pGeodeStateSet->getShader()->createProgram("shaders/PosNormVertex.glsl", "shaders/2_1MaterialFragment.glsl");

    pGeodeStateSet->getShader()->addUniform3v("material.ambient", TRSVec3(0.5f, 0.5f, 0.5f));
    pGeodeStateSet->getShader()->addUniform3v("material.diffuse", TRSVec3(1.0f, 0.5f, 0.31f));
    pGeodeStateSet->getShader()->addUniform3v("material.specular", TRSVec3(0.5f, 0.5f, 0.5f));
    pGeodeStateSet->getShader()->addUniformf("material.shininess", 32.0f);
    pGeodeStateSet->getShader()->addUniform3v("light.ambient", TRSVec3(0.5f, 0.5f, 0.5f));
    pGeodeStateSet->getShader()->addUniform3v("light.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
    pGeodeStateSet->getShader()->addUniform3v("light.specular", TRSVec3(1.0f, 1.0f, 1.0f));
    pGeodeStateSet->getShader()->addUniform3v("light.position", lightPos);
    pGeode->setUpdateCallBack(updateFunc);

    pGroup->addChild(pGeode);
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
    // pBoxNode->readFromVertex(BoxVerticesAndNormAndTex, sizeof(BoxVerticesAndNormAndTex) / sizeof(float), EnVertexNormTexture);
    TRSCube* cube = new TRSCube(true);
    TRSMesh* mesh = cube->getMesh();
    pBoxNode->setMesh(mesh);
    std::shared_ptr<TRSStateSet> pBoxSS = pBoxNode->getOrCreateStateSet();
    pBoxSS->getShader()->createProgram("shaders/PosNormTexVertex.glsl", "shaders/2_2DiffuseNormFragment.glsl");
    pBoxSS->getTexture()->createTexture("resources/textures/container2.jpg", "material.diffuse");//container2.png 加载后图片显示雪花转为jpg。
    pBoxSS->getTexture()->createTexture("resources/textures/container2_specular.jpg", "material.specular");
    //pBoxSS->getShader()->addUniform3v("material.specular", TRSVec3(0.5f, 0.5f, 0.5f));
    pBoxSS->getShader()->addUniformf("material.shininess", 32.0f);
    pBoxSS->getShader()->addUniform3v("light.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pBoxSS->getShader()->addUniform3v("light.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
    pBoxSS->getShader()->addUniform3v("light.specular", TRSVec3(1.0f, 1.0f, 1.0f));
    pBoxSS->getShader()->addUniform3v("light.position", lightPos);

    pGroup->addChild(pBoxNode);
    viewer->setSecenNode(pGroup);
    viewer->run();
}



