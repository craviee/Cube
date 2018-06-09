/********************************************************************************
** Form generated from reading UI file 'scrollarea.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCROLLAREA_H
#define UI_SCROLLAREA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>

QT_BEGIN_NAMESPACE

class Ui_ScrollArea
{
public:

    void setupUi(QScrollArea *ScrollArea)
    {
        if (ScrollArea->objectName().isEmpty())
            ScrollArea->setObjectName(QStringLiteral("ScrollArea"));
        ScrollArea->resize(640, 480);

        retranslateUi(ScrollArea);

        QMetaObject::connectSlotsByName(ScrollArea);
    } // setupUi

    void retranslateUi(QScrollArea *ScrollArea)
    {
        ScrollArea->setWindowTitle(QApplication::translate("ScrollArea", "ScrollArea", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScrollArea: public Ui_ScrollArea {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCROLLAREA_H
