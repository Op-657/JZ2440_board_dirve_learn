/* drv_chardev_early.c */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/device.h>

/* 自动创建设备文件节点的两大结构体 */
static struct class *char_dev_class; //class user to create /dev/node
static struct class_device *this_device;
 
#define DEVICE_NAME	"cdev_led"

/* led 寄存器基地址 */
unsigned int *base_addr = 0;

/* 控制3个led的 寄存器地址 */
#define rGPFCON (*(volatile unsigned *)(base_addr + 0))
#define rGPFDAT (*(volatile unsigned *)(base_addr + 1))
#define rGPFUP (*(volatile unsigned *)(base_addr + 2))

#define DEV_SIZE	(3)

int major = 0;

static int first_chardev_open(struct inode *pinode, struct file *pfile)
{
	printk("Linux chardev:%s is call\r\n",__FUNCTION__ );
	return 0;
}

#define MEMDEV_IOC_MAGIC		'k'

#define MEMDEV_IOCPRINT			_IO(MEMDEV_IOC_MAGIC, 1)
#define MEMDEV_IOCGETDATA		_IOR(MEMDEV_IOC_MAGIC, 2, int)
#define MEMDEV_IOCSETDATA		_IOW(MEMDEV_IOC_MAGIC, 3, int)

#define MEMDEV_IOC_MAXNR 		(3)


static int first_chardev_ioctl(
	struct inode *inode,
	struct file *file,
	unsigned int cmd,
	unsigned long arg)
{
	int err = 0;
	int ret = 0;
	int ioarg = 0;

	if(_IOC_TYPE(cmd) != MEMDEV_IOC_MAGIC)
	{
		printk("type(cmd) != MEMDDEV_IOC_MAGIC");
		return -EINVAL;
	}
	
	if (_IOC_NR(cmd) > MEMDEV_IOC_MAXNR)
		return -EINVAL;

	if(_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
	else if(_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd)); 
	if(err)
		return -EFAULT;

	switch(cmd){
		case MEMDEV_IOCPRINT:
			printk("cmd : print\r\n");
			break;

		case MEMDEV_IOCGETDATA:
			ioarg = 1101;	
			ret = __put_user(ioarg, (int *)arg);
			break;

		case MEMDEV_IOCSETDATA:
			ret =__get_user(ioarg, (int *)arg);
			printk("<---in kernel MEMDEV_IOCSETDATA ioarg=%d-----> \r\n", ioarg);
			break;
		
		default:
			return -EINVAL;
	}
	return ret;
}

ssize_t first_chardev_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	int retval = 0;
	char led_buffer[DEV_SIZE]= {2, 2, 2};
	printk(KERN_EMERG"Linux chardev: %s is call\r\n", __FUNCTION__);
	printk("count : %d\r\n", count);

	led_buffer[0] = !(rGPFDAT & (1 << 4));
	led_buffer[1] = !(rGPFDAT & (1 << 5));
	led_buffer[2] = !(rGPFDAT & (1 << 6));
	if (*offset >= DEV_SIZE)
	{
		printk("read offset out range\r\n");
		return 0;
	}
	
	if (*offset + count > DEV_SIZE)
		count = DEV_SIZE - *offset;
	if(copy_to_user(buf, &led_buffer[*offset], count)) 
	{
		retval = -EFAULT;
		return 0;
	}
	*offset += count;
	return count;
}

ssize_t first_chardev_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	char led_buffer1[DEV_SIZE]={3, 3, 3};
	int retval = 0;
	int i = 0;
	if(*offset >= DEV_SIZE)
	{
		printk("write out the range\r\n");
		return -1;
	}
	if(copy_from_user((&led_buffer1[*offset]), buf, count))
	{
		retval = -EFAULT;
		return -1;
	}

	for(i=0; i<count; i++)
	{

		printk("led_buffer1[%d]=",*offset + i);
		printk("%d\r\n", led_buffer1[*offset + i]);
		if(led_buffer1[(*offset) + i] == 1)
		{
			rGPFDAT &= ~(1 << (4 + *offset + i));
		}
		else if(led_buffer1[*offset + i] == 0)
			rGPFDAT |= ~(1 << (4 + *offset + i));
		else
		{
			printk("data only set 0 or 1\r\n");
		}
		
	}
	*offset += count;
	return retval;
}

loff_t first_chardev_llseek(struct file *file, loff_t off, int whence)
{
	
	loff_t newpos = 0;
	int offset = off;
	printk("Linux chardev: %s is call\r\n", __FUNCTION__);
	switch(whence)
	{
		case (SEEK_SET):
			newpos = offset;
			break;

		case SEEK_CUR:
			newpos = file->f_pos + offset;
			break;

		case SEEK_END:
			newpos = 4 + offset;
			break;
		
		default:
			return -EINVAL;
	}

	if(newpos < 0)
		return -EINVAL;

	file->f_pos = newpos;
	return newpos;
}

int first_chardev_release(struct inode *inode, struct file *file)
{

	printk("Linux chardev: %s is call\r\n",__FUNCTION__);
	return 0;
}

/* 填充 文件操纵结构体 */
static struct file_operations dev_fops={
	.owner = THIS_MODULE,
	.ioctl = first_chardev_ioctl,
	.open = first_chardev_open,
	.read = first_chardev_read,
	.write = first_chardev_write,
	.release = first_chardev_release, 
	.llseek = first_chardev_llseek,
};	


static int __init first_chardev_init(void)
{
	int ret;
	printk("module init now\r\n");
	
	/* 建立物理地址和虚拟地址映射 */
	base_addr = (unsigned int *)ioremap(0x56000050, 16);
	rGPFCON &= ~(3 << 8 | 3 << 10 | 3 << 12); // clear 
	rGPFCON |= (1 << 8 | 1 << 10 | 1 << 12); // config output

	rGPFUP &=~(1 << 4 | 1 << 5 | 1 << 6);//配置成上拉模式

	rGPFDAT &= ~(1 << 4 | 1 << 5 | 1 << 6); //端口置1， led初始不亮
	//rGPFDAT |= (1 << 3 | 1 << 4 | 1 << 6); //dengbuliang

	major = register_chrdev(0, DEVICE_NAME, &dev_fops);
	if(major < 0)
	{
		printk("major error");
		return 0;
	}
	printk("major : %d\r\n", major);
	printk("initialized\n");
	
	
	char_dev_class = class_create(THIS_MODULE, DEVICE_NAME);
	if(IS_ERR(char_dev_class))
	{
		printk("Err: failed in creating class\n");
		unregister_chrdev(major, DEVICE_NAME);
		return PTR_ERR(char_dev_class);
	}
	
	this_device = class_device_create(char_dev_class, NULL, MKDEV(major, 0), NULL, "node_file");
	if(IS_ERR(this_device))
	{
		ret = PTR_ERR(this_device);
		class_destroy(char_dev_class);
		unregister_chrdev(major, DEVICE_NAME);
		return 0;
	}
	ret = major;
	return (ret);
}

static void __exit first_chardev_exit(void)
{
	device_destroy(char_dev_class, MKDEV(major, 0));
	class_destroy(char_dev_class);
	unregister_chrdev(major, DEVICE_NAME);
	printk("Goodbye, cruel world!, priotity = 0\n");
}

module_init(first_chardev_init);
module_exit(first_chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ruiwen");
MODULE_DESCRIPTION("This is Character device driver");
