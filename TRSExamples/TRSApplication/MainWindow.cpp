#include "MainWindow.h"

#include <QtCore/QTranslator>
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
#include "SettingDialog.h"
#include "ImportStep.h"

MainWindow::MainWindow(QWidget *parent /*= nullptr*/)
    :QMainWindow(parent)
{
    m_canvas = new OpenGLWidget(this);
    setCentralWidget(m_canvas);
    
    // m_mdiArea = new QMdiArea(this);
    // setCentralWidget(m_mdiArea);
    // m_mdiArea->setViewMode(QMdiArea::TabbedView);
	// m_mdiArea->setTabPosition(QTabWidget::TabPosition::South);
	// m_mdiArea->setTabShape(QTabWidget::TabShape::Rounded);
	// m_mdiArea->setTabsMovable(true);
	// m_mdiArea->setDocumentMode(true);
	// m_mdiArea->setTabsClosable(true);
	// connect(m_internals->m_mdiArea, &QMdiArea::subWindowActivated, this, &pmMainWindow::subWindowActivated);    
    // QMdiSubWindow *subwindow = m_mdiArea->addSubWindow(xxxWidget);
    // subwindow->setWindowTitle("first");

    setupDefaultTranslator();
    createActions();
    createMenus();
    createLanguageMenu();
    createDockWidgets();
    createStatusBar();
    retranslateUi();

    resize(1400, 1000);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupDefaultTranslator()
{
    appTranslator = new QTranslator(this);
    qtTranslator = new QTranslator(this);
    translatePath = QCoreApplication::applicationDirPath() + "/languages"; // qm file directory

    QString locale = QLocale::system().name();
    switchTranslator(locale);
}

void MainWindow::createActions()
{
    m_loadStepAction = new QAction(this);
    connect(m_loadStepAction, SIGNAL(triggered()), this, SLOT(onLoadStep()));
    m_loadStlAction = new QAction(this);
    connect(m_loadStlAction, SIGNAL(triggered()), this, SLOT(onLoadSTL()));
    m_demoAction = new QAction(this);
    connect(m_demoAction, &QAction::triggered, this, &MainWindow::onDemo);

    m_settingAction = new QAction(this);
    connect(m_settingAction, &QAction::triggered, this, &MainWindow::onSetting);
}

void MainWindow::createMenus()
{
    m_fileMenu = new QMenu(this);
    m_fileMenu->addAction(m_loadStepAction);
    m_fileMenu->addAction(m_loadStlAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_demoAction);
    menuBar()->addMenu(m_fileMenu);

    m_toolsMenu = new QMenu(this);
    m_toolsMenu->addAction(m_settingAction);
    menuBar()->addMenu(m_toolsMenu);

}

void MainWindow::createDockWidgets()
{
    m_naviDock = new QDockWidget(this);
    m_naviDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QListWidget* customerList = new QListWidget(m_naviDock);
    customerList->addItems(QStringList()
        << "Hello"
        << "World"
        << "Good"
        << "Morning");
    m_naviDock->setWidget(customerList);
    addDockWidget(Qt::RightDockWidgetArea, m_naviDock);
}

void MainWindow::createStatusBar()
{
    m_status = new QLabel(this);
    statusBar()->addWidget(m_status, 1);
}

void MainWindow::updateStatus(const QString& status)
{
    m_status->setText(status);
}

void MainWindow::retranslateUi()
{
    this->setWindowTitle(QCoreApplication::translate("MainWindow", "Tiny Render System"));
    m_fileMenu->setTitle(QCoreApplication::translate("MainWindow", "File"));
    m_loadStepAction->setText(QCoreApplication::translate("MainWindow", "Import Step"));
    m_loadStlAction->setText(QCoreApplication::translate("MainWindow", "Import STL"));
    m_demoAction->setText(QCoreApplication::translate("MainWindow", "Demo"));
    m_toolsMenu->setTitle(QCoreApplication::translate("MainWindow", "Tools"));
    m_settingAction->setText(QCoreApplication::translate("MainWindow", "Setting"));

    languageMenu->setTitle(QCoreApplication::translate("MainWindow", "Language"));

    m_naviDock->setWindowTitle(QCoreApplication::translate("MainWindow", "Navigate"));
    m_status->setText(QCoreApplication::translate("MainWindow", "Ready"));
}

void MainWindow::switchTranslator(const QString& locale)
{
    QString appQmFile = "app_" + locale + ".qm";
    QString qtQmFile = "qt_" + locale + ".qm";

    qApp->removeTranslator(qtTranslator);
    qApp->removeTranslator(appTranslator);
    if (QFile::exists(translatePath + "/" + qtQmFile))
    {
        qtTranslator->load(qtQmFile, translatePath);
        qApp->installTranslator(qtTranslator);
    }
    if (QFile::exists(translatePath + "/" + appQmFile))
    {
        appTranslator->load(appQmFile, translatePath);
        qApp->installTranslator(appTranslator);
    }
}

void MainWindow::createLanguageMenu()
{
    languageMenu = new QMenu(this);
    menuBar()->addMenu(languageMenu);

    QActionGroup* group = new QActionGroup(this);
    connect(group, SIGNAL(triggered(QAction*)), this, SLOT(onSwitchLanguage(QAction*)));

    QDir qmDir(translatePath);
    QStringList fileNames = qmDir.entryList(QStringList("app_*.qm"));

    for (int idx = 0; idx < fileNames.size(); idx++)
    {
        QString locale = fileNames[idx];
        locale.remove(0, locale.indexOf('_') + 1);
        locale.chop(3);

        QTranslator translator;
        translator.load(fileNames[idx], qmDir.absolutePath());

        QString text = translator.translate("MainWindow", "English");
        if (text.isEmpty())
        {
            text = "English";
        }

        QAction* action = new QAction;
        action->setData(locale);
        action->setCheckable(true);
        action->setText(tr("%1 %2").arg(idx + 1).arg(text));

        languageMenu->addAction(action);
        group->addAction(action);

        if (locale == QLocale::system().name())
        {
            action->setChecked(true);
        }
    }
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
    TRSNode* root = ImportStep::readStepFile(fileName, errorMessage);
    if (!root)
    {
        updateStatus(QString::fromStdString(errorMessage));
        return;
    }
    m_canvas->setScene(root);
    updateStatus("Load scene successfully.");
}

void MainWindow::onLoadSTL()
{
    QString stepFile = QFileDialog::getOpenFileName(this, QString("Load STL"), QString("."), tr("Obj File (*.obj *.OBJ);;STL File(*.stl *.stl);;FBX File(*.fbx *.FBX);;Other(*.*)"));
    if (stepFile.isEmpty())
    {
        updateStatus("Please select a step file to load.");
        return;
    }
    std::string errorMessage;
    std::string strStepFile = stepFile.toStdString();
    const char* fileName = strStepFile.c_str();
    TRSNode* root = ImportStep::readSTLFile(fileName, errorMessage);
    if (!root)
    {
        updateStatus(QString::fromStdString(errorMessage));
        return;
    }
    m_canvas->setScene(root);
    updateStatus("Load scene successfully.");
}

void MainWindow::onDemo()
{
    m_canvas->setupDemo();
}

void MainWindow::onSwitchLanguage(QAction* action)
{
    QString locale = action->data().toString();

    switchTranslator(locale);

    retranslateUi();
}

void MainWindow::onSetting()
{
    if (!m_settingDialog)
    {
        TRSSettings* settings = m_canvas->getSettings();
        m_settingDialog = new SettingDialog(settings, m_canvas, this);
    }
    m_settingDialog->exec();
}
