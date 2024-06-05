#pragma once

#include <vector>

#include "TRS/TRSExport.h"
#include "TRS/TRSNode.h"
#include "TRS/TRSVector.h"
#include <TRS/TRSDefEnum.h>


class TRSTexture;
class TRSMesh;

class TRS_EXPORT TRSGeode : public TRSNode
{
public:
    TRSGeode();

    ~TRSGeode();

    void draw() override;

    TRSBox boundingBox() const override;

    void copyShadedMesh(TRSMesh* mesh);

    TRSMesh* getShadedMesh()const;

    TRSMesh* useWireframeMesh();

    TRSMesh* usePointsMesh();

    void setTexture(std::shared_ptr<TRSTexture> texture);

    TRSTexture* getTexture() const;

    void setShadedVertice(const std::vector<TRSPoint>& vertexs);

    void setShadedNormals(const std::vector<TRSVec3>& normals);

    void setShadedUVs(const std::vector<TRSVec2>& UVs);

    void setShadedIndice(const std::vector<unsigned int>& indices);

    void setActive() override;

    void setPolygonMode(int polyMode);

    void setRenderMode(int renderMode);

    void addRenderMode(RenderMode mode);

    void removeRenderMode(RenderMode mode);

    bool hasRenderMode(RenderMode mode);

    std::string debugInfo() override;

    void preProcess() override;

    void postProcess() override;


protected:
    int m_polygonMode;
    int m_renderMode = RenderMode::Shaded;
    std::shared_ptr<TRSTexture> m_pTexture;
    TRSMesh* m_shaded = nullptr;
    TRSMesh* m_wireframe = nullptr;
    TRSMesh* m_points = nullptr;
};

