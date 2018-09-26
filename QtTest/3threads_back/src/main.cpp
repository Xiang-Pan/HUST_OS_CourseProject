/* Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description:QtTest
 */
#include "mainwindow.h"
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
