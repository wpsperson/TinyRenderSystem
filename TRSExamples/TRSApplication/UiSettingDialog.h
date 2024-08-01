#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>

#include "AppConst.h"

class UiSettingDialog
{
public:
    // 1.1
    QGroupBox* categoryGBox = nullptr;
    QTreeWidget* treeWidget = nullptr;
    QTreeWidgetItem* itemApperance = nullptr;
    QTreeWidgetItem* itemBasic = nullptr;
    QTreeWidgetItem* itemCamera = nullptr;
    QTreeWidgetItem* itemAuxi = nullptr;
    // 1.2
    QGroupBox* detailGBox = nullptr;
    QStackedWidget* stackedWidget = nullptr;
    // 2
    QSpacerItem* verticalSpacer = nullptr;
    // 3
    QPushButton* okBtn = nullptr;
    QPushButton* cancelBtn = nullptr;

public:
    void SetupUi(QDialog* dialog)
    {
        QVBoxLayout* verticalLayout = new QVBoxLayout;
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        // first row
        QHBoxLayout* topLayout = new QHBoxLayout;
        {
            categoryGBox = new QGroupBox;
            categoryGBox->setMaximumWidth(200);
            {
                QVBoxLayout* gboxLayout = new QVBoxLayout;
                treeWidget = new QTreeWidget;
                {
                    itemApperance = new QTreeWidgetItem(treeWidget);
                    itemBasic = new QTreeWidgetItem(itemApperance);
                    itemCamera = new QTreeWidgetItem(itemApperance);
                    itemAuxi = new QTreeWidgetItem(treeWidget);
                    itemBasic->setData(0, Qt::UserRole, kPageBasic);
                    itemCamera->setData(0, Qt::UserRole, kPageCamera);
                    itemAuxi->setData(0, Qt::UserRole, kPageAuxi);
                    itemApperance->setExpanded(true);
                    treeWidget->setCurrentIndex(QModelIndex());
                    treeWidget->setHeaderHidden(true);
                    treeWidget->setSortingEnabled(false);
                }
                gboxLayout->addWidget(treeWidget);
                categoryGBox->setLayout(gboxLayout);
            }
            topLayout->addWidget(categoryGBox, 1);

            detailGBox = new QGroupBox;
            detailGBox->setTitle("");
            {
                QVBoxLayout* gboxLayout = new QVBoxLayout;
                stackedWidget = new QStackedWidget;
                gboxLayout->addWidget(stackedWidget);
                detailGBox->setLayout(gboxLayout);
            }
            topLayout->addWidget(detailGBox);
        }
        verticalLayout->addLayout(topLayout);

        // spacer row:
        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);
        verticalLayout->addItem(verticalSpacer);

        // buttons row
        QHBoxLayout* bottomLayout = new QHBoxLayout;
        {
            okBtn = new QPushButton;
            cancelBtn = new QPushButton;
            QSpacerItem* horiSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
            okBtn->setDefault(false);
            cancelBtn->setDefault(false);
            bottomLayout->addWidget(okBtn);
            bottomLayout->addWidget(cancelBtn);
            bottomLayout->addItem(horiSpacer);
        }
        verticalLayout->addLayout(bottomLayout);
        dialog->setLayout(verticalLayout);
        dialog->resize(600, 500);
    }
};
