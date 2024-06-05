#include "TRS/TRSGeode.h"

#include <iostream>

#include "TRS/TRSDefGL.h"
#include "TRS/TRSMesh.h"
#include "TRS/TRSShader.h"
#include "TRS/TRSTexture.h"


TRSGeode::TRSGeode()
    : TRSNode()
    , m_polygonMode(GL_FILL)
{
    m_shaded = new TRSMesh;
}

TRSGeode::~TRSGeode()
{
    delete m_shaded;
    if (m_wireframe)
    {
        delete m_wireframe;
    }
    if (m_points)
    {
        delete m_points;
    }
}

void TRSGeode::setActive()
{
    if (hasComponent(RenderMode::Shaded))
    {
        m_shaded->uploadOnce();
    }
    if (hasComponent(RenderMode::WireFrame))
    {
        m_wireframe->uploadOnce();
    }
    if (hasComponent(RenderMode::Points))
    {
        m_points->uploadOnce();
    }
}

void TRSGeode::preProcess(RenderMode mode)
{
    if (RenderMode::Shaded == mode)
    {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f); // more away to camera.
    }
    else if (RenderMode::WireFrame == mode)
    {
    }
    else
    {
        glPointSize(4.0);
    }

    if (DrawType::PATCHES == m_shaded->getDrawType())
    {
        glPatchParameteri(GL_PATCH_VERTICES, m_shaded->getDrawParam());
    }
    if (m_polygonMode != GL_FILL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
    }
}

void TRSGeode::draw(RenderMode mode)
{
    if (RenderMode::Shaded == mode)
    {
        m_shaded->bindMesh();
        m_shaded->drawMesh();
    }
    else if (RenderMode::WireFrame == mode)
    {
        m_wireframe->bindMesh();
        m_wireframe->drawMesh();
    }
    else if (RenderMode::Points == mode)
    {
        m_points->bindMesh();
        m_points->drawMesh();
    }
}

void TRSGeode::postProcess(RenderMode mode)
{
    if (RenderMode::Shaded == mode)
    {
        glDisable(GL_POLYGON_OFFSET_FILL);
    }
    else if (RenderMode::WireFrame == mode)
    {
    }
    else
    {

    }
    if (m_polygonMode != GL_FILL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

TRSBox TRSGeode::boundingBox() const
{
    return m_shaded->boundingBox();
}

void TRSGeode::copyShadedMesh(TRSMesh* mesh)
{
    m_shaded->copyMesh(mesh);
}

TRSMesh* TRSGeode::useShadedMesh()
{
    if (!m_shaded)
    {
        m_shaded = new TRSMesh;
        m_shaded->setDrawType(DrawType::TRIANGLES);
    }
    return m_shaded;
}

TRSMesh* TRSGeode::useWireframeMesh()
{
    if (!m_wireframe)
    {
        m_wireframe = new TRSMesh;
        m_wireframe->setDrawType(DrawType::LINES);
    }
    return m_wireframe;
}

TRSMesh* TRSGeode::usePointsMesh()
{
    if (!m_points)
    {
        m_points = new TRSMesh;
        m_points->setDrawType(DrawType::POINTS);
    }
    return m_points;
}

TRSMesh* TRSGeode::getComponentMesh(RenderMode mode) const
{
    if (RenderMode::Shaded == mode)
    {
        return m_shaded;
    }
    else if (RenderMode::WireFrame == mode)
    {
        return m_wireframe;
    }
    else
    {
        return m_points;
    }
}

void TRSGeode::setTexture(std::shared_ptr<TRSTexture> texture)
{
    m_pTexture = texture;
}

TRSTexture* TRSGeode::getTexture() const
{
    return m_pTexture.get();
}

void TRSGeode::setShadedVertice(const std::vector<TRSPoint>& vertexs)
{
    m_shaded->setVertex(vertexs);
}

void TRSGeode::setShadedNormals(const std::vector<TRSVec3>& normals)
{
    m_shaded->setNormal(normals);
}

void TRSGeode::setShadedUVs(const std::vector<TRSVec2>& UVs)
{
    m_shaded->setUV(UVs);
}

void TRSGeode::setShadedIndice(const std::vector<unsigned int>& indices)
{
    m_shaded->setIndices(indices);
}

void TRSGeode::setPolygonMode(int polyMode)
{
    m_polygonMode = polyMode;
}

void TRSGeode::setRenderMode(int renderMode)
{
    m_renderMode = renderMode;
}

void TRSGeode::addRenderMode(RenderMode mode)
{
    if (!hasRenderMode(mode))
    {
        m_renderMode = (m_renderMode | mode);
    }
}

void TRSGeode::removeRenderMode(RenderMode mode)
{
    if (hasRenderMode(mode))
    {
        m_renderMode = (m_renderMode ^ mode);
    }
}

bool TRSGeode::hasRenderMode(RenderMode mode) const
{
    return m_renderMode & mode;
}

bool TRSGeode::hasComponent(RenderMode mode) const
{
    // first check the mode
    if (!hasRenderMode(mode))
    {
        return false;
    }
    // second check mesh exist.
    if (RenderMode::Shaded == mode)
    {
        return (m_shaded != nullptr);
    }
    else if (RenderMode::WireFrame == mode)
    {
        return (m_wireframe != nullptr);;
    }
    else
    {
        return (m_points != nullptr);
    }
}

std::string TRSGeode::debugInfo()
{
    std::string strDebugInfo;
    if (TRSTexture* pTexture = getTexture())
    {
        strDebugInfo += pTexture->debugInfo();
    }
    return strDebugInfo;
}
