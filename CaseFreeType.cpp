#include "CaseFreeType.h"
#include <iostream>
#include <map>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "stb_image.h"
#include "TRSUtils.h"
#include "TRSResource.h"
#include "TRSWindowConfig.h"
#include "TRSCamera.h"
#include "TRSCallBackFunc.h"
#include "TRSConst.h"

#include <ft2build.h>
#include FT_FREETYPE_H

int BITMAPSIZE = 50;

struct ivec2
{
    ivec2() {}
    ivec2(int _x, int _y) :x(_x), y(_y) {}
    int x;
    int y;
};

struct Character
{
    GLuint      TextureID;
    ivec2       Size;       // glyph width / glyph height
    ivec2       Bearing;    // bearingX(distance between origin to left border of glyph) / bearingY (distance between origin to top border of glyph)
    GLuint      Advance;    // current glyph origin to next glyph origin distance * 64
};




void renderText(const std::map<GLchar, Character>& mapChars, int VBO, const std::string& strText, TRSVec3 pos, TRSVec3 right, TRSVec3 up, float textHeightInWorld)
{
    TRSVec3 curPos = pos;
    for (auto itr = strText.begin(); itr != strText.end(); itr++)
    {
        if (*itr == ' ')
        {
            curPos += right * textHeightInWorld*0.5;
            continue;
        }
        const Character c = mapChars.at(*itr);
        float scale = textHeightInWorld / BITMAPSIZE;
        float advanceInWorld = (c.Advance >> 6) * scale;
        float rightOffset = c.Bearing.x * scale;
        float upOffset = -(c.Size.y - c.Bearing.y) * scale;
        // four point in quad
        TRSVec3 leftBtm = curPos + right * rightOffset + up * upOffset;
        TRSVec3 leftTop = leftBtm + up*(c.Size.y *scale);
        TRSVec3 rightBtm = leftBtm + right * (c.Size.x * scale);
        TRSVec3 rightTop = leftBtm + right * (c.Size.x * scale) + up*(c.Size.y *scale);
        float arr[6][5]=
        {
            { leftBtm[0], leftBtm[1], leftBtm[2], 0.0f, 1.0f },
            { rightBtm[0], rightBtm[1], rightBtm[2], 1.0f, 1.0f },
            { leftTop[0], leftTop[1], leftTop[2], 0.0f, 0.0f },

            { leftTop[0], leftTop[1], leftTop[2], 0.0f, 0.0f },
            { rightBtm[0], rightBtm[1], rightBtm[2], 1.0f, 1.0f },
            { rightTop[0], rightTop[1], rightTop[2], 1.0f, 0.0f },
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6 * 5, arr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, c.TextureID);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        curPos += right * advanceInWorld;
    }
}

std::map<GLchar, Character> loadFreeTypeCharacters(unsigned int BitMapWeidth, unsigned BitMapHeight)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    std::string strArialFont = std::getenv("SystemRoot") + std::string("/Fonts/simfang.ttf");
    FT_Face face;
    if (FT_New_Face(ft, strArialFont.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    if (FT_Select_Charmap(face, ft_encoding_unicode))
        std::cout << "ERROR::FT_Select_Charmap" << std::endl;

    FT_Set_Pixel_Sizes(face, BitMapWeidth, BitMapHeight);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // code snippet for unicode characters
    //wchar_t I = L'我';
    //int index = FT_Get_Char_Index(face, I);
    //FT_Load_Glyph(face, index, FT_LOAD_RENDER);

    std::map<GLchar, Character> Characters;
    for (GLubyte c = 0; c < 128; c++)
    {
        // 加载字符的字形 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // 生成纹理
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
            face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        // 设置纹理选项
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 储存字符供之后使用
        Character character = {
            texture,
            ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return Characters;
}

int CaseFreeType()
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
    char* vertexShaderSource = readTextFile("shaders/FontsVertex.glsl");
    char* fragmentShaderSource = readTextFile("shaders/FontsFragment .glsl");
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*5, nullptr, GL_DYNAMIC_DRAW); // we allocate memory for a quad for single character (six vertex, 3 point coord+2 texture coord)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);//unbind VAO


    std::map<GLchar, Character> mapCharacter = loadFreeTypeCharacters(0, BITMAPSIZE);
    Character character = mapCharacter['A'];

    int nPosModelMatrix = glGetUniformLocation(shaderProgram, "model");
    int nPosViewMatrix = glGetUniformLocation(shaderProgram, "view");
    int nPosProjectMatrix = glGetUniformLocation(shaderProgram, "projection");
    int nColor = glGetUniformLocation(shaderProgram, "uColor");
    TRSMatrix IdentityMatrix;
    glUniformMatrix4fv(nPosModelMatrix, 1, GL_FALSE, &(IdentityMatrix[0][0]));
    glUniformMatrix4fv(nPosViewMatrix, 1, GL_FALSE, &(IdentityMatrix[0][0]));
    glUniformMatrix4fv(nPosProjectMatrix, 1, GL_FALSE, &(IdentityMatrix[0][0]));
    glUniform3f(nColor, 0.5f, 0.9f, 0.5f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);//这句可以不写，因为默认Texture0总是被激活。
        renderText(mapCharacter, VBO, "Hello, FreeType", G_ORIGIN, G_XDIR, G_YDIR, 0.1);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}
