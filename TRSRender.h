/*!  
 *@brief    ��Ⱦ��
 *@author   wangps
 *@date     2019��5��3��
 */

#pragma once
#pragma warning( disable: 4251 )
#include <vector>
#include <glm/glm.hpp>
#include "TRSExport.h"
#include "TRSVAO.h"
class GLFWwindow;
class TRSShader;
class TRSTexture;
class TRSVAO;

class TRS_EXPORT TRSRender
{
public:
    TRSRender(GLFWwindow * window);

    TRSRender(GLFWwindow * window, TRSVAO* vao, TRSShader* pShader, TRSTexture* pTex);

    ~TRSRender();

    void setTexture(TRSTexture* pTex);

    void setShaders(TRSShader* pShader);

    void setVAO(TRSVAO* vao);

    void setWindow(GLFWwindow* window);

    void setBGColor(float r, float g, float b, float a);

    void setBGColor(glm::vec4 color);

    void exec();

protected:
    virtual void defaultSetting();

protected:
    GLFWwindow* m_pWindow;
    glm::vec4 m_BGColor;

    TRSShader* m_pShader;
    TRSTexture* m_pTexture;
    TRSVAO* m_pVAO;

};
