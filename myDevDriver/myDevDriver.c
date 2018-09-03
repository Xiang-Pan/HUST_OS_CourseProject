#include "linux/kernel.h"
#include "linux/module.h"
#include "linux/fs.h"
#include "linux/init.h" // dd init and exit
#include "linux/types.h"
#include "linux/errno.h"
#include <linux/uaccess.h>
#include <linux/kdev_t.h>
#include <linux/types.h>

#define MAX_SIZE 1024

int my_open(struct inode *inode, struct file *file);
int my_release(struct inode *inode, struct file *file);
ssize_t my_read(struct file *file, char __user *user, size_t t, loff_t *f);
ssize_t my_write(struct file *file, const char __user *user, size_t t, loff_t *f);

char message[MAX_SIZE] = "--------welcome---------";



int devNum;           
char* devName = "myDrive";

struct file_operations pStruct=
{
   open : my_open,
   release : my_release,
   read : my_read,
   write : my_write,
};

/*
static struct char_device_struct 
{
	struct char_device_struct *next;
	unsigned int major;
	unsigned int baseminor;
	int minorct;
	char name[64];
	struct cdev *cdev;		// will die 
} *chrdevs[CHRDEV_MAJOR_HASH_SIZE];
*/

//D:init module
//I: 
//  devNum:0 present dynamic alloc
//  devName
//  fOp_ptr
//O:init result
int init_module()
{
    int ret = register_chrdev(0,devName,&pStruct);
    if(ret < 0)
    {
        printk("regist fail!\n");
        return -1;
    }
    else
    {
        printk("the myDrive has been registered!\n");
        devNum = ret;
        printk("<<---the myDrive's number(id): %d\n",ret);
        printk("<<---create a dev file ,please enter:\n");
        printk("<<----'mknod /dev/myDrive c %d 0'----\n",devNum);
        printk("<<---delete device,please enter < rm /dev/%s >",devName);
        printk("<<---delete module , please enter < rmmode device >");
        return 0;
    }

}

//D: unregister module
//I: devNum, devName
void unregister_module(void)
{
    unregister_chrdev(devNum,devName);
    printk("unregister success !\n");
}

int my_open(struct inode *inode, struct file *file)
{
    printk("open myDrive OK ! \n");
    try_module_get(THIS_MODULE);
    return 0;
}


int my_release(struct inode *inode, struct file *file)
{
    printk("Device released !\n");
    module_put(THIS_MODULE);  //Reference amount minus 1

    return 0;
}

ssize_t my_read(struct file *file, char __user *user, size_t t, loff_t *f)
{
    if (copy_to_user(user ,message,sizeof(message)))
    {
        return -2;
    }
    return sizeof(message);
}

ssize_t my_write(struct file *file, const char __user *user, size_t t, loff_t *f)
{
    if(copy_from_user(message,user,sizeof(message)))
    {
        return -3;
    }
    return sizeof(message);
}