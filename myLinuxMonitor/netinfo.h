/* FileName:    netinfo.h
 * Author:      Hover
 * E-Mail:      hover@hust.edu.cn
 * GitHub:      HoverWings
 * Description: net module
 */
#ifndef NETINFO_H
#define NETINFO_H

#include <QDebug>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <algorithm>

#define NET_INFO_PATH "/proc/net/dev"

#define NET_DIFF_TIME 1

using namespace std;


extern time_t net_previous_timeStamp;
extern time_t net_current_timeStamp;
extern double net_dif_time;

extern char net_file[64];
extern int net_off;
extern int line_num;
extern FILE *net_stream;
extern char net_buffer[256];
extern char *net_line_return;
extern char net_tmp_itemName[32];

extern int net_itemReceive;
extern int net_itemTransmit;

extern int net_current_receive_total;
extern int net_previous_receive_total;
extern int net_receive_speed;

extern int net_current_transmit_total;
extern int net_previous_transmit_total;
extern int net_transmit_speed;

extern float net_averageLoad_speed;
//(net_current_receive_total - net_previous_receive_total) + (net_current_transmit_total - net_previous_transmit_total) / 2
extern float net_result;

extern int max_net_speed;
extern vector<int> net_speed_60;


void  net_init();
void  net_update();
void  net_close();




#endif // NETINFO_H
