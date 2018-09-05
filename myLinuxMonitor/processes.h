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
#include <QVector>
#include <QString>
#include <iostream>

using namespace std;
/*
us: user cpu time (or) % CPU time spent in user space
sy: system cpu time (or) % CPU time spent in kernel space
ni: user nice cpu time (or) % CPU time spent on low priority processes
id: idle cpu time (or) % CPU time spent idle
wa: io wait cpu time (or) % CPU time spent in wait (on disk)
hi: hardware irq (or) % CPU time spent servicing/handling hardware interrupts
si: software irq (or) % CPU time spent servicing/handling software interrupts
st: steal time - - % CPU time in involuntary wait by virtual cpu while hypervisor is servicing another processor (or) % CPU time stolen from a virtual machine
*/
/*
 * utime: user
 * stime: system time
 * ntime: nice time, the time for modefiy the priority of cpu
 * itime: idle time
 * iowtime: io waiting time
 * irqtime: interuption time
 * sirqtime: soft interuption time
*/
struct cpu_info
{
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

#define die(...) { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); }

#define INIT_PROCS 50
#define THREAD_MULT 8


#define MAX_LINE 256
extern QVector<QVector<QString>> process_table;
extern QVector<QString> process_vec;
extern QVector<QString> process_title;
extern double cpu_user;
extern double cpu_sys;
extern struct proc_info **old_procs, **new_procs;
extern int num_old_procs, num_new_procs;
extern struct proc_info *free_procs;
extern int num_used_procs, num_free_procs;
extern int max_procs, delay, iterations, threads;
extern struct cpu_info old_cpu, new_cpu;

struct proc_info *alloc_proc(void);
void free_proc(struct proc_info *proc);
void read_procs(void);
int read_stat(char *filename, struct proc_info *proc);
void read_policy(int pid, struct proc_info *proc);
void add_proc(int proc_num, struct proc_info *proc);
int read_cmdline(char *filename, struct proc_info *proc);
int read_status(char *filename, struct proc_info *proc);
void print_procs(void);
struct proc_info *find_old_proc(pid_t pid, pid_t tid);
void free_old_procs(void);

extern int (*proc_cmp)(const void *a, const void *b);
//extern int (*a)(const void *a, const void *b);

int proc_cpu_cmp(const void *a, const void *b);
int proc_vss_cmp(const void *a, const void *b);
int proc_rss_cmp(const void *a, const void *b);
int proc_thr_cmp(const void *a, const void *b);
int numcmp(long long a, long long b);
void usage(char *cmd);

#endif // PROCESSES_H
