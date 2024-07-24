#pragma once
#include "TRS/TRSExport.h"
#include "TRS/TRSMatrix.h"
#include "TRS/TRSVector.h"
#include "TRS/TRSMatrix.h"

enum class ProjectionMode : unsigned char
{
    Parallel = 0,
    Perspective = 1,
};

class TRSBox;
class TRS_EXPORT TRSCamera
{
public:
    TRSCamera();
    ~TRSCamera();
    // get
    const TRSVec3& getPosition() const;
    const TRSVec3& getLookAt() const;
    const TRSVec3& getUp() const;
    const TRSVec3& getRight() const;
    TRSVec3 getFront() const;
    TRSMatrix getViewMatrix();
    float getNear() const;
    float getFar() const;
    float cameraWidth() const;
    int getWindowWidth() const;
    int getWindowHeight() const;
    ProjectionMode projectionMode() const;
    TRSMatrix getProjectMatrix();

    // set
    void setPosition(const TRSVec3& pos);
    void setLookAt(const TRSVec3& lookAt);
    void setUp(const TRSVec3& up);
    void setNear(float near);
    void setFar(float far);
    void setCameraWidth(float value);
    void setProjectionMode(ProjectionMode mode);
    void setWindowWidth(int width);
    void setWindowHeight(int height);

    // operate
    // keep lookat unchanged, rotate camera position about the m_up direction
    void Azimuth(double angle);
    // keep lookat unchanged, rotate camera position about the m_right direction
    void Elevation(double angle);
    // set lookup at center of box, and adjust pos to make camera see entire box
    void fitToBox(const TRSBox& box);

protected:
    void updateViewMatrix();
    void updateProjectMatrix();
protected:
    // view matrix parameter
    TRSVec3 m_pos;          // camera position
    TRSVec3 m_lookAt;       // the center position that camera is looking at.
    TRSVec3 m_right;        // camera right direction
    TRSVec3 m_up;           // camera up direction
    TRSMatrix m_viewMatrix;
    bool m_viewMatrixNeedUpdate = true;
    // project matrix parameter
    int m_width;            // keep the aspect with window change
    int m_height;           // keep the aspect with window change
    float m_cameraWidth;    // only used in parallel mode. frustrum width.
    float m_near;
    float m_far;
    float m_fFov;           // vertical angle, perspective mode only
    ProjectionMode m_projectionMode;    // whether it is parallel project or perspective project.
    TRSMatrix m_projectMatrix;
    bool m_projectMatrixNeedUpdate = true;
};

