#include "StepConverter.h"

#include "TRS/TRSGeode.h"
#include "TRS/TRSGroup.h"
#include "TRS/TrsMesh.h"

#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include "XCAFDoc_ShapeTool.hxx"
#include <IGESCAFControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <IGESControl_Reader.hxx>
#include <BRep_Builder.hxx>
#include <TDataStd_Name.hxx>
#include <Quantity_Color.hxx>
#include <XCAFDoc_ColorTool.hxx>

#include <BRepMesh_IncrementalMesh.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <BRep_Tool.hxx>

constexpr double GlobalLineDeflection = 0.002;

StepConverter::StepConverter()
{

}

StepConverter::~StepConverter()
{

}

bool StepConverter::readSTEP(const std::string& file_name, TRSGroup* root)
{
    Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
    Handle(TDocStd_Document) doc;
    anApp->NewDocument("MDTV-XCAF", doc);

    STEPCAFControl_Reader reader;
    reader.SetColorMode(true);
    reader.SetNameMode(true);
    reader.SetLayerMode(true);
    reader.SetMatMode(true);
    reader.SetGDTMode(true);
    IFSelect_ReturnStatus status = reader.ReadFile(file_name.c_str());
    bool result = reader.Transfer(doc);
    if (!result)
    {
        return false;
    }
    TDF_Label mainLabel = doc->Main();
    m_shapeTool = XCAFDoc_DocumentTool::ShapeTool(mainLabel);
    m_colorTool = XCAFDoc_DocumentTool::ColorTool(mainLabel);

    TDF_LabelSequence rootLabels;
    m_shapeTool->GetFreeShapes(rootLabels);
    int roots = rootLabels.Length();

    TDF_Label Label = rootLabels.Value(1);
    auto location = TopLoc_Location();

    createChild(Label, location, root);
    return true;
}

bool StepConverter::readIGES(const std::string& file, TRSGroup* root)
{
    return false;
}

std::string StepConverter::getLabelName(const TDF_Label& Label)
{
    std::string Name = "Body";
    Handle(TDataStd_Name) aName;
    if (Label.FindAttribute(TDataStd_Name::GetID(), aName))
    {
        TCollection_ExtendedString extendedName = aName->Get();
        Standard_PCharacter pCharacter = new Standard_Character[1000];
        extendedName.ToUTF8CString(pCharacter);
        Name = pCharacter;
        delete[] pCharacter;
    }
    return Name;
}

TopoDS_Shape StepConverter::getLabelShape(const opencascade::handle<XCAFDoc_ShapeTool>& shapeTool, const TDF_Label& Label)
{
    TopoDS_Shape shape;
    Standard_Boolean Res = shapeTool->GetShape(Label, shape);
    if (Res == false || shape.IsNull())
    {
        throw "we can not get this shape";
    }
    return shape;
}

Quantity_Color StepConverter::getLabelColor(const opencascade::handle<XCAFDoc_ColorTool>& ColorTool, const TDF_Label& Label, const TopoDS_Shape& Shape)
{
    NCollection_Vec3<float> color;
    color.SetValues(0.756f, 0.756f, 0.756f);
    Quantity_Color aColor = Quantity_Color(color);
    bool flag1 = ColorTool->GetInstanceColor(Shape, XCAFDoc_ColorType::XCAFDoc_ColorSurf, aColor);
    bool flag2 = ColorTool->GetInstanceColor(Shape, XCAFDoc_ColorType::XCAFDoc_ColorGen, aColor);
    bool flag3 = ColorTool->GetInstanceColor(Shape, XCAFDoc_ColorType::XCAFDoc_ColorCurv, aColor);
    if (flag1 || flag2 || flag3) {
        return aColor;
    }
    ColorTool->GetColor(Label, XCAFDoc_ColorType::XCAFDoc_ColorSurf, aColor);
    ColorTool->GetColor(Label, XCAFDoc_ColorType::XCAFDoc_ColorGen, aColor);
    ColorTool->GetColor(Label, XCAFDoc_ColorType::XCAFDoc_ColorCurv, aColor);
    return aColor;
}

