#include <QtWidgets/QApplication>

#include <string>

#include "MainWindow.h"

int main(int argc, char* argv[])
{
    // this must before QApplication constructor.
    // QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow mainwindow;
    mainwindow.show();
    return a.exec();
}
