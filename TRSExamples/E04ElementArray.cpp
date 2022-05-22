#include "E04ElementArray.h"
#include "TRSViewer.h"
#include "TRSGeode.h"
#include "TRSResource.h"


void E04ElementArray()
{
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();
    pNode->readFromVertex(Rectangle4Vertices, sizeof(Rectangle4Vertices) / sizeof(float), EnVertex, Rectangle6Indices, sizeof(Rectangle6Indices) / sizeof(unsigned int));
    viewer->setSecenNode(pNode);
    viewer->run();
}

