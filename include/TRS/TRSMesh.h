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
    void generateNormals();
    const TRSBox& boundingBox() const;
    int getDrawCount() const;
    int getElementCount() const;
    int getMeshStruct() const;
    DrawType getDrawType();
    void setDrawType(DrawType drawType);
    int getDrawParam();
    void setDrawParam(int param);

    void bindMesh();

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
    int m_vertexStructType = msVertex;
    DrawType m_nDrawType;
    int m_nPatchParam;
};
#pragma once



