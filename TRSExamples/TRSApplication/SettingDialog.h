#pragma once

#include <QtWidgets/QDialog>


class TRSSettings;
class OpenGLWidget;
class QLabel;
class QPushButton;
class QTreeWidgetItem;
class QCheckBox;
class QColorDialog;
class UiSettingDialog;
class BasicSettingPage;
class CameraSettingPage;
class AuxiliarySettingPage;

class SettingDialog : public  QDialog
{
public:
    SettingDialog(TRSSettings* data, OpenGLWidget* canvas, QWidget* parent = nullptr);

    ~SettingDialog();

    void setupUi();

    void retranslateUi();

    TRSSettings* getSettings() const;

    void updateCanvas();

protected:
    void changeEvent(QEvent* eve) override;

    void showEvent(QShowEvent* eve) override;

    void loadStackedWidget();

    void connectUi();

    void onOkButton();

    void onCancelButton();

    void onCurrentPageChange();

private:
    TRSSettings* m_data = nullptr;
    TRSSettings* m_copy = nullptr;
    OpenGLWidget* m_canvas = nullptr;
    UiSettingDialog* m_ui = nullptr;
    int m_pageIndex = 0;
    BasicSettingPage* m_basicPage = nullptr;
    CameraSettingPage* m_cameraPage = nullptr;
    AuxiliarySettingPage* m_auxiPage = nullptr;
};


class BasicSettingPage : public QWidget
{
public:
    BasicSettingPage(SettingDialog* settingDialog);

    void retranslateUi();

    void updateUiFromData();

    void onBGColorCustomMenu(const QPoint& pos);

    void onChangeRenderMode();

private:
    SettingDialog* m_settingDialog = nullptr;
    QLabel* m_bgLabel = nullptr;
    QPushButton* m_bgButton = nullptr;
    QColorDialog* m_colorDialog = nullptr;
    QCheckBox* m_shadedMode = nullptr;
    QCheckBox* m_wireframeMode = nullptr;
    QCheckBox* m_pointsMode = nullptr;
};

class QButtonGroup;
class QRadioButton;
class CameraSettingPage : public QWidget
{
public:
    CameraSettingPage(SettingDialog* settingDialog);

    void retranslateUi();

    void updateUiFromData();

    void onSwitchProjection();

private:
    SettingDialog* m_settingDialog = nullptr;
    QButtonGroup* m_projections = nullptr;
    QRadioButton* m_parallel = nullptr;
    QRadioButton* m_perspective = nullptr;
};


class AuxiliarySettingPage : public QWidget
{
public:
    AuxiliarySettingPage(SettingDialog* settingDialog);

    void retranslateUi();

    void updateUiFromData();

    void onDisplayAxis(int dis);

private:
    SettingDialog* m_settingDialog = nullptr;
    QCheckBox* m_displayAxis = nullptr;
};


