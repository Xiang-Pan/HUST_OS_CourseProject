#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QTabBar>
#include <QWidget>
#include <QTabWidget>
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPoint>
#include <QTimer>
#include <qmath.h>
#include <QGraphicsView>
#include <QChartView>
#include <QLineSeries>
#include <QScatterSeries>
#include <QValueAxis>
#include <QtCharts>
#include "processes.h"
#include "chart.h"
#include "filesystems.h"

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
public slots:
//    void update_resources();

private slots:
//    void on_tabWidget_currentChanged(int index);
    void on_kill_pushButton_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;

    void setProcess();
    void setProcess1();
    void setRescources();
    void setFileSystem(int device_num);

    double getData(double time);

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
