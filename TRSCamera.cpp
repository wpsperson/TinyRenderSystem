#include "TRSCamera.h"
#include <iostream>
#include "TRSConst.h"
#include "TRSConfig.h"
#include "TRSMathUtil.h"

TRSCamera::TRSCamera()
{
    m_pos = s_DefaultCameraPos;
    m_lookAt = TRSVec3(0, 0, 0);
    m_front = TRSVec3(0, 0, -1);
    m_right = TRSVec3(1, 0, 0);
    m_up = TRSVec3(0, 1, 0);

    m_width = DefaultWindowWidth;
    m_height = DefaultWindowHeight;
    m_near = DefaultNearDistance;
    m_far = DefaultFarDistance;
    m_fFov = 45.0f;
    m_parallelMode = false;
}


TRSCamera::~TRSCamera()
{
}

TRSMatrix TRSCamera::getViewMatrix()
{
    if (m_viewMatrixNeedUpdate)
    {
        updateViewMatrix();
        m_viewMatrixNeedUpdate = false;
    }
    return m_viewMatrix;
}

double TRSCamera::getWindowWidth() const
{
    return m_width;
}

double TRSCamera::getWindowHeight() const
{
    return m_height;
}

TRSMatrix TRSCamera::getProjectMatrix()
{
    if (m_projectMatrixNeedUpdate)
    {
        updateProjectMatrix();
        m_projectMatrixNeedUpdate = false;
    }
    return m_projectMatrix;
}

TRSVec3 TRSCamera::getPosition() const
{
    return m_pos;
}

TRSVec3 TRSCamera::getLookAt() const
{
    return m_lookAt;
}

TRSVec3 TRSCamera::getFront() const
{
    return m_front;
}

TRSVec3 TRSCamera::getRight() const
{
    return m_right;
}

TRSVec3 TRSCamera::getUp() const
{
    return m_up;
}

void TRSCamera::setPosition(const TRSVec3& pos)
{
    if (m_pos != pos)
    {
        m_pos = pos;
        m_viewMatrixNeedUpdate = true;
    }
}

void TRSCamera::setLookAt(const TRSVec3& lookAt)
{
    if (m_lookAt != lookAt)
    {
        m_lookAt = lookAt;
        m_viewMatrixNeedUpdate = true;
    }
}

void TRSCamera::setUp(const TRSVec3& up)
{
    TRSVec3 normalizedUp = up;
    normalizedUp.normalize();
    if (m_up != normalizedUp)
    {
        m_up = normalizedUp;
        m_viewMatrixNeedUpdate = true;
    }
}

void TRSCamera::setCameraMode(bool parallelMode)
{
    m_parallelMode = parallelMode;
}

void TRSCamera::setWindowWidth(double width)
{
    if (m_width != width)
    {
        m_width = width;
        m_projectMatrixNeedUpdate = true;
    }
}

void TRSCamera::setWindowHeight(double height)
{
    if (m_height != height)
    {
        m_height = height;
        m_projectMatrixNeedUpdate = true;
    }
}

void TRSCamera::Azimuth(double angle)
{
    TRSMatrix transLookAtToOrigin;
    transLookAtToOrigin.makeTranslate(m_lookAt * -1);

    TRSMatrix rotateMatrix;
    rotateMatrix.makeRotate(angle, m_up);

    TRSMatrix transRestore;
    transRestore.makeTranslate(m_lookAt);

    TRSMatrix totalMatrix = transRestore * rotateMatrix * transLookAtToOrigin;
    TRSVec3 newPosition = totalMatrix * m_pos;
    this->setPosition(newPosition);
    updateViewMatrix();// in order to update the m_right
}

void TRSCamera::Elevation(double angle)
{
    TRSMatrix transLookAtToOrigin;
    transLookAtToOrigin.makeTranslate(m_lookAt * -1);

    TRSMatrix rotateMatrix;
    rotateMatrix.makeRotate(angle, m_right);

    TRSMatrix transRestore;
    transRestore.makeTranslate(m_lookAt);

    TRSMatrix totalMatrix = transRestore * rotateMatrix * transLookAtToOrigin;
    TRSVec3 newPosition = totalMatrix * m_pos;
    this->setPosition(newPosition);
    updateViewMatrix();// in order to update the m_up
}

void TRSCamera::updateViewMatrix()
{
    m_front = m_lookAt - m_pos;
    m_front.normalize();

    m_right = m_front.cross(m_up);
    m_right.normalize();
    // because front may be not perpendicular to up; so orthoganalize them.
    m_up = m_right.cross(m_front);
    m_up.normalize();

    m_viewMatrix.makeLookat(m_pos, m_front, m_up);
}

void TRSCamera::updateProjectMatrix()
{
    if (m_parallelMode)
    {
        m_projectMatrix.makeOtho(-m_width / 2, m_width / 2, -m_height / 2, m_height / 2, m_near, m_far);
    }
    else
    {
        m_projectMatrix.makePerspective(toRadian(m_fFov), m_width / m_height, m_near, m_far);
    }
}
