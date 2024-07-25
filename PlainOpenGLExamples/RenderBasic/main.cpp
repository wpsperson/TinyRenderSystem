#include <string>

#include "RenderTriangle.h"
#include "RenderColorTriangle.h"
#include "RenderTexture.h"
#include "TessellationOnShader.h"
#include "StencilTest.h"
#include "ShadowMapping.h"

int main(int argn, char** argc)
{
    if (argn < 2)
    {
        RenderTriangle();
        return 0;
    }
    std::string argument(argc[1]);
    if (argument == "RenderTexture")
    {
        RenderTexture();
    }
    else if(argument == "RenderMultiTexture")
    {
        RenderMultiTexture();
    }
    else if (argument == "TessellationOnShader")
    {
        TessellationOnShader();
    }
    else if (argument == "StencilTest")
    {
        StencilTest();
    }
    else if (argument == "RenderColorTriangle")
    {
        RenderColorTriangle();
    }
    else if (argument == "renderShadowMapping")
    {
        renderShadowMapping();
    }
    return 0;
}