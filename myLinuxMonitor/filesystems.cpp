#include "filesystems.h"

int device_num;
char mount_on_device[20];
QString device_info[20][6];


char* kscale(unsigned long b, unsigned long bs)
{
    unsigned long long size = b * (unsigned long long)bs;
    if (size > G)
    {
        sprintf(str, "%0.2f GB", size/(G*1.0));
        return str;
    }
    else if (size > M)
    {
        sprintf(str, "%0.2f MB", size/(1.0*M));
        return str;
    }
    else if (size > K)
    {
        sprintf(str, "%0.2f K", size/(1.0*K));
        return str;
    }
    else
    {
        sprintf(str, "%0.2f B", size*1.0);
        return str;
    }
}

int mydf()
{
    device_num=0;
    char str[500];
    FILE* mount_table;
    struct mntent *mount_entry;
    struct statfs s;
    unsigned long blocks_used;
    unsigned blocks_percent_used;
    const char *disp_units_hdr = NULL;
    mount_table = NULL;
    mount_table = setmntent("/etc/mtab", "r");
    if (!mount_table)
    {
        fprintf(stderr, "set mount entry error/n");
        return -1;
    }
    disp_units_hdr = "     Size";
//    printf("Filesystem           %-15sUsed Available %s Mounted on/n",
//            disp_units_hdr, "Use%");
    while (1)
    {
        const char *device;
        const char *mount_point;
        if (mount_table)
        {
            mount_entry = getmntent(mount_table);
            if (!mount_entry)
            {
                endmntent(mount_table);
                break;
            }
        }
        else
            continue;
        device = mount_entry->mnt_fsname;
        mount_point = mount_entry->mnt_dir;
        //fprintf(stderr, "mount info: device=%s mountpoint=%s/n", device, mount_point);
        if (statfs(mount_point, &s) != 0)
        {
            fprintf(stderr, "statfs failed!/n");
            continue;
        }
        if ((s.f_blocks > 0) || !mount_table )
        {
            blocks_used = s.f_blocks - s.f_bfree;
            blocks_percent_used = 0;
            if (blocks_used + s.f_bavail)
            {
                blocks_percent_used = (blocks_used * 100ULL
                        + (blocks_used + s.f_bavail)/2
                        ) / (blocks_used + s.f_bavail);
            }
            /* GNU coreutils 6.10 skips certain mounts, try to be compatible.  */
            if (strcmp(device, "rootfs") == 0)
                continue;
            if (printf("/n%-20s" + 1, device) > 20)
                    printf("/n%-20s", "");
            char s1[20];
            char s2[20];
            char s3[20];
            strcpy(s1, kscale(s.f_blocks, s.f_bsize));
            strcpy(s2, kscale(s.f_blocks - s.f_bfree, s.f_bsize));
            strcpy(s3, kscale(s.f_bavail, s.f_bsize));
//            printf(" %9s %9s %9s %3u%% %s/n",
//                    s1,
//                    s2,
//                    s3,
//                    blocks_percent_used, mount_point);
//            printf(" %9s %9s %9s %3u%% %s/n",
//                    s1,
//                    s2,
//                    s3,
//                    blocks_percent_used, mount_point);
            device_info[device_num][0]=QString(device);
            device_info[device_num][1]=QString(s1);
            device_info[device_num][2]=QString(s2);
            device_info[device_num][3]=QString(s3);
//            qDebug()<<device_num<<"blocks_percent_used"<<blocks_percent_used;
            device_info[device_num][4]=QString::number(blocks_percent_used);
            device_info[device_num][5]=QString(mount_point);
            device_num++;
        }

    }
    return device_num;
//    printf("%s",str);
}
