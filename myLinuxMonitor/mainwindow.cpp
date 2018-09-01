#include "mainwindow.h"
#include "ui_mainwindow.h"

QT_CHARTS_USE_NAMESPACE
QChart *m_chart;
QLineSeries *m_series;
//QList<double> dataList;//存储业务数据
int maxSize = 100;
int timeId;
//extern void read_procs();
double cpu_user=0;
double cpu_sys=0;

//
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

static struct proc_info *alloc_proc(void)
{
    struct proc_info *proc;
    // to get the first free_procs
    if (free_procs)
    {
        proc = free_procs;
        free_procs = free_procs->next;
        num_free_procs--;
    }
    else
    {
        proc = (proc_info*)malloc(sizeof(*proc));
        if (!proc) die("Could not allocate struct process_info.\n");
    }

    num_used_procs++;

    return proc;
}



static void free_proc(struct proc_info *proc)
{
    proc->next = free_procs;
    free_procs = proc;

    num_used_procs--;
    num_free_procs++;
}

static void read_procs(void)
{
    DIR *proc_dir, *task_dir;
    struct dirent *pid_dir, *tid_dir;
    char filename[64];
    FILE *file;
    int proc_num;
    struct proc_info *proc;
    pid_t pid, tid;

    int i;

    proc_dir = opendir("/proc");
    if (!proc_dir) die("Could not open /proc.\n");

    // allocate new procs and the space is due to the threads
    new_procs = (proc_info**)calloc(INIT_PROCS * (threads ? THREAD_MULT : 1), sizeof(struct proc_info *));
    num_new_procs = INIT_PROCS * (threads ? THREAD_MULT : 1);

    //
    file = fopen("/proc/stat", "r");
    if (!file) die("Could not open /proc/stat.\n");
    fscanf(file, "cpu  %lu %lu %lu %lu %lu %lu %lu", &new_cpu.utime, &new_cpu.ntime, &new_cpu.stime,
            &new_cpu.itime, &new_cpu.iowtime, &new_cpu.irqtime, &new_cpu.sirqtime);
    fclose(file);

    proc_num = 0;
    while ((pid_dir = readdir(proc_dir)))
    {
        // if not digital, then those are Kernel info
        if (!isdigit(pid_dir->d_name[0]))
            continue;

        //convert digi 2 str
        pid = atoi(pid_dir->d_name);

        struct proc_info cur_proc;

        //single threads
        if (!threads)
        {
            proc = alloc_proc();

            proc->pid = proc->tid = pid; // tid= pid

            sprintf(filename, "/proc/%d/stat", pid);
            read_stat(filename, proc);

            sprintf(filename, "/proc/%d/cmdline", pid);
            read_cmdline(filename, proc);

            sprintf(filename, "/proc/%d/status", pid);
            read_status(filename, proc);

            read_policy(pid, proc);

            proc->num_threads = 0;
        }
        else
        {
            sprintf(filename, "/proc/%d/cmdline", pid);
            read_cmdline(filename, &cur_proc);

            sprintf(filename, "/proc/%d/status", pid);
            read_status(filename, &cur_proc);

            // do not allocate, NULL ptr
            proc = NULL;
        }

        // the process state
        //task?
        sprintf(filename, "/proc/%d/task", pid);
        task_dir = opendir(filename);
        if (!task_dir) continue;

        while ((tid_dir = readdir(task_dir)))
        {
            if (!isdigit(tid_dir->d_name[0]))
                continue;

            if (threads)
            {
                tid = atoi(tid_dir->d_name);

                proc = alloc_proc();

                proc->pid = pid; proc->tid = tid;

                sprintf(filename, "/proc/%d/task/%d/stat", pid, tid);
                read_stat(filename, proc);

                read_policy(tid, proc);

                strcpy(proc->name, cur_proc.name);
                proc->uid = cur_proc.uid;
                proc->gid = cur_proc.gid;

                add_proc(proc_num++, proc);
            }
            else
            {
                proc->num_threads++;
            }
        }

        closedir(task_dir);

        if (!threads)
            add_proc(proc_num++, proc);
    }

    for (i = proc_num; i < num_new_procs; i++)
        new_procs[i] = NULL;

    closedir(proc_dir);
}


