#include "RenderTriangle.h"

#include <iostream>

#include "glad\glad.h"
#include "glfw\glfw3.h"
#include "BasicUtils.h"

static const char * const VertexSource =
"#version 330 core                                           \n"
"layout(location = 0) in vec3 aPos;                          \n"
"layout(location = 1) in vec3 aColor;                        \n"
"out vec3 vColor;                                            \n"
"void main()                                                 \n"
"{                                                           \n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);        \n"
"    vColor = aColor;                                        \n"
"}                                                           \n";

static const char* const FragmentSource =
"#version 460 core                      \n"
"out vec4 FragColor;                    \n"
"in vec3 vColor;                        \n"
"void main()                            \n"
"{                                      \n"
"    FragColor = vec4(vColor, 1.0);     \n"
"}                                      \n";


static float g_VerticesColors[] = {
    -0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,         0.0f, 1.0f,0.0f,
    0.0f,  0.5f, 0.0f,        0.0f, 0.0f, 1.0f,
};

static float g_Vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f,
};
static float g_Colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f,0.0f,
    0.0f, 0.0f, 1.0f,
};


static float g_Vertices_Append_Colors[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f,

    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f,0.0f,
    0.0f, 0.0f, 1.0f,
};

int RenderColorTriangle()
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

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &VertexSource, NULL);
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
    glShaderSource(fragmentShader, 1, &FragmentSource, NULL);
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
    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);//bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // initialize, use glBufferStorage instead of glBufferData
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(g_VerticesColors), g_VerticesColors, GL_MAP_WRITE_BIT);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_triangleVertices), g_triangleVertices, GL_STATIC_DRAW);



    bool old = false;
    bool InterleaveAttribute = false;
    bool SeperateAttribute0 = false; // use seperate buffer.
    bool SeperateAttribute1 = false; // use single buffer but seperate location
    // void glVertexArrayAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
    // void glVertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
    // void glVertexArrayAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);

    SeperateAttribute1 = true;

    if (old)
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    else if (InterleaveAttribute)
    {
        unsigned int binding0 = 0;
        glVertexArrayVertexBuffer(VAO, binding0, VBO, 0, 6 * sizeof(float));

        glVertexArrayAttribBinding(VAO, 0, binding0);
        glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexAttribArray(0);

        glVertexArrayAttribBinding(VAO, 1, binding0);
        glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
        glEnableVertexAttribArray(1);
    }
    else if (SeperateAttribute0)
    {
        unsigned int VBO0 = 0;
        unsigned int VBO1 = 0;
        glGenBuffers(1, &VBO0);
        glGenBuffers(1, &VBO1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO0);
        glBufferStorage(GL_ARRAY_BUFFER, sizeof(g_Vertices), g_Vertices, GL_MAP_WRITE_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, VBO1);
        glBufferStorage(GL_ARRAY_BUFFER, sizeof(g_Colors), g_Colors, GL_MAP_WRITE_BIT);
        unsigned int binding0 = 0;
        unsigned int binding1 = 1;
        glVertexArrayVertexBuffer(VAO, binding0, VBO0, 0, 3 * sizeof(float));
        glVertexArrayVertexBuffer(VAO, binding1, VBO1, 0, 3 * sizeof(float));

        glVertexArrayAttribBinding(VAO, 0, binding0);
        glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexAttribArray(0);

        glVertexArrayAttribBinding(VAO, 1, binding1);
        glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexAttribArray(1);
    }
    else if (SeperateAttribute1)
    {
        unsigned int SeperateVbo = 0;
        glGenBuffers(1, &SeperateVbo);
        glBindBuffer(GL_ARRAY_BUFFER, SeperateVbo);
        glBufferStorage(GL_ARRAY_BUFFER, sizeof(g_Vertices_Append_Colors), g_Vertices_Append_Colors, GL_MAP_WRITE_BIT);

        int buffer_offset = sizeof(g_Vertices_Append_Colors) / 2;
        unsigned int binding0 = 0;
        unsigned int binding1 = 1;
        glVertexArrayVertexBuffer(VAO, binding0, SeperateVbo, 0,                3 * sizeof(float));
        glVertexArrayVertexBuffer(VAO, binding1, SeperateVbo, buffer_offset,    3 * sizeof(float));

        glVertexArrayAttribBinding(VAO, 0, binding0);
        glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexAttribArray(0);

        glVertexArrayAttribBinding(VAO, 1, binding1);
        glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexAttribArray(1);
    }

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


