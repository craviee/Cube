/********************************************************************************
** Form generated from reading UI file 'cameraviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERAVIEWER_H
#define UI_CAMERAVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_cameraviewer
{
public:

    void setupUi(QDialog *cameraviewer)
    {
        if (cameraviewer->objectName().isEmpty())
            cameraviewer->setObjectName(QStringLiteral("cameraviewer"));
        cameraviewer->resize(400, 300);

        retranslateUi(cameraviewer);

        QMetaObject::connectSlotsByName(cameraviewer);
    } // setupUi

    void retranslateUi(QDialog *cameraviewer)
    {
        cameraviewer->setWindowTitle(QApplication::translate("cameraviewer", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class cameraviewer: public Ui_cameraviewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERAVIEWER_H