static int read_stat(char *filename, struct proc_info *proc)
{
    FILE *file;
    char buf[MAX_LINE], *open_paren, *close_paren;
    int res, idx;

    file = fopen(filename, "r");
    if (!file) return 1;
    fgets(buf, MAX_LINE, file);
    fclose(file);

    /* Split at first '(' and last ')' to get process name. */
    open_paren = strchr(buf, '(');
    close_paren = strrchr(buf, ')');
    if (!open_paren || !close_paren) return 1;

    *open_paren = *close_paren = '\0';
    strncpy(proc->tname, open_paren + 1, THREAD_NAME_LEN);
    proc->tname[THREAD_NAME_LEN-1] = 0;

    /* Scan rest of string. */
    sscanf(close_paren + 1, " %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d "
                 "%lu %lu %*d %*d %*d %*d %*d %*d %*d %lu %ld",
                 &proc->state, &proc->utime, &proc->stime, &proc->vss, &proc->rss);

    return 0;
}


static void add_proc(int proc_num, struct proc_info *proc)
{
    int i;

    if (proc_num >= num_new_procs) {
        new_procs = (proc_info**)realloc(new_procs, 2 * num_new_procs * sizeof(struct proc_info *));
        if (!new_procs) die("Could not expand procs array.\n");
        for (i = num_new_procs; i < 2 * num_new_procs; i++)
            new_procs[i] = NULL;
        num_new_procs = 2 * num_new_procs;
    }
    new_procs[proc_num] = proc;
}



static int read_cmdline(char *filename, struct proc_info *proc) {
    FILE *file;
    char line[MAX_LINE];

    line[0] = '\0';
    file = fopen(filename, "r");
    if (!file) return 1;
    fgets(line, MAX_LINE, file);
    fclose(file);
    if (strlen(line) > 0) {
        strncpy(proc->name, line, PROC_NAME_LEN);
        proc->name[PROC_NAME_LEN-1] = 0;
    } else
        proc->name[0] = 0;
    return 0;
}

static void read_policy(int pid, struct proc_info *proc)
{
    /*
    SchedPolicy p;
    if (get_sched_policy(pid, &p) < 0)
        strcpy(proc->policy, "unk");
    else {
        if (p == SP_BACKGROUND)
            strcpy(proc->policy, "bg");
        else if (p == SP_FOREGROUND)
            strcpy(proc->policy, "fg");
        else
            strcpy(proc->policy, "er");
    }*/
}

static int read_status(char *filename, struct proc_info *proc)
{
    FILE *file;
    char line[MAX_LINE];
    unsigned int uid, gid;

    file = fopen(filename, "r");
    if (!file) return 1;
    while (fgets(line, MAX_LINE, file))
    {
        sscanf(line, "Uid: %u", &uid); //User Identification
        sscanf(line, "Gid: %u", &gid); //Group Indentification
    }
    fclose(file);
    proc->uid = uid; proc->gid = gid;
    return 0;
}

