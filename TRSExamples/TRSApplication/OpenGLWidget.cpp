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
#include "TRS/TRSTexture.h"
#include "TRS/TRSTextNode.h"
#include "TRS/TRSDynamicText.h"
#include "TRS/TRSCube.h"
#include "TRS/TRSAxis.h"


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

void OpenGLWidget::setScene(TRSNode* sceneGroup)
{
    makeCurrent();
    m_viewer->setSecenNode(sceneGroup);
    doneCurrent();
    TRSBox box = sceneGroup->boundingBox();
    m_viewer->getCamera()->fitToBox(box);
    m_pCameraHandler->setSceneBox(box);
}

void OpenGLWidget::setupDemo()
{
    //TRSCube cube;
    //TRSMesh* mesh = cube.getMesh();
    //TRSGeode* pNode = new TRSGeode;
    //pNode->copyShadedMesh(mesh);
    //TRSTexture* texture = pNode->useTexture();
    //texture->createTexture("resources/textures/opengl.png");
    //texture->createTexture("resources/textures/cube.png");

    //std::wstring text(L"中华人民共和国");
    //TRSTextNode* textNode = new TRSTextNode;
    //textNode->setText(text);
    //textNode->setPos(G_ORIGIN);
    //textNode->setDir(G_XDIR);

    //TRSDynamicText* dytext = new TRSDynamicText;
    //dytext->setOrigin(TRSPoint(0.5f, 0.5f, 0.5f));
    //dytext->setFontSize(0.2f);
    //dytext->setText("ABCD");

    TRSAxis* axis = new TRSAxis;
    axis->setSizeInfo(1.0f, 0.05f, 0.3f, 0.125f);

    // TRSGroup* root = new TRSGroup;
    // root->addChild(pNode);
    // root->addChild(textNode);
    // root->addChild(dytext);
    setScene(axis);
    update();
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
