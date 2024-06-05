#include "TRS/TRSShader.h"

#include <iostream>
#include <string>

#include "TRS/TRSDefGL.h"
#include "TRS/TRSUtils.h"


TRSShader::TRSShader()
    :vShader(0), fShader(0), tescShader(0), teseShader(0)
{

}

TRSShader::~TRSShader()
{
    freeShaderProgram();
}

void TRSShader::createVertexShader(const char* vShaderFile)
{
    vShader = createShader(vShaderFile, GL_VERTEX_SHADER);
}

void TRSShader::createFragmentShader(const char* fShaderFile)
{
    fShader = createShader(fShaderFile, GL_FRAGMENT_SHADER);
}

void TRSShader::createTessControlShader(const char* tescShaderFile)
{
    tescShader = createShader(tescShaderFile, GL_TESS_CONTROL_SHADER);
}

void TRSShader::createTessEvaluateShader(const char* teseShaderFile)
{
    teseShader = createShader(teseShaderFile, GL_TESS_EVALUATION_SHADER);
}

unsigned int TRSShader::createProgram()
{
    if (!vShader || !fShader)
    {
        return 0;
    }
    program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    if (tescShader && teseShader)
    {
        glAttachShader(program, tescShader);
        glAttachShader(program, teseShader);
    }
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
    return program;
}

unsigned int TRSShader::createProgram(const char* vShaderFile, const char* fShaderFile)
{
    createVertexShader(vShaderFile);
    createFragmentShader(fShaderFile);
    return createProgram();
}

void TRSShader::use()
{
    glUseProgram(program);
}

void TRSShader::addUniformi(const char* uniformName, int value)
{
    int loc = glGetUniformLocation(program, uniformName);
    if (loc >= 0)
    {
        glUniform1i(loc, value);
    }
}

void TRSShader::addUniformf(const char* uniformName, float value)
{
    int loc = glGetUniformLocation(program, uniformName);
    if (loc >= 0)
    {
        glUniform1f(loc, value);
    }
}

void TRSShader::addUniform3v(const char* uniformName, TRSVec3 vec3Color)
{
    int loc = glGetUniformLocation(program, uniformName);
    if (loc >= 0)
    {
        glUniform3f(loc, vec3Color[0], vec3Color[1], vec3Color[2]);
    }
}

void TRSShader::addUniform4v(const char* uniformName, TRSVec4 vec4Color)
{
    int loc = glGetUniformLocation(program, uniformName);
    if (loc >= 0)
    {
        glUniform4f(loc, vec4Color[0], vec4Color[1], vec4Color[2], vec4Color[3]);
    }
}

void TRSShader::addUniformMatrix4(const char* uniformName, TRSMatrix mat)
{
    int loc = glGetUniformLocation(program, uniformName);
    if (loc >= 0)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &(mat[0][0]));
    }
}

unsigned int TRSShader::createShader(const char* vShaderFile, unsigned int EnShaderType)
{
    unsigned int shader = glCreateShader(EnShaderType);
    const char* source = readTextContent(vShaderFile);
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
    delete[] source;
    return shader;
}

void TRSShader::freeShaderProgram()
{
    if (vShader)
    {
        glDetachShader(program, vShader);
        glDeleteShader(vShader);
        vShader = 0;
    }
    if (fShader)
    {
        glDetachShader(program, fShader);
        glDeleteShader(fShader);
        fShader = 0;
    }
    if (tescShader)
    {
        glDetachShader(program, tescShader);
        glDeleteShader(tescShader);
        tescShader = 0;
    }
    if (teseShader)
    {
        glDetachShader(program, teseShader);
        glDeleteShader(teseShader);
        teseShader = 0;
    }
    glDeleteProgram(program);
}

unsigned int TRSShader::getProgramId()
{
    return program;
}

std::string TRSShader::debugInfo()
{
    std::string strDebugInfo = "shader program: " + std::to_string(program) + ", vs: " + std::to_string(vShader) + ", fs: " + std::to_string(fShader);
    return strDebugInfo;
}
