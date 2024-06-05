#include "TRS/TRSMesh.h"

#include <map>
#include <vector>

#include "TRS/TRSDefGL.h"
#include "TRS/TRSVector.h"
#include "TRS/TRSVAO.h"



TRSMesh::TRSMesh()
    : m_nDrawType(DrawType::TRIANGLES)
{
    m_vao = new TRSVAO();
}

TRSMesh::~TRSMesh()
{
    delete m_vao;
}

void TRSMesh::setVertex(const std::vector<TRSVec3>& vertexs)
{
    m_vertexs = vertexs;
    updateBoundingBox();
    m_needUpload = true;
}

void TRSMesh::setNormal(const std::vector<TRSVec3>& normals)
{
    m_normals = normals;
    m_needUpload = true;
}

void TRSMesh::setColor(const std::vector<TRSVec3>& colors)
{
    m_colors = colors;
    m_needUpload = true;
}

void TRSMesh::setUV(const std::vector<TRSVec2>& uv)
{
    m_texCoords = uv;
    m_needUpload = true;
}

void TRSMesh::setIndices(const std::vector<unsigned int>& indices)
{
    m_indexs = indices;
    m_needUpload = true;
}

void TRSMesh::copyMesh(TRSMesh* rhs)
{
    m_vertexs = rhs->m_vertexs;
    m_normals = rhs->m_normals;
    m_colors = rhs->m_colors;
    m_texCoords = rhs->m_texCoords;
    updateBoundingBox();
    m_needUpload = true;
}

void TRSMesh::generateNormals()
{
    if (m_vertexs.empty())
    {
        return;
    }
    m_normals.reserve(m_vertexs.size());
    if (m_indexs.empty())
    {
        for (std::size_t idx = 0; idx < m_vertexs.size(); idx += 3)
        {
            const TRSPoint& pta = m_vertexs[idx + 0];
            const TRSPoint& ptb = m_vertexs[idx + 1];
            const TRSPoint& ptc = m_vertexs[idx + 2];
            TRSVec3 vec_ab = ptb - pta;
            TRSVec3 vec_ac = ptc - pta;
            TRSVec3 norm = vec_ab.cross(vec_ac);
            m_normals.emplace_back(norm);
            m_normals.emplace_back(norm);
            m_normals.emplace_back(norm);
        }
        return;
    }

    std::map<int, std::vector<TRSVec3>> vertexToFaceNormalsMap;
    int index1, index2, index3;
    for (size_t idx = 0; idx < m_indexs.size(); idx += 3)
    {
        index1 = m_indexs[idx];
        index2 = m_indexs[idx + 1];
        index3 = m_indexs[idx + 2];
        const TRSPoint& pta = m_vertexs[index1];
        const TRSPoint& ptb = m_vertexs[index2];
        const TRSPoint& ptc = m_vertexs[index3];
        TRSVec3 vec_ab = ptb - pta;
        TRSVec3 vec_ac = ptc - pta;
        TRSVec3 faceNormal = vec_ab.cross(vec_ac);

        vertexToFaceNormalsMap[index1].emplace_back(faceNormal);
        vertexToFaceNormalsMap[index2].emplace_back(faceNormal);
        vertexToFaceNormalsMap[index3].emplace_back(faceNormal);
    }

    for (int idx = 0; idx < m_vertexs.size(); idx++)
    {
        if (!vertexToFaceNormalsMap.count(idx))
        {
            m_normals.emplace_back(G_ZDIR);
            continue;
        }
        std::vector<TRSVec3>& normals = vertexToFaceNormalsMap[idx];
        TRSVec3 resultNormal(0, 0, 0);
        for (auto iter = normals.begin(); iter != normals.end(); iter++)
        {
            resultNormal = resultNormal + (*iter);
        }
        resultNormal.normalize();
        m_normals.emplace_back(resultNormal);
    }
}

const TRSBox& TRSMesh::boundingBox() const
{
    return m_boundingBox;
}

int TRSMesh::getDrawCount() const
{
    return static_cast<int>(m_vertexs.size());
}

int TRSMesh::getElementCount() const
{
    return static_cast<int>(m_indexs.size());
}

int TRSMesh::getMeshStruct() const
{
    return m_vertexStructType;
}

DrawType TRSMesh::getDrawType()
{
    return m_nDrawType;
}

