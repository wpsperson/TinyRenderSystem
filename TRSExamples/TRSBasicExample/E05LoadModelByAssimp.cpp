#include "E05LoadModelByAssimp.h"
#include "Core/TRSViewer.h"
#include "IO/TRSAssimpLoader.h"
#include "DataModel/TRSGroup.h"


void E05LoadModelByAssimp()
{
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    TRSAssimpLoader* pLoader = new TRSAssimpLoader();
    std::string strFile = std::string("resources/objects/nanosuit/nanosuit.obj");
    TRSGroup* pGroup = pLoader->loadByAssimp(strFile);
    std::string strDebugInfo = pGroup->debugInfo();
    std::shared_ptr<TRSGroup> pSharedGroup = std::shared_ptr<TRSGroup>(pGroup);
    viewer->setSecenNode(pSharedGroup);
    viewer->run();
}


