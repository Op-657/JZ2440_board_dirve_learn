#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "mycmdioctl.h" 

#define LED_SIZE	3

extern int errno;

#define 	MEMDEV_IOC_MAGIC		'k'

#define		MEMDEV_IOCPRINT			_IO(MEMDEV_IOC_MAGIC, 1) 
#define 	MEMDEV_IOCGETDATA		_IOR(MEMDEV_IOC_MAGIC, 2, int)
#define 	MEMDEV_IOCSETDATA		_IOW(MEMDEV_IOC_MAGIC, 3, int)

#define MEMDEV_IOC_MAXNR			3



int main()
{
	int fd=0;
	int cmd = 0;
	int i = 0;
	int count = 0;
	printf("after for\r\n");
	sleep(1);
	fd = open("/dev/node_file", O_RDWR);
	if(fd < 0)
	{
		perror("open error");
		return 0;
	}
	for (i=0; i<3; i++)
	{
		for(count=0; count<LED_SIZE; count++)	
		{
			
			ioctl(fd, MYCMD_IOC_LEDS_CUR_ON_SET, &cmd);
			ioctl(fd, MYCMD_IOC_POINT_INC);
			sleep(1);
		}
	ioctl(fd, MYCMD_IOC_LEDS_ALL_OFF_SET,&cmd);
	ioctl(fd, MYCMD_IOC_POINT_RESET);

	}
	return 0;
}	
