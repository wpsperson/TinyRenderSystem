#include "TRS/TRSPrograms.h"

#include "TRS/TRSDefGL.h"
#include "TRS/TRSShader.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSMesh.h"
#include "TRS/TRSTexture.h"
#include "TRS/TRSGLSL.h"

using namespace GLSL;


bool ShaderConfig::operator==(const ShaderConfig& rhs) const
{
    return (this->hasNormal == rhs.hasNormal) && (this->hasUV == rhs.hasUV)
        && (this->hasColor == rhs.hasColor) && (this->textureCount == rhs.textureCount)
        && (this->textureMask == rhs.textureMask);
}

std::size_t ShaderConfigHasher::operator()(const ShaderConfig& sc) const
{
    std::size_t result = 0;
    result += sc.textureCount;

    int base = 10;
    if (sc.textureMask)
    {
        result += base;
    }
    base *= 10;
    if (sc.hasColor)
    {
        result += base;
    }
    base *= 10;
    if (sc.hasUV)
    {
        result += base;
    }
    base *= 10;
    if (sc.hasNormal)
    {
        result += base;
    }
    base *= 10;
    return result;
}


TRSPrograms::TRSPrograms()
{
}

TRSPrograms::~TRSPrograms()
{
}

TRSShader* TRSPrograms::find2Shader(TRSGeode* geode, RenderMode mode)
{
    TRSMesh* mesh = geode->getComponentMesh(mode);
    int meshStruct = mesh->getMeshStruct();
    bool hasNormal = (meshStruct & msNormal);
    bool hasColor = (meshStruct & msColor);
    bool hasUV = (meshStruct & msUV);

    TRSShader* shader = nullptr;
    if (RenderMode::Shaded == mode)
    {
        TRSTexture* texture = geode->getTexture();
        if (hasUV && texture && texture->count() == 2)
        {
            shader = getOrCreateShader(ShaderType::DualTexture);
        }
        else if (hasUV && texture && hasNormal)
        {
            shader = getOrCreateShader(ShaderType::PhongTexture);
        }
        else if (hasColor && hasNormal)
        {
            shader = getOrCreateShader(ShaderType::PhongColor);
        }
        else if (hasNormal)
        {
            shader = getOrCreateShader(ShaderType::Phong);
        }
        else if (hasUV)
        {
            shader = getOrCreateShader(ShaderType::FontShader);
        }
        else
        {
            shader = getOrCreateShader(ShaderType::Default);
        }
    }
    else
    {
        if (hasNormal)
        {
            shader = getOrCreateShader(ShaderType::Phong);
        }
        else
        {
            shader = getOrCreateShader(ShaderType::Default);
        }
    }
    return shader;
}

TRSShader* TRSPrograms::getShader(const ShaderConfig &config)
{
    if (m_newShaders.contains(config))
    {
        return m_newShaders.at(config);
    }

    // create new shader for this config
    TRSShader* shader = new TRSShader;
    std::string vertexSource, fragmentSource;
    generateGLSL(config, vertexSource, fragmentSource);
    const char* vSource = vertexSource.c_str();
    const char* fSource = fragmentSource.c_str();
    shader->createProgramBySource(vSource, fSource);
    m_newShaders.insert(std::make_pair(config, shader));
    return shader;
}

ShaderConfig TRSPrograms::toConfig(TRSGeode* geode, RenderMode mode)
{
    TRSMesh* mesh = geode->getComponentMesh(mode);
    int meshStruct = mesh->getMeshStruct();
    bool isFont = (NodeType::ntTextNode == geode->nodeType()) 
        || (NodeType::ntDynamicText == geode->nodeType());
    TRSTexture* texture = geode->getTexture();
    ShaderConfig config;
    config.hasNormal = (meshStruct & msNormal);
    config.hasColor = (meshStruct & msColor);
    config.hasUV = (meshStruct & msUV);
    config.textureCount = 0;
    if (texture)
    {
        config.textureCount = texture->count();
    }
    else if (isFont)
    {
        config.textureCount = 1; // specially, font has default texture.
    }
    config.textureMask = config.hasUV && isFont;
    return config;
}

void TRSPrograms::useProgram(ShaderType type)
{
    TRSShader* shader = getOrCreateShader(type);
    shader->use();
    m_active = shader;
}

void TRSPrograms::endProgram()
{
    glUseProgram(0);
}

TRSShader* TRSPrograms::getOrCreateShader(ShaderType type)
{
    if (m_shaders.count(type))
    {
        return m_shaders.at(type);
    }

    TRSShader* shader = new TRSShader;
    if (ShaderType::Default == type)
    {
        shader->createProgramByFiles("shaders/DefaultVertex.glsl", "shaders/DefaultFragment.glsl");
    }
    else if (ShaderType::Phong == type)
    {
        shader->createProgramByFiles("shaders/PhongVertex.glsl", "shaders/PhongFragment.glsl");
    }
    else if (ShaderType::PhongTexture == type)
    {
        shader->createProgramByFiles("shaders/PosNormTexVertex.glsl", "shaders/PosNormTexFragment.glsl");
    }
    else if (ShaderType::PhongColor == type)
    {
        shader->createProgramByFiles("shaders/PhongColorVertex.glsl", "shaders/PhongColorFragment.glsl");
    }
    else if (ShaderType::DualTexture == type)
    {
        shader->createProgramByFiles("shaders/PosColorTexMVPVertex.glsl", "shaders/MultiTextureFragment.glsl");
    }
    else if (ShaderType::FontShader == type)
    {
        shader->createProgramByFiles("shaders/FontsVertex.glsl", "shaders/FontsFragment .glsl");
    }
    shader->setType(type);
    m_shaders.insert(std::make_pair(type, shader));
    return shader;
}

