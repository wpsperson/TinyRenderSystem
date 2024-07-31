#include "SettingDialog.h"
#include "UiSettingDialog.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QColorDialog>

SettingDialog::SettingDialog(QWidget* parent) : QDialog(parent)
{
    Qt::WindowFlags flags = Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint | Qt::Window;
    setWindowFlags(flags);

    setupUi();
}

SettingDialog::~SettingDialog()
{
    delete m_ui;
}

void SettingDialog::setData(Setting* data)
{
    m_data = data;
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
    m_ui->applyBtn->setText(QCoreApplication::translate("SettingDialog", "Apply"));
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
    connect(m_ui->applyBtn, &QAbstractButton::pressed, this, &SettingDialog::onApplyButton);

    connect(m_ui->treeWidget, &QTreeWidget::itemSelectionChanged, this, &SettingDialog::onCurrentPageChange);
}

void SettingDialog::onOkButton()
{
}

void SettingDialog::onCancelButton()
{
}

void SettingDialog::onApplyButton()
{
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



ColorSettingPage::ColorSettingPage(QWidget* parent) : QWidget(parent)
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

void ColorSettingPage::onBGColorCustomMenu(const QPoint& pos)
{
    QPoint globalPos = m_bgButton->mapToGlobal(pos);
    m_colorDialog->move(globalPos);
    int result = m_colorDialog->exec();
    if (QDialog::Accepted == result)
    {
        QColor color = m_colorDialog->currentColor();
        m_bgButton->setStyleSheet(QString("QPushButton{ background: %1; }").arg(color.name()));
    }
}


CameraSettingPage::CameraSettingPage(QWidget* parent) : QWidget(parent)
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
}

void CameraSettingPage::retranslateUi()
{
    m_parallel->setText(QCoreApplication::translate("SettingDialog", "Parallel Projection"));
    m_perspective->setText(QCoreApplication::translate("SettingDialog", "Perspective Projection"));
}

AuxiliarySettingPage::AuxiliarySettingPage(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* vertLayout = new QVBoxLayout;
    m_displayAxis = new QCheckBox;
    vertLayout->addWidget(m_displayAxis);
    QSpacerItem* vertSpacer = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Expanding);
    vertLayout->addItem(vertSpacer);
    setLayout(vertLayout);
}

void AuxiliarySettingPage::retranslateUi()
{
    m_displayAxis->setText(QCoreApplication::translate("SettingDialog", "Display Navigate Axis"));
}