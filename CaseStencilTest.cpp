#include "CaseStencilTest.h"

#include <iostream>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "stb_image.h"
#include "TRSUtils.h"
#include "TRSResource.h"
#include "TRSWindowConfig.h"
#include "TRSCamera.h"
#include "TRSObserveCamera.h"
#include "TRSCallBackFunc.h"
#include "TRSConst.h"
#include "Windows.h"


extern TRSCamera* g_pCamera;

int CaseStencilTest()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, g_OpenGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, g_OpenGLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(DefaultWindowWidth, DefaultWindowHeight, "LearnOpenGL", NULL, NULL);
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
    char* vertexShaderSource = readTextFile("shaders/1_5MultiTextureVertex.glsl");
    char* fragmentShaderSource = readTextFile("shaders/1_5MultiTextureFragment.glsl");//1_1BasicFragment.glsl  
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

    unsigned int shaderBorder = createShaderBorder();



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

    unsigned int BoxTexture;
    glGenTextures(1, &BoxTexture);
    glBindTexture(GL_TEXTURE_2D, BoxTexture);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ���ز���������
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);//����ͼƬ��ת
    unsigned char *data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    unsigned int FaceTexture;
    glGenTextures(1, &FaceTexture);
    glBindTexture(GL_TEXTURE_2D, FaceTexture);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char *data2 = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);



    int nPosModelMatrix = glGetUniformLocation(shaderProgram, "model");
    int nPosViewMatrix = glGetUniformLocation(shaderProgram, "view");
    int nPosProjectMatrix = glGetUniformLocation(shaderProgram, "projection");

    int nPosModelMatrix2 = glGetUniformLocation(shaderBorder, "model");
    int nPosViewMatrix2 = glGetUniformLocation(shaderBorder, "view");
    int nPosProjectMatrix2 = glGetUniformLocation(shaderBorder, "projection");

    TRSMatrix IdentityMatrix;
    TRSMatrix ScaleUpMatrix;
    ScaleUpMatrix.makeScale(1.05f, 1.05f, 1.05f);
    TRSMatrix TranslateMatrix;
    TranslateMatrix.makeTranslate(0.8f, 0.5f, 0.1f);
    TRSMatrix SecondModelScaleUpMatrix = TranslateMatrix * ScaleUpMatrix;
    //glUniform1i����ÿ���������ķ�ʽ����OpenGLÿ����ɫ�������������ĸ�����Ԫ������ֻ��Ҫ����һ�μ��ɣ���������������Ⱦѭ����ǰ�棺
    //��ɫ����texture0��ӦGL_TEXTURE0��BoxTexture��
    //��ɫ����texture1��ӦGL_TEXTURE1��FaceTexture��
    int nPosTexture0 = glGetUniformLocation(shaderProgram, "texture0");
    int nPosTexture1 = glGetUniformLocation(shaderProgram, "texture1");
    glUniform1i(nPosTexture0, 0);
    glUniform1i(nPosTexture1, 1);
    glEnable(GL_DEPTH_TEST);

    TRSWindowConfig::registerUserInputFunc(window);
    g_pCamera = new TRSObserveCamera(window);
    double m_fCurTime = 0;
    double m_fLastTime = 0;


    // ����ģ�����
    glEnable(GL_STENCIL_TEST); //***

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        m_fCurTime = glfwGetTime();
        double timeDiff = m_fCurTime - m_fLastTime;
        if (timeDiff < 15)
        {
            Sleep(15 - timeDiff);
        }
        m_fLastTime = m_fCurTime;
        TRSKeyboardCallBack(window);

        glStencilFunc(GL_ALWAYS, 1, 0xff); //***
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //***
        //glStencilMask(0xff); //***
        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        TRSMatrix projMatrix = g_pCamera->getProjectMatrix();
        TRSMatrix viewMatrix = g_pCamera->getViewMatrix();
        glUniformMatrix4fv(nPosModelMatrix, 1, GL_FALSE, &(IdentityMatrix[0][0]));
        glUniformMatrix4fv(nPosViewMatrix, 1, GL_FALSE, &(viewMatrix[0][0]));
        glUniformMatrix4fv(nPosProjectMatrix, 1, GL_FALSE, &(projMatrix[0][0]));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, BoxTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, FaceTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUniformMatrix4fv(nPosModelMatrix, 1, GL_FALSE, &(TranslateMatrix[0][0]));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glStencilFunc(GL_NOTEQUAL, 1, 0xff); //***
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //***
        //glStencilMask(0x00); //***
        glDisable(GL_DEPTH_TEST); //***
        glBindVertexArray(VAO);
        glUseProgram(shaderBorder);
        glUniformMatrix4fv(nPosModelMatrix2, 1, GL_FALSE, &(ScaleUpMatrix[0][0]));
        glUniformMatrix4fv(nPosViewMatrix2, 1, GL_FALSE, &(viewMatrix[0][0]));
        glUniformMatrix4fv(nPosProjectMatrix2, 1, GL_FALSE, &(projMatrix[0][0]));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, BoxTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, FaceTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUniformMatrix4fv(nPosModelMatrix2, 1, GL_FALSE, &(SecondModelScaleUpMatrix[0][0]));
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

int createShaderBorder()
{
    char* vertexShaderSource = readTextFile("shaders/DefaultVertex.glsl");
    char* fragmentShaderSource = readTextFile("shaders/1_1BasicFragment.glsl");//  
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
    return shaderProgram;
}