void TRSPrograms::generateGLSL(const ShaderConfig& config, std::string& vertexSource, std::string& fragmentSource)
{
    bool hasNormal = config.hasNormal;
    bool hasUV = config.hasUV;
    bool hasColor = config.hasColor;
    bool hasMatrix = true;
    bool haslight = hasNormal;
    bool textureMask = config.textureMask;
    int textureCount = config.textureCount;

    vertexSource += header_version + endline;
    vertexSource += define_attribute_pos + endline;
    if (hasNormal)
    {
        vertexSource += define_attribute_normal + endline;
    }
    if (hasUV)
    {
        vertexSource += define_attribute_uv + endline;
    }
    if (hasColor)
    {
        vertexSource += define_attribute_color + endline;
    }
    if (hasNormal)
    {
        vertexSource += out_normal + endline;
        vertexSource += out_fragpos + endline;
    }
    if (hasUV)
    {
        vertexSource += out_uv + endline;
    }
    if (hasColor)
    {
        vertexSource += out_color + endline;
    }
    if (hasMatrix)
    {
        vertexSource += def_uniform_model + endline;
        vertexSource += def_uniform_viewproj + endline;
    }

    vertexSource += main_begin;
    vertexSource += (hasMatrix ? calc_gl_position2 : calc_gl_position1) + endline;
    if (hasNormal)
    {
        vertexSource += calc_normal + endline;
        vertexSource += calc_fragpos +endline;
    }
    if (hasUV)
    {
        vertexSource += calc_uv +endline;
    }
    if (hasColor)
    {
        vertexSource += calc_color +endline;
    }
    vertexSource += main_end;


    // fragment source
    fragmentSource += header_version + endline;
    fragmentSource += out_fragcolor + endline;
    if (hasNormal)
    {
        fragmentSource += in_normal + endline;
        fragmentSource += in_fragpos + endline;
    }
    if (hasUV)
    {
        fragmentSource += in_uv +endline;
    }
    if (hasColor)
    {
        fragmentSource += in_color +endline;
    }
    else // if color attribute doesn't exist, we usually use baseColor uniform.
    {
        fragmentSource += def_uniform_basecolor + endline;
    }
    if (hasNormal)
    {
        fragmentSource += def_uniform_viewpos + endline;
        fragmentSource += def_uniform_lightpos + endline;
    }
    for (int idx = 0; idx < textureCount; idx++)
    {
        std::string strIndex = std::to_string(idx);
        std::string def_uniform_texture_idx = replaceParameter(def_uniform_texture, tokenIndex, strIndex);
        fragmentSource += def_uniform_texture_idx + endline;
    }

    fragmentSource += main_begin;
    if (textureMask)
    {
        fragmentSource += calc_fragcolor4 + endline;
    }
    else if (textureCount > 0)
    {
        if (textureCount == 1)
        {
            fragmentSource += calc_fragcolor3 + endline;
        }
        else
        {
            fragmentSource += calc_texture0 + endline;
            fragmentSource += calc_texture1 + endline;
            fragmentSource += calc_special_alpha + endline;
            fragmentSource += calc_fragcolor3_mix + endline;
        }
    }
    else if (haslight)
    {
        std::string destColor = hasColor ? "Color" : "baseColor";
        std::string ambient_express = replaceParameter(calc_ambient, tokenColor, destColor);
        std::string diffuse_part4 = replaceParameter(calc_diffuse_part4, tokenColor, destColor);
        fragmentSource += ambient_express + endline;
        fragmentSource += calc_diffuse_part1 + endline;
        fragmentSource += calc_diffuse_part2 + endline;
        fragmentSource += calc_diffuse_part3 + endline;
        fragmentSource += diffuse_part4 + endline;
        fragmentSource += calc_specular_part1 + endline;
        fragmentSource += calc_specular_part2 + endline;
        fragmentSource += calc_specular_part3 + endline;
        fragmentSource += calc_specular_part4 + endline;
        fragmentSource += calc_fragcolor2 + endline;
    }
    else
    {
        fragmentSource += calc_fragcolor1 + endline;
    }
    fragmentSource += main_end;
}

std::string TRSPrograms::replaceParameter(const std::string& expression, const std::string& src, const std::string& dest)
{
    std::string result = expression;
    std::size_t pos = result.find(src);
    if (pos >= 0)
    {
        result.replace(pos, src.length(), dest);
    }
    return result;
}
