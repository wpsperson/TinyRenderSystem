/*!  
 *@brief    ��Ⱦϵͳ����ɫ��
 *@author   wangps
 *@date     2019��5��3��
 */

#pragma once
#include <string>
#include <glad/glad.h>
#include "TRSExport.h"
#include "glm/glm.hpp"

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

    void setTexUniform(int nTexCount);

    void setUniformi(const std::string uniformName, int value);

    void setUniform4v(const std::string uniformName, glm::vec4 vec4Color);

    void setUniformMatrix4(const std::string& uniformName, glm::mat4 mat);

private:
    //return 0 when failure
    unsigned int createShader(const std::string vShaderFile, GLenum EnShaderType);

public:
    unsigned int program;
    unsigned int vShader;
    unsigned int fShader;
};

