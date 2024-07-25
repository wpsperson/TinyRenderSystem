#include "ShadowMapping.h"

#include <iostream>
#include <vector>

#include "glad\glad.h"
#include "glfw\glfw3.h"
#include "BasicUtils.h"
#include "TRS/TRSMatrix.h"

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

const char* kDisplayTextureVert =
"#version 330 core                                                  \n"
"layout (location = 0) in vec3 aPos;                                \n"
"layout (location = 1) in vec2 aUV;                                 \n"
"out vec2 vUV;                                                      \n"
"void main()                                                        \n"
"{                                                                  \n"
"    vUV = aUV;                                                     \n"
"    gl_Position = vec4(aPos, 1.0);                                 \n"
"}                                                                  \n";

const char* kDisplayTextureFrag =
"#version 330 core                              \n"
"out vec4 FragColor;                            \n"
"in vec2 vUV;                                   \n"
"uniform sampler2D ourTexture;                  \n"
"void main()                                    \n"
"{                                              \n"
"    float tex = texture(ourTexture, vUV).r;    \n"
"    FragColor = vec4(tex, tex, tex, 1.0);      \n"
"}                                              \n";

const char* kMainVert =
"#version 330 core                                                  \n"
"layout (location = 0) in vec3 aPos;                                \n"
"out vec4 vFragPos;                                                 \n"
"uniform mat4 ViewProject;                                          \n"
"uniform mat4 LightViewProject;                                     \n"
"                                                                   \n"
"void main()                                                        \n"
"{                                                                  \n"
"    gl_Position = ViewProject * vec4(aPos, 1.0);                   \n"
"    vFragPos =  LightViewProject* vec4(aPos, 1.0);                 \n"
"}                                                                  \n";

const char* kMainFrag =
"#version 330 core                                                  \n"
"out vec4 FragColor;                                                \n"
"in vec4 vFragPos;                                                  \n"
"                                                                   \n"
"uniform vec3 color;                                                \n"
"uniform sampler2D DepthTexture;                                    \n"
"                                                                   \n"
"float isInShadow(vec3 pos)                                         \n"
"{                                                                  \n"
"    vec2 uv = pos.xy;                                              \n"
"    float curDepth = pos.z;                                        \n"
"    float refDepth = texture(DepthTexture, uv).r;                  \n"
"    float shadow = (curDepth > refDepth+0.0001) ? 0.5 : 0.0;       \n"
"    return  shadow;                                                \n"
"}                                                                  \n"
"                                                                   \n"
"void main()                                                        \n"
"{                                                                  \n"
"    vec3 ndc = vFragPos.xyz / vFragPos.w;                          \n"
"    vec3 ndc2 = ndc * 0.5 + 0.5; // range 0~1                      \n"
"    float shadow = isInShadow(ndc2);                               \n"
"    vec3 black = vec3(0,0,0);                                      \n"
"    vec3 result = mix(color, black, shadow);                       \n"
"    FragColor = vec4(result, 1.0);                                 \n"
"}                                                                  \n";


constexpr float kSize = 100.0f;
constexpr float kOccludeSize = 20.0f;

float kTopQuadVertices[] = {
    -kOccludeSize, -kOccludeSize, kOccludeSize,
    kOccludeSize, -kOccludeSize, kOccludeSize,
    kOccludeSize,  kOccludeSize, kOccludeSize,
    kOccludeSize,  kOccludeSize, kOccludeSize,
    -kOccludeSize, kOccludeSize, kOccludeSize,
    -kOccludeSize, -kOccludeSize, kOccludeSize,
};

float kBackGroundQuadVertices[] = {
    -kSize, -kSize, 0.0f,
    kSize, -kSize, 0.0f,
    kSize,  kSize, 0.0f,
    kSize,  kSize, 0.0f,
    -kSize, kSize, 0.0f,
    -kSize, -kSize, 0.0f,
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
        float lightOffset = kOccludeSize * 2.0f;
        TRSVec3 lightPos(-lightOffset, 0.0f, lightOffset);
        TRSVec3 center(0, 0, 0);
        TRSVec3 front = center - lightPos;
        TRSMatrix viewMatrix;
        viewMatrix.makeLookat(lightPos, front, G_YDIR);
        TRSMatrix projectMatrix;
        float xhalf = kSize;
        float near = float(lightOffset * 2 - kSize) * sqrtf(2) * 0.5f;
        float far = float(lightOffset * 2 + kSize) * sqrtf(2) * 0.5f;
        projectMatrix.makeOtho(-xhalf, xhalf, -xhalf, xhalf, near, far);
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
    unsigned int useShadowProgram = createProgramByShaderContent(kDisplayTextureVert, kDisplayTextureFrag);
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

    unsigned int mainProgram = createProgramByShaderContent(kMainVert, kMainFrag);
    TRSMatrix mainViewProject;
    {
        TRSVec3 cameraPos(0.0f, 0.0f, kOccludeSize * 2);
        TRSVec3 center(0, 0, 0);
        TRSVec3 front = center - cameraPos;
        TRSMatrix viewMatrix;
        viewMatrix.makeLookat(cameraPos, front, G_YDIR);
        TRSMatrix projectMatrix;
        float xhalf = kSize * 1.2f;
        float near = 0;
        float far = kSize;
        projectMatrix.makeOtho(-xhalf, xhalf, -xhalf, xhalf, near, far);
        mainViewProject = projectMatrix * viewMatrix;
    }

    int debug = 0;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (debug)
        {
            glUseProgram(useShadowProgram);
            int posUniformTex = glGetUniformLocation(useShadowProgram, "ourTexture");
            glUniform1i(posUniformTex, 0);// 0 mean GL_TEXTURE0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(consumerVAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
        else
        {
            glUseProgram(mainProgram);
            int locVP = glGetUniformLocation(mainProgram, "ViewProject");
            glUniformMatrix4fv(locVP, 1, GL_FALSE, &(mainViewProject[0][0]));
            int locLVP = glGetUniformLocation(mainProgram, "LightViewProject");
            glUniformMatrix4fv(locLVP, 1, GL_FALSE, &(view_project[0][0]));
            int locColor = glGetUniformLocation(mainProgram, "color");
            int locTexture = glGetUniformLocation(mainProgram, "DepthTexture");
            glUniform1i(locTexture, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            glUniform3f(locColor, 0.8f, 0.4f, 0.4f);
            glBindVertexArray(topVAO);//bind VAO
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glUniform3f(locColor, 0.4f, 0.8f, 0.4f);
            glBindVertexArray(backVAO);//bind VAO
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

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
