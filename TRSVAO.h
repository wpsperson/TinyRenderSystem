/*!  
 *@brief    VAO
 *@author   wangps
 *@date     2019Äê5ÔÂ3ÈÕ
 */

#pragma once
#include "TRSExport.h"
#include "TRSConst.h"

class TRS_EXPORT TRSVAO
{
public:
    TRSVAO();
    ~TRSVAO();
    //rule: 0 aVert 1 aTexture 2 Color
    void createVAO(float* vertices, int verticeSize, EnVertexStruct EnVertType);

    void bind();

    int getDrawCount();

protected:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;


    int m_nDrawCount;
};


