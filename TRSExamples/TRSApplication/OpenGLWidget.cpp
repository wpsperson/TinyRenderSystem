#include "OpenGLWidget.h"

#include <iostream>

#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QFileDialog>

#include "TRS/TRSViewer.h"
#include "TRS/TRSEventDispatcher.h"
#include "TRS/TRSDefaultCameraHandler.h"
#include "TRS/TRSCamera.h"
#include "TRS/TRSGeode.h"
#include "TRS/TRSGroup.h"


OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{

}

OpenGLWidget::~OpenGLWidget()
{
    if (m_pCameraHandler)
    {
        delete m_pCameraHandler;
    }
    if (m_dispatcher)
    {
        delete m_dispatcher;
    }
    if (m_viewer)
    {
        delete m_viewer;
    }
}

void OpenGLWidget::setScene(std::shared_ptr<TRSNode> sceneGroup)
{
    m_viewer->setSecenNode(sceneGroup);
    TRSBox box = sceneGroup->boundingBox();
    m_viewer->getCamera()->fitToBox(box);
    m_pCameraHandler->setSceneBox(box);
}

void OpenGLWidget::initializeGL()
{
    m_viewer = new TRSViewer;
    if (!m_viewer->loadOpenGL())
    {
        return;
    }
    m_viewer->initialViewer();
    m_dispatcher = new TRSEventDispatcher;
    m_pCameraHandler = new TRSDefaultCameraHandler(m_viewer->getCamera());
    m_dispatcher->addEventHandler(m_pCameraHandler);

    //// initialize some scene
    //TRSCube* cube = new TRSCube;
    //TRSMesh* mesh = cube->useShadedMesh();
    //std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();
    //pNode->copyShadedMesh(mesh);
    //pSS->getTexture()->createTexture("resources/textures/opengl.png");
    //pSS->getTexture()->createTexture("resources/textures/cube.png");
    //pSS->getShader()->createProgram("shaders/PosColorTexMVPVertex.glsl", "shaders/MultiTextureFragment.glsl");

    //std::shared_ptr<TRSGroup> root = std::make_shared<TRSGroup>();
    //root->addChild(pNode);

    //m_viewer->setSecenNode(root);
    //m_pCameraHandler->setSceneBox(root->boundingBox());
}

void OpenGLWidget::resizeGL(int width, int height)
{
    m_width = width;
    m_height = height;
    m_viewer->getCamera()->setWindowWidth(width);
    m_viewer->getCamera()->setWindowHeight(height);
}

void OpenGLWidget::paintGL()
{
    m_viewer->frame();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    int delta = event->angleDelta().y();
    int scroll = delta > 0 ? 1 : -1;
    m_dispatcher->dispatchMouseScroll(scroll);
    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    int modifiers = static_cast<int>(event->modifiers());
    m_dispatcher->dispatchKeyPress(key, modifiers);
    update();
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    int key = event->key();
    int modifiers = static_cast<int>(event->modifiers());
    m_dispatcher->dispatchKeyRelease(key, modifiers);
    update();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    int x = event->pos().x();
    int y = m_height - event->pos().y();
    Qt::MouseButtons btns = event->buttons();
    Qt::MouseButton btn = event->button();
    int modifiers = static_cast<int>(event->modifiers());
    if (Qt::MouseButton::LeftButton == btn)
    {
        m_dispatcher->dispatchLeftMousePress(x, y, modifiers);
        update();
    }
    if (Qt::MouseButton::MiddleButton == btn)
    {
        m_dispatcher->dispatchMiddleMousePress(x, y, modifiers);
        update();
    }
    if (Qt::MouseButton::RightButton == btn)
    {
        m_dispatcher->dispatchRightMousePress(x, y, modifiers);
        update();
    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    int x = event->pos().x();
    int y = m_height - event->pos().y();
    Qt::MouseButtons btns = event->buttons();
    Qt::MouseButton btn = event->button();
    int modifiers = static_cast<int>(event->modifiers());
    if (Qt::MouseButton::LeftButton == btn)
    {
        m_dispatcher->dispatchLeftMouseRelease(x, y, modifiers);
        update();
    }
    if (Qt::MouseButton::MiddleButton == btn)
    {
        m_dispatcher->dispatchMiddleMouseRelease(x, y, modifiers);
        update();
    }
    if (Qt::MouseButton::RightButton == btn)
    {
        m_dispatcher->dispatchRightMouseRelease(x, y, modifiers);
        update();
    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    int x = event->pos().x();
    int y = m_height - event->pos().y();
    m_dispatcher->dispatchMouseMove(x, y);
    update();
}
