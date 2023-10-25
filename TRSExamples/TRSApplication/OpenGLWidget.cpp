#include "OpenGLWidget.h"

#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>


OpenGLWidget::OpenGLWidget()
{

}

OpenGLWidget::~OpenGLWidget()
{

}

void OpenGLWidget::initializeGL()
{

}

void OpenGLWidget::resizeGL(int w, int h)
{
    m_width = w;
    m_height = h;
}

void OpenGLWidget::paintGL()
{

}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{

    QWidget::mousePressEvent(event);
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget::mouseReleaseEvent(event);
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{

    QWidget::mouseMoveEvent(event);
}
