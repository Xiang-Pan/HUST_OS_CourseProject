/********************************************************************************
** Form generated from reading UI file 'add_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADD_DIALOG_H
#define UI_ADD_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_add_Dialog
{
public:
    QLabel *label;

    void setupUi(QDialog *add_Dialog)
    {
        if (add_Dialog->objectName().isEmpty())
            add_Dialog->setObjectName(QStringLiteral("add_Dialog"));
        add_Dialog->resize(400, 300);
        label = new QLabel(add_Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 80, 321, 121));

        retranslateUi(add_Dialog);

        QMetaObject::connectSlotsByName(add_Dialog);
    } // setupUi

    void retranslateUi(QDialog *add_Dialog)
    {
        add_Dialog->setWindowTitle(QApplication::translate("add_Dialog", "Dialog", nullptr));
        label->setText(QApplication::translate("add_Dialog", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class add_Dialog: public Ui_add_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADD_DIALOG_H
