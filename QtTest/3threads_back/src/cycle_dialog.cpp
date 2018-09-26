#include "cycle_dialog.h"
#include "ui_cycle_dialog.h"

cycle_Dialog::cycle_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cycle_Dialog)
{
    ui->setupUi(this);


}

cycle_Dialog::~cycle_Dialog()
{
    delete ui;
}

void cycle_Dialog::update_cycle()
{
    cycle_num=(cycle_num+1)%10;
//    ui->label->setTex
    ui->label->setText(QString::number(cycle_num));
}
