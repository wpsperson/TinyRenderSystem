#include "Core\TRSShader.h"
#include "Util\TRSUtils.h"
#include "Core\TRSConst.h"


TRSShader::TRSShader()
    :vShader(0), fShader(0), tescShader(0), teseShader(0)
{

}

TRSShader::~TRSShader()
{
    freeShaderProgram();
}

void TRSShader::createVertexShader(const std::string vShaderFile)
{
    vShader = createShader(vShaderFile, GL_VERTEX_SHADER);
}

void TRSShader::createFragmentShader(const std::string fShaderFile)
{
    fShader = createShader(fShaderFile, GL_FRAGMENT_SHADER);
}

void TRSShader::createTessControlShader(const std::string tescShaderFile)
{
    tescShader = createShader(tescShaderFile, GL_TESS_CONTROL_SHADER);
}

void TRSShader::createTessEvaluateShader(const std::string teseShaderFile)
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

unsigned int TRSShader::createProgram(const std::string vShaderFile, const std::string fShaderFile)
{
    createVertexShader(vShaderFile);
    createFragmentShader(fShaderFile);
    return createProgram();
}

void TRSShader::use()
{
    glUseProgram(program);
}

void TRSShader::addUniformi(const std::string uniformName, int value)
{
    UniformData oData;
    oData.enType = EnInt;
    oData.nValue = value;
    m_mapUniformValue.insert(std::make_pair(uniformName, oData));
}

void TRSShader::addUniformf(const std::string uniformName, float value)
{
    UniformData oData;
    oData.enType = EnFloat;
    oData.fValue = value;
    m_mapUniformValue.insert(std::make_pair(uniformName, oData));
}

void TRSShader::addUniform3v(const std::string uniformName, TRSVec3 vec3Color)
{
    UniformData oData;
    oData.enType = EnVec3;
    oData.vec3Value = vec3Color;
    m_mapUniformValue.insert(std::make_pair(uniformName, oData));
}

void TRSShader::addUniform4v(const std::string uniformName, TRSVec4 vec4Color)
{
    UniformData oData;
    oData.enType = EnVec4;
    oData.vec4Value = vec4Color;
    m_mapUniformValue.insert(std::make_pair(uniformName, oData));
}

void TRSShader::addUniformMatrix4(const std::string& uniformName, TRSMatrix mat)
{
    UniformData oData;
    oData.enType = EnMat4;
    oData.mat4Value = mat;
    m_mapUniformValue.insert(std::make_pair(uniformName, oData));
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

void TRSShader::applayAllStaticUniform()
{
    std::map<std::string, UniformData>::iterator itr = m_mapUniformValue.begin();
    for (; itr!=m_mapUniformValue.end(); itr++)
    {
        std::string strName = itr->first;
        int loc = glGetUniformLocation(program, strName.c_str());
        EnUniformType oType = itr->second.enType;
        UniformData& oData = itr->second;
        switch (oType)
        {
        case EnFloat:
            glUniform1f(loc, oData.fValue);
            break;
        case EnInt:
            glUniform1i(loc, oData.nValue);
            break;
        case EnVec2:
            glUniform2f(loc, oData.vec2Value[0], oData.vec2Value[1]);
            break;
        case EnVec3:
            glUniform3f(loc, oData.vec3Value[0], oData.vec3Value[1], oData.vec3Value[2]);
            break;
        case EnVec4:
            glUniform4f(loc, oData.vec4Value[0], oData.vec4Value[1], oData.vec4Value[2], oData.vec4Value[3]);
            break;
        case EnMat4:
            glUniformMatrix4fv(loc, 1, GL_FALSE, &(oData.mat4Value[0][0]));
            break;
        default:
            break;
        }
    }
    m_mapUniformValue.clear();
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
