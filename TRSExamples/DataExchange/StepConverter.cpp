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
#include <TopoDS_Edge.hxx>

#include "TRS/TRSGeode.h"
#include "TRS/TRSGroup.h"
#include "TRS/TrsMesh.h"
#include "TRS/TRSShader.h"

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
    std::vector<TRSNode*> rootNodes;
    TopLoc_Location origin;
    for(int idx = 1; idx <= freeShapeCount; idx++)
    {
        TDF_Label rootLabel = freeShapes.Value(idx);
        bool isAssembly = m_shapeTool->IsAssembly(rootLabel);
        bool isSimpleShp = m_shapeTool->IsSimpleShape(rootLabel);
        if (isSimpleShp)
        {
            TRSGeode* geode = createGeodeNode(rootLabel, origin);
            rootNodes.emplace_back(geode);
        }
        else if (isAssembly)
        {
            TRSGroup* group = createGroupNode(rootLabel, origin);
            rootNodes.emplace_back(group);
        }
    }
    if (rootNodes.size() == 1)
    {
        return rootNodes.front();
    }
    else if (rootNodes.size() > 1)
    {
        TRSGroup* rootGroup = new TRSGroup;
        for (TRSNode* node : rootNodes)
        {
            rootGroup->addChild(std::shared_ptr<TRSNode>(node));
        }
        return rootGroup;
    }
    return nullptr;
}

std::string StepConverter::getLabelName(const TDF_Label& Label)
{
    std::string Name = "Solid";
    Handle(TDataStd_Name) aName;
    if (Label.FindAttribute(TDataStd_Name::GetID(), aName))
    {
        TCollection_ExtendedString extendedName = aName->Get();
        Standard_PCharacter pCharacter = new Standard_Character[extendedName.LengthOfCString() + 1];
        extendedName.ToUTF8CString(pCharacter);
        Name = pCharacter;
        delete[] pCharacter;
    }
    return Name;
}

TRSGroup* StepConverter::createGroupNode(const TDF_Label& assembly, TopLoc_Location parentLocation)
{
    TDF_LabelSequence components;
    if (!m_shapeTool->GetComponents(assembly, components))
    {
        return nullptr;
    }

    TRSGroup* group = new TRSGroup;
    TopLoc_Location currentLocation = m_shapeTool->GetLocation(assembly);
    TopLoc_Location localLocation = parentLocation * currentLocation;
    int componentCount = components.Length();
    for (Standard_Integer compIndex = 1; compIndex <= componentCount; ++compIndex)
    {
        TDF_Label childLabel = components.Value(compIndex);
        TDF_Label referredShape;
        if (m_shapeTool->GetReferredShape(childLabel, referredShape))
        {
            TopLoc_Location instanceLocation = m_shapeTool->GetLocation(childLabel);
            TopLoc_Location finalLocation = localLocation * instanceLocation;
            if (m_shapeTool->IsAssembly(referredShape))
            {
                TRSGroup* subGroup = createGroupNode(referredShape, finalLocation);
                if (!subGroup)
                {
                    continue;
                }
                group->addChild(std::shared_ptr<TRSGroup>(subGroup));
            }
            else if(m_shapeTool->IsSimpleShape(referredShape))
            {
                TRSGeode* geode = createGeodeNode(referredShape, finalLocation);
                if (!geode)
                {
                    continue;
                }
                group->addChild(std::shared_ptr<TRSGeode>(geode));
            }
            else
            {
                assert(false);
            }
        }
        else if (m_shapeTool->IsAssembly(childLabel))
        {
            TRSGroup* subGroup = createGroupNode(childLabel, localLocation);
            if (!subGroup)
            {
                continue;
            }
            group->addChild(std::shared_ptr<TRSGroup>(subGroup));
        }
        else if(m_shapeTool->IsSimpleShape(childLabel))
        {
            TRSGeode* geode = createGeodeNode(childLabel, localLocation);
            if (!geode)
            {
                continue;
            }
            group->addChild(std::shared_ptr<TRSGeode>(geode));
        }
        else
        {
            assert(false);
        }
    }
    if (0 == group->childNum())
    {
        delete group;
        return nullptr;
    }
    return group;
}