void TRSMesh::setDrawType(DrawType drawType)
{
    m_nDrawType = drawType;
}

int TRSMesh::getDrawParam()
{
    return m_nPatchParam;
}

void TRSMesh::setDrawParam(int param)
{
    m_nPatchParam = param;
}

void TRSMesh::bindMesh()
{
    if (m_needUpload)
    {
        if (!checkValid())
        {
            throw "TRSMesh::updateMesh invalid";
        }
        upload();
        m_needUpload = false;
    }
    m_vao->bindVAO();
}

int TRSMesh::computeVertexAttribStride(int vertexStructType)
{
    int vertexStructSize = 3; // default vertex coordinate.
    if (vertexStructType & msNormal)
    {
        vertexStructSize += 3;
    }
    if (vertexStructType& msUV)
    {
        vertexStructSize += 2;
    }
    if (vertexStructType& msColor)
    {
        vertexStructSize += 3;
    }
    return vertexStructSize;
}

bool TRSMesh::checkValid()
{
    std::size_t ptNum = m_vertexs.size();
    if (ptNum <=0)
    {
        return false;
    }
    std::size_t normalNum = m_normals.size();
    if (normalNum >0 && normalNum != ptNum)
    {
        return false;
    }
    std::size_t uvNum = m_texCoords.size();
    if (uvNum > 0 && uvNum != ptNum)
    {
        return false;
    }
    std::size_t colorNum = m_colors.size();
    if (colorNum > 0 && colorNum != ptNum)
    {
        return false;
    }
    return true;
}

void TRSMesh::upload()
{
    m_vao->deleteOldBuffer();
    m_vao->createVAO();
    m_vao->bindVAO();
    m_vao->createVBO();
    int bufferSize;
    float* buffer = combineMeshData(bufferSize);
    m_vao->uploadVBO(buffer, bufferSize);
    delete[]buffer;
    m_vao->setVertexAttrib(m_vertexStructType);
    if (!m_indexs.empty())
    {
        unsigned int* indice = &(m_indexs[0]);
        int indiceNum = static_cast<int>(m_indexs.size());
        m_vao->createEBO();
        m_vao->uploadEBO(indice, indiceNum);
    }
    m_vao->unBindVAO();
}

// we use interleaving method to combine the mesh data.
float * TRSMesh::combineMeshData(int& bufferSize)
{
    bool existNormal    = !m_normals.empty();
    bool existUV        = !m_texCoords.empty();
    bool existColor     = !m_colors.empty();
    if (existNormal)
    {
        m_vertexStructType |= msNormal;
    }
    if (existUV)
    {
        m_vertexStructType |= msUV;
    }
    if (existColor)
    {
        m_vertexStructType |= msColor;
    }

    // compute data size and allocate memory.
    int unitSize = computeVertexAttribStride(m_vertexStructType);
    int ptNum = static_cast<int>(m_vertexs.size());

    bufferSize = ptNum * unitSize;
    float *uploadData = new float[bufferSize];

    float* dst = uploadData;
    float* vertexSrc = reinterpret_cast<float*>(&(m_vertexs[0]));
    float* normalSrc = existNormal ? reinterpret_cast<float*>(&(m_normals[0])) : nullptr;
    float* texCoorSrc = existUV ? reinterpret_cast<float*>(&(m_texCoords[0])) : nullptr;
    float* colorSrc = existColor ? reinterpret_cast<float*>(&(m_colors[0])) : nullptr;
    for (int i=0; i<ptNum; i++)
    {
        memcpy(dst, vertexSrc, 3 * sizeof(float));
        vertexSrc += 3;
        dst += 3;
        if (existNormal)
        {
            memcpy(dst, normalSrc, 3 * sizeof(float));
            normalSrc += 3;
            dst += 3;
        }
        if (existUV)
        {
            memcpy(dst, texCoorSrc, 2 * sizeof(float));
            texCoorSrc += 2;
            dst += 2;
        }
        if (existColor)
        {
            memcpy(dst, colorSrc, 3 * sizeof(float));
            colorSrc += 3;
            dst += 3;
        }
    }
    return uploadData;
}

void TRSMesh::updateBoundingBox()
{
    for (const TRSPoint& pt : m_vertexs)
    {
        m_boundingBox.mergePoint(pt);
    }
}
