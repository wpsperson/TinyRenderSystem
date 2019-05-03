/*!  
 *@brief    VAO
 *@author   wangps
 *@date     2019��5��3��
 */

#pragma once
#include "TRSExport.h"

class TRS_EXPORT TRSVAO
{
public:
    enum EnVertexStruct
    {
        EnVertex,
        EnVertexTexture,
        EnVertexTextureColor,
        EnVertexColorTexture
    };
    TRSVAO();
    ~TRSVAO();
    //rule: 0 aVert 1 aTexture 2 Color
    void createVAO(float* vertices, int verticeSize, EnVertexStruct EnVertType);

    void bind();

protected:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;


};


