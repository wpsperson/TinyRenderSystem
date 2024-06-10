#pragma once
#include <QtWidgets/QMainWindow>

class QAction;
class QLabel;
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
    void createStatusBar();

    void updateStatus(const QString& status);

private slots:
    void onLoadStep();
    void onLoadSTL();
private:
    QMenu *m_fileMenu = nullptr;
    QAction *m_loadStepAction = nullptr;
    QAction* m_loadStlAction = nullptr;
    OpenGLWidget* m_canvas = nullptr;
    QLabel* m_status = nullptr;
};
