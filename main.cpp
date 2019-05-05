#include "TRSUtils.h"
#include "TRSConfig.h"
#include "TRSRender.h"
#include "TRSShader.h"
#include "TRSTexture.h"
#include "TRSVAO.h"
#include "TRSResource.h"
#include "TRSConst.h"
#include "TRSCamera.h"

TRSCamera* g_pCamera;

int main()
{
    GLFWwindow* window(nullptr);
    if (!TRSConfig::initGlfwWindowAndGLAD(DefaultWindowWidth, DefaultWindowHeight, &window))
    {
        return -1;
    }
    //äÖÈ¾×é¼þ£ºshader vao texture camera
    TRSShader* pShader = new TRSShader;
    pShader->createProgram("vertex.glsl", "fragment.glsl");
    TRSVAO* pVAO = new TRSVAO;
    pVAO->createVAO(BoxVertices, sizeof(BoxVertices) / sizeof(float), TRSVAO::EnVertexTextureColor);
    TRSTexture* pTexture = new TRSTexture(2, std::string("container.jpg"), std::string("awesomeface.png"));
    TRSCamera* pCamera = new TRSCamera;
    g_pCamera = pCamera;
    TRSConfig::registerUserInputFunc(window);//reg user input callback
    //äÖÈ¾Æ÷
    TRSRender* pRender = new TRSRender(window, pVAO, pShader, pTexture);
    pRender->setCamera(pCamera);
    pRender->exec();
    return 0;
}

