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
#include "TRSAssimpLoader.h"
#include "TRSStateset.h"
#include "glfw\glfw3.h"

#include <iostream>
#include "TRSUtils.h"
#include "stb_image.h"
#include "TRSMathUtil.h"



//最简单的，第一个三角形
int CaseFirstTriangle()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
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
    char* vertexShaderSource = readTextFile("shaders/1_1BasicVertex.glsl");
    char* fragmentShaderSource = readTextFile("shaders/1_1BasicFragment.glsl");
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
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLES, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
//采用ElementArray绘制
int CaseElementArray()
{
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = { // 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
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
    char* vertexShaderSource = readTextFile("shaders/1_1BasicVertex.glsl");
    char* fragmentShaderSource = readTextFile("shaders/1_1BasicFragment.glsl");
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

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VEO;
    glGenBuffers(1, &VEO);
    glBindVertexArray(VAO);//bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);//unbind VAO

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
//随时间变化的颜色统一值，三角形颜色渐变
int CaseColorAnimation()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
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
    char* vertexShaderSource = readTextFile("shaders/1_2ColorAnimationVertex.glsl");
    char* fragmentShaderSource = readTextFile("shaders/1_2ColorAnimationFragment.glsl");
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

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        //Color Animation
        double timeValue = glfwGetTime();
        double greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
//三角形更多属性，每个顶点一个颜色。
int CaseEachVertexColor()
{
    float vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
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
    char* vertexShaderSource = readTextFile("shaders/1_3EachVertexColorVertex.glsl");
    char* fragmentShaderSource = readTextFile("shaders/1_3EachVertexColorFragment.glsl");
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

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);//bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);//unbind VAO

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

//纹理和每个顶点颜色混合
int CaseTextureColorBasic()
{
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };
    unsigned int indices[] = { // 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
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
    char* vertexShaderSource = readTextFile("shaders/1_4TextureColorVertex.glsl");
    char* fragmentShaderSource = readTextFile("shaders/1_4TextureColorFragment.glsl");
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

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VEO;
    glGenBuffers(1, &VEO);
    glBindVertexArray(VAO);//bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);//unbind VAO

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int width, height, nrChannels;
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


    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);//这句可以不写，因为默认Texture0总是被激活。
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
//两个纹理混合（两个纹理混合兼每个顶点颜色混合）
int CaseMultiTexture()
{
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };
    unsigned int indices[] = { // 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
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
    char* vertexShaderSource = readTextFile("shaders/1_5MultiTextureVertex.glsl");
    char* fragmentShaderSource = readTextFile("shaders/1_5MultiTextureFragment.glsl");
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

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VEO;
    glGenBuffers(1, &VEO);
    glBindVertexArray(VAO);//bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);//加载图片翻转
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
    // 为当前绑定的纹理对象设置环绕、过滤方式
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

    //这部分本来用不到，但因为后面使用TRS渲染引擎，修改了共同的shader，所以此处适配一下
    int nPosModelMatrix = glGetUniformLocation(shaderProgram, "model");
    int nPosViewMatrix = glGetUniformLocation(shaderProgram, "view");
    int nPosProjectMatrix = glGetUniformLocation(shaderProgram, "projection");
    TRSMatrix IdentityMatrix;
    // to do
    glUniformMatrix4fv(nPosModelMatrix, 1, GL_FALSE, &(IdentityMatrix[0][0]));
    glUniformMatrix4fv(nPosViewMatrix, 1, GL_FALSE, &(IdentityMatrix[0][0]));
    glUniformMatrix4fv(nPosProjectMatrix, 1, GL_FALSE, &(IdentityMatrix[0][0]));

    //glUniform1i设置每个采样器的方式告诉OpenGL每个着色器采样器属于哪个纹理单元。我们只需要设置一次即可，所以这个会放在渲染循环的前面：
    //着色器中texture0对应GL_TEXTURE0（BoxTexture）
    //着色器中texture1对应GL_TEXTURE1（FaceTexture）
    int nPosTexture0 = glGetUniformLocation(shaderProgram, "texture0");
    int nPosTexture1 = glGetUniformLocation(shaderProgram, "texture1");
    glUniform1i(nPosTexture0, 0);
    glUniform1i(nPosTexture1, 1);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);//这句可以不写，因为默认Texture0总是被激活。
        glBindTexture(GL_TEXTURE_2D, BoxTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, FaceTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

//----------- 使用TRS渲染引擎 ----------------------

//开心盒子 shader未开启默认摄像机系统
void CaseTextureColorArray()
{
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();
    pNode->readFromVertex(BoxVerticesAndColorAndTex, sizeof(BoxVerticesAndColorAndTex) / sizeof(float), EnVertexColorTexture);
    std::shared_ptr<TRSStateSet> pSS = pNode->getOrCreateStateSet();
    pSS->getTexture()->createTexture("resources/textures/container.jpg");
    pSS->getTexture()->createTexture("resources/textures/awesomeface.png");
    pSS->getShader()->createProgram("shaders/1_5MultiTextureVertex.glsl", "shaders/1_5MultiTextureFragment.glsl");

    std::shared_ptr<TRSGeode> pWireFrame = std::make_shared<TRSGeode>(*pNode, false);
    pWireFrame->setPolygonMode(GL_LINE);
    pWireFrame->setColor(TRSVec4(1, 1, 1, 1));

    std::shared_ptr<TRSGroup> root = std::make_shared<TRSGroup>();
    root->addChild(pWireFrame);
    root->addChild(pNode);

    viewer->setSecenNode(root);
    viewer->run();
}

//多个开心盒子场景，shader未开启默认摄像机系统
void CaseManyFunnyBoxRotate()
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
    std::shared_ptr<TRSStateSet> pSS = pTemplateNode->getOrCreateStateSet();
    pSS->getTexture()->createTexture("resources/textures/container.jpg");//container.jpg
    pSS->getTexture()->createTexture("resources/textures/awesomeface.png");
    pSS->getShader()->createProgram("shaders/1_5MultiTextureVertex.glsl", "shaders/1_5MultiTextureFragment.glsl");
    pTemplateNode->readFromVertex(BoxVerticesAndColorAndTex, sizeof(BoxVerticesAndColorAndTex) / sizeof(float), EnVertexColorTexture);
    int nBoxCount = sizeof(cubePositions) / sizeof(cubePositions[0]);
    for (int i=0; i<nBoxCount; i++)
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



//第二章 基本光照
void updateFunc(TRSNode* pNode)
{
    TRSVec3 lightColor;
    lightColor[0] = float(sin(glfwGetTime() * 2.0f));
    lightColor[1] = float(sin(glfwGetTime() * 0.7f));
    lightColor[2] = float(sin(glfwGetTime() * 1.3f));

    TRSVec3 diffuseColor = lightColor   * 0.5f; // 降低影响
    TRSVec3 ambientColor = diffuseColor * 0.2f; // 很低的影响
    pNode->getStateSet()->getShader()->addUniform3v("light.ambient", ambientColor);
    pNode->getStateSet()->getShader()->addUniform3v("light.diffuse", diffuseColor);
}

void CaseMaterial_AmbientDiffuseSpecular()
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
void CaseMaterial_DiffuseNormal()
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


void CaseDirectionLight()
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
    pBoxSS->getTexture()->createTexture("resources/textures/container2.jpg", "material.diffuse");//container2.png 加载后图片显示雪花转为jpg。
    pBoxSS->getTexture()->createTexture("resources/textures/container2_specular.jpg", "material.specular");
    //pBoxSS->getShader()->addUniform3v("material.specular", TRSVec3(0.5f, 0.5f, 0.5f));
    pBoxSS->getShader()->addUniformf("material.shininess", 32.0f);
    pBoxSS->getShader()->addUniform3v("light.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pBoxSS->getShader()->addUniform3v("light.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
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


void CasePointAttenuationLight()
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
    pBoxSS->getTexture()->createTexture("resources/textures/container2.jpg", "material.diffuse");//container2.png 加载后图片显示雪花转为jpg。
    pBoxSS->getTexture()->createTexture("resources/textures/container2_specular.jpg", "material.specular");
    //pBoxSS->getShader()->addUniform3v("material.specular", TRSVec3(0.5f, 0.5f, 0.5f));
    pBoxSS->getShader()->addUniformf("material.shininess", 32.0f);
    pBoxSS->getShader()->addUniform3v("light.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pBoxSS->getShader()->addUniform3v("light.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
    pBoxSS->getShader()->addUniform3v("light.specular", TRSVec3(1.0f, 1.0f, 1.0f));
    pBoxSS->getShader()->addUniform3v("light.position", lightPos);
    //对于衰减的点光源的公式中的三个项  光强度 = 1/(Kc + Kl*d + Kq * d*d)
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
void CaseSpotlight()
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
    pBoxSS->getTexture()->createTexture("resources/textures/container2.jpg", "material.diffuse");//container2.png 加载后图片显示雪花转为jpg。
    pBoxSS->getTexture()->createTexture("resources/textures/container2_specular.jpg", "material.specular");
    pBoxSS->getShader()->addUniformf("material.shininess", 32.0f);
    pBoxSS->getShader()->addUniform3v("light.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pBoxSS->getShader()->addUniform3v("light.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
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

void CaseMultiLightSource()
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
    pBoxSS->getTexture()->createTexture("resources/textures/container2.jpg", "material.diffuse");//container2.png 加载后图片显示雪花转为jpg。
    pBoxSS->getTexture()->createTexture("resources/textures/container2_specular.jpg", "material.specular");
    pBoxSS->getShader()->addUniformf("material.shininess", 32.0f);
    //一个平行光
    TRSVec3 lightDirection = TRSVec3(-0.2f, -1.0f, -0.3f);
    pBoxSS->getShader()->addUniform3v("dirLight.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pBoxSS->getShader()->addUniform3v("dirLight.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
    pBoxSS->getShader()->addUniform3v("dirLight.specular", TRSVec3(1.0f, 1.0f, 1.0f));
    pBoxSS->getShader()->addUniform3v("dirLight.direction", lightDirection);
    //四个点光源
    TRSVec3 pointLightPositions[] = {
        TRSVec3(0.7f,  0.2f,  2.0f),
        TRSVec3(2.3f, -3.3f, -4.0f),
        TRSVec3(-4.0f,  2.0f, -12.0f),
        TRSVec3(0.0f,  0.0f, -3.0f)
    };
    int nPointLightNum = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);
    for (int i=0; i<nPointLightNum; i++)
    {
        std::string strLightName = "pointLights[" + std::to_string(i)+"]";
        pBoxSS->getShader()->addUniform3v(strLightName + ".ambient", TRSVec3(0.2f, 0.2f, 0.2f));
        pBoxSS->getShader()->addUniform3v(strLightName + ".diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
        pBoxSS->getShader()->addUniform3v(strLightName + ".specular", TRSVec3(1.0f, 1.0f, 1.0f));
        pBoxSS->getShader()->addUniform3v(strLightName + ".position", pointLightPositions[i]);
        //对于衰减的点光源的公式中的三个项  光强度 = 1/(Kc + Kl*d + Kq * d*d)
        pBoxSS->getShader()->addUniformf(strLightName + ".constant", 1.0f);
        pBoxSS->getShader()->addUniformf(strLightName + ".linear", 0.09f);
        pBoxSS->getShader()->addUniformf(strLightName + ".quadratic", 0.032f);

        //顺便添加灯光的节点
        std::shared_ptr<TRSGeode> pLightNode = std::make_shared<TRSGeode>(*pTemplateNode.get(), false);
        TRSMatrix lightMat;
        lightMat.translate(pointLightPositions[i]);
        lightMat.scale(0.2f);
        pLightNode->setMatrix(lightMat);
        std::shared_ptr<TRSStateSet> pLightStateSet = pLightNode->getOrCreateStateSet();
        pLightStateSet->getShader()->createProgram("shaders/2_1LightNodeVertex.glsl", "shaders/2_1LightNodeFragment.glsl");
        pGroup->addChild(pLightNode);
    }
    // 聚光灯
    pBoxSS->getShader()->addUniform3v("spotLight.ambient", TRSVec3(0.2f, 0.2f, 0.2f));
    pBoxSS->getShader()->addUniform3v("spotLight.diffuse", TRSVec3(0.5f, 0.5f, 0.5f)); // 将光照调暗了一些以搭配场景
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

void CaseElementArrayTRS()
{
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();
    pNode->readFromVertex(Rectangle4Vertices, sizeof(Rectangle4Vertices) / sizeof(float), EnVertex, Rectangle6Indices, sizeof(Rectangle6Indices) / sizeof(unsigned int));
    viewer->setSecenNode(pNode);
    viewer->run();
}

void CaseLoadModelByAssimp()
{
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    TRSAssimpLoader* pLoader = new TRSAssimpLoader();
    //"resources/objects/test.obj"; //std::string("resources/objects/nanosuit/nanosuit.obj");
    std::string strFile = std::string("resources/objects/nanosuit/nanosuit.obj");
    TRSGroup* pGroup = pLoader->loadByAssimp(strFile);
    std::string strDebugInfo = pGroup->debugInfo();
    std::shared_ptr<TRSGroup> pSharedGroup = std::shared_ptr<TRSGroup>(pGroup);
    viewer->setSecenNode(pSharedGroup);
    viewer->run();
}
