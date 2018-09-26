/* Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:QtTest
 */
#ifndef ADD_DIALOG_H
#define ADD_DIALOG_H

#include <QDialog>

namespace Ui {
class add_Dialog;
}

class add_Dialog : public QDialog
{
    Q_OBJECT

public:
    int add_num=0;
    int total=0;
    explicit add_Dialog(QWidget *parent = nullptr);
    ~add_Dialog();

private:
    Ui::add_Dialog *ui;

public slots:
    void update_add();
};

#endif // ADD_DIALOG_H
