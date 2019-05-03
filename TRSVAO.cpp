#include "TRSVAO.h"
#include <glad/glad.h>



TRSVAO::TRSVAO()
    :VAO(0), VBO(0), EBO(0)
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
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticeSize * sizeof(float), vertices, GL_STATIC_DRAW);
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//Texture
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//Color
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
    default:
        break;
    }

    glBindVertexArray(0);
}

void TRSVAO::bind()
{
    glBindVertexArray(VAO);
}
