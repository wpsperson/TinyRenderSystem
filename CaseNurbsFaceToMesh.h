#pragma once
#include "TRSEventHandler.h"
#include <memory>
#include <vector>

class TRSGeode;
class TRSCamera;
class BSplineSurface;

void CaseDelauneyTriangulation();

int CaseNurbsFaceToMesh(int argn, char** argc);

class InsertParametricPointHandler : public TRSEventHandler
{
public:
    InsertParametricPointHandler(BSplineSurface* nurbs, TRSCamera* camera);

    std::shared_ptr<TRSGeode> getParametricSpaceMesh();

    std::shared_ptr<TRSGeode> get3DSpaceMesh();

    void processLeftMousePress(double xpos, double ypos, int mods) override;
    void processKeyPress(int key);

    void initMesh();
    void updateMesh();
    void saveParameterToFile(const std::string& fileName);
    void loadParameterFromFile(const std::string& fileName);
private:
    TRSCamera* m_camera = nullptr;
    BSplineSurface* bsSurface = nullptr;
    std::shared_ptr<TRSGeode> Triangles3d;
    std::shared_ptr<TRSGeode> Triangles2d;
    std::vector<double> uvCoords;
    std::vector<float> parametricSpacePoints;// represent each parametric point in 3d format by set z=0: u, v, 0, u, v, 0...
    std::vector<float> parametricSpacePointsNormals;// represent the points and normals in 3d space.
};
