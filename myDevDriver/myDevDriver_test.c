#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_SIZE 1024

int main()
{
	int fd;
	char buf[MAX_SIZE]; 
	char get[MAX_SIZE]; // to be written

	char devName[20];
	char dir[50] = "/dev/";
	system("ls /dev/");
	printf("Please input the device's name: ");
	gets(devName);
	strcat(dir, devName);
	fd = open(dir, O_RDWR | O_NONBLOCK); 
	if( fd != -1)
	{
		// get str from buf
		read( fd , buf , sizeof(buf) );
		printf( "%s\n" , buf);

        // read
		printf( "Please input a string : ");
		gets(get);
		write( fd , get , sizeof(get) );

		// read back
		read(fd, buf, sizeof(buf));
		printf("device Message : %s\n", buf);

		close(fd);
		return 0;
	}
	else
	{
		printf("Device open failed !\n");
		return -1;
	}

}