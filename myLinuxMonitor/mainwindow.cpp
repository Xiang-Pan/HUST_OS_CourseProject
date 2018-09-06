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
    net_init();

    setRescources();

    ui->Process_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->Process_tableView->setSelectionMode( QAbstractItemView::SingleSelection);//设置选中单个
    ui->Process_tableView->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    QHeaderView *hv = ui->Process_tableView->horizontalHeader();
    hv->setSortIndicatorShown(true); //显示排序三角
    hv->sectionsClickable();//设置标题可以被单击
    connect(hv, SIGNAL(sectionClicked(int)), this, SLOT(onHeaderClicked(int)));

    fs_view=new TableView(this);
    fs_model=fs_view->tableModel();
    fs_view->setModel(fs_model);
    fs_view->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    fs_view->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    fs_view->setSelectionMode(QAbstractItemView::SingleSelection);//设置选中单个
    QStringList fs_titleList;
    fs_titleList<<QString("Device")
                <<QString("Total")
                <<QString("Used")
                <<QString("Avaliable")
                <<QString("Persentage")
                <<QString("Directory")
                <<QString(" ");
    fs_model->setHorizontalHeader(fs_titleList);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(fs_view);
    ui->tab_3->setLayout(layout);
    connect(fs_view, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(rowDoubleClicked(const QModelIndex &)));

//    for (QStringList& v : data)
//    {
//        int value =v.at(1).toInt();
//        qDebug() << value;
//        if (value < 100)
//        {
//            value += 2;
//            qDebug() << value;
//            v[1] = QString::number(value);
//            emit fs_model->layoutChanged();
//        }
//    }


//    QHeaderView *hv = ui->Process_tableView->horizontalHeader();
    max_procs = 0;
    delay = 1;
    iterations = -1;

    free_procs = NULL;

    num_new_procs = num_old_procs = 0;
    new_procs = old_procs = NULL;

    int device_num=mydf();
    setFileSystem(device_num);

    //set cpu
    old_procs = new_procs;
    num_old_procs = num_new_procs;
    memcpy(&old_cpu, &new_cpu, sizeof(old_cpu));
    read_procs();
    setProcess(true);
    free_old_procs();

}


void MainWindow::rowDoubleClicked(const QModelIndex index)
{
//    QModelIndex nowIndex =fs_view->currentIndex();
    QVector<QStringList>& data = fs_model->DataVector();
    if (index.isValid())
    {
        QString path=data[index.row()][5];
        QProcess *path_proc = new QProcess();
        path_proc->start("nautilus "+path);
    }
}

void MainWindow::onHeaderClicked(int colNum)
{
    qDebug()<<"colNum"<<colNum;
    _colNum=colNum;
}


MainWindow::~MainWindow()
{

}

void MainWindow::show_procs(bool update_process)
{
    if(update_process)
    {
            process_model->clear();;
    }

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
    if(!update_process)
    {
        return;
    }
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

void MainWindow::setProcess(bool update_process)
{
    show_procs(update_process);
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
//    qDebug()<<cpu_series->useOpenGL();

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
       mem_series->append(i,0);
    }
    mem_series->setUseOpenGL(true);
//    qDebug()<<mem_series->useOpenGL();

    QValueAxis *mem_axisX = new QValueAxis;
    mem_axisX->setRange(0,maxSize);
    mem_axisX->setLabelFormat("%g");

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
       swap_series->append(i,0);
    }
    swap_series->setUseOpenGL(true);//openGl
    qDebug()<<swap_series->useOpenGL();
    mem_chart->setAxisX(mem_axisX,swap_series);
    mem_chart->setAxisY(mem_axisY,swap_series);
    mem_chart->legend()->hide();
    mem_chart->setTitle("Memory History");
    layout->addWidget(mem_chartView);


    //add pie mem
    QHBoxLayout *Hbox=new QHBoxLayout();//水平布局管理器（父管理器）；
    mem_pieseries = new QPieSeries();
    mem_pieseries->setHoleSize(0.35);

    QChartView *mem_piechartView = new QChartView();
    mem_piechartView->setRenderHint(QPainter::Antialiasing);
    mem_piechartView->chart()->setTitle("Mem");
    mem_piechartView->chart()->addSeries(mem_pieseries);
    mem_piechartView->chart()->legend()->setAlignment(Qt::AlignBottom);
    mem_piechartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    mem_piechartView->chart()->legend()->setFont(QFont("Arial", 7));
    Hbox->addWidget(mem_piechartView);


    swap_pieseries = new QPieSeries();
    swap_pieseries->setHoleSize(0.35);
    swap_piechartView = new QChartView();
    swap_piechartView->setRenderHint(QPainter::Antialiasing);
    swap_piechartView->chart()->setTitle("SWAP");
    swap_piechartView->chart()->addSeries(swap_pieseries);
    swap_piechartView->chart()->legend()->setAlignment(Qt::AlignBottom);
    swap_piechartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    swap_piechartView->chart()->legend()->setFont(QFont("Arial", 7));
    Hbox->addWidget(swap_piechartView);

    layout->addLayout(Hbox);


