/********************************************************************************
** Form generated from reading UI file 'cycle_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CYCLE_DIALOG_H
#define UI_CYCLE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_cycle_Dialog
{
public:
    QLabel *label;

    void setupUi(QDialog *cycle_Dialog)
    {
        if (cycle_Dialog->objectName().isEmpty())
            cycle_Dialog->setObjectName(QStringLiteral("cycle_Dialog"));
        cycle_Dialog->resize(400, 300);
        label = new QLabel(cycle_Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(150, 130, 68, 19));

        retranslateUi(cycle_Dialog);

        QMetaObject::connectSlotsByName(cycle_Dialog);
    } // setupUi

    void retranslateUi(QDialog *cycle_Dialog)
    {
        cycle_Dialog->setWindowTitle(QApplication::translate("cycle_Dialog", "Dialog", nullptr));
        label->setText(QApplication::translate("cycle_Dialog", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class cycle_Dialog: public Ui_cycle_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CYCLE_DIALOG_H
