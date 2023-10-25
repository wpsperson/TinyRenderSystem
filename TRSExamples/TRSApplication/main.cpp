#include <QtWidgets/QApplication>
#include "OpenGLWidget.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    OpenGLWidget* dialog = new OpenGLWidget;
    dialog->show();
    return a.exec();
}
