#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setProcess();
    //ui->tabWidget->tabPosition()


    //connect(processTimer, SIGNAL(timeout()), widProcesses, SLOT(refreshTimer()));
//    connect(socketTimer, SIGNAL(timeout()), widResources, SLOT(refreshTimer()));
//    connect(&dlgSettings, SIGNAL(settingsChanged()), this, SLOT(refreshSettings()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch (index)
    {
    case 0:
        qDebug()<<"setProcess";
        setProcess();
        break;
    case 1:
        setRescources();
        break;
    case 2:
        setFileSystem();
        break;
    default:
        break;
    }
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

}





void MainWindow::setFileSystem()
{

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
