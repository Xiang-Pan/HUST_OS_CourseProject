#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    printf("hello");
    w.setWindowTitle("myMonitor");
    w.show();

    return a.exec();
}
