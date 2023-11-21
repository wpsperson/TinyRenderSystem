#include "StencilTest.h"
#include <iostream>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "stb_image.h"
#include "Windows.h"
#include "BasicUtils.h"

static float BoxVerticesAndColorAndTex[] =
{
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f
};


int StencilTest()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, g_OpenGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, g_OpenGLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(DefaultWindowWidth, DefaultWindowHeight, "TinyRenderSystem", NULL, NULL);
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
    unsigned int shaderProgram = createProgramByShaderFiles("shaders/DefaultVertex.glsl", "shaders/DefaultFragment.glsl");
    unsigned int shaderBorder = createProgramByShaderFiles("shaders/DefaultVertex.glsl", "shaders/DefaultFragment.glsl");

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);//bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BoxVerticesAndColorAndTex), BoxVerticesAndColorAndTex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);//unbind VAO

    int nPosModelMatrix = glGetUniformLocation(shaderProgram, "model");
    int nPosViewMatrix = glGetUniformLocation(shaderProgram, "view");
    int nPosProjectMatrix = glGetUniformLocation(shaderProgram, "projection");
    int nPosBaseColor = glGetUniformLocation(shaderProgram, "baseColor");

    int nPosModelMatrix2 = glGetUniformLocation(shaderBorder, "model");
    int nPosViewMatrix2 = glGetUniformLocation(shaderBorder, "view");
    int nPosProjectMatrix2 = glGetUniformLocation(shaderBorder, "projection");
    int nPosBaseColor2 = glGetUniformLocation(shaderProgram, "baseColor");

    float modelMatrix[16] = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };
    float modelMatrixScale[16] = { 1.05f,0,0,0,  0,1.05f,0,0,  0,0,1.05f,0,  0,0,0,1 };
    float projMatrix[16] = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };

    glEnable(GL_DEPTH_TEST);
    // 启动模板测试
    glEnable(GL_STENCIL_TEST); //***

    double rotateAngle = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        rotateAngle += 0.001;
        if (rotateAngle > 6.28)
        {
            rotateAngle = 0.0;
        }
        double c = cos(rotateAngle);
        double s = sin(rotateAngle);
        float viewMatrix[16] = { c,s,0,0,  -s,c,0,0,  0,0,1,0,  0,0,0,1 };

        glStencilFunc(GL_ALWAYS, 1, 0xff); //***
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //***
        //glStencilMask(0xff); //***
        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(nPosModelMatrix, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(nPosViewMatrix, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(nPosProjectMatrix, 1, GL_FALSE, projMatrix);
        glUniform4f(nPosBaseColor, 0.6f, 0.6f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glStencilFunc(GL_NOTEQUAL, 1, 0xff); //***
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //***
        //glStencilMask(0x00); //***
        glDisable(GL_DEPTH_TEST); //***
        glBindVertexArray(VAO);
        glUseProgram(shaderBorder);
        glUniformMatrix4fv(nPosModelMatrix2, 1, GL_FALSE, modelMatrixScale);
        glUniformMatrix4fv(nPosViewMatrix2, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(nPosProjectMatrix2, 1, GL_FALSE, projMatrix);
        glUniform4f(nPosBaseColor2, 0.6f, 1.0f, 0.6f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //glStencilMask(0xFF); //***
        glEnable(GL_DEPTH_TEST); //***
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
