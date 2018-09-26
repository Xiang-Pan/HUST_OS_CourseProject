#include "add_dialog.h"
#include "ui_add_dialog.h"

add_Dialog::add_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_Dialog)
{
    ui->setupUi(this);
}

add_Dialog::~add_Dialog()
{
    delete ui;
}



void add_Dialog::update_add()
{
    add_num+=1;
    total+=add_num;
    ui->label->setText("Add:"+QString::number(add_num)+"Total:"+QString::number(total));
    if(add_num==100)
    {
        add_num=0;
        total=0;
    }
}
