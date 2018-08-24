#ifndef PROCESSES_H
#define PROCESSES_H

#include <ctype.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


class Processes
{
public:
    Processes();

};

struct cpu_info {
    long unsigned utime, ntime, stime, itime;
    long unsigned iowtime, irqtime, sirqtime;
};

#define PROC_NAME_LEN 64
#define THREAD_NAME_LEN 32

struct proc_info
{
    struct proc_info *next;
    pid_t pid;
    pid_t tid;
    uid_t uid;
    gid_t gid;
    char name[PROC_NAME_LEN];
    char tname[THREAD_NAME_LEN];
    char state;
    long unsigned utime;
    long unsigned stime;
    long unsigned delta_utime;
    long unsigned delta_stime;
    long unsigned delta_time;
    long vss;
    long rss;
    int num_threads;
    char policy[32];
};

struct proc_list
{
    struct proc_info **array;
    int size;
};

#define INIT_PROCS 50
#define THREAD_MULT 8












#endif // PROCESSES_H
