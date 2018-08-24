#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QTabBar>
#include <QTabWidget>
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPoint>
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_kill_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    void setProcess();
    void setRescources();
    void setFileSystem();

protected:
    //virtual void paintEvent( QPaintEvent * e);
};

#endif // MAINWINDOW_H
