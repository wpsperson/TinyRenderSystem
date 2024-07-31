#include "SettingDialog.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QAbstractButton>

#include "TRS/TRSSettings.h"
#include "UiSettingDialog.h"
#include "OpenGLWidget.h"


SettingDialog::SettingDialog(TRSSettings* data, OpenGLWidget* canvas, QWidget* parent) : QDialog(parent), m_data(data), m_canvas(canvas)
{
    Qt::WindowFlags flags = Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint | Qt::Window;
    setWindowFlags(flags);
    m_copy = new TRSSettings;
    setupUi();
}

SettingDialog::~SettingDialog()
{
    delete m_ui;
    delete m_copy;
}

void SettingDialog::setupUi()
{
    m_ui = new UiSettingDialog;
    m_ui->SetupUi(this);
    loadStackedWidget();
    connectUi();
    retranslateUi();
}

void SettingDialog::retranslateUi()
{
    setWindowTitle(QCoreApplication::translate("SettingDialog", "Setting"));
    m_ui->categoryGBox->setTitle(QCoreApplication::translate("SettingDialog", "Category"));
    m_ui->itemApperance->setText(0, QCoreApplication::translate("SettingDialog", "Appearance"));
    m_ui->itemColor->setText(0, QCoreApplication::translate("SettingDialog", "Color"));
    m_ui->itemCamera->setText(0, QCoreApplication::translate("SettingDialog", "Camera"));
    m_ui->itemAuxi->setText(0, QCoreApplication::translate("SettingDialog", "Auxiliary"));

    m_ui->okBtn->setText(QCoreApplication::translate("SettingDialog", "OK"));
    m_ui->cancelBtn->setText(QCoreApplication::translate("SettingDialog", "Cancel"));
}

TRSSettings* SettingDialog::getSettings() const
{
    return m_data;
}

void SettingDialog::updateCanvas()
{
    m_canvas->update();
}

void SettingDialog::changeEvent(QEvent* eve)
{
    if (QEvent::LanguageChange == eve->type())
    {
        retranslateUi();
        m_colorPage->retranslateUi();
        m_cameraPage->retranslateUi();
        m_auxiPage->retranslateUi();
    }
    QDialog::changeEvent(eve);
}

void SettingDialog::showEvent(QShowEvent* eve)
{
    *m_copy = *m_data;
    m_colorPage->updateUiFromData();
    m_cameraPage->updateUiFromData();
    m_auxiPage->updateUiFromData();
    QDialog::showEvent(eve);
}

void SettingDialog::loadStackedWidget()
{
    m_colorPage = new ColorSettingPage(this);
    m_cameraPage = new CameraSettingPage(this);
    m_auxiPage = new AuxiliarySettingPage(this);
    QStackedWidget* stackWidget = m_ui->stackedWidget;
    stackWidget->insertWidget(kPageColor, m_colorPage);
    stackWidget->insertWidget(kPageCamera, m_cameraPage);
    stackWidget->insertWidget(kPageAuxi, m_auxiPage);
    m_colorPage->retranslateUi();
    m_cameraPage->retranslateUi();
    m_auxiPage->retranslateUi();
}

void SettingDialog::connectUi()
{
    connect(m_ui->okBtn, &QAbstractButton::pressed, this, &SettingDialog::onOkButton);
    connect(m_ui->cancelBtn, &QAbstractButton::pressed, this, &SettingDialog::onCancelButton);

    connect(m_ui->treeWidget, &QTreeWidget::itemSelectionChanged, this, &SettingDialog::onCurrentPageChange);
}

void SettingDialog::onOkButton()
{
    this->accept();
}

void SettingDialog::onCancelButton()
{
    *m_data = *m_copy; // restore all data.
    this->reject();
}

void SettingDialog::onCurrentPageChange()
{
    QTreeWidgetItem* item = m_ui->treeWidget->currentItem();
    QVariant var = item->data(0, Qt::UserRole);
    if (!var.isValid())
    {
        return;
    }
    m_pageIndex = var.toInt();
    m_ui->stackedWidget->setCurrentIndex(m_pageIndex);
}


ColorSettingPage::ColorSettingPage(SettingDialog* settingDialog) : QWidget(nullptr), m_settingDialog(settingDialog)
{
    QVBoxLayout* vertLayout = new QVBoxLayout;
    QHBoxLayout* horiLayout = new QHBoxLayout;
    {
        m_bgLabel = new QLabel;
        m_bgButton = new QPushButton;
        m_bgButton->setFocusPolicy(Qt::NoFocus);
        m_bgButton->setFixedSize(QSize(16, 16));
        QSpacerItem* spacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed);
        horiLayout->addWidget(m_bgLabel);
        horiLayout->addWidget(m_bgButton);
        horiLayout->addItem(spacer);
    }
    vertLayout->addLayout(horiLayout);
    QSpacerItem* vertSpacer = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Expanding);
    vertLayout->addItem(vertSpacer);
    setLayout(vertLayout);

    m_colorDialog = new QColorDialog(this);

    m_bgButton->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_bgButton, &QWidget::customContextMenuRequested, this, &ColorSettingPage::onBGColorCustomMenu);
}

