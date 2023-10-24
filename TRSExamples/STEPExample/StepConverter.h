#pragma once

#include <string>

// #include <XCAFApp_Application.hxx>
// #include <XCAFDoc_DocumentTool.hxx>
// #include "XCAFDoc_ShapeTool.hxx"
// #include <IGESCAFControl_Reader.hxx>
// #include <STEPCAFControl_Reader.hxx>
// #include <IGESControl_Reader.hxx>
// #include <BRep_Builder.hxx>
// #include <TDataStd_Name.hxx>
// #include <Quantity_Color.hxx>
// #include <XCAFDoc_ColorTool.hxx>

#include <Standard_Handle.hxx>
#include <TopoDS_Shape.hxx>
#include <Quantity_Color.hxx>

class TDF_Label;
class XCAFDoc_ShapeTool;
class XCAFDoc_ColorTool;

class TRSGroup;
class TRSGeode;

class StepConverter
{
public:
    StepConverter();
    ~StepConverter();

    bool readSTEP(const std::string& file, TRSGroup* root);

    bool readIGES(const std::string &file, TRSGroup* root);

    static std::string getLabelName(const TDF_Label& Label);

    static TopoDS_Shape getLabelShape(const opencascade::handle<XCAFDoc_ShapeTool>& shapeTool, const TDF_Label& Label);

    static Quantity_Color getLabelColor(const opencascade::handle<XCAFDoc_ColorTool>& ColorTool, const TDF_Label& Label, const TopoDS_Shape& Shape);

    void createChild(const TDF_Label& Label, TopLoc_Location Location, TRSGroup* group);

    static void convertShapeToGeode(const TopoDS_Shape& topo_shape, TRSGeode* geode);
    static void generateTriangles(const TopoDS_Shape& topo_shape, TRSGeode* geode);
    static void generateLines(const TopoDS_Shape& topo_shape, TRSGeode* geode);
    static void generateVertices(const TopoDS_Shape& topo_shape, TRSGeode* geode);

private:
    TRSGroup* m_group = nullptr;
    opencascade::handle<XCAFDoc_ShapeTool> m_shapeTool;
    opencascade::handle<XCAFDoc_ColorTool> m_colorTool;
};