static void print_procs(void)
{
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
                new_procs[i]->delta_utime = new_procs[i]->utime - old_proc->utime; // user space time
                new_procs[i]->delta_stime = new_procs[i]->stime - old_proc->stime; // sys  space time
            }
            else
            {
                new_procs[i]->delta_utime = 0;
                new_procs[i]->delta_stime = 0;
            }
            new_procs[i]->delta_time = new_procs[i]->delta_utime + new_procs[i]->delta_stime;
        }
    }

    // analysis the cpu run time
    total_delta_time = (new_cpu.utime + new_cpu.ntime + new_cpu.stime + new_cpu.itime
                        + new_cpu.iowtime + new_cpu.irqtime + new_cpu.sirqtime)
                     - (old_cpu.utime + old_cpu.ntime + old_cpu.stime + old_cpu.itime
                        + old_cpu.iowtime + old_cpu.irqtime + old_cpu.sirqtime);

    //qsort(new_procs, num_new_procs, sizeof(struct proc_info *), proc_cmp);

    printf("\n\n\n");
    cpu_user=((new_cpu.utime + new_cpu.ntime) - (old_cpu.utime + old_cpu.ntime)) * 100  / total_delta_time;
    cpu_sys=((new_cpu.stime ) - (old_cpu.stime)) * 100 / total_delta_time;
    printf("User %ld%%, System %ld%%, IOW %ld%%, IRQ %ld%%\n",
            ((new_cpu.utime + new_cpu.ntime) - (old_cpu.utime + old_cpu.ntime)) * 100  / total_delta_time,
            ((new_cpu.stime ) - (old_cpu.stime)) * 100 / total_delta_time,
            ((new_cpu.iowtime) - (old_cpu.iowtime)) * 100 / total_delta_time,
            ((new_cpu.irqtime + new_cpu.sirqtime)
                    - (old_cpu.irqtime + old_cpu.sirqtime)) * 100 / total_delta_time);
    printf("User %ld + Nice %ld + Sys %ld + Idle %ld + IOW %ld + IRQ %ld + SIRQ %ld = %ld\n",
            new_cpu.utime - old_cpu.utime,
            new_cpu.ntime - old_cpu.ntime,
            new_cpu.stime - old_cpu.stime,
            new_cpu.itime - old_cpu.itime,
            new_cpu.iowtime - old_cpu.iowtime,
            new_cpu.irqtime - old_cpu.irqtime,
            new_cpu.sirqtime - old_cpu.sirqtime,
            total_delta_time);
    printf("\n");

    // single threads
    if (!threads)
        printf("%5s %4s %1s %5s %7s %7s %3s %-8s %s\n", "PID", "CPU%", "S", "#THR", "VSS", "RSS", "PCY", "UID", "Name");
    else
        printf("%5s %5s %4s %1s %7s %7s %3s %-8s %-15s %s\n", "PID", "TID", "CPU%", "S", "VSS", "RSS", "PCY", "UID", "Thread", "Proc");


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
            printf("%5d %3ld%% %c %5d %6ldK %6ldK %3s %-8.8s %s\n", proc->pid, proc->delta_time * 100 / total_delta_time, proc->state, proc->num_threads,
                proc->vss / 1024, proc->rss * getpagesize() / 1024, proc->policy, user_str, proc->name[0] != 0 ? proc->name : proc->tname);
        else
            printf("%5d %5d %3ld%% %c %6ldK %6ldK %3s %-8.8s %-15s %s\n", proc->pid, proc->tid, proc->delta_time * 100 / total_delta_time, proc->state,
                proc->vss / 1024, proc->rss * getpagesize() / 1024, proc->policy, user_str, proc->tname, proc->name);
    }
}

static struct proc_info *find_old_proc(pid_t pid, pid_t tid)
{
    int i;
    // travel to find the old proc
    for (i = 0; i < num_old_procs; i++)
        if (old_procs[i] && (old_procs[i]->pid == pid) && (old_procs[i]->tid == tid))
            return old_procs[i];

    return NULL;
}

static void free_old_procs(void)
{
    int i;
    for (i = 0; i < num_old_procs; i++)
        if (old_procs[i])
            free_proc(old_procs[i]);

    free(old_procs);
}




/*
 * vss: Virtual Set Size, including the share lib
 *      only when the first time the process access the mem, then mapping between the real and vir mem establish
 * rss: Resident Set Size, including the share lib
 * pss: Proportional Size , the share lib's size shared
*/
static int proc_vss_cmp(const void *a, const void *b)
{
    struct proc_info *pa, *pb;

    pa = *((struct proc_info **)a); pb = *((struct proc_info **)b);

    if (!pa && !pb) return 0;
    if (!pa) return 1;
    if (!pb) return -1;

    return -numcmp(pa->vss, pb->vss);
}

static int proc_rss_cmp(const void *a, const void *b) {
    struct proc_info *pa, *pb;

    pa = *((struct proc_info **)a); pb = *((struct proc_info **)b);

    if (!pa && !pb) return 0;
    if (!pa) return 1;
    if (!pb) return -1;

    return -numcmp(pa->rss, pb->rss);
}

// D: thread num compare
static int proc_thr_cmp(const void *a, const void *b)
{
    struct proc_info *pa, *pb;

    pa = *((struct proc_info **)a); pb = *((struct proc_info **)b);

    if (!pa && !pb) return 0;
    if (!pa) return 1;
    if (!pb) return -1;

    return -numcmp(pa->num_threads, pb->num_threads);
}

