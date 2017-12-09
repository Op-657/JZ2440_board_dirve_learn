/* drv_chardev_early.c */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>

/* 设备的名字 */
#define DEVICE_NAME	"first_chardev"

/* 主设备号 */
int major = 0;


static int first_chardev_open(struct inode *pinode, struct file *pfile)
{
	printk("Linux chardev:%s is call\r\n",__FUNCTION__ );
	return 0;
}


static int first_chardev_ioctl(
	struct inode *inode,
	struct file *file,
	unsigned int cmd,
	unsigned long arg)
{
	printk("Linux chardev: %s is call\r\n", __FUNCTION__);
	return 0;
}

ssize_t first_chardev_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	printk(KERN_EMERG"Linux chardev: %s is call\r\n", __FUNCTION__);
	return count;
}

ssize_t first_chardev_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	printk("Linux chardev: %s is call\r\n", __FUNCTION__);
	return count;
}
loff_t first_chardev_llseek(struct file *file, loff_t loff, int origin)
{
	
	printk("Linux chardev: %s is call\r\n", __FUNCTION__);
	return loff;
}

int first_chardev_release(struct inode *inode, struct file *file)
{

	printk("Linux chardev: %s is call\r\n",__FUNCTION__);
	return 0;
}

/* 文件操作描述结构体 */
static struct file_operations dev_fops={
	.owner = THIS_MODULE,
	.ioctl = first_chardev_ioctl,
	.open = first_chardev_open,
	.read = first_chardev_read,
	.write = first_chardev_write,
	.release = first_chardev_release, 
	.llseek = first_chardev_llseek,
};	


/*static struct miscdevice misc = {
	.name = DEVICE_NAME,
	.fops = &dev_fops,
}
*/

/* 模块的初始化函数， 但运行 insmod 时， 调用该函数 */
static int __init first_chardev_init(void)
{
	int ret;
	printk("module init now\r\n");
	
	/* 向内核注册一个驱动名称， 以及和该名称绑定的对文件操作的驱动函数， 返回值为该驱动程序的主设备号 */
	major = register_chrdev(0, DEVICE_NAME, &dev_fops);
	printk("major: %d\r\n", major);
	
	/* 差错控制 */
	if(major < 0)
		printk("major error");
	
	printk("initialized\n");
	ret = major;
	return (ret);
}
/* 在卸载模块时， 对模块申请的资源做清理工作 */
static void __exit first_chardev_exit(void)
{
	unregister_chrdev(major, DEVICE_NAME);
	printk("Goodbye, cruel world!, priotity = 0\n");
}
/* 告诉内核， 模块初始化和退出时， 系统分别该调用的函数 */
module_init(first_chardev_init);
module_exit(first_chardev_exit);

/* 一些必要的声明 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("XYD");
MODULE_DESCRIPTION("This is samlpe drv_test");
