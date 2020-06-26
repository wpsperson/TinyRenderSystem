/*!  
 *@brief    VAO
 *@author   wangps
 *@date     2019年5月3日
 */

#pragma once
#include "TRSExport.h"
#include "TRSConst.h"

class TRS_EXPORT TRSVAO
{
public:
    TRSVAO();
    ~TRSVAO();

    void createVAO(float* vertices, int verticeSize, EnVertexStruct EnVertType);

    void createVAO(float* vertices, int verticeSize, EnVertexStruct EnVertType, unsigned int* indice, int indexCount);

    //获取VBO
    unsigned int getVBO() const;
    //设置VBO
    void setVBO(unsigned int vbo);
    //Buff的类型
    EnVertexStruct getBufferType() const;
    //设置Buff的类型
    void setBuffType(EnVertexStruct buffType);
    //绘制数组的数量
    int getDrawCount() const;
    int getElementCount() const;
    void setDrawCount(int nCount);

    void genVAO(bool bBind = true);
    unsigned int getVAO();
    void bind();

    void unBind();

protected:
    void createVBO(float* vertices, int verticeSize);

    void createEBO(unsigned int* indice, int indexCount);

    void setVertexAttrib(EnVertexStruct EnVertType);

    void calcDrawCount(EnVertexStruct EnVertType, int verticeSize);

protected:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    EnVertexStruct m_EnVertType;///<顶点数组的格式类型
    int m_nDrawCount;           ///<顶点数组绘制数量
    int m_nElementCount;        ///<索引数组的绘制数量
};


