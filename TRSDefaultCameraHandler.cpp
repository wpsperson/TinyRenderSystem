#include "TRSDefaultCameraHandler.h"
#include "TRSCamera.h"
#include "TRSEventDefine.h"
#include <glad/glad.h>
#include "TRSUtils.h"


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
    m_lastX = xpos;
    m_lastY = ypos;
}

void TRSDefaultCameraHandler::processMiddleMousePress(double xpos, double ypos, int mods)
{
    m_lastX = xpos;
    m_lastY = ypos;
}

void TRSDefaultCameraHandler::processRightMousePress(double xpos, double ypos, int mods)
{
    m_lastX = xpos;
    m_lastY = ypos;
}

void TRSDefaultCameraHandler::processLeftMouseMove(double xpos, double ypos, int mods)
{
    float curX = xpos;
    float curY = ypos;
    float deltaX = (curX - m_lastX) / m_camera->getWindowWidth();
    float delatY = (curY - m_lastY) / m_camera->getWindowHeight();
    float azimuthAngle = - deltaX * TRS_PI * 2; // deltaX forward right, rotate is clockwise.
    float elevatAngle = - delatY * TRS_PI * 2;    // deltaY forward down, rotate is clockwise.
    m_camera->Azimuth(azimuthAngle);
    m_camera->Elevation(elevatAngle);

    m_lastX = curX;
    m_lastY = curY;
}

void TRSDefaultCameraHandler::processMiddleMouseMove(double xpos, double ypos, int mods)
{
    float curX = xpos;
    float curY = ypos;
    float deltaX = (curX - m_lastX) / m_camera->getWindowWidth();
    float delatY = (curY - m_lastY) / m_camera->getWindowHeight();
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
    const float coffScroll = 0.1;
    TRSVec3 front = m_camera->getFront();
    TRSVec3 pos = m_camera->getPosition();
    TRSVec3 lookAt = m_camera->getLookAt();
    float dist = lookAt.distance(pos);
    float scrollDist = dist * coffScroll * yScroll;
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
    if (key == TRS_KEY_W)
    {
        move += front * m_MoveSpeed;
    }
    else if (key == TRS_KEY_S)
    {
        move -= front * m_MoveSpeed;
    }
    else if (key == TRS_KEY_A)
    {
        move -= right * m_MoveSpeed;
    }
    else if (key == TRS_KEY_D)
    {
        move += right * m_MoveSpeed;
    }
    else if (key == TRS_KEY_Q)
    {
        move += up * m_MoveSpeed;
    }
    else if (key == TRS_KEY_E)
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
