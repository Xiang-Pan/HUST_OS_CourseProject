#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cyc=new cycle_Dialog();
    cyc->show();

    add=new add_Dialog();
    add->show();

    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::timerUpdate(void)
{
    //update time
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->time_label->setText(str);

    //update cycle
    cyc->update_cycle();
    add->update_add();
    //update add

}

void MainWindow::cycle_fun()
{
    qDebug()<<"asdasd";
}







