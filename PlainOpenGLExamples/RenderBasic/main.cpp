#include "E00PlainOpenGL.h"
#include "E06StencilTest.h"

int main(int argn, char** argc)
{
    CaseFirstTriangle();
    CaseElementArray();
    CaseColorAnimation();
    CaseEachVertexColor();
    CaseTextureColorBasic();
    CaseMultiTexture();
    CaseTessellationOnShader();
    E06StencilTest();
    return 0;
}