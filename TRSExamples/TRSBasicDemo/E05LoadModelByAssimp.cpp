#include "E05LoadModelByAssimp.h"
#include "TRSViewer.h"
#include "TRSAssimpLoader.h"
#include "TRSGroup.h"


void E05LoadModelByAssimp()
{
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    TRSAssimpLoader* pLoader = new TRSAssimpLoader();
    //"resources/objects/test.obj"; //std::string("resources/objects/nanosuit/nanosuit.obj");
    std::string strFile = std::string("resources/objects/nanosuit/nanosuit.obj");
    TRSGroup* pGroup = pLoader->loadByAssimp(strFile);
    std::string strDebugInfo = pGroup->debugInfo();
    std::shared_ptr<TRSGroup> pSharedGroup = std::shared_ptr<TRSGroup>(pGroup);
    viewer->setSecenNode(pSharedGroup);
    viewer->run();
}


