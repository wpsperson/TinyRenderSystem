#include "TRS/TRSPrograms.h"

#include "TRS/TRSDefGL.h"
#include "TRS/TRSShader.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSMesh.h"
#include "TRS/TRSTexture.h"

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
        shader->createProgram("shaders/DefaultVertex.glsl", "shaders/DefaultFragment.glsl");
    }
    else if (ShaderType::Phong == type)
    {
        shader->createProgram("shaders/PhongVertex.glsl", "shaders/PhongFragment.glsl");
    }
    else if (ShaderType::PhongTexture == type)
    {
        shader->createProgram("shaders/PosNormTexVertex.glsl", "shaders/PosNormTexFragment.glsl");
    }
    else if (ShaderType::PhongColor == type)
    {
        shader->createProgram("shaders/PhongColorVertex.glsl", "shaders/PhongColorFragment.glsl");
    }
    else if (ShaderType::DualTexture == type)
    {
        shader->createProgram("shaders/PosColorTexMVPVertex.glsl", "shaders/MultiTextureFragment.glsl");
    }
    else if (ShaderType::FontShader == type)
    {
        shader->createProgram("shaders/FontsVertex.glsl", "shaders/FontsFragment .glsl");
    }
    shader->setType(type);
    m_shaders.insert(std::make_pair(type, shader));
    return shader;
}
