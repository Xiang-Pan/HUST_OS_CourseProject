/* Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:QtTest
 */
#ifndef CYCLE_DIALOG_H
#define CYCLE_DIALOG_H

#include <QDialog>
#include <QSharedMemory>
#include <QBuffer>
#include <QString>

namespace Ui
{
    class cycle_Dialog;
}

class cycle_Dialog : public QDialog
{
    Q_OBJECT

public:
    int cycle_num=0;
    explicit cycle_Dialog(QWidget *parent = nullptr);
    ~cycle_Dialog();

private:
    Ui::cycle_Dialog *ui;

public slots:
    void update_cycle();
};

#endif // CYCLE_DIALOG_H
