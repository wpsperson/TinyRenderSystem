#include "TRSShader.h"
#include "TRSUtils.h"


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
    if (delShader)//�ͷ�Shader�ڴ�
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

void TRSShader::setUniformi(const std::string uniformName, int value)
{
    int loc = glGetUniformLocation(program, uniformName.c_str());
    glUniform1i(loc, value);
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
    delete[] source;//�ͷ��ڴ�
    return shader;
}