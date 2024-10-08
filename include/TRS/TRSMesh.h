#pragma once

#include <vector>

#include "TRS/TRSExport.h"
#include "TRS/TRSDefEnum.h"
#include "TRS/TRSVector.h"
#include "TRS/TRSBox.h"

class TRSVAO;

class TRS_EXPORT TRSMesh
{
public:
    TRSMesh();

    ~TRSMesh();

    void setVertex(const std::vector<TRSVec3>& vertexs);

    void setNormal(const std::vector<TRSVec3>& normals);

    void setColor(const std::vector<TRSVec3>& colors);

    void setUV(const std::vector<TRSVec2>& uv);

    void setIndices(const std::vector<unsigned int>& indices);

    std::vector<TRSVec3>& getVertices();

    std::vector<TRSVec3>& getNormals();

    std::vector<TRSVec3>& getColors();

    std::vector<TRSVec2>& getUVs();

    std::vector<unsigned int>& getIndices();

    void setNeedUpload();

    void reset();

    void copyMesh(TRSMesh* rhs);

    void generateNormals();

    const TRSBox& boundingBox() const;

    int getDrawCount() const;

    int getElementCount() const;

    int getMeshStruct() const;

    DrawType getDrawType();

    void setDrawType(DrawType drawType);

    int getDrawParam();

    void setDrawParam(int param);

    void uploadOnce();

    void bindMesh();

    void drawMesh();

    static int computeVertexAttribStride(int vertexStructType);

protected:
    bool checkValid();

    void upload();

    float *combineMeshData(int &bufferSize);

    void updateBoundingBox();

protected:

private:
    std::vector<TRSVec3> m_vertexs;
    std::vector<TRSVec3> m_normals;
    std::vector<TRSVec3> m_colors;
    std::vector<TRSVec2> m_texCoords;
    std::vector<unsigned int> m_indexs;
    TRSBox m_boundingBox;
    bool m_needUpload = false;

    TRSVAO* m_vao = nullptr;
    DrawType m_nDrawType;
    int m_nPatchParam = 0;
};



