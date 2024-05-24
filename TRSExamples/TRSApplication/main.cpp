#include <QtWidgets/QApplication>
#include "OpenGLWidget.h"
#include "ImportStep.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    ImportStep importor;
    importor.readStepFile("");
    OpenGLWidget* dialog = new OpenGLWidget;
    dialog->show();
    return a.exec();
}
