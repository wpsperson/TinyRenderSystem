#include "TRS/TRSAxis.h"

#include "TRS/TRSDefConst.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSDynamicText.h"
#include "TRS/TRSMesh.h"

constexpr int kAxisResolution = 16;

TRSAxis::TRSAxis()
{
    m_xCylinder = new TRSGeode;
    m_yCylinder = new TRSGeode;
    m_zCylinder = new TRSGeode;
    m_xCone = new TRSGeode;
    m_yCone = new TRSGeode;
    m_zCone = new TRSGeode;
    m_xlabel = new TRSDynamicText;
    m_ylabel = new TRSDynamicText;
    m_zlabel = new TRSDynamicText;
    addChild(m_xCylinder);
    addChild(m_yCylinder);
    addChild(m_zCylinder);
    addChild(m_xCone);
    addChild(m_yCone);
    addChild(m_zCone);
    addChild(m_xlabel);
    addChild(m_ylabel);
    addChild(m_zlabel);
}

TRSAxis::~TRSAxis()
{

}

void TRSAxis::initialize(TRSViewer* viewer)
{
    TRSMesh* mesh0 = m_xCylinder->useShadedMesh();
    TRSMesh* mesh1 = m_yCylinder->useShadedMesh();
    TRSMesh* mesh2 = m_zCylinder->useShadedMesh();
    TRSMesh* mesh3 = m_xCone->useShadedMesh();
    TRSMesh* mesh4 = m_yCone->useShadedMesh();
    TRSMesh* mesh5 = m_zCone->useShadedMesh();
    buildCylinderMesh(G_YDIR, G_ZDIR, mesh0);
    buildCylinderMesh(-G_XDIR, G_ZDIR, mesh1);
    buildCylinderMesh(G_XDIR, G_YDIR, mesh2);
    buildConeMesh(G_YDIR, G_ZDIR, mesh3);
    buildConeMesh(-G_XDIR, G_ZDIR, mesh4);
    buildConeMesh(G_XDIR, G_YDIR, mesh5);
    m_xlabel->setText("X");
    m_ylabel->setText("Y");
    m_zlabel->setText("Z");
    float fontSize = m_conelen;
    float coord = m_cylen + m_conelen + fontSize / 2;
    m_xlabel->setOrigin(TRSPoint(coord, 0.0f, 0.0f));
    m_ylabel->setOrigin(TRSPoint(0.0f, coord, 0.0f));
    m_zlabel->setOrigin(TRSPoint(0.0f, 0.0f, coord));
    m_xlabel->setFontSize(fontSize);
    m_ylabel->setFontSize(fontSize);
    m_zlabel->setFontSize(fontSize);

    TRSVec4 red(1.0f, 0.0f, 0.0f, 1.0f);
    TRSVec4 green(0.0f, 1.0f, 0.0f, 1.0f);
    TRSVec4 blue(0.0f, 0.0f, 1.0f, 1.0f);
    m_xCylinder->setColor(red);
    m_yCylinder->setColor(green);
    m_zCylinder->setColor(blue);
    m_xCone->setColor(red);
    m_yCone->setColor(green);
    m_zCone->setColor(blue);
    m_xlabel->setColor(red);
    m_ylabel->setColor(green);
    m_zlabel->setColor(blue);

    TRSGroup::initialize(viewer);
}

TRSBox TRSAxis::boundingBox() const
{
    TRSBox result(G_ORIGIN, m_cylen + m_conelen);
    return result;
}

void TRSAxis::setSizeInfo(float cylen, float cyRadius, float conelen, float coneRadius)
{
    m_cylen = cylen;
    m_cyRadius = cyRadius;
    m_conelen = conelen;
    m_coneRadius = coneRadius;
}