void StepConverter::createChild(const TDF_Label& Label, TopLoc_Location Location, TRSGroup* group)
{
    TDF_LabelSequence components;
    if (m_shapeTool->GetComponents(Label, components))
    {
        TDF_LabelSequence childComponents;
        for (Standard_Integer compIndex = 1; compIndex <= components.Length(); ++compIndex)
        {
            TDF_Label ChildLabel = components.Value(compIndex);
            if (m_shapeTool->IsReference(ChildLabel))
            {
                TDF_Label ShapeLabel;
                if (m_shapeTool->GetReferredShape(ChildLabel, ShapeLabel))
                {
                    TopLoc_Location LocalLocation = Location * m_shapeTool->GetLocation(ChildLabel);
                    std::string name = getLabelName(ChildLabel);
                    TopoDS_Shape childShape = getLabelShape(m_shapeTool, ChildLabel);
                    bool isComponent = m_shapeTool->GetComponents(ChildLabel, childComponents);
                    Quantity_Color color = getLabelColor(m_colorTool, ChildLabel, childShape);

                    if (!isComponent)
                    {
                        std::shared_ptr<TRSGeode> geode = std::make_shared<TRSGeode>();
                        geode->setName(name);
                        geode->setColor(TRSVec4(color.Red(), color.Green(), color.Blue(), 1.0f));
                        convertShapeToGeode(childShape, geode.get());
                        group->addChild(geode);

                        //auto part = new pmGeometryNode(m_document);
                        //part->setName(name);
                        //part->setColor(base3::Color3(color.Red(), color.Green(), color.Blue()));
                        //part->setOccShape(childShape);
                        //assembly->addNode(part);

                    }
                    else
                    {
                        std::shared_ptr<TRSGroup> subGroup = std::make_shared<TRSGroup>();
                        subGroup->setName(name);
                        subGroup->setColor(TRSVec4(color.Red(), color.Green(), color.Blue(), 1.0f));
                        createChild(ChildLabel, LocalLocation, subGroup.get());
                        group->addChild(subGroup);

                        //auto childAssembly = new pmGeometryGroup(m_document);
                        //childAssembly->setName(name);
                        //childAssembly->setColor(base3::Color3(color.Red(), color.Green(), color.Blue()));
                        //childAssembly->setOccShape(childShape);
                        //assembly->addNode(childAssembly);
                        //createChild(ShapeTool, ColorTool, ChildLabel, LocalLocation, childAssembly);
                    }
                }
            }
        }
    }
    else
    {
        std::string name = getLabelName(Label);
        TopoDS_Shape shape = getLabelShape(m_shapeTool, Label);
        Quantity_Color color = getLabelColor(m_colorTool, Label, shape);

        std::shared_ptr<TRSGeode> geode = std::make_shared<TRSGeode>();
        geode->setName(name);
        geode->setColor(TRSVec4(color.Red(), color.Green(), color.Blue(), 1.0f));
        convertShapeToGeode(shape, geode.get());
        group->addChild(geode);
    }
}

void StepConverter::convertShapeToGeode(const TopoDS_Shape& topo_shape, TRSGeode* geode)
{
    if (topo_shape.IsNull())
        return;

    BRepMesh_IncrementalMesh incrementalMesh(topo_shape, GlobalLineDeflection, Standard_True, 0.5, Standard_True);

    generateTriangles(topo_shape, geode);
    // in fact, we should generate all sort of geometry, but so far geode only has one geometry, so to do.
    //generateLines(topo_shape, geode);
    //generateVertices(topo_shape, geode);

}

void StepConverter::generateTriangles(const TopoDS_Shape& topo_shape, TRSGeode* geode)
{
    TopExp_Explorer faceExplorer;
    std::vector<TRSPoint> meshVertexs;
    std::vector<TRSVec3> normals;
    std::vector<TRSVec2> UVs;
    std::vector<TRSVec3> colors;
    std::vector<unsigned int> meshIndexs;
    int nodeLen = 0;
    int triLen = 0;
    int faceIndex = 0;
    for (faceExplorer.Init(topo_shape, TopAbs_FACE); faceExplorer.More(); faceExplorer.Next())
    {
        TopLoc_Location loc;
        TopoDS_Face aFace = TopoDS::Face(faceExplorer.Current());
        TopAbs_Orientation orientation = aFace.Orientation();
        Handle_Poly_Triangulation triFace = BRep_Tool::Triangulation(aFace, loc);
        if (triFace.IsNull())
        {
            continue;
        }

        int  nNodes = triFace->NbNodes();
        bool hasNormal = triFace->HasNormals();
        bool hasUV = triFace->HasUVNodes();
        for (int i = 1; i <= nNodes; i++)
        {
            gp_Pnt vertex1 = triFace->Node(i).Transformed(loc.Transformation());
            meshVertexs.push_back(TRSPoint(vertex1.X(), vertex1.Y(), vertex1.Z()));
            if (hasNormal)
            {
                gp_Dir dir = triFace->Normal(i);
                normals.emplace_back(dir.X(), dir.Y(), dir.Z());
            }
            if (hasUV)
            {
                gp_Pnt2d uv = triFace->UVNode(i);
                UVs.emplace_back(uv.X(), uv.Y());
            }
        }
        int nVertexIndex1 = 0;
        int nVertexIndex2 = 0;
        int nVertexIndex3 = 0;
        int  nTriangles = triFace->NbTriangles();
        for (int i = 1; i <= nTriangles; i++)
        {
            Poly_Triangle aTriangle = triFace->Triangle(i);
            aTriangle.Get(nVertexIndex1, nVertexIndex2, nVertexIndex3);
            if (orientation == TopAbs_REVERSED)
            {
                meshIndexs.push_back(nVertexIndex1 + nodeLen - 1);
                meshIndexs.push_back(nVertexIndex3 + nodeLen - 1);
                meshIndexs.push_back(nVertexIndex2 + nodeLen - 1);
            }
            else {
                meshIndexs.push_back(nVertexIndex1 + nodeLen - 1);
                meshIndexs.push_back(nVertexIndex2 + nodeLen - 1);
                meshIndexs.push_back(nVertexIndex3 + nodeLen - 1);
            }
        }

        nodeLen += nNodes;
        triLen += nTriangles;
    }
    TRSMesh *mesh = geode->getMesh();
    mesh->setVertex(meshVertexs);
    mesh->setUV(UVs);
    mesh->setIndices(meshIndexs);
    if (normals.empty())
    {
        mesh->generateNormals();
    }
    else
    {
        mesh->setNormal(normals);
    }
}

void StepConverter::generateLines(const TopoDS_Shape& topo_shape, TRSGeode* geode)
{
}

void StepConverter::generateVertices(const TopoDS_Shape& topo_shape, TRSGeode* geode)
{
}
