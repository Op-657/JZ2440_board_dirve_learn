#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <linux/cdev.h> /* linux kernel 2.6 up need */ 

#define DEVICE_NAME	"drv_chardev_standard"

/* 主设备号 */ 
int major  = 0;

/* 设备号 */ 
dev_t dev_nr = 0;

static int first_chardev_open(struct inode *pinode, struct file *pfile)
{
	printk("linux 2.6 chardev: %s is call", __FUNCTION__);
	return 0;
}

static int first_chardev_ioctl(
	struct inode *inode,
	struct file *file,
	unsigned int cmd,
	unsigned long arg)
{
	printk("linux 2.6 chardev: %s is call", __FUNCTION__);
	return 0;
}

ssize_t first_chardev_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	printk("linux 2.6 chardev: %s is call", __FUNCTION__);
	return count;
}

ssize_t first_chardev_write(struct file *flip, const char __user *buf, size_t count, loff_t *offset)
{
	printk("linux 2.6 chardev: %s is call", __FUNCTION__);
	return count;
}

loff_t first_chardev_llseek(struct file *file, loff_t loff, int origin)
{
	printk("linux 2.6 chardev: %s is call", __FUNCTION__);
	return loff;
}

int first_chardev_release(struct inode *inode, struct file *file)
{
	printk("linux 2.6 chardev: %s is call", __FUNCTION__);
	return 0;
}

/* 文件操作结构体 */ 
static struct file_operations dev_fops = {
	.owner = THIS_MODULE,
	.ioctl = first_chardev_ioctl,
	.open = first_chardev_open,
	.read = first_chardev_read,
	.write = first_chardev_write,
	.release = first_chardev_release,
	.llseek = first_chardev_llseek,
};

/* 标准字符设备驱动核心结构体 */ 
struct cdev my_cdev;

static int __init first_chardev_init(void)
{
	int ret = 0;
	printk("module init now\r\n");
	if(major > 0)  /* 当设备号固定时， 执行该分支 */ 
	{
		/* 静态的向内核注册设备号， 和设备号相匹配的驱动名 */ 
		ret = register_chrdev_region(MKDEV(major, 0), 1, DEVICE_NAME);
		/* 差错检查 */ 
		if(major < 0)
		{
			printk("register error\r\n");
			return ret;
		}
	}
	else   /* 当动态申请设备号时， 执行该分支 */ 
	{
		/**
		* feature： 动态的申请设备编号
		* param_1：仅用于输出参数，成功完成调用后， 将保存已分配编号的第一个编号
		* param_2: 要使用的被请求的第一个此设备号， 通常为0
		* param_3: 所请求连续设备编号的个数
		* param_4: 和该设备号关联的设备名称
		*
		* return：0 :success -1: failure
		*/   
		ret = alloc_chrdev_region(&dev_nr, 0, 1, DEVICE_NAME);
		if(ret < 0)
		{
			printk("get major error\r\n");
			return ret;
		}
		/* 取出主设备号 */ 
		major = MAJOR(dev_nr);
		printk("major : %d\r\n", major);

	}
	
//	/* 初始化 cdev 结构体， 填充 .file_operations 属性 	
	cdev_init(&my_cdev, &dev_fops);
	
	my_cdev.owner = THIS_MODULE;
	
	/* 将字符设备驱动结构体， 添加进内核， 并和相应的设备号绑定 */	
	ret = cdev_add(&my_cdev, dev_nr, 1);
	
	/* 差错处理 */ 
	if(ret < 0)
	{
		unregister_chrdev_region(dev_nr, 1);
	}
	printk("tinitialized\r\n");
	return (ret);
}

static void __exit first_chardev_exit(void)
{
	/* 清理cdev结构体申请的资源 */ 
	cdev_del(&my_cdev);
	
	/* 注销注册 */ 
	unregister_chrdev_region(dev_nr, 1);
	printk("Goodbye ");
}

module_init(first_chardev_init);
module_exit(first_chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ruiwen");
MODULE_DESCRIPTION("This the samlpe dev_test");

