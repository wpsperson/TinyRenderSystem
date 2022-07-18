#include "E01TextureBox.h"
#include <memory>
#include "glad\glad.h"
#include "Core\TRSViewer.h"
#include "E06MeshPerformance.h"
#include "Core\TRSStateSet.h"
#include "Core\TRSTexture.h"
#include "Core\TRSShader.h"
#include "DataModel\TRSGeode.h"
#include "DataModel\TRSGroup.h"
#include "Util\TRSResource.h"
#include "Math\TRSMathUtil.h"
#include "Geometry\TRSCube.h"
#include "DataModel\TRSMesh.h"

constexpr int MaxNum = 100;
constexpr double width = 1.0;
constexpr double height = 1.0;

void E06MeshPerformance()
{
    std::shared_ptr<TRSViewer> viewer = std::make_shared<TRSViewer>();
    std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();

    double len = width*2.0 / MaxNum;
    double radius = len / 2;
    TRSVec3 ptA, ptB, ptC;
    ptA[0] = 0.0, ptA[1] = 0, ptA[2] = 0.0;
    ptB[0] = len, ptB[1] = 0, ptB[2] = 0.0;
    ptC[0] = len / 2, ptC[1] = len, ptC[2] = 0.0;

    std::vector<TRSVec3> points;
    points.reserve(MaxNum * MaxNum * MaxNum);

    double xStep = width * 2.0 / MaxNum;
    double yStep = height * 2.0 / MaxNum;
    double zStep = 0.01;
    double x, y, z;
    for (int i=0; i<MaxNum; i++)
    {
        x = -width + i * xStep;
        for (int j=0; j<MaxNum; j++)
        {
            y = -height + j * yStep;
            for (int k=0; k<MaxNum; k++)
            {
                z = k * zStep;
                points.emplace_back(TRSVec3(ptA[0] + x, ptA[1] + y, ptA[2] + z));
                points.emplace_back(TRSVec3(ptB[0] + x, ptB[1] + y, ptB[2] + z));
                points.emplace_back(TRSVec3(ptC[0] + x, ptC[1] + y, ptC[2] + z));
            }
        }
    }

    TRSMesh* mesh = new TRSMesh;
    mesh->setVertex(points);
    pNode->setMesh(mesh);

    viewer->setSecenNode(pNode);
    viewer->run();
}
