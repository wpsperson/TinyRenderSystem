#include "StepConverter.h"

#include <cassert>

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

#include "TRS/TRSGeode.h"
#include "TRS/TRSGroup.h"
#include "TRS/TrsMesh.h"
#include "TRS/TRSShader.h"
#include "TRS/TRSStateSet.h"

constexpr double GlobalLineDeflection = 0.002;
static const TRSVec4 DefaultColor(0.8f, 0.8f, 0.8f, 1.0f);

StepConverter::StepConverter()
{

}

StepConverter::~StepConverter()
{

}

TRSNode* StepConverter::readStepFile(const char* stepFile)
{
    Handle(XCAFApp_Application) occApplication = XCAFApp_Application::GetApplication();
    Handle(TDocStd_Document) occDocument;
    occApplication->NewDocument("MDTV-XCAF", occDocument);

    STEPCAFControl_Reader reader;
    reader.SetColorMode(true);
    reader.SetNameMode(true);
    reader.SetLayerMode(true);
    reader.SetMatMode(true);
    reader.SetGDTMode(true);
    IFSelect_ReturnStatus status = reader.ReadFile(stepFile);
    bool result = reader.Transfer(occDocument);
    if (!result)
    {
        return nullptr;
    }
    TDF_Label mainLabel = occDocument->Main();
    m_shapeTool = XCAFDoc_DocumentTool::ShapeTool(mainLabel);
    m_colorTool = XCAFDoc_DocumentTool::ColorTool(mainLabel);

    TDF_LabelSequence freeShapes;
    m_shapeTool->GetFreeShapes(freeShapes);
    int freeShapeCount = freeShapes.Length();
    if (!freeShapeCount)
    {
        return nullptr;
    }
    TDF_Label rootLabel = freeShapes.Value(1);

    bool isAssembly = m_shapeTool->IsAssembly(rootLabel);
    bool isSimpleShp = m_shapeTool->IsSimpleShape(rootLabel);
    if (isSimpleShp)
    {
        TRSGeode* geode = createGeodeNode(rootLabel);
        return geode;
    }
    else if (isAssembly)
    {
        TopLoc_Location origin;
        TRSGroup* group = createGroupNode(rootLabel, origin);
        return group;
    }
    return nullptr;
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

TRSGroup* StepConverter::createGroupNode(const TDF_Label& assembly, TopLoc_Location Location)
{
    TDF_LabelSequence components;
    if (!m_shapeTool->GetComponents(assembly, components))
    {
        return nullptr;
    }

    TRSGroup* group = new TRSGroup;
    int componentCount = components.Length();
    for (Standard_Integer compIndex = 1; compIndex <= componentCount; ++compIndex)
    {
        TDF_Label ChildLabel = components.Value(compIndex);
        TopLoc_Location localLocation = Location * m_shapeTool->GetLocation(ChildLabel);
        if (m_shapeTool->IsAssembly(ChildLabel))
        {
            TRSGroup* subGroup = createGroupNode(ChildLabel, localLocation);
            if (!subGroup)
            {
                continue;
            }
            group->addChild(std::shared_ptr<TRSGroup>(subGroup));
        }
        else
        {
            TRSGeode* geode = createGeodeNode(ChildLabel);
            if (!geode)
            {
                continue;
            }
            group->addChild(std::shared_ptr<TRSGeode>(geode));
        }
    }
    if (0 == group->childNum())
    {
        delete group;
        return nullptr;
    }
    return group;
}

TRSGeode* StepConverter::createGeodeNode(const TDF_Label& shapeLabel)
{
    TopoDS_Shape occShape;
    Standard_Boolean Res = m_shapeTool->GetShape(shapeLabel, occShape);
    if (!Res || occShape.IsNull())
    {
        return nullptr;
    }

    // get name
    std::string name;
    if (m_shapeTool->IsReference(shapeLabel))
    {
        TDF_Label referredShape;
        if (m_shapeTool->GetReferredShape(shapeLabel, referredShape))
        {
            name = getLabelName(referredShape);
        }
    }
    else
    {
        name = getLabelName(shapeLabel);
    }

    // get color
    TRSVec4 color = DefaultColor;
    Quantity_Color occColor;
    if (findNodeColor(shapeLabel, occShape, occColor))
    {
        color = toTRSVec4(occColor);
    }

    // generate Geode and fill the mesh
    TRSGeode* geode = new TRSGeode;
    geode->setName(name);
    geode->setColor(color);
    // populate mesh
    TRSMesh* mesh = geode->getMesh();
    populateMesh(occShape, mesh);
    // update shader.
    std::shared_ptr<TRSStateSet> stateSet = geode->getOrCreateStateSet();
    stateSet->getShader()->createProgram("shaders/PhongVertex.glsl", "shaders/PhongFragment.glsl");
    return geode;
}

bool StepConverter::findNodeColor(const TDF_Label& Label, const TopoDS_Shape& Shape, Quantity_Color& occColor)
{
    if (m_colorTool->GetInstanceColor(Shape, XCAFDoc_ColorType::XCAFDoc_ColorSurf, occColor))
    {
        return true;
    }
    if (m_colorTool->GetInstanceColor(Shape, XCAFDoc_ColorType::XCAFDoc_ColorGen, occColor))
    {
        return true;
    }
    if (m_colorTool->GetInstanceColor(Shape, XCAFDoc_ColorType::XCAFDoc_ColorCurv, occColor))
    {
        return true;
    }
    if (m_colorTool->GetColor(Label, XCAFDoc_ColorType::XCAFDoc_ColorSurf, occColor))
    {
        return true;
    }
    if (m_colorTool->GetColor(Label, XCAFDoc_ColorType::XCAFDoc_ColorGen, occColor))
    {
        return true;
    }
    if (m_colorTool->GetColor(Label, XCAFDoc_ColorType::XCAFDoc_ColorCurv, occColor))
    {
        return true;
    }
    return false;
}

void StepConverter::populateMesh(const TopoDS_Shape& topo_shape, TRSMesh* mesh)
{
    BRepMesh_IncrementalMesh incrementalMesh(topo_shape, GlobalLineDeflection, Standard_True, 0.5, Standard_True);
    TopExp_Explorer faceExplorer;
    std::vector<TRSPoint> meshVertexs;
    std::vector<TRSVec3> normals;
    std::vector<TRSVec2> UVs;
    std::vector<TRSVec3> colors;
    std::vector<unsigned int> meshIndexs;
    int nodeLen = 0;
    int triLen = 0;
    for (faceExplorer.Init(topo_shape, TopAbs_FACE); faceExplorer.More(); faceExplorer.Next())
    {
        TopLoc_Location loc;
        TopoDS_Face aFace = TopoDS::Face(faceExplorer.Current());
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
            meshVertexs.push_back(toTRSVec(vertex1));
            if (hasNormal)
            {
                gp_Dir dir = triFace->Normal(i);
                normals.emplace_back(toTRSVec(dir));
            }
            if (hasUV)
            {
                gp_Pnt2d uv = triFace->UVNode(i);
                UVs.emplace_back(toTRSVec2(uv));
            }
        }

        TopAbs_Orientation orientation = aFace.Orientation();
        int  nTriangles = triFace->NbTriangles();
        for (int i = 1; i <= nTriangles; i++)
        {
            Poly_Triangle aTriangle = triFace->Triangle(i);
            int nVertexIndex1 = 0;
            int nVertexIndex2 = 0;
            int nVertexIndex3 = 0;
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
    mesh->setVertex(meshVertexs);
    // mesh->setUV(UVs); // do not use UV now
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

TRSVec3 StepConverter::toTRSVec(const gp_Pnt& pt)
{
    return TRSVec3(static_cast<float>(pt.X()), static_cast<float>(pt.Y()), static_cast<float>(pt.Z()));
}

TRSVec3 StepConverter::toTRSVec(const gp_Dir& dir)
{
    return TRSVec3(static_cast<float>(dir.X()), static_cast<float>(dir.Y()), static_cast<float>(dir.Z()));
}

TRSVec2 StepConverter::toTRSVec2(const gp_Pnt2d& pt2d)
{
    return TRSVec2(static_cast<float>(pt2d.X()), static_cast<float>(pt2d.Y()));
}

TRSVec4 StepConverter::toTRSVec4(const Quantity_Color& occColor)
{
    float red = static_cast<float>(occColor.Red());
    float green = static_cast<float>(occColor.Green());
    float blue = static_cast<float>(occColor.Blue());
    return TRSVec4(red, green, blue, 1.0f);
}

