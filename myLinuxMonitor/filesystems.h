#ifndef FILESYSTEMS_H
#define FILESYSTEMS_H


#include <QString>
#include <QTimer>
#include <QProcess>
#include <QDebug>
#include <QObject>
#include "sys/statfs.h"
#include <stdio.h>
#include <mntent.h>
#include <string.h>
#include <sys/vfs.h>

static int m_timer_interval = 1000;
static double m_disk_read = 0;
static double m_disk_write = 0;

static const unsigned long long G = 1024*1024*1024ull;
static const unsigned long long M = 1024*1024;
static const unsigned long long K = 1024;
static char str[20];


bool get_disk_speed();
bool get_disk_space();
bool get_path_space(QString  path);

static int device_num;
static char mount_on_device[20];
static QString device_info[20][5];

char* kscale(unsigned long b, unsigned long bs);
int mydf();







#endif //FILESYSTEMS_H
