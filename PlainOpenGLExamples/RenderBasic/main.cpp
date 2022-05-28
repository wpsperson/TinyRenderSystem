#include "RenderTriangle.h"
#include "RenderTexture.h"
#include "TessellationOnShader.h"
#include "StencilTest.h"

int main(int argn, char** argc)
{
    RenderTriangle();

    RenderTexture();
    RenderMultiTexture();

    TessellationOnShader();

    StencilTest();
    return 0;
}