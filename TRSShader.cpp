#include "TRSShader.h"
#include "TRSUtils.h"
#include "TRSConst.h"


TRSShader::TRSShader()
    :vShader(0), fShader(0)
{

}

TRSShader::~TRSShader()
{

}

void TRSShader::createVertexShader(const std::string vShaderFile)
{
    vShader = createShader(vShaderFile, GL_VERTEX_SHADER);
}

void TRSShader::createFragmentShader(const std::string fShaderFile)
{
    fShader = createShader(fShaderFile, GL_FRAGMENT_SHADER);
}

unsigned int TRSShader::createProgram(bool delShader)
{
    if (!vShader || !fShader)
    {
        return 0;
    }
    program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    int ret;
    glGetProgramiv(program, GL_LINK_STATUS, &ret);
    if (!ret)
    {
        char info[256];
        glGetProgramInfoLog(program, 256, nullptr, info);
        std::cout << "ERROR::PROGRAM::LINK: " << info << std::endl;
        return 0;
    }
    if (delShader)//释放Shader内存
    {
        glDeleteShader(vShader);
        glDeleteShader(fShader);
    }
    return program;
}

unsigned int TRSShader::createProgram(const std::string vShaderFile, const std::string fShaderFile, bool delShader/* = true*/)
{
    createVertexShader(vShaderFile);
    createFragmentShader(fShaderFile);
    return createProgram(delShader);
}

void TRSShader::use()
{
    glUseProgram(program);
}

void TRSShader::setTexUniform(int nTexCount)
{
    for (int i=0; i<nTexCount; i++)
    {
        std::string strUniformName = std::string(s_TextUnitPrefix) + std::to_string(i);
        setUniformi(strUniformName, i);
    }
}

unsigned int TRSShader::createShader(const std::string vShaderFile, GLenum EnShaderType)
{
    unsigned int shader = glCreateShader(EnShaderType);
    const char* source = readTextFile(vShaderFile);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int ret;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ret);
    if (!ret)
    {
        char info[256];
        glGetShaderInfoLog(shader, 256, nullptr, info);
        std::cout << "ERROR::SHADER::COMPILE: " << info << std::endl;
        return 0;
    }
    delete[] source;//释放内存
    return shader;
}

void TRSShader::setUniformi(const std::string uniformName, int value)
{
    int loc = glGetUniformLocation(program, uniformName.c_str());
    glUniform1i(loc, value);
}

void TRSShader::setUniform4v(const std::string uniformName, glm::vec4 vec4Color)
{
    int loc = glGetUniformLocation(program, uniformName.c_str());
    glUniform4f(loc, vec4Color.r, vec4Color.g, vec4Color.b, vec4Color.a);
}

void TRSShader::setUniformMatrix4(const std::string& uniformName, glm::mat4 mat)
{
    int loc = glGetUniformLocation(program, uniformName.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}