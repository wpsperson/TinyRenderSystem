/*!  
 *@brief    VAO
 *@author   wangps
 *@date     2019��5��3��
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

    //��ȡVBO
    unsigned int getVBO() const;
    //����VBO
    void setVBO(unsigned int vbo);
    //Buff������
    EnVertexStruct getBufferType() const;
    //����Buff������
    void setBuffType(EnVertexStruct buffType);
    //�������������
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
    EnVertexStruct m_EnVertType;///<��������ĸ�ʽ����
    int m_nDrawCount;           ///<���������������
    int m_nElementCount;        ///<��������Ļ�������
};


