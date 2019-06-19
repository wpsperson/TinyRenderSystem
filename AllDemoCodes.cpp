#include "AllDemoCodes.h"
#include "TRSTexture.h"
#include "TRSVAO.h"
#include "TRSResource.h"
#include "TRSConst.h"
#include "TRSCamera.h"
#include "TRSViewer.h"
#include "TRSGeode.h"
#include "TRSGroup.h"
#include "TRSShader.h"
#include "glm\gtc\matrix_transform.hpp"
#include "TRSStateset.h"
#include "glfw\glfw3.h"
TRSCamera* g_pCamera;

#include <iostream>
#include "TRSUtils.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
int CaseFirstTriangle()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    char* vertexShaderSource = readTextFile("1_1vertex.glsl");
    char* fragmentShaderSource = readTextFile("1_1fragment.glsl");
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);




    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}


//第一章 基础纹理
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


//第二章 基本光照
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

//第二章 漫反射贴图和法线贴图
void CaseMaterial_DiffuseNormal()
{
    glm::vec3 lightPos = glm::vec3(0.8f, 0.8f, 2.0f);
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGroup> pGroup = std::make_shared<TRSGroup>();
    std::shared_ptr<TRSGeode> pGeode = std::make_shared<TRSGeode>();
    pGeode->readFromVertex(BoxVerticesAndTex, sizeof(BoxVerticesAndTex) / sizeof(float), EnVertexTexture);

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

