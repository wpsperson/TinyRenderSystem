#pragma once

#include <string>

#include "TRS/TRSExport.h"
#include "TRS/TRSDefEnum.h"
#include "TRS/TRSVector.h"
#include "TRS/TRSMatrix.h"

class TRS_EXPORT TRSShader
{
public:
    TRSShader();
    ~TRSShader();

    void setType(ShaderType type);

    void createVertexShader(const char* vShaderFile);

    void createFragmentShader(const char* fShaderFile);

    void createTessControlShader(const char* tescShaderFile);

    void createTessEvaluateShader(const char* teseShaderFile);

    unsigned int createProgram();

    unsigned int createProgram(const char* vShaderFile, const char* fShaderFile);

    void use();

    void setUniformi(const char* uniformName, int value);

    void setUniformf(const char* uniformName, float value);

    void setUniform3v(const char* uniformName, TRSVec3 vec3Color);

    void setUniform4v(const char* uniformName, TRSVec4 vec4Color);

    void setUniformMatrix4(const char* uniformName, TRSMatrix mat);

    unsigned int getProgramId();

    std::string debugInfo();

private:
    //return 0 when failure
    unsigned int createShader(const char* vShaderFile, unsigned int EnShaderType);

    void freeShaderProgram();

public:
    ShaderType m_type = ShaderType::None;
    unsigned int program;
    unsigned int vShader;
    unsigned int fShader;
    unsigned int tescShader;
    unsigned int teseShader;
};

