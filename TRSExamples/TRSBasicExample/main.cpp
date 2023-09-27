#include <cstdlib>
#include <string>
#include "E01TextureBox.h"
#include "E02BasicMaterial.h"
#include "E03BasicLight.h"
#include "E04ElementArray.h"
//#include "E05LoadModelByAssimp.h"
#include "E06MeshPerformance.h"
//#include "E07TessellationOnShader.h"
//#include "E08PNTriangles.h"
//#include "E09FreeType.h"
//#include "E10NurbsFaceToMesh.h"

int main(int argn, char** argc)
{
    size_t leng;
    char buffer[256];
    errno_t err = getenv_s(&leng, buffer, 256, "SystemRoot"); // std::getenv("SystemRoot")
    if (err == 0)
    {
        
    }
    std::string m_fontFile = std::string(buffer) + std::string("/Fonts/simfang.ttf");
    //E01TextureColorArray();
    //E01ManyBoxRotate();

    //E02Material_AmbientDiffuseSpecular();
    //E02Material_DiffuseNormal();
    //E03DirectionLight();
    //E03PointAttenuationLight();
    //E03Spotlight();
    //E03MultiLightSource();
    E04ElementArray();

    //E05LoadModelByAssimp();
    //E06MeshPerformance();

    //CaseTessHermiteCurve(argn, argc);
    //CaseTessBezierCurve(argn, argc);
    //CaseTessBezierSurface(argn, argc);
    //CaseTraditionalBSplineCurve(argn, argc);
    //CaseTraditionalBSplineSurface(argn, argc);
    //CaseTessBSplineCurve(argn, argc);
    //CaseTessBSplineSurface(argn, argc);

    //CaseTraditional3DModel(argn, argc);
    //CasePNTriangles(argn, argc);

    //CaseTextNode();

    //CaseDelauneyTriangulation();
    //CaseNurbsFaceToMesh(argn, argc);

    return 0;
}