#pragma once

#include <unordered_map>

#include "TRS/TRSExport.h"
#include "TRS/TRSDefEnum.h"


class TRSShader;
class TRSGeode;

class TRS_EXPORT TRSPrograms
{
public:
    TRSPrograms();

    ~TRSPrograms();

    TRSShader* find2Shader(TRSGeode* node, RenderMode mode);

    void useProgram(ShaderType type);

    void endProgram();

private:
    TRSShader* getOrCreateShader(ShaderType type);

    void generateGLSL(int meshStruct, std::string& vertexSource, std::string& fragmentSource);

private:
    std::unordered_map<ShaderType, TRSShader*> m_shaders;
    TRSShader* m_active = nullptr;
};