void TRSAxis::buildCylinderMesh(const TRSVec3& dirx, const TRSVec3& diry, TRSMesh* mesh)
{
    std::vector<TRSVec3> vertices;
    std::vector<TRSVec3> normals;
    std::vector<unsigned int> indices;
    vertices.reserve(kAxisResolution * 4 + 2);
    normals.reserve(kAxisResolution * 4 + 2);
    indices.reserve(kAxisResolution * 12);

    // Top plane
    TRSVec3 dirz = dirx.cross(diry);
    TRSPoint top = dirz * m_cylen;
    vertices.emplace_back(top);
    normals.emplace_back(dirz);
    float delta = kM2PIf / kAxisResolution;
    for (int idx = 0; idx < kAxisResolution; idx++)
    {
        float angle = idx * delta;
        TRSPoint point = dirx * (std::cos(angle) * m_cyRadius) + diry * (std::sin(angle) * m_cyRadius) + dirz * m_cylen;
        vertices.emplace_back(point);
        normals.emplace_back(dirz);
    }
    // bottom plane
    TRSPoint bottom = G_ORIGIN;
    vertices.emplace_back(bottom);
    normals.emplace_back(-dirz);
    for (int idx = 0; idx < kAxisResolution; idx++)
    {
        float angle = idx * delta;
        TRSPoint point = dirx * (std::cos(angle) * m_cyRadius) + diry * (std::sin(angle) * m_cyRadius);
        vertices.emplace_back(point);
        normals.emplace_back(-dirz);
    }
    // lateral plane
    for (int idx = 0; idx < kAxisResolution; idx++)
    {
        float angle = idx * delta;
        TRSPoint point = dirx * (std::cos(angle) * m_cyRadius) + diry * (std::sin(angle) * m_cyRadius);
        TRSPoint normal = dirx * std::cos(angle) + diry * std::sin(angle);
        vertices.emplace_back(point);
        normals.emplace_back(normal);
    }
    for (int idx = 0; idx < kAxisResolution; idx++)
    {
        float angle = idx * delta;
        TRSPoint point = dirx * (std::cos(angle) * m_cyRadius) + diry * (std::sin(angle) * m_cyRadius) + dirz * m_cylen;
        TRSPoint normal = dirx * std::cos(angle) + diry * std::sin(angle);
        vertices.emplace_back(point);
        normals.emplace_back(normal);
    }
    // indices
    unsigned int offset = 1;
    for (int idx = 0; idx < kAxisResolution; idx++)
    {
        unsigned int next = idx + 1;
        if (next == kAxisResolution)
        {
            next = 0;
        }
        indices.emplace_back(0); // top center indice
        indices.emplace_back(idx + offset);
        indices.emplace_back(next + offset);
    }
    offset = kAxisResolution + 2;
    for (int idx = 0; idx < kAxisResolution; idx++)
    {
        unsigned int next = idx + 1;
        if (next == kAxisResolution)
        {
            next = 0;
        }
        indices.emplace_back(kAxisResolution + 1); // bottom center indice
        indices.emplace_back(idx + offset);
        indices.emplace_back(next + offset);
    }
    offset = kAxisResolution * 2 + 2;
    unsigned int topOffset = kAxisResolution * 3 + 2;
    for (int idx = 0; idx < kAxisResolution; idx++)
    {
        unsigned int next = idx + 1;
        if (next == kAxisResolution)
        {
            next = 0;
        }
        indices.emplace_back(idx + offset);
        indices.emplace_back(next + offset);
        indices.emplace_back(next + topOffset);
        indices.emplace_back(next + topOffset);
        indices.emplace_back(idx + topOffset);
        indices.emplace_back(idx + offset);
    }
    mesh->setVertex(vertices);
    mesh->setNormal(normals);
    mesh->setIndices(indices);
}

void TRSAxis::buildConeMesh(const TRSVec3& dirx, const TRSVec3& diry, TRSMesh* mesh)
{
    std::vector<TRSVec3> vertices;
    std::vector<TRSVec3> normals;
    std::vector<unsigned int> indices;
    vertices.reserve(kAxisResolution * 2 + 2);
    normals.reserve(kAxisResolution * 2 + 2);
    indices.reserve(kAxisResolution * 6);
    // sector plane
    TRSVec3 dirz = dirx.cross(diry);
    TRSPoint top = dirz * (m_cylen + m_conelen);
    vertices.emplace_back(top);
    normals.emplace_back(dirz);
    float delta = kM2PIf / kAxisResolution;
    for (int idx = 0; idx < kAxisResolution; idx++)
    {
        float angle = idx * delta;
        TRSPoint point = dirx * (std::cos(angle) * m_coneRadius) + diry * (std::sin(angle) * m_coneRadius) + dirz * m_cylen;
        TRSPoint normal = dirx * (std::cos(angle) * m_conelen) + diry * (std::sin(angle) * m_conelen) + dirz * m_coneRadius;
        vertices.emplace_back(point);
        normals.emplace_back(normal);
    }
    // bottom plane
    TRSPoint center = dirz * m_cylen;
    vertices.emplace_back(center);
    normals.emplace_back(-dirz);
    for (int idx = 0; idx < kAxisResolution; idx++)
    {
        float angle = idx * delta;
        TRSPoint point = dirx * (std::cos(angle) * m_coneRadius) + diry * (std::sin(angle) * m_coneRadius) + dirz * m_cylen;
        vertices.emplace_back(point);
        normals.emplace_back(-dirz);
    }
    // indices
    unsigned int offset = 1;
    for (int idx = 0; idx < kAxisResolution; idx++)
    {
        unsigned int next = idx + 1;
        if (next == kAxisResolution)
        {
            next = 0;
        }
        indices.emplace_back(0); // top center indice
        indices.emplace_back(idx + offset);
        indices.emplace_back(next + offset);
    }
    offset = kAxisResolution + 2;
    for (int idx = 0; idx < kAxisResolution; idx++)
    {
        unsigned int next = idx + 1;
        if (next == kAxisResolution)
        {
            next = 0;
        }
        indices.emplace_back(kAxisResolution + 1); // bottom center indice
        indices.emplace_back(idx + offset);
        indices.emplace_back(next + offset);
    }

    mesh->setVertex(vertices);
    mesh->setNormal(normals);
    mesh->setIndices(indices);
}
