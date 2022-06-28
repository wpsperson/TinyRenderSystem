/*!  
 *@brief    represent a mesh
 *@author   wangps
 *@date     2022-6-25
 */

#pragma once
#include "TRSExport.h"
#include <vector>
#include "TRSVector.h"
#include "TRSConst.h"

class TRSVAO;

class TRS_EXPORT TRSMesh
{
public:
    TRSMesh();
    ~TRSMesh();
    void setVertex(const std::vector<TRSVec3>& vertexs);
    void setNormal(const std::vector<TRSVec3>& normals);
    void setColor(const std::vector<TRSVec3>& colors);
    void setUV(const std::vector<TRSVec2>& uv);
    void setIndices(const std::vector<unsigned int>& indices);

    //绘制数组的数量
    int getDrawCount() const;
    int getElementCount() const;
    int getDrawType();
    void setDrawType(int drawType);
    int getDrawParam();
    void setDrawParam(int param);
    void meshBind();

    void updateMesh();

    static int computeVertexAttribStride(int vertexStructType);

protected:
    bool checkValid();

    void upload();

    bool combineMeshData();

protected:

private:
    std::vector<TRSVec3> m_vertexs;
    std::vector<TRSVec3> m_normals;
    std::vector<TRSVec3> m_colors;
    std::vector<TRSVec2> m_texCoords;
    std::vector<unsigned int> m_indexs;
    bool m_dirty = false;
    // temp
    TRSVAO* m_vao = nullptr;

    int m_vertexStructType = msVertex;
    float* m_uploadData = nullptr;
    int m_uploadSize = 0;
    int m_normalStartPos = 0;
    int m_uvStartPos = 0;
    int m_colorStartPos = 0;

    int m_nDrawType;                ///<绘制类型例如 GL_TRIANGLES GL_POINTS GL_LINES GL_PATCHES
    int m_nPatchParam;              ///<绘制类型为GL_PATCHES时，表示碎片的参数
};
#pragma once



