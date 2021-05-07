#include "TRSVAO.h"
#include <glad/glad.h>
#include "TRSConst.h"


TRSVAO::TRSVAO()
    :VAO(0), VBO(0), EBO(0), m_nDrawCount(0), m_nElementCount(0)
{

}

TRSVAO::~TRSVAO()
{
    if (VAO)
    {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBO)
    {
        glDeleteBuffers(1, &VBO);
    }
    if (EBO)
    {
        glDeleteBuffers(1, &EBO);
    }
}


//rule: 0 aVert 1 aTexture 2 Color
void TRSVAO::createVAO(float* vertices, int verticeSize, EnVertexStruct EnVertType)
{
    genVAO(true);
    createVBO(vertices, verticeSize);
    setVertexAttrib(EnVertType);
    calcDrawCount(EnVertType, verticeSize);
    unBind();
}

void TRSVAO::createVAO(float* vertices, int verticeSize, EnVertexStruct EnVertType, unsigned int* indice, int indexCount)
{
    genVAO(true);
    createVBO(vertices, verticeSize);
    createEBO(indice, indexCount);
    setVertexAttrib(EnVertType);
    m_nElementCount = indexCount;
    unBind();
}

void TRSVAO::createVBO(float* vertices, int verticeSize)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticeSize * sizeof(float), vertices, GL_STATIC_DRAW);//注意这里数量的大小就是vertices的内存大小
}

void TRSVAO::createEBO(unsigned int* indice, int indexCount)
{
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount*sizeof(unsigned int), indice, GL_STATIC_DRAW);//注意这里数量的大小就是indice的内存大小
}

void TRSVAO::setVertexAttrib(EnVertexStruct EnVertType)
{
    m_EnVertType = EnVertType;
    switch (EnVertType)
    {
    case EnVertex:          // vvv;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        break;
    case EnVertexTexture:          // vvvtt;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        break;
    case EnVertexColorTexture:          // vvvccctt;
    case EnVertexNormTexture:       //vvvnnntt;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//Texture
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//Color
        glEnableVertexAttribArray(2);
        break;
    case EnVertexTextureColor:          // vvvttccc;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//Texture
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));//Color
        glEnableVertexAttribArray(2);
        break;
    case EnVertexNormal:          // vvvnnn;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        break;
    case EnAssimpFormat:        // vvvnnnttt't't'bbb
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
        break;
    default:
        break;
    }
}

void TRSVAO::calcDrawCount(EnVertexStruct EnVertType, int verticeSize)
{
    switch (EnVertType)
    {
    case EnVertex:          // vvv;
        m_nDrawCount = verticeSize / 3;
        break;
    case EnVertexTexture:          // vvvtt;
        m_nDrawCount = verticeSize / 5;
        break;
    case EnVertexColorTexture:          // vvvccctt;
    case EnVertexNormTexture:           //vvvnnntt;
        m_nDrawCount = verticeSize / 8;
        break;
    case EnVertexTextureColor:          // vvvttccc;
        m_nDrawCount = verticeSize / 8;
        break;
    case EnVertexNormal:          // vvvnnn;
        m_nDrawCount = verticeSize / 6;
        break;
    case EnAssimpFormat:        // vvvnnnttt't't'bbb
        m_nDrawCount = verticeSize / 14;
        break;
    default:
        break;
    }
}

unsigned int TRSVAO::getVBO() const
{
    return VBO;
}

EnVertexStruct TRSVAO::getBufferType() const
{
    return m_EnVertType;
}

void TRSVAO::setBuffType(EnVertexStruct buffType)
{
    m_EnVertType = buffType;
    setVertexAttrib(m_EnVertType);
}

int TRSVAO::getDrawType()
{
    return m_nDrawType;
}

void TRSVAO::setDrawType(int drawType)
{
    m_nDrawType = drawType;
}

int TRSVAO::getDrawParam()
{
    return m_nPatchParam;
}

void TRSVAO::setDrawParam(int param)
{
    m_nPatchParam = param;
}

void TRSVAO::setVBO(unsigned int vbo)
{
    VBO = vbo;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void TRSVAO::bind()
{
    glBindVertexArray(VAO);
}

void TRSVAO::unBind()
{
    glBindVertexArray(0);
}

int TRSVAO::getDrawCount() const
{
    return m_nDrawCount;
}

int TRSVAO::getElementCount() const
{
    return m_nElementCount;
}

void TRSVAO::setDrawCount(int nCount)
{
    m_nDrawCount = nCount;
}

void TRSVAO::genVAO(bool bBind /*= true*/)
{
    glGenVertexArrays(1, &VAO);
    if (bBind)
    {
        glBindVertexArray(VAO);
    }
}

unsigned int TRSVAO::getVAO()
{
    return VAO;
}