//    set net
//    download
    net_chart = new QChart;
    QChartView *net_chartView = new QChartView(net_chart);
    layout->addWidget(net_chartView);
    net_chartView->setRubberBand(QChartView::RectangleRubberBand);

    download_series = new QLineSeries;
    net_chart->addSeries(download_series);

    for(int i=0;i<maxSize;++i)
    {
       download_series->append(i,0);
    }
//    download_series->setUseOpenGL(true);//openGl

    QValueAxis *net_axisX = new QValueAxis;
    net_axisX->setRange(0,maxSize);
    net_axisX->setLabelFormat("%g");

    net_axisY = new QValueAxis;
    net_axisY->setRange(0,100);
    net_axisY->setTitleText("Net Rate");

    net_chart->setAxisX(net_axisX,download_series);
//    // upload
    upload_series = new QLineSeries;
    net_chart->addSeries(upload_series);
    for(int i=0;i<maxSize;++i)
    {
       upload_series->append(i,0);
    }
    upload_series->setUseOpenGL(true);//openGl 加速
//    qDebug()<<swap_series->useOpenGL();
    net_chart->setAxisX(net_axisX,upload_series);
    net_chart->setAxisY(net_axisY,upload_series);
    net_chart->setAxisY(net_axisY,download_series);

//    net_chart->legend()->hide();
    net_chart->setTitle("Network History");


    //set net info
//    QHBoxLayout *net_Hbox=new QHBoxLayout();
    QGridLayout *gridLayout = new QGridLayout;
    QLabel* download_label = new QLabel();
    QImage *download_img = new QImage(":/icon/download.jpg");
    QImage *scaled_logo_img = new QImage();
    *scaled_logo_img=download_img->scaled(32,32,Qt::KeepAspectRatio);
    download_label->setPixmap(QPixmap::fromImage(*scaled_logo_img));
//    net_Hbox->addWidget(download_label);
    gridLayout->addWidget(download_label,0,0);
    gridLayout->setSpacing(0);
    QVBoxLayout *d_Vbox=new QVBoxLayout();
    receiving_label=new QLabel();
    receiving_label->setText("Receiving");
    totalReceived_label=new QLabel();
    totalReceived_label->setText("Total Received");
    d_Vbox->addWidget(receiving_label);
    d_Vbox->addWidget(totalReceived_label);
    gridLayout->addWidget(receiving_label,0,2,1,3);
    gridLayout->addWidget(totalReceived_label,1,2,1,3);
//    net_Hbox->addLayout(d_Vbox);


    QLabel* upload_label = new QLabel();
    QImage *upload_img = new QImage(":/icon/upload.jpg");
    *scaled_logo_img=upload_img->scaled(32,32,Qt::KeepAspectRatio);
    upload_label->setPixmap(QPixmap::fromImage(*scaled_logo_img));
    gridLayout->addWidget(upload_label,0,3);

    sending_label=new QLabel();
    sending_label->setText("Sending");
    totalsent_label=new QLabel();
    totalsent_label->setText("Total Sent");
    gridLayout->addWidget(sending_label,0,4,1,3);
    gridLayout->addWidget(totalsent_label,1,4);



    layout->addLayout(gridLayout);

//    layout->setStretchFactor(cpu_chartView, 8);
//    layout->setStretchFactor(mem_chartView, 8);
//    layout->setStretchFactor(mem_piechartView,8);
//    layout->setStretchFactor(net_chartView, 8);
//    layout->setStretchFactor(net_Hbox, 1);



    ui->tab_2->setLayout(layout);


    timeId = startTimer(1000);
    process_timeId=startTimer(10000);
}


