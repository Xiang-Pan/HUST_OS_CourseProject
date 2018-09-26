/* FileName:    mainwindow.h
 * Author:      Hover
 * E-Mail:      hover@hust.edu.cn
 * GitHub:      HoverWings
 * Description: mainwindow
 */
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
#include "filesystems.h"
#include "progressbardelegate.h"
#include "tablemodel.h"
#include "tableview.h"
#include "netinfo.h"
namespace Ui
{
    class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    TableView *tv;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QChart *cpu_chart;
    QLineSeries *cpu_series;
    QChart *mem_chart;
    QLineSeries *mem_series;
    QLineSeries *swap_series;
    QChart *net_chart;
    QLineSeries *upload_series;
    QLineSeries *download_series;


    QLabel *receiving_label;
    QLabel * totalReceived_label;
    QLabel *sending_label;
    QLabel * totalsent_label;
    int maxSize = 60;
    int timeId;
    int process_timeId;
    bool set_Process=true;
    QStandardItemModel *process_model;
//    QStandardItemModel *fs_model;
    // process info
    QModelIndex focus_index;
    QString process_focus_pid="0";
    int process_focou_row=0;
    int process_focus_col=0;
    bool selected=false;

    // mem info var
    QFile memFile; //用于打开系统文件
    QString memTotal;
    QString memFree;
    QString memUsed;
    QString swapTotal;
    QString swapFree;
    QString swapUsed;
    QString MemAvailable;
    int nMemAvailable,nMemTotal, nMemFree, nMemUsed, nSwapTotal, nSwapFree, nSwapUsed;
    QString tempStr;
    int pos;
    QPieSeries *mem_pieseries;
    QPieSeries *swap_pieseries;
    QChartView *mem_piechartView;
    QChartView *swap_piechartView;

    TableView* fs_view;
    TableModel* fs_model;
    QValueAxis *net_axisY;
    int net_max_ax;


public slots:
//    void update_resources();
    void onHeaderClicked(int _colNum);
    void searchModelandItem(QString ID);
    void rowDoubleClicked(const QModelIndex index);
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

    void setProcess(bool update_process);

    void setRescources();
    void setMem();
    void setNet();
    void setFileSystem(int device_num);
    void updateFileSystem();
    void show_procs(bool update_process);
    double getData(double time);

    void updateCPUGraph();
    void updateMemGraph();
    void updateSwapGraph();
    void updateNetGraph();



protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
};
#endif // MAINWINDOW_H
