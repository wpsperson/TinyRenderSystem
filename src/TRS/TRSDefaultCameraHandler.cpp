#include "TRS/TRSDefaultCameraHandler.h"

#include "TRS/TRSDefGL.h"
#include "TRS/TRSDefMath.h"
#include "TRS/TRSCamera.h"
#include "TRS/TRSEventDefine.h"
#include "TRS/TRSUtils.h"


TRSDefaultCameraHandler::TRSDefaultCameraHandler(TRSCamera* camera)
    :m_camera(camera)
{

}

TRSDefaultCameraHandler::~TRSDefaultCameraHandler()
{

}

void TRSDefaultCameraHandler::setSceneBox(const TRSBox& box)
{
    m_sceneBox = box;
    updateNearFar();
}

void TRSDefaultCameraHandler::processWindowSizeChange(int width, int height)
{
    if (height < 2)
    {
        height = 1;
    }
    m_camera->setWindowWidth(width);
    m_camera->setWindowHeight(height);
    glViewport(0, 0, width, height);
}

void TRSDefaultCameraHandler::processLeftMousePress(int xpos, int ypos, int mods)
{
    m_lastX = xpos;
    m_lastY = ypos;
}

void TRSDefaultCameraHandler::processMiddleMousePress(int xpos, int ypos, int mods)
{
    m_lastX = xpos;
    m_lastY = ypos;
}

void TRSDefaultCameraHandler::processRightMousePress(int xpos, int ypos, int mods)
{
    m_lastX = xpos;
    m_lastY = ypos;
}

void TRSDefaultCameraHandler::processLeftMouseMove(int xpos, int ypos, int mods)
{
    double deltaX = static_cast<double>(xpos - m_lastX) / m_camera->getWindowWidth();
    double delatY = static_cast<double>(ypos - m_lastY) / m_camera->getWindowHeight();
    double azimuthAngle = - deltaX * kM2PI; // deltaX forward right, rotate is clockwise.
    double elevatAngle = delatY * kM2PI;    // deltaY forward down, rotate is clockwise.
    m_camera->Azimuth(azimuthAngle);
    m_camera->Elevation(elevatAngle);

    m_lastX = xpos;
    m_lastY = ypos;
    updateNearFar();
}

void TRSDefaultCameraHandler::processMiddleMouseMove(int xpos, int ypos, int mods)
{
    float deltaX = static_cast<float>(xpos - m_lastX) / m_camera->getWindowWidth();
    float delatY = static_cast<float>(ypos - m_lastY) / m_camera->getWindowHeight();
    TRSVec3 pos = m_camera->getPosition();
    TRSVec3 lookAt = m_camera->getLookAt();
    TRSVec3 right = m_camera->getRight();
    TRSVec3 up = m_camera->getUp();
    float dist = lookAt.distance(pos);
    TRSVec3 move = right * -deltaX * dist + up * -delatY * dist;

    pos += move;
    m_camera->setPosition(pos);
    lookAt += move;
    m_camera->setLookAt(lookAt);
    m_lastX = xpos;
    m_lastY = ypos;
    updateNearFar();
}

void TRSDefaultCameraHandler::processRightMouseMove(int xpos, int ypos, int mods)
{

}

void TRSDefaultCameraHandler::processMouseScroll(int scroll)
{
    //scroll (value is 1/-1) increase, fov decrease, then scene looks bigger;
    const float coffScroll = 0.1f;
    TRSVec3 front = m_camera->getFront();
    TRSVec3 pos = m_camera->getPosition();
    TRSVec3 lookAt = m_camera->getLookAt();
    float dist = lookAt.distance(pos);
    float scrollDist = static_cast<float>(dist * coffScroll * scroll);
    pos += front * scrollDist;
    m_camera->setPosition(pos);
    updateNearFar();
}

void TRSDefaultCameraHandler::processKeyPress(int key)
{
    TRSVec3 front = m_camera->getFront();
    TRSVec3 right = m_camera->getRight();
    TRSVec3 up = m_camera->getUp();
    TRSVec3 move(0, 0, 0);
    if (key == TRS_KEY_UP)
    {
        move += front * m_MoveSpeed;
    }
    else if (key == TRS_KEY_DOWN)
    {
        move -= front * m_MoveSpeed;
    }
    else if (key == TRS_KEY_LEFT)
    {
        move -= right * m_MoveSpeed;
    }
    else if (key == TRS_KEY_RIGHT)
    {
        move += right * m_MoveSpeed;
    }
    else if (key == TRS_KEY_PAGE_UP)
    {
        move += up * m_MoveSpeed;
    }
    else if (key == TRS_KEY_PAGE_DOWN)
    {
        move -= up * m_MoveSpeed;
    }
    if (move != G_ORIGIN)
    {
        TRSVec3 pos = m_camera->getPosition();
        TRSVec3 lookAt = m_camera->getLookAt();
        pos += move;
        lookAt += move;
        m_camera->setPosition(pos);
        m_camera->setLookAt(lookAt);
    }
}

void TRSDefaultCameraHandler::updateNearFar()
{
    const TRSPoint& pos = m_camera->getPosition();
    TRSPoint center = m_sceneBox.center();
    float distance = (pos - center).length();
    float radius = m_sceneBox.outSphereRadius();
    float far = (distance + radius) * 1.5f;
    float near = far * 0.001f;
    m_camera->setFar(far);
    m_camera->setNear(near);
}
