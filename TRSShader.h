/*!  
 *@brief    渲染系统的着色器
 *@author   wangps
 *@date     2019年5月3日
 */

#pragma once
#include <string>
#include <glad/glad.h>
#include "TRSExport.h"

class TRS_EXPORT TRSShader
{
public:
    TRSShader();
    ~TRSShader();

    void createVertexShader(const std::string vShaderFile);

    void createFragmentShader(const std::string fShaderFile);

    unsigned int createProgram(bool delShader = true);

    unsigned int createProgram(const std::string vShaderFile, const std::string fShaderFile, bool delShader = true);

    void use();

    void setUniformi(const std::string uniformName, int value);

private:
    //return 0 when failure
    unsigned int createShader(const std::string vShaderFile, GLenum EnShaderType);

public:
    unsigned int program;
    unsigned int vShader;
    unsigned int fShader;
};