void MainWindow::setMem()
{
    memFile.setFileName("/proc/meminfo"); //打开内存信息文件
    if (!memFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("The meminfo file can not open!"), QMessageBox::Yes);
        return ;
    }
    while (1)
    {
        tempStr = memFile.readLine();
//        qDebug()<<"mememem"<<tempStr;
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
//            qDebug()<<nMemFree<<"memem";
        }
        else if (pos = tempStr.indexOf("SwapTotal"), pos != -1)
        {
            swapTotal = tempStr.mid(pos+11, tempStr.length()-14);
            swapTotal = swapTotal.trimmed();
            nSwapTotal = swapTotal.toInt()/1024;
        }
        else if(pos = tempStr.indexOf("MemAvailable"), pos != -1)
        {
//            qDebug()<<tempStr;
            MemAvailable = tempStr.mid(pos+14, tempStr.length()-17);
//            qDebug()<<MemAvailable;
            MemAvailable = MemAvailable.trimmed();
            nMemAvailable = MemAvailable.toInt()/1024;
//            qDebug()<<nMemAvailable<<"nMemAvailable";
        }
        else if (pos = tempStr.indexOf("SwapFree"), pos != -1)
        {
            swapFree = tempStr.mid(pos+10,tempStr.length()-13);
            swapFree = swapFree.trimmed();
            nSwapFree = swapFree.toInt()/1024;
            break;
        }
    }
    nMemUsed = nMemTotal - nMemAvailable;
    nSwapUsed = nSwapTotal - nSwapFree;
    memUsed = QString::number(nMemUsed, 10);
    swapUsed = QString::number(nSwapUsed, 10);
    memFree = QString::number(nMemFree, 10);
    memTotal = QString::number(nMemTotal, 10);
    swapFree = QString::number(nSwapFree, 10);
    swapTotal = QString::number(nSwapTotal, 10);
    memFile.close();
}


