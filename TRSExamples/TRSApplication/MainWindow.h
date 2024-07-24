#pragma once
#include <QtWidgets/QMainWindow>

class QAction;
class QLabel;
class OpenGLWidget;
class QDockWidget;
class QTranslator;

class  MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    void setupDefaultTranslator();

    void createActions();

    void createMenus();

    void createDockWidgets();

    void createStatusBar();

    void updateStatus(const QString& status);

    void retranslateUi();

    void switchTranslator(const QString& locale);

    void createLanguageMenu();

private slots:
    void onLoadStep();
    void onLoadSTL();
    void onSwitchLanguage(QAction* action);

private:
    QMenu *m_fileMenu = nullptr;
    QAction* m_loadStepAction = nullptr;
    QAction* m_loadStlAction = nullptr;
    QAction* m_demoAction = nullptr;
    OpenGLWidget* m_canvas = nullptr;
    QLabel* m_status = nullptr;

    QDockWidget* m_naviDock = nullptr;

    QMenu* languageMenu;
    QString translatePath;
    QTranslator* appTranslator;
    QTranslator* qtTranslator;
};
