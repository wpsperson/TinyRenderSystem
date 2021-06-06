#include "CasePNTriangles.h"
#include "TRSTexture.h"
#include "TRSVAO.h"
#include "TRSResource.h"
#include "TRSConst.h"
#include "TRSCamera.h"
#include "TRSViewer.h"
#include "TRSGeode.h"
#include "TRSGroup.h"
#include "TRSShader.h"
#include "TRSAssimpLoader.h"
#include "TRSStateset.h"
#include "glfw\glfw3.h"

#include <iostream>
#include "TRSUtils.h"
#include "stb_image.h"


int CaseTraditional3DModel(int argn, char** argc)
{
    std::string strFile = std::string("resources/objects/bunny.obj");
    if (argn == 2)
    {
        strFile = std::string(argc[1]);
    }
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    TRSAssimpLoader* pLoader = new TRSAssimpLoader();
    TRSGroup* pGroup = pLoader->loadByAssimp(strFile);
    std::string strDebugInfo = pGroup->debugInfo();
    std::shared_ptr<TRSGroup> pSharedGroup = std::shared_ptr<TRSGroup>(pGroup);
    viewer->setSecenNode(pSharedGroup);
    viewer->run();
    return 0;
}
