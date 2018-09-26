/* FileName:netinfo.cpp
 * Author:Hover
 * E-Mail:hover@hust.edu.cn
 * GitHub:HoverWings
 * Description: net module
 */
#include "netinfo.h"

time_t net_previous_timeStamp;
time_t net_current_timeStamp;
double net_dif_time;

char net_file[64]= {NET_INFO_PATH};
int net_off;
int line_num;
FILE *net_stream;
char net_buffer[256];
char *net_line_return;
char net_tmp_itemName[32];

int net_itemReceive;
int net_itemTransmit;

int net_current_receive_total;
int net_previous_receive_total;
int net_receive_speed;

int net_current_transmit_total;
int net_previous_transmit_total;
int net_transmit_speed;

float net_averageLoad_speed;
//(net_current_receive_total - net_previous_receive_total) + (net_current_transmit_total - net_previous_transmit_total) / 2
float net_result;

bool first=true;
int max_net_speed;
vector<int> net_speed_60;


using namespace std;

void net_init()
{
    max_net_speed=0;
    net_line_return = "INIT";
    net_stream = fopen (net_file, "r");
    net_off = fseek(net_stream, 0, SEEK_SET);

    net_update();

    net_previous_receive_total = net_current_receive_total;
    net_previous_transmit_total = net_current_transmit_total;

    net_receive_speed = 0;
    net_transmit_speed = 0;
    net_averageLoad_speed = 0.0;
    net_previous_timeStamp = net_current_timeStamp = time(NULL);
    net_dif_time = 0;

}


void net_update()
{
    net_previous_receive_total = net_current_receive_total;
    net_previous_transmit_total = net_current_transmit_total;
    net_current_receive_total = 0;
    net_current_transmit_total = 0;


    net_off = fseek(net_stream, 0, SEEK_SET);

    line_num = 1;
    net_line_return = fgets (net_buffer, sizeof(net_buffer), net_stream);//读取第一行
    //printf("[net_update] line %d: %s\n", line_num, net_line_return);
    line_num++;
    net_line_return = fgets (net_buffer, sizeof(net_buffer), net_stream);//读取第二行
    //printf("[net_update] line %d: %s\n", line_num, net_line_return);

    net_itemReceive = 0;
    net_itemTransmit = 0;
    int flag = 1;
    while(flag == 1)
    {
        line_num++;
        net_line_return = fgets (net_buffer, sizeof(net_buffer), net_stream);
        net_itemReceive = 0;
        net_itemTransmit = 0;
        if(net_line_return != NULL)
        {
            sscanf( net_buffer,
                "%s%d%d%d%d%d%d%d%d%d",
                net_tmp_itemName,
                &net_itemReceive,
                &net_itemTransmit,
                &net_itemTransmit,
                &net_itemTransmit,
                &net_itemTransmit,
                &net_itemTransmit,
                &net_itemTransmit,
                &net_itemTransmit,
                &net_itemTransmit);
            net_current_receive_total += net_itemReceive;
            net_current_transmit_total += net_itemTransmit;
        }
        else
        {
            flag = -1;
        }

    }
    net_receive_speed = (net_current_receive_total - net_previous_receive_total) / NET_DIFF_TIME;
    net_transmit_speed = (net_current_transmit_total - net_previous_transmit_total) / NET_DIFF_TIME;
    net_averageLoad_speed = (net_receive_speed + net_transmit_speed) / 2;
    if(first)
    {
        net_receive_speed=0;
        net_transmit_speed=0;
        first=false;
    }

    net_dif_time = (double)(net_current_timeStamp - net_previous_timeStamp);
    net_current_timeStamp = time(NULL);
    if( (net_dif_time) >= 60 )
    {
        net_previous_timeStamp = net_current_timeStamp;
        auto max=max_element(net_speed_60.begin(),net_speed_60.end());
        max_net_speed=*max;
        net_speed_60.clear();
    }
    else
    {
        if(net_receive_speed>max_net_speed)
        {
            max_net_speed=net_receive_speed;
            net_previous_timeStamp = net_current_timeStamp;
            net_speed_60.clear();
        }
        if(net_transmit_speed>max_net_speed)
        {
            max_net_speed=net_transmit_speed;
            net_previous_timeStamp = net_current_timeStamp;
            net_speed_60.clear();
        }
        net_speed_60.insert(net_speed_60.begin(),net_receive_speed);
        net_speed_60.insert(net_speed_60.begin(),net_transmit_speed);
    }
}


void net_close()
{
    fclose(net_stream);     //关闭文件net_stream
}
