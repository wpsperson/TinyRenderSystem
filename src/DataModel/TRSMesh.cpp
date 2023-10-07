#include "DataModel\TRSMesh.h"
#include "Math\TRSVector.h"
#include "Core\TRSVAO.h"
#include <glad/glad.h>


TRSMesh::TRSMesh()
    : m_nDrawType(GL_TRIANGLES)
{
    m_vao = new TRSVAO();
}

TRSMesh::~TRSMesh()
{
    delete m_vao;
    if (m_uploadData)
    {
        delete[] m_uploadData;
        m_uploadData = nullptr;
    }
}

void TRSMesh::setVertex(const std::vector<TRSVec3>& vertexs)
{
    m_vertexs = vertexs;
    m_dirty = true;
}

void TRSMesh::setNormal(const std::vector<TRSVec3>& normals)
{
    m_normals = normals;
    m_dirty = true;
}

void TRSMesh::setColor(const std::vector<TRSVec3>& colors)
{
    m_colors = colors;
    m_dirty = true;
}

void TRSMesh::setUV(const std::vector<TRSVec2>& uv)
{
    m_texCoords = uv;
    m_dirty = true;
}

void TRSMesh::setIndices(const std::vector<unsigned int>& indices)
{
    m_indexs = indices;
    m_dirty = true;
}

int TRSMesh::getDrawCount() const
{
    return static_cast<int>(m_vertexs.size());
}

int TRSMesh::getElementCount() const
{
    return static_cast<int>(m_indexs.size());
}

int TRSMesh::getDrawType()
{
    return m_nDrawType;
}

void TRSMesh::setDrawType(int drawType)
{
    if (drawType == GL_LINE)
    {
        throw std::exception("GL_LINE is not a valid primitive type! use GL_LINES!");
    }
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

void TRSMesh::meshBind()
{
    uploadMesh();
    m_vao->bindVAO();
}

void TRSMesh::uploadMesh()
{
    if (m_dirty)
    {
        if (!checkValid())
        {
            throw "TRSMesh::updateMesh invalid";
        }
        upload();
        m_dirty = false;
    }
}

std::vector<TRSVec3> TRSMesh::convertToVec3Array(float* arr, int count)
{
    std::vector<TRSVec3> result;
    int result_count = count / 3;
    result.reserve(result_count);
    TRSVec3 pt;
    for (int ii = 0; ii < result_count; ii++)
    {
        pt[0] = arr[ii * 3 + 0];
        pt[1] = arr[ii * 3 + 1];
        pt[2] = arr[ii * 3 + 2];
        result.emplace_back(pt);
    }
    return result;
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
    combineMeshData();
    m_vao->uploadVBO(m_uploadData, m_uploadSize);
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
bool TRSMesh::combineMeshData()
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
    m_uploadSize = ptNum * unitSize;
    if (m_uploadData)
    {
        delete[] m_uploadData;
        m_uploadData = nullptr;
    }
    m_uploadData = new float[m_uploadSize];


    float* dst = m_uploadData;
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
    return true;
}
