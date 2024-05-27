#pragma once

#include <memory>

#include <Standard_Handle.hxx>
#include <TopoDS_Shape.hxx>
#include <Quantity_Color.hxx>

#include "TRS/TRSVector.h"

class TDF_Label;
class XCAFDoc_ShapeTool;
class XCAFDoc_ColorTool;

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

    TRSGroup* createGroupNode(const TDF_Label& assembly, TopLoc_Location Location);

    TRSGeode* createGeodeNode(const TDF_Label& shapeLabel);

private:
    bool findNodeColor(const TDF_Label& Label, const TopoDS_Shape& Shape, Quantity_Color &occColor);

    static void populateMesh(const TopoDS_Shape& topo_shape, TRSMesh* mesh);

    static TRSVec3 toTRSVec(const gp_Pnt& pt);

    static TRSVec3 toTRSVec(const gp_Dir& dir);

    static TRSVec2 toTRSVec2(const gp_Pnt2d& pt2d);

    static TRSVec4 toTRSVec4(const Quantity_Color& occColor);

private:
    opencascade::handle<XCAFDoc_ShapeTool> m_shapeTool;
    opencascade::handle<XCAFDoc_ColorTool> m_colorTool;
};
