#include "OpenGLWidget.h"

#include <iostream>

#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QFileDialog>

#include "Core/TRSViewer.h"
#include "Core/TRSStateSet.h"
#include "Event/TRSEventDispatcher.h"
#include "Camera/TRSDefaultCameraHandler.h"
#include "Core\TRSConst.h"
#include "Core\TRSTexture.h"
#include "Core\TRSShader.h"
#include "Core\TRSBox.h"
#include "DataModel\TRSGeode.h"
#include "DataModel\TRSGroup.h"
#include "Util\TRSResource.h"
#include "Math\TRSMathUtil.h"
#include "Geometry\TRSCube.h"


OpenGLWidget::OpenGLWidget()
{

}

OpenGLWidget::~OpenGLWidget()
{
    TRSStateSetManager::free();
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

    // initialize some scene
    TRSCube* cube = new TRSCube;
    TRSMesh* mesh = cube->getMesh();
    std::shared_ptr<TRSGeode> pNode = std::make_shared<TRSGeode>();
    pNode->setMesh(mesh);
    std::shared_ptr<TRSStateSet> pSS = pNode->getOrCreateStateSet();
    pSS->getTexture()->createTexture("resources/textures/opengl.png");
    pSS->getTexture()->createTexture("resources/textures/cube.png");
    pSS->getShader()->createProgram("shaders/PosColorTexMVPVertex.glsl", "shaders/MultiTextureFragment.glsl");

    std::shared_ptr<TRSGroup> root = std::make_shared<TRSGroup>();
    root->addChild(pNode);

    m_viewer->setSecenNode(root);
    m_pCameraHandler->setSceneBox(root->boundingBox());
}

void OpenGLWidget::resizeGL(int w, int h)
{
    m_width = w;
    m_height = h;
}

void OpenGLWidget::paintGL()
{
    m_viewer->frame();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    int delta = event->angleDelta().y();
    int yscroll = delta > 0 ? 1 : -1;
    m_dispatcher->dispatchMouseScroll(double(0), yscroll);
    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    m_dispatcher->dispatchKeyPress(key);
    update();
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    int key = event->key();
    m_dispatcher->dispatchKeyRelease(key);
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
