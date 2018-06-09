/********************************************************************************
** Form generated from reading UI file 'cameraarea.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERAAREA_H
#define UI_CAMERAAREA_H

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

class Ui_cameraArea
{
public:
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QDialog *cameraArea)
    {
        if (cameraArea->objectName().isEmpty())
            cameraArea->setObjectName(QStringLiteral("cameraArea"));
        cameraArea->resize(400, 300);
        horizontalLayout = new QHBoxLayout(cameraArea);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        scrollArea = new QScrollArea(cameraArea);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 384, 284));
        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(scrollArea);


        retranslateUi(cameraArea);

        QMetaObject::connectSlotsByName(cameraArea);
    } // setupUi

    void retranslateUi(QDialog *cameraArea)
    {
        cameraArea->setWindowTitle(QApplication::translate("cameraArea", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class cameraArea: public Ui_cameraArea {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERAAREA_H
