#include "ShadowMapping.h"

#include <iostream>
#include <vector>

#include "glad\glad.h"
#include "glfw\glfw3.h"
#include "BasicUtils.h"
#include "Math/TRSMatrix.h"

const char* kGenShadowVert =
"#version 330 core                                                  \n"
"layout (location = 0) in vec3 aPos;                                \n"
"                                                                   \n"
"uniform mat4 lightSpaceMatrix;                                     \n"
"                                                                   \n"
"void main()                                                        \n"
"{                                                                  \n"
"    gl_Position = lightSpaceMatrix * vec4(aPos, 1.0);              \n"
"}                                                                  \n";

const char* kGenShadowFrag =
"#version 330 core                          \n"
"void main()                                \n"
"{                                          \n"
"}                                          \n";

const char* kUseShadowVert =
"#version 330 core                                                  \n"
"layout (location = 0) in vec3 aPos;                                \n"
"layout (location = 1) in vec2 aUV;                                 \n"
"out vec2 vUV;                                                      \n"
"void main()                                                        \n"
"{                                                                  \n"
"    vUV = aUV;                                                     \n"
"    gl_Position = vec4(aPos, 1.0);                                 \n"
"}                                                                  \n";

const char* kUseShadowFrag =
"#version 330 core                              \n"
"out vec4 FragColor;                            \n"
"in vec2 vUV;                                   \n"
"uniform sampler2D ourTexture;                  \n"
"void main()                                    \n"
"{                                              \n"
"    float tex = texture(ourTexture, vUV).r;    \n"
"    FragColor = vec4(tex, tex, tex, 1.0);      \n"
"}                                              \n";

constexpr float kSize = 100.0f;
constexpr float kOccludeSize = 20.0f;
constexpr float kHeight = 40.0f;

float kTopQuadVertices[] = {
    -kSize, -kSize, 0.0f,
    kSize, -kSize, 0.0f,
    kSize,  kSize, 0.0f,
    kSize,  kSize, 0.0f,
    -kSize, kSize, 0.0f,
    -kSize, -kSize, 0.0f,
};

float kBackGroundQuadVertices[] = {
    -kOccludeSize, -kOccludeSize, kHeight,
    kOccludeSize, -kOccludeSize, kHeight,
    kOccludeSize,  kOccludeSize, kHeight,
    kOccludeSize,  kOccludeSize, kHeight,
    -kOccludeSize, kOccludeSize, kHeight,
    -kOccludeSize, -kOccludeSize, kHeight,
};
float kDisplayTextureVertices[] = {
    //     ---- vertex ----    - uv -
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // left-bttm
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // right-bttm
    0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // right-top
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f    // left-top
};


int renderShadowMapping()
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // glViewPort
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    unsigned int texture;
    int width = 800;
    int height = 800;
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        //std::vector<float> buffer;
        //buffer.resize(width * height);
        //for (int ii = 0; ii < width * height; ii++)
        //{
        //    buffer[ii] = static_cast<float>(ii) / (width * height);
        //}
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, buffer.data());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    // generate shadow
    unsigned int genShadowProgram = createProgramByShaderContent(kGenShadowVert, kGenShadowFrag);
    TRSMatrix view_project;
    {
        TRSVec3 lightPos(-kOccludeSize * 2.0f, 0.0f, kOccludeSize * 2.0f);
        TRSVec3 center(0, 0, 0);
        TRSVec3 front = center - lightPos;
        TRSMatrix model;
        TRSMatrix viewMatrix;
        viewMatrix.makeLookat(lightPos, front, G_YDIR);
        TRSMatrix projectMatrix;
        float xhalf = kSize;
        projectMatrix.makeOtho(-xhalf, xhalf, -xhalf, xhalf, 0, kSize);
        view_project = projectMatrix * viewMatrix;
    }
    unsigned int topVAO;
    unsigned int backVAO;
    {
        unsigned int VBO;
        glGenVertexArrays(1, &topVAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(topVAO);//bind VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(kTopQuadVertices), kTopQuadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);//unbind VAO
    }
    {
        unsigned int VBO;
        glGenVertexArrays(1, &backVAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(backVAO);//bind VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(kBackGroundQuadVertices), kBackGroundQuadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);//unbind VAO
    }

    glViewport(0, 0, width, height);
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(genShadowProgram);
    int matrixLoc = glGetUniformLocation(genShadowProgram, "lightSpaceMatrix");
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &(view_project[0][0]));
    glBindVertexArray(topVAO);//bind VAO
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(backVAO);//bind VAO
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // now display the texture.
    glViewport(0, 0, DefaultWindowWidth, DefaultWindowHeight);
    unsigned int useShadowProgram = createProgramByShaderContent(kUseShadowVert, kUseShadowFrag);
    unsigned int consumerVAO;
    {
        unsigned int VBO;
        glGenVertexArrays(1, &consumerVAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(consumerVAO);//bind VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(kDisplayTextureVertices), kDisplayTextureVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);//unbind VAO
    }

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(useShadowProgram);
        int posUniformTex = glGetUniformLocation(useShadowProgram, "ourTexture");
        glUniform1i(posUniformTex, 0);// 0 mean GL_TEXTURE0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(consumerVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void setupScene()
{
}

void renderScene()
{
}
