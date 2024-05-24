#include "MainWindow.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QListWidget>

#include "OpenGLWidget.h"
#include "ImportStep.h"

MainWindow::MainWindow(QWidget *parent /*= nullptr*/)
    :QMainWindow(parent)
{
    m_canvas = new OpenGLWidget(this);
    setCentralWidget(m_canvas);

    createActions();
    createMenus();
    createDockWidgets();

    resize(800, 600);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions()
{
    m_loadStepAction = new QAction(tr("&New"), this);
    connect(m_loadStepAction, SIGNAL(triggered()), this, SLOT(onLoadStep()));
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_loadStepAction);
}

void MainWindow::createDockWidgets()
{
    QDockWidget* dock = new QDockWidget(tr("Navigator"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QListWidget* customerList = new QListWidget(dock);
    customerList->addItems(QStringList()
        << "Hello"
        << "World"
        << "Good"
        << "Morning");
    dock->setWidget(customerList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}


void MainWindow::onLoadStep()
{
    // TODO
}
