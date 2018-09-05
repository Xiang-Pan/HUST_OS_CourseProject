#include "mainwindow.h"
#include "ui_mainwindow.h"

QT_CHARTS_USE_NAMESPACE

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
    process_model= new QStandardItemModel();

    setRescources();

    ui->Process_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->Process_tableView->setSelectionMode( QAbstractItemView::SingleSelection);//设置选中单个
    QHeaderView *hv = ui->Process_tableView->horizontalHeader();
    hv->setSortIndicatorShown(true); //显示排序三角
    hv->sectionsClickable();//设置标题可以被单击
    connect(hv, SIGNAL(sectionClicked(int)), this, SLOT(onHeaderClicked(int)));

    max_procs = 0;
    delay = 1;
    iterations = -1;

    free_procs = NULL;

    num_new_procs = num_old_procs = 0;
    new_procs = old_procs = NULL;

//    read_procs();
}

void MainWindow::onHeaderClicked(int colNum)
{
    qDebug()<<"colNum"<<colNum;
    _colNum=colNum;
}


MainWindow::~MainWindow()
{

}

void MainWindow::show_procs(void)
{
    process_model->clear();
    process_vec.clear();
    process_title.clear();
    process_table.clear();
    int i;
    struct proc_info *old_proc, *proc;
    long unsigned total_delta_time;
    struct passwd *user;
    struct group *group;
    char *user_str, user_buf[20];
    char *group_str, group_buf[20];

    for (i = 0; i < num_new_procs; i++)
    {
        if (new_procs[i])
        {
            old_proc = find_old_proc(new_procs[i]->pid, new_procs[i]->tid);
            if (old_proc)
            {
                new_procs[i]->delta_utime = new_procs[i]->utime - old_proc->utime;
                new_procs[i]->delta_stime = new_procs[i]->stime - old_proc->stime;
            }
            else
            {
                new_procs[i]->delta_utime = 0;
                new_procs[i]->delta_stime = 0;
            }
            new_procs[i]->delta_time = new_procs[i]->delta_utime + new_procs[i]->delta_stime;
        }
    }

    total_delta_time = (new_cpu.utime + new_cpu.ntime + new_cpu.stime + new_cpu.itime
                        + new_cpu.iowtime + new_cpu.irqtime + new_cpu.sirqtime)
                     - (old_cpu.utime + old_cpu.ntime + old_cpu.stime + old_cpu.itime
                        + old_cpu.iowtime + old_cpu.irqtime + old_cpu.sirqtime);


    // change proc_cmp while call
    qsort(new_procs, num_new_procs, sizeof(struct proc_info *), proc_cmp);
    cpu_user= ((new_cpu.utime + new_cpu.ntime) - (old_cpu.utime + old_cpu.ntime)) * 100  / total_delta_time;
    cpu_sys= ((new_cpu.stime ) - (old_cpu.stime)) * 100 / total_delta_time;


    printf("\n\n\n");
//    printf("User %ld%%, System %ld%%, IOW %ld%%, IRQ %ld%%\n",
//            ((new_cpu.utime + new_cpu.ntime) - (old_cpu.utime + old_cpu.ntime)) * 100  / total_delta_time,
//            ((new_cpu.stime ) - (old_cpu.stime)) * 100 / total_delta_time,
//            ((new_cpu.iowtime) - (old_cpu.iowtime)) * 100 / total_delta_time,
//            ((new_cpu.irqtime + new_cpu.sirqtime)
//                    - (old_cpu.irqtime + old_cpu.sirqtime)) * 100 / total_delta_time);
//    printf("User %ld + Nice %ld + Sys %ld + Idle %ld + IOW %ld + IRQ %ld + SIRQ %ld = %ld\n",
//            new_cpu.utime - old_cpu.utime,
//            new_cpu.ntime - old_cpu.ntime,
//            new_cpu.stime - old_cpu.stime,
//            new_cpu.itime - old_cpu.itime,
//            new_cpu.iowtime - old_cpu.iowtime,
//            new_cpu.irqtime - old_cpu.irqtime,
//            new_cpu.sirqtime - old_cpu.sirqtime,
//            total_delta_time);
    printf("\n");

    //Print Title
    QStringList titleList;
    if (!threads)
    {
        titleList<<QString("PID")
                     <<QString("CPU%")
                     <<QString("S")
                     <<QString("#THR")
                     <<QString("VSS")
                     <<QString("RSS")
                     <<QString("UID")
                     <<QString("Name");
    }
    else
    {
        titleList<<QString("PID")
                     <<QString("TID")
                     <<QString("CPU%")
                     <<QString("S")
                     <<QString("VSS")
                     <<QString("RSS")
                     <<QString("UID")
                     <<QString("Thread")
                     <<QString("Proc");
    }
    if(set_Process==true)
    {
        process_model->setHorizontalHeaderLabels(titleList);
//        set_Process=false;
    }

    QList<QStandardItem*> strList;
    for (i = 0; i < num_new_procs; i++)
    {
        proc = new_procs[i];

        if (!proc || (max_procs && (i >= max_procs)))
            break;
        user  = getpwuid(proc->uid);
        group = getgrgid(proc->gid);
        if (user && user->pw_name)
        {
            user_str = user->pw_name;
        }
        else
        {
            snprintf(user_buf, 20, "%d", proc->uid);
            user_str = user_buf;
        }
        if (group && group->gr_name)
        {
            group_str = group->gr_name;
        }
        else
        {
            snprintf(group_buf, 20, "%d", proc->gid);
            group_str = group_buf;
        }
        if (!threads)
        {
            strList<<new QStandardItem(QString::number(proc->pid))
                   <<new QStandardItem(QString::number(proc->delta_time * 100 / total_delta_time))
                   <<new QStandardItem(QString::number(proc->state))
                   <<new QStandardItem(QString::number(proc->num_threads))
                   <<new QStandardItem(QString::number(proc->vss / 1024))
                   <<new QStandardItem(QString::number(proc->rss * getpagesize() / 1024))
//                     <<new QStandardItem(proc->policy)
                   <<new QStandardItem(user_str)
                   <<new QStandardItem(proc->name[0] != 0 ? proc->name : proc->tname);
            process_model->appendRow(strList); //在第0行插入一条记录
            strList.clear();
        }
        else
        {
            strList<<new QStandardItem(proc->pid)
                   <<new QStandardItem(proc->tid)
                   <<new QStandardItem(proc->delta_time * 100 / total_delta_time)
                   <<new QStandardItem(proc->state)
                   <<new QStandardItem(proc->vss / 1024)
                   <<new QStandardItem(proc->rss * getpagesize() / 1024)
//                     <<new QStandardItem(proc->policy)
                   <<new QStandardItem(user_str)
                   <<new QStandardItem(proc->tname)
                   <<new QStandardItem(proc->name);
            process_model->appendRow(strList);
            strList.clear();
        }
    }
}



