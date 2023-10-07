#include "E04ElementArray.h"
#include "Core\TRSViewer.h"
#include "DataModel\TRSGeode.h"
#include "Util\TRSResource.h"
#include "Geometry/TRSCylinder.h"
#include "Core/TRSStateSet.h"
#include "Core/TRSShader.h"
#include "Core/TRSTexture.h"


void E04ElementArray()
{
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();
    pNode->getOrCreateStateSet()->getShader()->createProgram("shaders/PosNormTexVertex.glsl", "shaders/PosNormTexFragment.glsl");
    pNode->getOrCreateStateSet()->getTexture()->createTexture("resources/textures/cube.png");
    TRSCylinder* cy = new TRSCylinder;
    cy->setCentRadius(G_ORIGIN, 10, 4);
    cy->setResolution(6);
    pNode->setMesh(cy->getMesh());
    viewer->setSecenNode(pNode);
    viewer->run();
}

