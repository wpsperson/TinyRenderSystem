#pragma once

#include <string>

#include "TRS/TRSExport.h"
#include "TRS/TRSVector.h"
#include "TRS/TRSMatrix.h"

class TRS_EXPORT TRSShader
{
public:
    TRSShader();
    ~TRSShader();

    void createVertexShader(const char* vShaderFile);

    void createFragmentShader(const char* fShaderFile);

    void createTessControlShader(const char* tescShaderFile);

    void createTessEvaluateShader(const char* teseShaderFile);

    unsigned int createProgram();

    unsigned int createProgram(const char* vShaderFile, const char* fShaderFile);

    void use();

    void addUniformi(const char* uniformName, int value);

    void addUniformf(const char* uniformName, float value);

    void addUniform3v(const char* uniformName, TRSVec3 vec3Color);

    void addUniform4v(const char* uniformName, TRSVec4 vec4Color);

    void addUniformMatrix4(const char* uniformName, TRSMatrix mat);

    unsigned int getProgramId();

    std::string debugInfo();

private:
    //return 0 when failure
    unsigned int createShader(const char* vShaderFile, unsigned int EnShaderType);

    void freeShaderProgram();

public:
    unsigned int program;
    unsigned int vShader;
    unsigned int fShader;
    unsigned int tescShader;
    unsigned int teseShader;
};