void MainWindow::updateCPUGraph()
{
    double cpu_data=cpu_user+cpu_sys;
    QTime dataTime(QTime::currentTime());
    long int eltime = dataTime.elapsed();
    int lastpointtime = 0;
    //int size = (eltime - lastpointtime);//数据个数
    int size=1;
    if(isVisible())
    {
        //update cpu graph
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


void MainWindow::updateMemGraph()
{
    double mem_data=100*(nMemTotal-nMemAvailable)/nMemTotal;
    QTime dataTime(QTime::currentTime());
    long int eltime = dataTime.elapsed();
    int lastpointtime = 0;
    //int size = (eltime - lastpointtime);//数据个数
    int size=1;
    if(isVisible())
    {
        //update cpu graph
        QVector<QPointF> oldPoints = mem_series->pointsVector();//Returns the points in the series as a vector
        QVector<QPointF> points;
        for(int i=size;i<oldPoints.count();++i)
        {
            points.append(QPointF(i-size ,oldPoints.at(i).y()));//替换数据用
        }
        qint64 sizePoints = points.count();
        points.append(QPointF(sizePoints,mem_data));
        mem_series->replace(points);
        lastpointtime = eltime;
    }
    mem_pieseries->clear();
    mem_pieseries->append("MemUsed:"+QString::number(nMemUsed/1024)+"GiB("+QString::number(100*(float)nMemUsed/nMemTotal)+"%) of "+QString::number(nMemTotal/1024)+"GiB",nMemUsed);
    mem_pieseries->append("", nMemAvailable);

}

void MainWindow::updateSwapGraph()
{
    double swap_data=100*nSwapUsed/nSwapTotal;
    QTime dataTime(QTime::currentTime());
    long int eltime = dataTime.elapsed();
    int lastpointtime = 0;
    //int size = (eltime - lastpointtime);//数据个数
    int size=1;
    if(isVisible())
    {
        //update cpu graph
        QVector<QPointF> oldPoints = swap_series->pointsVector();//Returns the points in the series as a vector
        QVector<QPointF> points;
        for(int i=size;i<oldPoints.count();++i)
        {
            points.append(QPointF(i-size ,oldPoints.at(i).y()));//替换数据用
        }
        qint64 sizePoints = points.count();
        points.append(QPointF(sizePoints,swap_data));
        swap_series->replace(points);
        lastpointtime = eltime;
    }
    swap_pieseries->clear();
    swap_pieseries->append("SwapUsed:"+QString::number(nSwapUsed/1024)+"GiB("+QString::number(100*(float)nSwapUsed/nSwapTotal)+"%) of "+QString::number(nSwapTotal/1024)+"GiB",nSwapUsed);
    swap_pieseries->append("", nSwapFree);
}


void MainWindow::updateNetGraph()
{
//    double download_data=100*nSwapUsed/nSwapTotal;
    QTime dataTime(QTime::currentTime());
    long int eltime = dataTime.elapsed();
    int lastpointtime = 0;
    //int size = (eltime - lastpointtime);//数据个数
    int size=1;
    if(isVisible())
    {
        QVector<QPointF> oldPoints = download_series->pointsVector();//Returns the points in the series as a vector
        QVector<QPointF> points;
        for(int i=size;i<oldPoints.count();++i)
        {
            points.append(QPointF(i-size ,oldPoints.at(i).y()));//替换数据用
        }
        qint64 sizePoints = points.count();
        points.append(QPointF(sizePoints,net_receive_speed));
        download_series->replace(points);

        oldPoints.clear();
        points.clear();
        oldPoints = upload_series->pointsVector();//Returns the points in the series as a vector
        for(int i=size;i<oldPoints.count();++i)
        {
            points.append(QPointF(i-size ,oldPoints.at(i).y()));//替换数据用
        }
        sizePoints = points.count();
        points.append(QPointF(sizePoints,net_transmit_speed));
        upload_series->replace(points);

        char* tempc;
        tempc = (char *)malloc(100 * sizeof(char));
        tempc=kscale(net_current_receive_total,1);
        totalReceived_label->setText("TotalReceived:"+QString(tempc));

        tempc=kscale(net_current_transmit_total,1);
        totalsent_label->setText("TotalSent:"+QString(tempc));

        tempc=kscale(net_receive_speed,1);
        receiving_label->setText("Receiving:"+QString(tempc));

        tempc=kscale(net_transmit_speed,1);
        sending_label->setText("Sending:"+QString(tempc));



        tempc=kscale(max_net_speed,1);
        QString net_spped_max=QString(tempc);
        QString temp=net_spped_max;
        temp.chop(1);
        float max=temp.toFloat();
        if(net_spped_max.right(1).compare("B")==0)
        {
            net_axisY->setRange(0,1024);
        }
        else if(net_spped_max.right(1).compare("K")==0)
        {
            net_axisY->setRange(0,ceil(max*1.5)*1024);
        }
        else if(net_spped_max.right(1).compare("M")==0)
        {
            net_axisY->setRange(0,ceil(max*1.5)*1024*1024);
        }
//        net_chart->setAxisY(net_axisY,upload_series);
//        net_chart->setAxisY(net_axisY,download_series);
        lastpointtime = eltime;
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==timeId)//定时器到时间,//模拟数据填充
    {
        //update net
        net_update();
        updateNetGraph();
        //update mem and swap info
        setMem();

        //update cpu info but not process
        old_procs = new_procs;
        num_old_procs = num_new_procs;
        memcpy(&old_cpu, &new_cpu, sizeof(old_cpu));
        read_procs();
        setProcess(false);
        free_old_procs();

        //update_graph
        updateCPUGraph();
        updateMemGraph();
        updateSwapGraph();
    }
    if(event->timerId()==process_timeId)
    {
        qDebug()<<"asdasd";
        setMem();
        old_procs = new_procs;
        num_old_procs = num_new_procs;
        memcpy(&old_cpu, &new_cpu, sizeof(old_cpu));
//        sleep(4);
        read_procs();
        setProcess(true);
        free_old_procs();

    }
}

void MainWindow::updateFileSystem()
{
//    qDebug()<<device_info;
}

void MainWindow::setFileSystem(int device_num)
{
    //get file system info
    updateFileSystem();
//    fs_model->
//    fs_model->clear();
    QStringList strList;
//    QVector<QStringList> data;
    QVector<QStringList>& data = fs_model->DataVector();
    data.clear();
    for(int i=0;i<device_num;i++)
    {
        QString device=device_info[i][0];

        if(device.mid(0,4).compare("/dev")==0)
        {
            qDebug()<<i
                    <<device_info[i][0]
                    <<device_info[i][1]
                    <<device_info[i][2]
                    <<device_info[i][3]
                    <<device_info[i][4].toInt()
                    <<device_info[i][5];
            strList<<device_info[i][0]
                    <<device_info[i][1]
                    <<device_info[i][2]
                    <<device_info[i][3]
                    <<device_info[i][4]
                    <<device_info[i][5];
            data.append(strList);
            strList.clear();
        }
    }
    fs_model->setData(data);
    emit fs_model->layoutChanged();
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
                return;
            }

        }
    }
}



