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

void TRSGeode::draw()
{
    drawInternal();
}

TRSBox TRSGeode::boundingBox() const
{
    return m_shaded->boundingBox();
}

void TRSGeode::copyShadedMesh(TRSMesh* mesh)
{
    m_shaded->copyMesh(mesh);
}

TRSMesh* TRSGeode::getShadedMesh() const
{
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

void TRSGeode::setActive()
{
    m_shaded->uploadOnce();
    if (hasRenderMode(RenderMode::WireFrame) && m_wireframe)
    {
        m_wireframe->uploadOnce();
    }
    if (hasRenderMode(RenderMode::Points) && m_points)
    {
        m_points->uploadOnce();
    }
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

bool TRSGeode::hasRenderMode(RenderMode mode)
{
    return m_renderMode & mode;
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

void TRSGeode::preProcess()
{
    if (DrawType::PATCHES == m_shaded->getDrawType())
    {
        glPatchParameteri(GL_PATCH_VERTICES, m_shaded->getDrawParam());
    }

    if (m_polygonMode != GL_FILL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
    }
}

void TRSGeode::drawInternal()
{
    m_shaded->bindMesh();
    m_shaded->drawMesh();
    if (hasRenderMode(RenderMode::WireFrame) && m_wireframe)
    {
        m_wireframe->bindMesh();
        m_wireframe->drawMesh();
    }
    if (hasRenderMode(RenderMode::Points) && m_points)
    {
        m_points->bindMesh();
        m_points->drawMesh();
    }
}

void TRSGeode::postProcess()
{
    if (m_polygonMode != GL_FILL)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
