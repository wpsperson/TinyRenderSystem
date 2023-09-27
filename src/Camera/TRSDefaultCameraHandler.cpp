#include "Camera\TRSDefaultCameraHandler.h"
#include "Camera\TRSCamera.h"
#include "Event\TRSEventDefine.h"
#include <glad/glad.h>
#include "Util\TRSUtils.h"


TRSDefaultCameraHandler::TRSDefaultCameraHandler(TRSCamera* camera)
    :m_camera(camera)
{

}

TRSDefaultCameraHandler::~TRSDefaultCameraHandler()
{

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

void TRSDefaultCameraHandler::processLeftMousePress(double xpos, double ypos, int mods)
{
    m_lastX = static_cast<float>(xpos);
    m_lastY = static_cast<float>(ypos);
}

void TRSDefaultCameraHandler::processMiddleMousePress(double xpos, double ypos, int mods)
{
    m_lastX = static_cast<float>(xpos);
    m_lastY = static_cast<float>(ypos);
}

void TRSDefaultCameraHandler::processRightMousePress(double xpos, double ypos, int mods)
{
    m_lastX = static_cast<float>(xpos);
    m_lastY = static_cast<float>(ypos);
}

void TRSDefaultCameraHandler::processLeftMouseMove(double xpos, double ypos, int mods)
{
    double curX = xpos;
    double curY = ypos;
    double deltaX = (curX - m_lastX) / m_camera->getWindowWidth();
    double delatY = (curY - m_lastY) / m_camera->getWindowHeight();
    double azimuthAngle = - deltaX * TRS_PI * 2; // deltaX forward right, rotate is clockwise.
    double elevatAngle = - delatY * TRS_PI * 2;    // deltaY forward down, rotate is clockwise.
    m_camera->Azimuth(azimuthAngle);
    m_camera->Elevation(elevatAngle);

    m_lastX = static_cast<float>(curX);
    m_lastY = static_cast<float>(curY);
}

void TRSDefaultCameraHandler::processMiddleMouseMove(double xpos, double ypos, int mods)
{
    float curX = static_cast<float>(xpos);
    float curY = static_cast<float>(ypos);
    float deltaX = static_cast<float>((curX - m_lastX) / m_camera->getWindowWidth());
    float delatY = static_cast<float>((curY - m_lastY) / m_camera->getWindowHeight());
    TRSVec3 pos = m_camera->getPosition();
    TRSVec3 lookAt = m_camera->getLookAt();
    TRSVec3 right = m_camera->getRight();
    TRSVec3 up = m_camera->getUp();
    float dist = lookAt.distance(pos);
    TRSVec3 move = right * -deltaX * dist + up * delatY * dist;

    pos += move;
    m_camera->setPosition(pos);
    lookAt += move;
    m_camera->setLookAt(lookAt);
    m_lastX = curX;
    m_lastY = curY;
}

void TRSDefaultCameraHandler::processRightMouseMove(double xpos, double ypos, int mods)
{

}

void TRSDefaultCameraHandler::processMouseScroll(double xScroll, double yScroll)
{
    //yScroll (value is 1/-1) increase, fov decrease, then scene looks bigger;
    const float coffScroll = 0.1f;
    TRSVec3 front = m_camera->getFront();
    TRSVec3 pos = m_camera->getPosition();
    TRSVec3 lookAt = m_camera->getLookAt();
    float dist = lookAt.distance(pos);
    float scrollDist = static_cast<float>(dist * coffScroll * yScroll);
    pos += front * scrollDist;
    m_camera->setPosition(pos);
    // todo if dist is too small, move look At?
    if (dist < 0.001)
    {

    }
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
