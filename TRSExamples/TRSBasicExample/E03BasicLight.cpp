#include "E03BasicLight.h"
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
#include "TRSCamera.h"


void E03DirectionLight()
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

    TRSVec3 lightDirection = TRSVec3(-0.2f, -1.0f, -0.3f);
    pTemplateNode->readFromVertex(BoxVerticesAndNormAndTex, sizeof(BoxVerticesAndNormAndTex) / sizeof(float), EnVertexNormTexture);
    std::shared_ptr<TRSStateSet> pBoxSS = pTemplateNode->getOrCreateStateSet();
    pBoxSS->getShader()->createProgram("shaders/2_3DirectionLightNormVertex.glsl", "shaders/2_3DirectionLightNormFragment.glsl");
    pBoxSS->getTexture()->createTexture("resources/textures/container2.jpg", "material.diffuse");//container2.png ���غ�ͼƬ��ʾѩ��תΪjpg��
    pBoxSS->getTexture()->createTexture("resources/textures/container2_specular.jpg", "material.specular");
    //pBoxSS->getShader()->addUniform3v("material.specular", TRSVec3(0.5f, 0.5f, 0.5f));
    pBoxSS->getShader()->addUniformf("material.shininess", 32.0f);
    pBoxSS->getShader()->addUniform3v("light.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pBoxSS->getShader()->addUniform3v("light.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // �����յ�����һЩ�Դ��䳡��
    pBoxSS->getShader()->addUniform3v("light.specular", TRSVec3(1.0f, 1.0f, 1.0f));
    pBoxSS->getShader()->addUniform3v("light.direction", lightDirection);

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


void E03PointAttenuationLight()
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

    TRSVec3 lightPos = TRSVec3(0.8f, 0.8f, 2.0f);
    pTemplateNode->readFromVertex(BoxVerticesAndNormAndTex, sizeof(BoxVerticesAndNormAndTex) / sizeof(float), EnVertexNormTexture);
    std::shared_ptr<TRSStateSet> pBoxSS = pTemplateNode->getOrCreateStateSet();
    pBoxSS->getShader()->createProgram("shaders/2_4PointAttenuationLightVertex.glsl", "shaders/2_4PointAttenuationLightFragment.glsl");
    pBoxSS->getTexture()->createTexture("resources/textures/container2.jpg", "material.diffuse");//container2.png ���غ�ͼƬ��ʾѩ��תΪjpg��
    pBoxSS->getTexture()->createTexture("resources/textures/container2_specular.jpg", "material.specular");
    //pBoxSS->getShader()->addUniform3v("material.specular", TRSVec3(0.5f, 0.5f, 0.5f));
    pBoxSS->getShader()->addUniformf("material.shininess", 32.0f);
    pBoxSS->getShader()->addUniform3v("light.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pBoxSS->getShader()->addUniform3v("light.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // �����յ�����һЩ�Դ��䳡��
    pBoxSS->getShader()->addUniform3v("light.specular", TRSVec3(1.0f, 1.0f, 1.0f));
    pBoxSS->getShader()->addUniform3v("light.position", lightPos);
    //����˥���ĵ��Դ�Ĺ�ʽ�е�������  ��ǿ�� = 1/(Kc + Kl*d + Kq * d*d)
    pBoxSS->getShader()->addUniformf("light.constant", 1.0f);
    pBoxSS->getShader()->addUniformf("light.linear", 0.09f);
    pBoxSS->getShader()->addUniformf("light.quadratic", 0.032f);

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

    std::shared_ptr<TRSGeode> pLightNode = std::make_shared<TRSGeode>(*pTemplateNode.get(), false);
    TRSMatrix lightMat;
    lightMat.translate(lightPos);
    lightMat.scale(0.2f);
    std::shared_ptr<TRSStateSet> pLightStateSet = pLightNode->getOrCreateStateSet();
    pLightStateSet->getShader()->createProgram("shaders/2_1LightNodeVertex.glsl", "shaders/2_1LightNodeFragment.glsl");
    pGroup->addChild(pLightNode);

    viewer->setSecenNode(pGroup);
    viewer->run();
}


TRSCamera* globalCamera = nullptr;
void SpotlightUpdateFunc(TRSNode* pNode)
{
    pNode->getStateSet()->getShader()->addUniform3v("light.position", globalCamera->getPosition());
    pNode->getStateSet()->getShader()->addUniform3v("light.direction", globalCamera->getFront());
    pNode->getStateSet()->getShader()->addUniformf("light.cutOff", std::cos(toRadian(12.5f)));
    pNode->getStateSet()->getShader()->addUniformf("light.outerCutOff", std::cos(toRadian(17.5f)));
}
void E03Spotlight()
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
    globalCamera = viewer->getCamera();
    TRSVec3 lightPos = TRSVec3(0.8f, 0.8f, 2.0f);
    pTemplateNode->readFromVertex(BoxVerticesAndNormAndTex, sizeof(BoxVerticesAndNormAndTex) / sizeof(float), EnVertexNormTexture);
    std::shared_ptr<TRSStateSet> pBoxSS = pTemplateNode->getOrCreateStateSet();
    pBoxSS->getShader()->createProgram("shaders/2_5SpotLightVertex.glsl", "shaders/2_5SpotLightFragment.glsl");
    pBoxSS->getTexture()->createTexture("resources/textures/container2.jpg", "material.diffuse");//container2.png ���غ�ͼƬ��ʾѩ��תΪjpg��
    pBoxSS->getTexture()->createTexture("resources/textures/container2_specular.jpg", "material.specular");
    pBoxSS->getShader()->addUniformf("material.shininess", 32.0f);
    pBoxSS->getShader()->addUniform3v("light.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pBoxSS->getShader()->addUniform3v("light.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // �����յ�����һЩ�Դ��䳡��
    pBoxSS->getShader()->addUniform3v("light.specular", TRSVec3(1.0f, 1.0f, 1.0f));
    pBoxSS->getShader()->addUniform3v("light.position", lightPos);
    pTemplateNode->setUpdateCallBack(SpotlightUpdateFunc);

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

void MultiLightSpotlightUpdateFunc(TRSNode* pNode)
{
    pNode->getStateSet()->getShader()->addUniform3v("spotLight.position", globalCamera->getPosition());
    pNode->getStateSet()->getShader()->addUniform3v("spotLight.direction", globalCamera->getFront());
    pNode->getStateSet()->getShader()->addUniformf("spotLight.cutOff", std::cos(toRadian(12.5f)));
    pNode->getStateSet()->getShader()->addUniformf("spotLight.outerCutOff", std::cos(toRadian(17.5f)));
}

void E03MultiLightSource()
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
    globalCamera = viewer->getCamera();

    pTemplateNode->readFromVertex(BoxVerticesAndNormAndTex, sizeof(BoxVerticesAndNormAndTex) / sizeof(float), EnVertexNormTexture);
    std::shared_ptr<TRSStateSet> pBoxSS = pTemplateNode->getOrCreateStateSet();
    pBoxSS->getShader()->createProgram("shaders/2_6MultiLightSourceVertex.glsl", "shaders/2_6MultiLightSourceFragment.glsl");
    pBoxSS->getTexture()->createTexture("resources/textures/container2.jpg", "material.diffuse");//container2.png ���غ�ͼƬ��ʾѩ��תΪjpg��
    pBoxSS->getTexture()->createTexture("resources/textures/container2_specular.jpg", "material.specular");
    pBoxSS->getShader()->addUniformf("material.shininess", 32.0f);
    //һ��ƽ�й�
    TRSVec3 lightDirection = TRSVec3(-0.2f, -1.0f, -0.3f);
    pBoxSS->getShader()->addUniform3v("dirLight.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pBoxSS->getShader()->addUniform3v("dirLight.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // �����յ�����һЩ�Դ��䳡��
    pBoxSS->getShader()->addUniform3v("dirLight.specular", TRSVec3(1.0f, 1.0f, 1.0f));
    pBoxSS->getShader()->addUniform3v("dirLight.direction", lightDirection);
    //�ĸ����Դ
    TRSVec3 pointLightPositions[] = {
        TRSVec3(0.7f,  0.2f,  2.0f),
        TRSVec3(2.3f, -3.3f, -4.0f),
        TRSVec3(-4.0f,  2.0f, -12.0f),
        TRSVec3(0.0f,  0.0f, -3.0f)
    };
    int nPointLightNum = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);
    for (int i = 0; i < nPointLightNum; i++)
    {
        std::string strLightName = "pointLights[" + std::to_string(i) + "]";
        pBoxSS->getShader()->addUniform3v(strLightName + ".ambient", TRSVec3(0.2f, 0.2f, 0.2f));
        pBoxSS->getShader()->addUniform3v(strLightName + ".diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // �����յ�����һЩ�Դ��䳡��
        pBoxSS->getShader()->addUniform3v(strLightName + ".specular", TRSVec3(1.0f, 1.0f, 1.0f));
        pBoxSS->getShader()->addUniform3v(strLightName + ".position", pointLightPositions[i]);
        //����˥���ĵ��Դ�Ĺ�ʽ�е�������  ��ǿ�� = 1/(Kc + Kl*d + Kq * d*d)
        pBoxSS->getShader()->addUniformf(strLightName + ".constant", 1.0f);
        pBoxSS->getShader()->addUniformf(strLightName + ".linear", 0.09f);
        pBoxSS->getShader()->addUniformf(strLightName + ".quadratic", 0.032f);

        //˳����ӵƹ�Ľڵ�
        std::shared_ptr<TRSGeode> pLightNode = std::make_shared<TRSGeode>(*pTemplateNode.get(), false);
        TRSMatrix lightMat;
        lightMat.translate(pointLightPositions[i]);
        lightMat.scale(0.2f);
        pLightNode->setMatrix(lightMat);
        std::shared_ptr<TRSStateSet> pLightStateSet = pLightNode->getOrCreateStateSet();
        pLightStateSet->getShader()->createProgram("shaders/2_1LightNodeVertex.glsl", "shaders/2_1LightNodeFragment.glsl");
        pGroup->addChild(pLightNode);
    }
    // �۹��
    pBoxSS->getShader()->addUniform3v("spotLight.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pBoxSS->getShader()->addUniform3v("spotLight.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // �����յ�����һЩ�Դ��䳡��
    pBoxSS->getShader()->addUniform3v("spotLight.specular", TRSVec3(1.0f, 1.0f, 1.0f));
    pTemplateNode->setUpdateCallBack(MultiLightSpotlightUpdateFunc);


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