void MainWindow::setProcess()
{
    show_procs();
    ui->Process_tableView->setModel(process_model);
    ui->Process_tableView->setSortingEnabled(true);
    ui->Process_tableView->sortByColumn(_colNum);
    // search to focous
//    ui->Process_tableView->findChild(process_focus_pid);
    //setfocus
    if(selected)
    {
        searchModelandItem(process_focus_pid);
        ui->Process_tableView->selectRow(focus_index.row());
        //set scroll
        int maxValue = ui->Process_tableView->verticalScrollBar()->maximum();		// 当前SCROLLER最大显示值
        int pageValue = ((focus_index.row())*maxValue/ui->Process_tableView->model()->rowCount());
        ui->Process_tableView->verticalScrollBar()->setSliderPosition(pageValue+4);
    }
}



void MainWindow::setRescources()
{
    //setCPU
    cpu_chart = new QChart;
    QChartView *cpu_chartView = new QChartView(cpu_chart);
    cpu_chartView->setRubberBand(QChartView::RectangleRubberBand);

    cpu_series = new QLineSeries;
    cpu_chart->addSeries(cpu_series);

    for(int i=0;i<maxSize;++i)
    {
       cpu_series->append(i,0);
    }
    cpu_series->setUseOpenGL(true);//openGl 加速
    qDebug()<<cpu_series->useOpenGL();

    QValueAxis *cpu_axisX = new QValueAxis;
    cpu_axisX->setRange(0,maxSize);
    cpu_axisX->setLabelFormat("%g");

    QValueAxis *cpu_axisY = new QValueAxis;
    cpu_axisY->setRange(0,100);
    cpu_axisY->setTitleText("CPU Rate");

    cpu_chart->setAxisX(cpu_axisX,cpu_series);
    cpu_chart->setAxisY(cpu_axisY,cpu_series);
    cpu_chart->legend()->hide();
    cpu_chart->setTitle("CPU History");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(cpu_chartView);


    //setMemory and swap
    mem_chart = new QChart;
    QChartView *mem_chartView = new QChartView(mem_chart);
    mem_chartView->setRubberBand(QChartView::RectangleRubberBand);

    mem_series = new QLineSeries;
    mem_chart->addSeries(mem_series);

    for(int i=0;i<maxSize;++i)
    {
       mem_series->append(i,20);
    }
    mem_series->setUseOpenGL(true);//openGl 加速
    qDebug()<<mem_series->useOpenGL();

    QValueAxis *mem_axisX = new QValueAxis;
    mem_axisX->setRange(0,maxSize);
    mem_axisX->setLabelFormat("%g");
//    axisX->setTitleText("axisX");

    QValueAxis *mem_axisY = new QValueAxis;
    mem_axisY->setRange(0,100);
    mem_axisY->setTitleText("Memory Rate");

    mem_chart->setAxisX(mem_axisX,mem_series);
    mem_chart->setAxisY(mem_axisY,mem_series);

    // SWAP

    swap_series = new QLineSeries;
    mem_chart->addSeries(swap_series);

    for(int i=0;i<maxSize;++i)
    {
       swap_series->append(i,10);
    }
    swap_series->setUseOpenGL(true);//openGl 加速
    qDebug()<<swap_series->useOpenGL();
    mem_chart->setAxisX(mem_axisX,swap_series);
    mem_chart->setAxisY(mem_axisY,swap_series);


    mem_chart->legend()->hide();
    mem_chart->setTitle("Memory History");

//    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mem_chartView);





    ui->tab_2->setLayout(layout);
    timeId = startTimer(1000);
}



