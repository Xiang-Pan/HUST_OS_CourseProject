#include "mainwindow.h"
#include <QApplication>
//#include <mainwindow.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    int p1,p2;
    p1=fork();
    if(p1==0)
    {
        QApplication a(argc, argv);
        MainWindow1 w;
        w.move(200,200);
        w.show();
        return a.exec();
    }
    else
    {
        p2=fork();
        if(p2==0)
        {
            QApplication b(argc, argv);
            MainWindow2 x;
            x.move(700,200);
            x.show();
            return b.exec();
        }
        else{
            QApplication c(argc, argv);
            MainWindow3 y;
            y.move(500,400);
            y.show();
            return c.exec();
        }
    }
}
