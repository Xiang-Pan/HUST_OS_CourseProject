#include "mainwindow.h"
#include "ui_mainwindow.h"

QT_CHARTS_USE_NAMESPACE
QChart *m_chart;
QLineSeries *m_series;
//QList<double> dataList;//存储业务数据
int maxSize = 100;
int timeId;
//extern void read_procs();
//proc_cmp=;


#include "processes.h"
/*
 * us — 用户空间占用CPU的百分比。
 * sy — 内核空间占用CPU的百分比。
 * ni — 改变过优先级的进程占用CPU的百分比
 * id — 空闲CPU百分比
 * wa — IO等待占用CPU的百分比
 * hi — 硬中断（Hardware IRQ）占用CPU的百分比
 * si — 软中断（Software Interrupts）占用CPU的百分比
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setProcess();
    setRescources();
    // init process
    int i;

    max_procs = 0;
    delay = 1;
    iterations = -1;

    free_procs = NULL;

    num_new_procs = num_old_procs = 0;
    new_procs = old_procs = NULL;

    read_procs();
}

MainWindow::~MainWindow()
{

}


void MainWindow::setProcess()
{
    QFile tempFile; // for open sys info file
    QDir qd("/proc");
    QStringList qsList = qd.entryList();
    QString qs = qsList.join("\n");
    QString id_of_pro;
    bool ok;
    int find_start = 3;
    int a, b;
    int nProPid; //进程PID
    int number_of_sleep = 0, number_of_run = 0, number_of_zombie = 0;
    int totalProNum = 0; //进程总数
    QString proName; //进程名
    QString proState; //进程状态
    QString proPri; //进程优先级
    QString proMem; //进程占用内存
    QString tempStr;
    QListWidgetItem *title = new QListWidgetItem("ProcessName\t" + QString::fromUtf8("User") + "\t" +
                                                 QString::fromUtf8("%CPU") + "\t" +
                                                 QString::fromUtf8("PID") + "\t" +
                                                 QString::fromUtf8("Memory")+ "\t" +
                                                 QString::fromUtf8("Disk Read Total")+ "\t"+
                                                 QString::fromUtf8("Disk Write")+ "\t"
                                                 , ui->listWidget);
    //循环读取进程
    while (1)
    {
        //获取进程PID
        a = qs.indexOf("\n", find_start);
        b = qs.indexOf("\n", a+1);
        find_start = b;
        id_of_pro = qs.mid(a+1, b-a-1);
        totalProNum++;
        nProPid = id_of_pro.toInt(&ok, 10);
        if(!ok)
        {
            break;
        }

        //打开PID所对应的进程状态文件
        tempFile.setFileName("/proc/" + id_of_pro + "/stat");
        if ( !tempFile.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr("The pid stat file can not open!"), QMessageBox::Yes);
            return;
        }
        tempStr = tempFile.readLine();
        if (tempStr.length() == 0)
        {
            break;
        }
        a = tempStr.indexOf("(");
        b = tempStr.indexOf(")");
        proName = tempStr.mid(a+1, b-a-1);
        proName.trimmed(); //删除两端的空格
        proState = tempStr.section(" ", 2, 2);
        proPri = tempStr.section(" ", 17, 17);
        proMem = tempStr.section(" ", 22, 22);
        switch ( proState.at(0).toLatin1() )
        {
        case 'S':   number_of_sleep++; break; //Sleep
        case 'R':   number_of_run++; break; //Running
        case 'Z':   number_of_zombie++; break; //Zombie
        default :   break;
        }
        if (proName.length() >= 12)
        {
            QListWidgetItem *item = new QListWidgetItem(id_of_pro + "\t" +
                                                        proName + "\t" +
                                                        proState + "\t" +
                                                        proPri + "\t" +
                                                        proMem, ui->listWidget);
        }
        else
        {
            QListWidgetItem *item = new QListWidgetItem(id_of_pro + "\t" +
                                                        proName + "\t\t" +
                                                        proState + "\t" +
                                                        proPri + "\t" +
                                                        proMem, ui->listWidget);
        }
    }
    tempFile.close(); //关闭该PID进程的状态文件
}



void MainWindow::setRescources()
{
    m_chart = new QChart;
    QChartView *chartView = new QChartView(m_chart);
    chartView->setRubberBand(QChartView::RectangleRubberBand);

    m_series = new QLineSeries;
    m_chart->addSeries(m_series);

    for(int i=0;i<maxSize;++i)
    {
       m_series->append(i,0);
    }
    m_series->setUseOpenGL(true);//openGl 加速
    qDebug()<<m_series->useOpenGL();

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0,maxSize);
    axisX->setLabelFormat("%g");
//    axisX->setTitleText("axisX");

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0,100);
    axisY->setTitleText("CPU Rate");

    m_chart->setAxisX(axisX,m_series);
    m_chart->setAxisY(axisY,m_series);
    m_chart->legend()->hide();
    m_chart->setTitle("CPU");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    ui->tab_2->setLayout(layout);
    timeId = startTimer(1000);

}



void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==timeId)//定时器到时间,//模拟数据填充
    {
        old_procs = new_procs;
        num_old_procs = num_new_procs;
        memcpy(&old_cpu, &new_cpu, sizeof(old_cpu));
//        sleep(4);
        read_procs();
        print_procs();
        free_old_procs();
        double cpu_data=cpu_user+cpu_sys;

        //update disk info
        get_disk_space();
        get_disk_speed();
        get_path_space("/");
        int device_num=mydf();
        setFileSystem(device_num);
        //qDebug()<<m_timer_interval;

         QTime dataTime(QTime::currentTime());
        long int eltime = dataTime.elapsed();
         int lastpointtime = 0;
        //int size = (eltime - lastpointtime);//数据个数
        int size=1;
        qDebug()<<"size-->"<<size;
        if(isVisible())
        {
            QVector<QPointF> oldPoints = m_series->pointsVector();//Returns the points in the series as a vector
            QVector<QPointF> points;

            for(int i=size;i<oldPoints.count();++i)
            {
                points.append(QPointF(i-size ,oldPoints.at(i).y()));//替换数据用
            }
            qint64 sizePoints = points.count();
            points.append(QPointF(sizePoints,cpu_data));
            m_series->replace(points);
            lastpointtime = eltime;
       }
    }
}



void MainWindow::setFileSystem(int device_num)
{
    //get file system info
    for(int i=0;i<device_num-1;i++)
    {
        ui->FileSys_tableWidget->setItem(i,0,new QTableWidgetItem(device_info[i][0]));
        ui->FileSys_tableWidget->setItem(i,1,new QTableWidgetItem(device_info[i][0]));
        ui->FileSys_tableWidget->setItem(i,2,new QTableWidgetItem("Jan"));
        ui->FileSys_tableWidget->setItem(i,3,new QTableWidgetItem(device_info[i][1]));
        ui->FileSys_tableWidget->setItem(i,4,new QTableWidgetItem(device_info[i][2]));
        ui->FileSys_tableWidget->setItem(i,5,new QTableWidgetItem(device_info[i][3]));
    }
}




void MainWindow::on_kill_pushButton_clicked()
{
    //获得进程号
    QListWidgetItem *item = ui->listWidget->currentItem();
    QString pro = item->text();
    pro = pro.section("\t", 0, 0);
    system("kill " + pro.toLatin1());
    QMessageBox::warning(this, tr("kill"), QString::fromUtf8("该进程已被杀死!"), QMessageBox::Yes);
    //回到进程信息tab表
//    setProcess();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch (index)
    {
    case 0:
        setProcess();
        break;
    case 1:
        break;
    case 2:
        break;
    }
}
