#pragma once

#include <Standard_Handle.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <Quantity_Color.hxx>

#include "TRS/TRSVector.h"

class TDF_Label;
class XCAFDoc_ShapeTool;
class XCAFDoc_ColorTool;
class Poly_Triangulation;

class TRSMesh;
class TRSNode;
class TRSGroup;
class TRSGeode;

class StepConverter
{
public:
    StepConverter();
    ~StepConverter();

    TRSNode* readStepFile(const char *stepFile);

    static std::string getLabelName(const TDF_Label& Label);

    TRSGroup* createGroupNode(const TDF_Label& assembly, TopLoc_Location parentLocation);

    TRSGeode* createGeodeNode(const TDF_Label& shapeLabel, TopLoc_Location parentLocation);

private:
    bool findNodeColor(const TDF_Label& Label, const TopoDS_Shape& Shape, Quantity_Color &occColor);

    bool referToAssembly(const TDF_Label& Label, TDF_Label &referredShape);

    bool hasMesh(const TopoDS_Shape& topo_shape);

    void populateShadedMesh(const TopoDS_Shape& topo_shape, TRSMesh* mesh, TopLoc_Location parentLocation);

    static void populateWireframeMesh(const TopoDS_Shape& topo_shape, TRSMesh* mesh, TopLoc_Location parentLocation);

    static void populatePointsMesh(const TopoDS_Shape& topo_shape, TRSMesh* mesh, TopLoc_Location parentLocation);

    bool hasColorArray(const TopoDS_Shape& topo_shape, std::vector<TRSVec3>& colors);

    opencascade::handle<Poly_Triangulation> faceTriangulation(const TopoDS_Face &face);

    void processInfiniteParameter(double& p1, double& p2);

    static TRSVec3 toTRSVec(const gp_Pnt& pt);

    static TRSVec3 toTRSVec(const gp_Dir& dir);

    static TRSVec2 toTRSVec2(const gp_Pnt2d& pt2d);

    static TRSVec3 toTRSColor(const Quantity_Color& occColor);

private:
    opencascade::handle<XCAFDoc_ShapeTool> m_shapeTool;
    opencascade::handle<XCAFDoc_ColorTool> m_colorTool;
};
