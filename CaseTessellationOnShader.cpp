#include "CaseTessellationOnShader.h"
#include <iostream>

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

#include "stb_image.h"
#include "TRSUtils.h"
#include "TRSResource.h"
#include "TRSWindowConfig.h"
#include "TRSCamera.h"
#include "TRSCallBackFunc.h"
#include "TRSConst.h"


int CaseTessellationOnShader()
{
    float vertices[] = {
        -0.8f, 0.0f, 0.0f,
        -0.4f, 0.0f, 0.0f,
        -0.4f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f
    };
    float tangent[] = {
        0.5f, 0.866f, 0.0f,
        0.5f, 0.866f, 0.0f
    };
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
    char* vertexShaderSource = readTextFile("shaders/hermite_curve_vert.glsl");
    char* fragmentShaderSource = readTextFile("shaders/hermite_curve_frag.glsl");
    char* tessControlShaderSource = readTextFile("shaders/hermite_curve_tess.glsl");
    char* tessEvaluateShaderSource = readTextFile("shaders/hermite_curve_eval.glsl");

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
        std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int tessControlShader;
    tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tessControlShader, 1, &tessControlShaderSource, NULL);
    glCompileShader(tessControlShader);
    glGetShaderiv(tessControlShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(tessControlShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::TESS_CONTROL::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int tessEvaluateShader;
    tessEvaluateShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tessEvaluateShader, 1, &tessEvaluateShaderSource, NULL);
    glCompileShader(tessEvaluateShader);
    glGetShaderiv(tessEvaluateShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(tessEvaluateShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::TESS_EVALUATE::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, tessControlShader);
    glAttachShader(shaderProgram, tessEvaluateShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);//bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);//unbind VAO

    GLuint locTan0 = glGetUniformLocation(shaderProgram, "vTan0");
    GLuint locTan1 = glGetUniformLocation(shaderProgram, "vTan1");
    glUniform3fv(locTan0, 1, &tangent[0]);
    glUniform3fv(locTan1, 1, &tangent[3]);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);

        glPatchParameteri(GL_PATCH_VERTICES, 2);
        glDrawArrays(GL_PATCHES, 0, 4);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
