/*!  
 *@brief    相机
 *@author   wangps
 *@date     2019年5月3日
 */

#pragma once
#include "Core\TRSExport.h"
#include "Math\TRSMatrix.h"
#include "Math\TRSVector.h"
#include "Math\TRSMatrix.h"

class TRS_EXPORT TRSCamera
{
public:
    TRSCamera();
    ~TRSCamera();
    // get
    const TRSVec3& getPosition() const;
    const TRSVec3& getLookAt() const;
    const TRSVec3& getUp() const;
    const TRSVec3& getFront() const;
    const TRSVec3& getRight() const;
    TRSMatrix getViewMatrix();
    float getNear();
    float getFar();
    double getWindowWidth() const;
    double getWindowHeight() const;
    TRSMatrix getProjectMatrix();

    // set
    void setPosition(const TRSVec3& pos);
    void setLookAt(const TRSVec3& lookAt);
    void setUp(const TRSVec3& up);
    void setNear(float near);
    void setFar(float far);
    void setCameraMode(bool parallelMode);
    void setWindowWidth(double width);
    void setWindowHeight(double height);

    // operate
    // keep lookat unchanged, rotate camera position about the m_up direction
    void Azimuth(double angle);
    // keep lookat unchanged, rotate camera position about the m_right direction
    void Elevation(double angle);

protected:
    void updateViewMatrix();
    void updateProjectMatrix();
protected:
    // view matrix parameter
    TRSVec3 m_pos;        //相机当前位置
    TRSVec3 m_lookAt;       // the center position that camera is looking at.
    TRSVec3 m_front;      //相机的前向方向，相机当前位置指向观察中心点的向量
    TRSVec3 m_right;      //相机的右手方向，即局部坐标系的X轴向在世界坐标系中的向量
    TRSVec3 m_up;         //相机的头顶方向，即局部坐标系的Y轴向在世界坐标系中的向量
    TRSMatrix m_viewMatrix;
    bool m_viewMatrixNeedUpdate = true;
    // project matrix parameter
    float m_width;            // keep the aspect with window change
    float m_height;           // keep the aspect with window change
    float m_near;
    float m_far;
    float m_fFov;           // vertical angle, perspective mode only
    bool m_parallelMode;    // whether it is parallel project or perspective project.
    TRSMatrix m_projectMatrix;
    bool m_projectMatrixNeedUpdate = true;
};

