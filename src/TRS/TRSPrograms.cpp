#include "TRS/TRSPrograms.h"

#include "TRS/TRSDefGL.h"
#include "TRS/TRSShader.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSMesh.h"
#include "TRS/TRSTexture.h"
#include "TRS/TRSGLSL.h"

using namespace GLSL;

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

void TRSPrograms::generateGLSL(int meshStruct, std::string& vertexSource, std::string& fragmentSource)
{
    bool hasNormal = (meshStruct & msNormal);
    bool hasUV = (meshStruct & msUV);
    bool hasColor = (meshStruct & msColor);
    bool hasMatrix = true;
    bool hasSampler = false;
    bool hasBaseColor = true;
    bool haslight = hasNormal;
    bool isFont = false;
    //bool hasSpecular = true;
    //double ambientFactor = 0.2;
    //double diffuseFactor = 0.5;
    //double specularFactor = 0.3;

    vertexSource += header_version + endline;
    vertexSource += define_attribute_pos + endline;
    if (hasNormal)
    {
        vertexSource += define_attribute_normal + endline;
        vertexSource += out_normal + endline;
        vertexSource += out_fragpos + endline;
    }
    if (hasUV)
    {
        vertexSource += define_attribute_uv + endline;
        vertexSource += out_uv + endline;
    }
    if (hasColor)
    {
        vertexSource += define_attribute_color + endline;
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
        fragmentSource += def_uniform_viewpos + endline;
        fragmentSource += def_uniform_lightpos + endline;
    }
    if (hasUV)
    {
        fragmentSource += in_uv +endline;
    }
    if (hasColor)
    {
        fragmentSource += in_color +endline;
    }

    if (hasBaseColor)
    {
        fragmentSource += def_uniform_basecolor + endline;
    }
    if (hasSampler)
    {
        fragmentSource += def_uniform_sampler2d + endline;
    }

    fragmentSource += main_begin;
    if (haslight)
    {
        fragmentSource += calc_ambient + endline;
        fragmentSource += calc_diffuse_part1 + endline;
        fragmentSource += calc_diffuse_part2 + endline;
        fragmentSource += calc_diffuse_part3 + endline;
        fragmentSource += calc_diffuse_part4 + endline;
        fragmentSource += calc_specular_part1 + endline;
        fragmentSource += calc_specular_part2 + endline;
        fragmentSource += calc_specular_part3 + endline;
        fragmentSource += calc_specular_part4 + endline;
        fragmentSource += calc_fragcolor2 + endline;
    }
    else if (isFont)
    {
        fragmentSource += calc_fragcolor4 + endline;
    }
    else if (hasSampler)
    {
        fragmentSource += calc_fragcolor3 + endline;
    }
    else
    {
        fragmentSource += calc_fragcolor1 + endline;
    }
    fragmentSource += main_end;
}
