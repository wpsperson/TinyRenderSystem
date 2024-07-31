#pragma once

#include <QtWidgets/QDialog>
class Setting;
class QLabel;
class QPushButton;
class QTreeWidgetItem;
class QCheckBox;
class QColorDialog;
class UiSettingDialog;
class ColorSettingPage;
class CameraSettingPage;
class AuxiliarySettingPage;

class SettingDialog : public  QDialog
{
public:
    SettingDialog(QWidget* parent = nullptr);

    ~SettingDialog();

    void setData(Setting *data);

    void setupUi();

    void retranslateUi();

protected:
    void changeEvent(QEvent* eve) override;

    void loadStackedWidget();

    void connectUi();

    void onOkButton();

    void onCancelButton();

    void onApplyButton();

    void onCurrentPageChange();

private:
    Setting* m_data = nullptr;
    UiSettingDialog* m_ui = nullptr;
    int m_pageIndex = 0;
    ColorSettingPage* m_colorPage = nullptr;
    CameraSettingPage* m_cameraPage = nullptr;
    AuxiliarySettingPage* m_auxiPage = nullptr;
};


class ColorSettingPage : public QWidget
{
public:
    ColorSettingPage(QWidget* parent = nullptr);

    void retranslateUi();

    void onBGColorCustomMenu(const QPoint& pos);

private:
    QLabel* m_bgLabel = nullptr;
    QPushButton* m_bgButton = nullptr;
    QColorDialog* m_colorDialog = nullptr;
};

class QButtonGroup;
class QRadioButton;
class CameraSettingPage : public QWidget
{
public:
    CameraSettingPage(QWidget* parent = nullptr);

    void retranslateUi();

private:
    QButtonGroup* m_projections = nullptr;
    QRadioButton* m_parallel = nullptr;
    QRadioButton* m_perspective = nullptr;
};


class AuxiliarySettingPage : public QWidget
{
public:
    AuxiliarySettingPage(QWidget* parent = nullptr);

    void retranslateUi();

private:
    QCheckBox* m_displayAxis = nullptr;
};


