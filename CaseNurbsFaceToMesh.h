#pragma once
#include "TRSEventHandler.h"
#include <memory>
#include <vector>
#include <string>

class TRSGeode;
class TRSCamera;
class BSplineSurface;

void CaseDelauneyTriangulation();

int CaseNurbsFaceToMesh(int argn, char** argc);

class InsertParametricPointHandler : public TRSEventHandler
{
public:
    InsertParametricPointHandler(BSplineSurface* nurbs, TRSCamera* camera);

    void setReferrenceSurfaceMesh(std::shared_ptr<TRSGeode> surfaceMesh);

    std::shared_ptr<TRSGeode> getParametricSpaceMesh();

    std::shared_ptr<TRSGeode> get3DSpaceMesh();

    void processRightMousePress(double xpos, double ypos, int mods) override;
    void processKeyPress(int key);

    void initMesh();
    void updateMesh();
    void saveParameterToFile(const std::string& fileName);
    void loadParameterFromFile(const std::string& fileName);
private:
    TRSCamera* m_camera = nullptr;
    BSplineSurface* m_BSSurface = nullptr;          // denote the original surface.
    std::shared_ptr<TRSGeode> m_triangles3d;
    std::shared_ptr<TRSGeode> m_triangles2d;
    std::shared_ptr<TRSGeode> m_refSurfaceMesh;
    std::vector<double> m_uvCoords;                 // denote the u v coordinate in parametric space
    std::vector<float> m_parametricSpacePoints;     // denote each parametric point in 3d format by set z=0: u, v, 0, u, v, 0...
    std::vector<float> m_3DSpacePointsNormals;      // denote the points and normals in 3d space.
};
