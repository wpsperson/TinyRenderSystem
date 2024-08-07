#pragma once

#include <QtWidgets/QOpenGLWidget>

class TRSNode;
class TRSViewer;
class TRSSettings;
class TRSEventDispatcher;
class TRSDefaultCameraHandler;
class TRSShortcutKeyHandler;

class OpenGLWidget : public QOpenGLWidget
{
public:
    OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

    void setScene(TRSNode* sceneGroup);

    void setupDemo();

    TRSSettings* getSettings() const;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    int m_width;
    int m_height;
    TRSViewer* m_viewer = nullptr;
    TRSEventDispatcher* m_dispatcher = nullptr;
    TRSDefaultCameraHandler* m_pCameraHandler = nullptr;
};
