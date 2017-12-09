#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <error.h>

#define DEVICE_NAME		"cdev_1" 
int main(void)
{
	int fd;
	char buf[3];
	fd = open(DEVICE_NAME, O_RDWR);
	if(fd < 0)
		perror("fd error");
	read(fd, buf, 1);
	write(fd, buf, 1);
	ioctl(fd, 0);
	close(fd);
	return 0;
}
