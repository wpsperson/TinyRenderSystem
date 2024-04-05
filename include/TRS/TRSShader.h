/*!  
 *@brief    渲染系统的着色器
 *@author   wangps
 *@date     2019年5月3日
 */

#pragma once
#include <string>
#include <vector>
#include "TRS/TRSExport.h"
#include "TRS/TRSVector.h"
#include "TRS/TRSMatrix.h"
#include <map>

enum EnUniformType
{
    EnFloat = 0,
    EnInt,
    EnVec2,
    EnVec3,
    EnVec4,
    EnMat4
};

struct UniformData
{
    EnUniformType enType;
    union
    {
        float fValue;
        int  nValue;
        TRSVec2 vec2Value;
        TRSVec3 vec3Value;
        TRSVec4 vec4Value;
        TRSMatrix mat4Value;
    };
    UniformData()
        :enType(EnFloat), fValue(0.0f)
    {

    }
    UniformData(const UniformData& ref)
    {
        enType = ref.enType;
        switch (enType)
        {
        case EnFloat:
            fValue = ref.fValue;
            break;
        case EnInt:
            nValue = ref.nValue;
            break;
        case EnVec2:
            vec2Value = ref.vec2Value;
            break;
        case EnVec3:
            vec3Value = ref.vec3Value;
            break;
        case EnVec4:
            vec4Value = ref.vec4Value;
            break;
        case EnMat4:
            mat4Value = ref.mat4Value;
            break;
        default:
            break;
        }
    }
    ~UniformData()
    {
    }
};

class TRS_EXPORT TRSShader
{
public:
    TRSShader();
    ~TRSShader();

    void createVertexShader(const std::string vShaderFile);

    void createFragmentShader(const std::string fShaderFile);

    void createTessControlShader(const std::string tescShaderFile);

    void createTessEvaluateShader(const std::string teseShaderFile);

    unsigned int createProgram();

    unsigned int createProgram(const std::string vShaderFile, const std::string fShaderFile);

    void use();

    void addUniformi(const std::string uniformName, int value);
    void addUniformf(const std::string uniformName, float value);
    void addUniform3v(const std::string uniformName, TRSVec3 vec3Color);
    void addUniform4v(const std::string uniformName, TRSVec4 vec4Color);
    void addUniformMatrix4(const std::string& uniformName, TRSMatrix mat);
    void applayAllStaticUniform();

    unsigned int getProgramId();
    std::string debugInfo();

private:
    //return 0 when failure
    unsigned int createShader(const std::string vShaderFile, unsigned int EnShaderType);

    void freeShaderProgram();

public:
    unsigned int program;
    unsigned int vShader;
    unsigned int fShader;
    unsigned int tescShader;
    unsigned int teseShader;
    std::map<std::string, UniformData> m_mapUniformValue;
};

