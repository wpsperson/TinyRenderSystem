#pragma once
#include "TRSEventHandler.h"

class TRSCamera;

class TRSDefaultCameraHandler : public TRSEventHandler
{
public:
    TRSDefaultCameraHandler(TRSCamera* camera);
    ~TRSDefaultCameraHandler();
    void processWindowSizeChange(int width, int height) override;
    void processLeftMousePress(double xpos, double ypos, int mods) override;
    void processMiddleMousePress(double xpos, double ypos, int mods) override;
    void processRightMousePress(double xpos, double ypos, int mods) override;
    void processLeftMouseMove(double xpos, double ypos, int mods) override;
    void processMiddleMouseMove(double xpos, double ypos, int mods) override;
    void processRightMouseMove(double xpos, double ypos, int mods) override;
    void processMouseScroll(double xScroll, double yScroll);
    void processKeyPress(int key) override;
protected:
    // to do Scene Box is needed.
    TRSCamera* m_camera;
    float m_MoveSpeed = 0.05f;
    float m_lastX = 0.0f;
    float m_lastY = 0.0f;
};