static int proc_cpu_cmp(void const  *a, void const *b)
{
    struct proc_info *pa, *pb;
    pa = *((struct proc_info **)a); pb = *((struct proc_info **)b);

    if (!pa && !pb) return 0;
    if (!pa) return 1;
    if (!pb) return -1;
    return -numcmp(pa->delta_time, pb->delta_time);
}

static int numcmp(long long a, long long b)
{
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}



//static void usage(char *cmd)
//{
//    fprintf(stderr, "Usage: %s [ -m max_procs ] [ -n iterations ] [ -d delay ] [ -s sort_column ] [ -t ] [ -h ]\n"
//                    "    -m num  Maximum number of processes to display.\n"
//                    "    -n num  Updates to show before exiting.\n"
//                    "    -d num  Seconds to wait between updates.\n"
//                    "    -s col  Column to sort by (cpu,vss,rss,thr).\n"
//                    "    -t      Show threads instead of processes.\n"
//                    "    -h      Display this help screen.\n",
//        cmd);
//}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setProcess();
    setRescources();
    // init process
    int i;
    num_used_procs = num_free_procs = 0;

    max_procs = 0;
    delay = 1;
    iterations = -1;
    int (*proc_cmp)(const void *a, const void *b)=NULL;
    proc_cmp = &proc_cpu_cmp;

    free_procs = NULL;

    num_new_procs = num_old_procs = 0;
    new_procs = old_procs = NULL;

    read_procs();
//    while ((iterations == -1) || (iterations-- > 0))
//    {
//        old_procs = new_procs;
//        num_old_procs = num_new_procs;
//        memcpy(&old_cpu, &new_cpu, sizeof(old_cpu));
//        sleep(delay);
//        read_procs();
//        print_procs();
//        free_old_procs();
//    }
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
    QListWidgetItem *title = new QListWidgetItem("PID\t" + QString::fromUtf8("名称") + "\t\t" +
                                                 QString::fromUtf8("状态") + "\t" +
                                                 QString::fromUtf8("优先级") + "\t" +
                                                 QString::fromUtf8("占用内存"), ui->listWidget);
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
//    QString temp;
//    temp = QString::number(totalProNum, 10);
//    ui->label_pNum->setText(temp);
//    temp = QString::number(number_of_run, 10);
//    ui->label_pRun->setText(temp);
//    temp = QString::number(number_of_sleep, 10);
//    ui->label_pSleep->setText(temp);
//    temp = QString::number(number_of_zombie, 10);
//    ui->label_pZombie->setText(temp);
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
    axisX->setTitleText("axisX");

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0,100);
    axisY->setTitleText("axisY");

    m_chart->setAxisX(axisX,m_series);
    m_chart->setAxisY(axisY,m_series);
    m_chart->legend()->hide();
    m_chart->setTitle("demo");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    ui->tab_2->setLayout(layout);
    timeId = startTimer(100);

}


double MainWindow::getData(double time)
{
//    qDebug()<<time;
//    double s = qCos( time * M_PI * 2 ) ;
    double s;
    QString tempStr; //读取文件信息字符串
    QFile tempFile; //用于打开系统文件
    int pos; //读取文件的位置
    tempFile.setFileName("/proc/meminfo"); //打开内存信息文件
    if ( !tempFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this, tr("warning"), tr("The meminfo file can not open!"), QMessageBox::Yes);
        return s;
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

//    ui->label_RAM_Used->setText(memUsed+" MB");
//    ui->label_RAM_Left->setText(memFree+" MB");
//    ui->label_RAM_Total->setText(memTotal+" MB");
//    ui->label_SWAP_Used->setText(swapUsed+" MB");
//    ui->label_SWAP_Left->setText(swapFree+" MB");
//    ui->label_SWAP_Total->setText(swapTotal+" MB");

//    ui->progressBar_RAM->setValue(nMemUsed*100/nMemTotal);
//    ui->progressBar_SWAP->setValue(nSwapUsed*100/nSwapTotal);
    tempFile.close(); //关闭内存信息文件
    return s;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==timeId)//定时器到时间,//模拟数据填充
    {
        // use top to analysis
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

        static QTime dataTime(QTime::currentTime());
        long int eltime = dataTime.elapsed();
        static int lastpointtime = 0;
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
//            for(int k=0;k<size;++k)
//            {
////                points.append(QPointF(k+sizePoints,getData((((double)lastpointtime+k+1)/1000))));


//            }
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
    setProcess();
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
