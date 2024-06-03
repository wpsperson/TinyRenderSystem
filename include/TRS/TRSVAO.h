#pragma once

#include "TRS/TRSExport.h"
#include "TRS/TRSConst.h"

// interleave vertex attribute to organize the vertex data in VBO
class TRS_EXPORT TRSVAO
{
public:
    TRSVAO();
    ~TRSVAO();

    void bindVAO();

    void unBindVAO();

    void createVAO();

    void createVBO();

    void uploadVBO(float* vertices, int verticeSize);

    void createEBO();

    void uploadEBO(unsigned int* indice, int indexCount);

    void setVertexAttrib(int MeshStructs);

    void deleteOldBuffer();

protected:
    unsigned int m_vao = 0;
    unsigned int m_elementBuffer = 0;
    unsigned int m_vertexBuffer = 0;
};


