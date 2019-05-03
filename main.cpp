#include "TRSUtils.h"
#include "TRSConfig.h"
#include "TRSRender.h"
#include "TRSShader.h"
#include "TRSTexture.h"
#include "TRSVAO.h"
#include "TRSResource.h"
#include "TRSConst.h"



int main()
{
    GLFWwindow* window(nullptr);
    if (!TRSConfig::initGlfwWindowAndGLAD(DefaultWindowWidth, DefaultWindowHeight, &window))
    {
        return -1;
    }
    //äÖÈ¾×é¼þ£ºshader vao texture
    TRSShader* pShader = new TRSShader;
    pShader->createProgram("vertex.glsl", "fragment.glsl");
    TRSVAO* pVAO = new TRSVAO;
    pVAO->createVAO(BoxVertices, sizeof(BoxVertices) / sizeof(float), TRSVAO::EnVertexTextureColor);
    TRSTexture* pTexture = new TRSTexture(2, std::string("container.jpg"), std::string("awesomeface.png"));
    //äÖÈ¾Æ÷
    TRSRender* render = new TRSRender(window, pVAO, pShader, pTexture);
    render->exec();
    return 0;
}

