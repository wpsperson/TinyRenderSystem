#pragma once

#include <vector>

#include "TRS/TRSExport.h"
#include "TRS/TRSNode.h"
#include "TRS/TRSVector.h"
#include <TRS/TRSDefEnum.h>


class TRSTexture;
class TRSMesh;
class TRSViewer;

class TRS_EXPORT TRSGeode : public TRSNode
{
public:
    TRSGeode();

    ~TRSGeode();

    NodeType nodeType() const override;

    virtual void initialize(TRSViewer *viewer);

    virtual void draw(RenderMode mode);

    virtual void preProcess(RenderMode mode);

    virtual void postProcess(RenderMode mode);

    TRSBox boundingBox() const override;

    void copyShadedMesh(TRSMesh* mesh);

    TRSMesh* useShadedMesh();

    TRSMesh* useWireframeMesh();

    TRSMesh* usePointsMesh();

    TRSMesh* getComponentMesh(RenderMode mode) const;

    void setTexture(std::shared_ptr<TRSTexture> texture);

    TRSTexture* getTexture() const;

    void setShadedVertice(const std::vector<TRSPoint>& vertexs);

    void setShadedNormals(const std::vector<TRSVec3>& normals);

    void setShadedUVs(const std::vector<TRSVec2>& UVs);

    void setShadedIndice(const std::vector<unsigned int>& indices);

    void setPolygonMode(int polyMode);

    void setRenderMode(int renderMode);

    void addRenderMode(RenderMode mode);

    void removeRenderMode(RenderMode mode);

    bool hasRenderMode(RenderMode mode) const;

    bool hasComponent(RenderMode mode) const;

    std::string debugInfo() override;

protected:
    int m_polygonMode;
    int m_renderMode = RenderMode::Shaded;
    std::shared_ptr<TRSTexture> m_pTexture;
    TRSMesh* m_shaded = nullptr;
    TRSMesh* m_wireframe = nullptr;
    TRSMesh* m_points = nullptr;
};