void ColorSettingPage::retranslateUi()
{
    m_bgLabel->setText(QCoreApplication::translate("SettingDialog", "Background Color"));
}

void ColorSettingPage::updateUiFromData()
{
    TRSSettings* settings = m_settingDialog->getSettings();
    const TRSColor& clr = settings->backgroundColor();
    QColor color(int(clr[0] * 255), int(clr[1] * 255), int(clr[2] * 255));
    m_colorDialog->setCurrentColor(color);
    m_bgButton->setStyleSheet(QString("QPushButton{ background: %1; }").arg(color.name()));
}

void ColorSettingPage::onBGColorCustomMenu(const QPoint& pos)
{
    QPoint globalPos = m_bgButton->mapToGlobal(pos);
    m_colorDialog->move(globalPos);
    int result = m_colorDialog->exec();
    if (QDialog::Accepted == result)
    {
        QColor color = m_colorDialog->currentColor();
        m_bgButton->setStyleSheet(QString("QPushButton{ background: %1; }").arg(color.name()));
        TRSColor bgColor;
        bgColor[0] = color.red() / 255.0f;
        bgColor[1] = color.green() / 255.0f;
        bgColor[2] = color.blue() / 255.0f;
        TRSSettings* settings = m_settingDialog->getSettings();
        settings->setBGColor(bgColor);
        m_settingDialog->updateCanvas();
    }
}


CameraSettingPage::CameraSettingPage(SettingDialog* settingDialog) : QWidget(nullptr), m_settingDialog(settingDialog)
{
    QVBoxLayout* vertLayout = new QVBoxLayout;
    m_parallel = new QRadioButton;
    m_perspective = new QRadioButton;
    vertLayout->addWidget(m_parallel);
    vertLayout->addWidget(m_perspective);
    m_projections = new QButtonGroup(this);
    m_projections->setExclusive(true);
    m_projections->addButton(m_parallel);
    m_projections->addButton(m_perspective);
    QSpacerItem* vertSpacer = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Expanding);
    vertLayout->addItem(vertSpacer);
    setLayout(vertLayout);

    connect(m_parallel, &QAbstractButton::toggled, this, &CameraSettingPage::onSwitchProjection);
    connect(m_perspective, &QAbstractButton::toggled, this, &CameraSettingPage::onSwitchProjection);
}

void CameraSettingPage::retranslateUi()
{
    m_parallel->setText(QCoreApplication::translate("SettingDialog", "Parallel Projection"));
    m_perspective->setText(QCoreApplication::translate("SettingDialog", "Perspective Projection"));
}

void CameraSettingPage::updateUiFromData()
{
    TRSSettings* settings = m_settingDialog->getSettings();
    ProjectionMode mode = settings->projMode();
    if (ProjectionMode::Parallel == mode)
    {
        m_parallel->setChecked(true);
    }
    else
    {
        m_perspective->setChecked(true);
    }
}

void CameraSettingPage::onSwitchProjection()
{
    QAbstractButton* button = m_projections->checkedButton();
    ProjectionMode mode = (button == m_parallel) ? ProjectionMode::Parallel : ProjectionMode::Perspective;
    TRSSettings* settings = m_settingDialog->getSettings();
    settings->setProjMode(mode);
    m_settingDialog->updateCanvas();
}

AuxiliarySettingPage::AuxiliarySettingPage(SettingDialog* settingDialog) : QWidget(nullptr), m_settingDialog(settingDialog)
{
    QVBoxLayout* vertLayout = new QVBoxLayout;
    m_displayAxis = new QCheckBox;
    vertLayout->addWidget(m_displayAxis);
    QSpacerItem* vertSpacer = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Expanding);
    vertLayout->addItem(vertSpacer);
    setLayout(vertLayout);

    connect(m_displayAxis, &QCheckBox::stateChanged, this, &AuxiliarySettingPage::onDisplayAxis);
}

void AuxiliarySettingPage::retranslateUi()
{
    m_displayAxis->setText(QCoreApplication::translate("SettingDialog", "Display Navigate Axis"));
}

void AuxiliarySettingPage::updateUiFromData()
{
    TRSSettings* settings = m_settingDialog->getSettings();
    bool display = settings->displayAxis();
    m_displayAxis->setChecked(display);
}

void AuxiliarySettingPage::onDisplayAxis(int dis)
{
    TRSSettings* settings = m_settingDialog->getSettings();
    settings->setDisplayAxis(dis!=0);
    m_settingDialog->updateCanvas();
}
