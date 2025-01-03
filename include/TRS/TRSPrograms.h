#pragma once

#include <unordered_map>

#include "TRS/TRSExport.h"
#include "TRS/TRSDefEnum.h"


class TRSShader;
class TRSGeode;

struct ShaderConfig
{
public:
    // mesh information
    bool hasNormal = true;
    bool hasUV = false;
    bool hasColor = false;

    // material information
    int textureCount = 0;
    bool textureMask = false;

    // light information
    // int lightCount = 1;
    //LightType lighttype = LightType::PointLight;
    //bool hasSpecular = true;
    //double ambientFactor = 0.2;
    //double diffuseFactor = 0.5;
    //double specularFactor = 0.3;
    bool operator==(const ShaderConfig& rhs)const;
};

class ShaderConfigHasher
{
public:
    std::size_t operator()(const ShaderConfig& ct) const;
};


class TRS_EXPORT TRSPrograms
{
public:
    TRSPrograms();

    ~TRSPrograms();

    TRSShader* find2Shader(TRSGeode* geode, RenderMode mode);

    TRSShader* getShader(const ShaderConfig& config);

    static ShaderConfig toConfig(TRSGeode* geode, RenderMode mode);

    void useProgram(ShaderType type);

    void endProgram();

private:
    TRSShader* getOrCreateShader(ShaderType type);

    void generateGLSL(const ShaderConfig& config, std::string& vertexSource, std::string& fragmentSource);

    std::string replaceParameter(const std::string& expression, const std::string& src, const std::string& dest);

private:
    std::unordered_map<ShaderType, TRSShader*> m_shaders;
    std::unordered_map<ShaderConfig, TRSShader*, ShaderConfigHasher> m_newShaders;
    TRSShader* m_active = nullptr;
};
