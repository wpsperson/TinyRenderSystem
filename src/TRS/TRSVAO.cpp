#include "TRS/TRSVAO.h"

#include <exception>

#include "TRS/TRSDefGL.h"
#include "TRS/TRSMesh.h"


TRSVAO::TRSVAO()
{

}

TRSVAO::~TRSVAO()
{
    deleteOldBuffer();
}

void TRSVAO::createVAO()
{
    glGenVertexArrays(1, &m_vao);
}

void TRSVAO::bindVAO()
{
    glBindVertexArray(m_vao);
}

void TRSVAO::unBindVAO()
{
    glBindVertexArray(0);
}

void TRSVAO::createVBO()
{
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
}

void TRSVAO::uploadVBO(float* vertices, int verticeSize)
{
    glBufferData(GL_ARRAY_BUFFER, verticeSize * sizeof(float), vertices, GL_STATIC_DRAW);
}

void TRSVAO::createEBO()
{
    glGenBuffers(1, &m_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
}

void TRSVAO::uploadEBO(unsigned int* indice, int indexCount)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indice, GL_STATIC_DRAW);
}

void TRSVAO::setVertexAttrib(int MeshStructs)
{
    int vertexAttribStride = TRSMesh::computeVertexAttribStride(MeshStructs);
    int stride = vertexAttribStride * sizeof(float);
    int offset = 0;
    if (MeshStructs & msVertex)
    {
        glVertexAttribPointer(VertexIndex, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(VertexIndex);
        offset += 3;
    }
    if (MeshStructs & msNormal)
    {
        glVertexAttribPointer(NormalIndex, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(NormalIndex);
        offset += 3;
    }
    if (MeshStructs & msUV)
    {
        glVertexAttribPointer(TextureCoordIndex0, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(TextureCoordIndex0);
        offset += 2;
    }
    if (MeshStructs & msColor)
    {
        glVertexAttribPointer(ColorIndex, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(ColorIndex);
        offset += 3;
    }
}

void TRSVAO::deleteOldBuffer()
{
    if (m_vao)
    {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    if (m_vertexBuffer)
    {
        glDeleteBuffers(1, &m_vertexBuffer);
        m_vertexBuffer = 0;
    }
    if (m_elementBuffer)
    {
        glDeleteBuffers(1, &m_elementBuffer);
        m_elementBuffer = 0;
    }
}