void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==timeId)//定时器到时间,//模拟数据填充
    {
        // update cpu process
        old_procs = new_procs;
        num_old_procs = num_new_procs;
        memcpy(&old_cpu, &new_cpu, sizeof(old_cpu));
//        sleep(4);
        read_procs();
        setProcess();
        free_old_procs();
        double cpu_data=cpu_user+cpu_sys;

        // update mem and swap info
        QFile memFile; //用于打开系统文件
        memFile.setFileName("/proc/meminfo"); //打开内存信息文件
        if (!memFile.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr("The meminfo file can not open!"), QMessageBox::Yes);
            return ;
        }
        QString memTotal;
        QString memFree;
        QString memUsed;
        QString swapTotal;
        QString swapFree;
        QString swapUsed;
        int nMemTotal, nMemFree, nMemUsed, nSwapTotal, nSwapFree, nSwapUsed;

        while (1)
        {
            tempStr = tempFile.readLine();
            pos = tempStr.indexOf("MemTotal");
            if (pos != -1)
            {
                memTotal = tempStr.mid(pos+10, tempStr.length()-13);
                memTotal = memTotal.trimmed();
                nMemTotal = memTotal.toInt()/1024;
            }
            else if (pos = tempStr.indexOf("MemFree"), pos != -1)
            {
                memFree = tempStr.mid(pos+9, tempStr.length()-12);
                memFree = memFree.trimmed();
                nMemFree = memFree.toInt()/1024;
            }
            else if (pos = tempStr.indexOf("SwapTotal"), pos != -1)
            {
                swapTotal = tempStr.mid(pos+11, tempStr.length()-14);
                swapTotal = swapTotal.trimmed();
                nSwapTotal = swapTotal.toInt()/1024;
            }
            else if (pos = tempStr.indexOf("SwapFree"), pos != -1)
            {
                swapFree = tempStr.mid(pos+10,tempStr.length()-13);

                swapFree = swapFree.trimmed();
                nSwapFree = swapFree.toInt()/1024;
                break;
            }
        }
        nMemUsed = nMemTotal - nMemFree;
        nSwapUsed = nSwapTotal - nSwapFree;
        memUsed = QString::number(nMemUsed, 10);
        swapUsed = QString::number(nSwapUsed, 10);
        memFree = QString::number(nMemFree, 10);
        memTotal = QString::number(nMemTotal, 10);
        swapFree = QString::number(nSwapFree, 10);
        swapTotal = QString::number(nSwapTotal, 10);
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
            QVector<QPointF> oldPoints = cpu_series->pointsVector();//Returns the points in the series as a vector
            QVector<QPointF> points;

            for(int i=size;i<oldPoints.count();++i)
            {
                points.append(QPointF(i-size ,oldPoints.at(i).y()));//替换数据用
            }
            qint64 sizePoints = points.count();
            points.append(QPointF(sizePoints,cpu_data));
            cpu_series->replace(points);
            lastpointtime = eltime;
       }
    }
}



