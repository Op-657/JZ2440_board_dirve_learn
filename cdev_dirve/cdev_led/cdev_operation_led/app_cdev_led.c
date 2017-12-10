#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>

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
	char led_buff[LED_SIZE] = {1, 1, 1};
	int count = 0;
	for (count = 0; count < LED_SIZE; count++)
	{
		printf("led_buff[%d] = %d\r\n", count, led_buff[count]);
	}
	printf("after for\r\n");
	sleep(1);
	fd = open("/dev/node_file", O_RDWR);
	if(fd < 0)
	{
		perror("open error");
		return 0;
	}
	count = write(fd, led_buff, 3);
	printf("after write: count: %d\r\n", count);
	count = lseek(fd, 0 , SEEK_CUR);
	printf("after lseek: count: %d\r\n", count);
	ioctl(fd, MEMDEV_IOCGETDATA);
	ioctl(fd, MEMDEV_IOCSETDATA, 12);
	ioctl(fd, MEMDEV_IOCPRINT, 13);
	

}	
