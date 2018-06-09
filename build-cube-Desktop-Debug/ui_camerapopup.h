/********************************************************************************
** Form generated from reading UI file 'camerapopup.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERAPOPUP_H
#define UI_CAMERAPOPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_cameraPopUp
{
public:
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QDialog *cameraPopUp)
    {
        if (cameraPopUp->objectName().isEmpty())
            cameraPopUp->setObjectName(QStringLiteral("cameraPopUp"));
        cameraPopUp->resize(400, 300);
        horizontalLayout = new QHBoxLayout(cameraPopUp);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        scrollArea = new QScrollArea(cameraPopUp);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 384, 284));
        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(scrollArea);


        retranslateUi(cameraPopUp);

        QMetaObject::connectSlotsByName(cameraPopUp);
    } // setupUi

    void retranslateUi(QDialog *cameraPopUp)
    {
        cameraPopUp->setWindowTitle(QApplication::translate("cameraPopUp", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class cameraPopUp: public Ui_cameraPopUp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERAPOPUP_H