void MainWindow::setFileSystem(int device_num)
{
    //get file system info
    for(int i=0;i<device_num-1;i++)
    {
//        ui->FileSys_tableView->setItem(i,0,new QTableWidgetItem(device_info[i][0]));
//        ui->FileSys_tableView->setItem(i,1,new QTableWidgetItem(device_info[i][0]));
//        ui->FileSys_tableView->setItem(i,2,new QTableWidgetItem("Jan"));
//        ui->FileSys_tableView->setItem(i,3,new QTableWidgetItem(device_info[i][1]));
//        ui->FileSys_tableView->setItem(i,4,new QTableWidgetItem(device_info[i][2]));
//        ui->FileSys_tableView->setItem(i,5,new QTableWidgetItem(device_info[i][3]));
    }
}




void MainWindow::on_kill_pushButton_clicked()
{
    int pid=process_focus_pid.toInt();

    QString cmd=QString("kill ") +QString::number(pid);
    system(cmd.toLatin1());
    selected=false;
    QMessageBox::warning(this, tr("kill"), QString::fromUtf8("The process has been killed!"), QMessageBox::Yes);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch (index)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    }
}



void MainWindow::on_Process_tableView_clicked(const QModelIndex &index)
{
    selected=true;
    qDebug()<<"index"<<index;
    QModelIndex pid_index = process_model->index(index.row(),0);
    process_focus_pid = process_model->data(pid_index).toString();

}

void MainWindow::on_lineEdit_returnPressed()
{
    QString str=ui->lineEdit->text();
    if (str.isEmpty())
           return;
    searchModelandItem(str);
    selected=true;

}

void MainWindow::searchModelandItem(QString ID)
{
    QStandardItemModel * model;
    model=process_model;
    int rows = model->rowCount();//1
    int column = model->columnCount();//1
    QModelIndex item_index;
    QVariant item_data;
//    QModelIndex pid_index = process_model->index(2,0);
//    QVariant asd_data = process_model->data(pid_index);
    for (int i = 0; i<rows; ++i)
    {
        for (int j = 0; j<column; ++j)
        {
            item_index = model->index(i, j);
            item_data=model->data(item_index);
            if (item_data== ID)
            {
                focus_index = item_index;
                break;
            }

        }
    }
}



