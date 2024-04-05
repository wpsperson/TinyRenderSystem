#pragma once
#include "TRS/TRSEventHandler.h"
#include "TRS/TRSBox.h"

class TRSCamera;

class TRS_EXPORT TRSDefaultCameraHandler : public TRSEventHandler
{
public:
    TRSDefaultCameraHandler(TRSCamera* camera);
    ~TRSDefaultCameraHandler();
    void setSceneBox(const TRSBox& box);
    void processWindowSizeChange(int width, int height) override;
    void processLeftMousePress(int xpos, int ypos, int mods) override;
    void processMiddleMousePress(int xpos, int ypos, int mods) override;
    void processRightMousePress(int xpos, int ypos, int mods) override;
    void processLeftMouseMove(int xpos, int ypos, int mods) override;
    void processMiddleMouseMove(int xpos, int ypos, int mods) override;
    void processRightMouseMove(int xpos, int ypos, int mods) override;
    void processMouseScroll(int scroll);
    void processKeyPress(int key) override;
protected:
    void updateNearFar();

protected:
    TRSBox m_sceneBox;
    TRSCamera* m_camera;
    float m_MoveSpeed = 0.05f;
    int m_lastX = 0;
    int m_lastY = 0;
};


