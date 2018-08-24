#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTimer>
#include <QDateTime>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QApplication>
#include <QDebug>
#include <QDialog>
#include "cycle_dialog.h"
#include "add_dialog.h"
//#include "

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    cycle_Dialog* cyc;
    add_Dialog* add;
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void timerUpdate(void);
    void cycle_fun();
};

#endif // MAINWINDOW_H
