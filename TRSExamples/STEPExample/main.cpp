#include <cstdlib>
#include <string>
#include "StepConverter.h"
#include "Core\TRSViewer.h"
#include "Core\TRSStateSet.h"
#include "Core\TRSTexture.h"
#include "Core\TRSShader.h"
#include "DataModel\TRSGeode.h"
#include "DataModel\TRSGroup.h"
#include "Util\TRSResource.h"
#include "Math\TRSMathUtil.h"
#include "Geometry\TRSCube.h"

int main(int argn, char** argc)
{
    if (argn != 2)
    {
        return -1;
    }

    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGroup> root = std::make_shared<TRSGroup>();

    StepConverter converter;
    std::string file_name(argc[1]);
    if (!converter.readSTEP(file_name, root.get()))
    {
        return -1;
    }

    //std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();
    //TRSCube* cube = new TRSCube;
    //TRSMesh* mesh = cube->getMesh();
    //pNode->setMesh(mesh);
    //std::shared_ptr<TRSStateSet> pSS = pNode->getOrCreateStateSet();
    //pSS->getShader()->createProgram("shaders/PhongVertex.glsl", "shaders/PhongFragment.glsl");
    //root->addChild(pNode);

    viewer->setSecenNode(root);
    viewer->run();

    return 0;
}