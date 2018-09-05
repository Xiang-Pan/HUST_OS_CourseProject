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
#include <QStandardItemModel>
#include <QTableView>
//#include <Qtab>
#include <QProcess>
#include <QTableWidget>
#include <QTableWidgetItem>

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
#include "qstandarditemmodelex.h"

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

    QChart *cpu_chart;
    QLineSeries *cpu_series;
    QChart *mem_chart;
    QLineSeries *mem_series;
    QLineSeries *swap_series;

    //QList<double> dataList;//存储业务数据
    int maxSize = 100;
    int timeId;
    //extern void read_procs();
    //proc_cmp=;
    bool set_Process=true;
    QStandardItemModel *process_model;

    QModelIndex focus_index;
    QString process_focus_pid="0";
    int process_focou_row=0;
    int process_focus_col=0;
    bool selected=false;

public slots:
//    void update_resources();
    void onHeaderClicked(int _colNum);
    void searchModelandItem(QString ID);

private slots:
//    void on_tabWidget_currentChanged(int index);
    void on_kill_pushButton_clicked();

    void on_tabWidget_currentChanged(int index);


    void on_Process_tableView_clicked(const QModelIndex &index);

//    void on_lineEdit_editingFinished();

    void on_lineEdit_returnPressed();

private:
    int _colNum;
    Ui::MainWindow *ui;

    void setProcess();
    void setProcess1();
    void setRescources();
    void setFileSystem(int device_num);
    void show_procs(void);
    double getData(double time);



protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
};
#endif // MAINWINDOW_H
