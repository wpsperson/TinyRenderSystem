#pragma once
#include <QtWidgets/QMainWindow>

class QAction;
class OpenGLWidget;

class  MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:

    void createActions();
    void createMenus();
    void createDockWidgets();

private slots:
    void onLoadStep();
private:
    QMenu *m_fileMenu = nullptr;
    QAction *m_loadStepAction = nullptr;
    OpenGLWidget* m_canvas = nullptr;
};