TRSGeode* StepConverter::createGeodeNode(const TDF_Label& shapeLabel, TopLoc_Location parentLocation)
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
    TopLoc_Location currentLocation = m_shapeTool->GetLocation(shapeLabel);
    TopLoc_Location localLocation = parentLocation * currentLocation;
    TRSMesh* shadedMesh = geode->getShadedMesh();
    populateShadedMesh(occShape, shadedMesh, localLocation);
    TRSMesh* wireMesh = geode->useWireframeMesh();
    populateWireframeMesh(occShape, wireMesh, localLocation);
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

bool StepConverter::referToAssembly(const TDF_Label& Label, TDF_Label& referredShape)
{
    if (m_shapeTool->IsReference(Label))
    {
        if (m_shapeTool->GetReferredShape(Label, referredShape))
        {
            bool isAssembly = m_shapeTool->IsAssembly(referredShape);
            return isAssembly;
        }
    }
    return false;
}

void StepConverter::populateShadedMesh(const TopoDS_Shape& topo_shape, TRSMesh* mesh, TopLoc_Location parentLocation)
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
        TopLoc_Location faceLocation;
        TopoDS_Face aFace = TopoDS::Face(faceExplorer.Current());
        Handle_Poly_Triangulation triFace = BRep_Tool::Triangulation(aFace, faceLocation);
        TopLoc_Location localLocation =  parentLocation * faceLocation;
        if (triFace.IsNull())
        {
            continue;
        }

        int  nNodes = triFace->NbNodes();
        bool hasNormal = triFace->HasNormals();
        bool hasUV = triFace->HasUVNodes();
        for (int i = 1; i <= nNodes; i++)
        {
            gp_Pnt vertex = triFace->Node(i);
            gp_Pnt vertex1 = vertex.Transformed(localLocation.Transformation());
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

void StepConverter::populateWireframeMesh(const TopoDS_Shape& topo_shape, TRSMesh* mesh, TopLoc_Location parentLocation)
{
    TopExp_Explorer edgeExplorer;
    std::vector<TRSPoint> edgeVertexs;
    std::vector<unsigned int>  edgeIndexs;
    int nodeLen = 0;
    for (edgeExplorer.Init(topo_shape, TopAbs_EDGE); edgeExplorer.More(); edgeExplorer.Next())
    {
        TopLoc_Location lineLocation;
        TopoDS_Edge occEdge = TopoDS::Edge(edgeExplorer.Current());
        if (!BRep_Tool::IsGeometric(occEdge))
        {
            continue;
        }
        auto orientation = occEdge.Orientation();
        if (orientation == TopAbs_REVERSED)
        {
            continue;
        }
        BRepMesh_IncrementalMesh tempMesh(occEdge, GlobalLineDeflection);
        Handle_Poly_Polygon3D polyline = BRep_Tool::Polygon3D(occEdge, lineLocation);
        TopLoc_Location localLocation = parentLocation * lineLocation;
        int nodeNum = polyline->NbNodes();
        for (int i = 1; i <= nodeNum; i++)
        {
            gp_Pnt point = polyline->Nodes().Value(i).Transformed(localLocation.Transformation());
            TRSPoint vertex = toTRSVec(point);
            edgeVertexs.push_back(vertex);
        }
        for (int i = 0; i < nodeNum - 1; i++)
        {
            edgeIndexs.push_back(i + nodeLen);
            edgeIndexs.push_back(i + nodeLen + 1);
        }

        nodeLen += nodeNum;
    }
    mesh->setVertex(edgeVertexs);
    mesh->setIndices(edgeIndexs);
}

void StepConverter::populatePointsMesh(const TopoDS_Shape& topo_shape, TRSMesh* mesh, TopLoc_Location parentLocation)
{
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

