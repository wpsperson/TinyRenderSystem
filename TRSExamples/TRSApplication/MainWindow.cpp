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
    createStatusBar();

    resize(1400, 1000);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions()
{
    m_loadStepAction = new QAction(tr("ImportStep"), this);
    connect(m_loadStepAction, SIGNAL(triggered()), this, SLOT(onLoadStep()));
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("File"));
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

void MainWindow::createStatusBar()
{
    m_status = new QLabel("Ready");
    statusBar()->addWidget(m_status, 1);
}

void MainWindow::updateStatus(const QString& status)
{
    m_status->setText(status);
}

void MainWindow::onLoadStep()
{
    QString stepFile =  QFileDialog::getOpenFileName(this, QString("Load Step"), QString("."), tr("Step File (*.stp *.step)"));
    if (stepFile.isEmpty())
    {
        updateStatus("Please select a step file to load.");
        return;
    }
    std::string errorMessage;
    std::string strStepFile = stepFile.toStdString();
    const char *fileName = strStepFile.c_str();
    TRSNode* stepNode = ImportStep::readStepFile(fileName, errorMessage);
    if (!stepNode)
    {
        updateStatus(QString::fromStdString(errorMessage));
        return;
    }
    std::shared_ptr<TRSNode> root(stepNode);
    m_canvas->setScene(root);
    updateStatus("Load scene successfully.");
}
