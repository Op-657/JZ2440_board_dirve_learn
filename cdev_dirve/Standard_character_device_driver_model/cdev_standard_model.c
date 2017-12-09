#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <linux/cdev.h> /* linux kernel 2.6 up need */ 

#define DEVICE_NAME	"drv_chardev_standard"

/* ���豸�� */ 
int major  = 0;

/* �豸�� */ 
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

/* �ļ������ṹ�� */ 
static struct file_operations dev_fops = {
	.owner = THIS_MODULE,
	.ioctl = first_chardev_ioctl,
	.open = first_chardev_open,
	.read = first_chardev_read,
	.write = first_chardev_write,
	.release = first_chardev_release,
	.llseek = first_chardev_llseek,
};

/* ��׼�ַ��豸�������Ľṹ�� */ 
struct cdev my_cdev;

static int __init first_chardev_init(void)
{
	int ret = 0;
	printk("module init now\r\n");
	if(major > 0)  /* ���豸�Ź̶�ʱ�� ִ�и÷�֧ */ 
	{
		/* ��̬�����ں�ע���豸�ţ� ���豸����ƥ��������� */ 
		ret = register_chrdev_region(MKDEV(major, 0), 1, DEVICE_NAME);
		/* ����� */ 
		if(major < 0)
		{
			printk("register error\r\n");
			return ret;
		}
	}
	else   /* ����̬�����豸��ʱ�� ִ�и÷�֧ */ 
	{
		/**
		* feature�� ��̬�������豸���
		* param_1������������������ɹ���ɵ��ú� �������ѷ����ŵĵ�һ�����
		* param_2: Ҫʹ�õı�����ĵ�һ�����豸�ţ� ͨ��Ϊ0
		* param_3: �����������豸��ŵĸ���
		* param_4: �͸��豸�Ź������豸����
		*
		* return��0 :success -1: failure
		*/   
		ret = alloc_chrdev_region(&dev_nr, 0, 1, DEVICE_NAME);
		if(ret < 0)
		{
			printk("get major error\r\n");
			return ret;
		}
		/* ȡ�����豸�� */ 
		major = MAJOR(dev_nr);
		printk("major : %d\r\n", major);

	}
	
//	/* ��ʼ�� cdev �ṹ�壬 ��� .file_operations ���� 	
	cdev_init(&my_cdev, &dev_fops);
	
	my_cdev.owner = THIS_MODULE;
	
	/* ���ַ��豸�����ṹ�壬 ��ӽ��ںˣ� ������Ӧ���豸�Ű� */	
	ret = cdev_add(&my_cdev, dev_nr, 1);
	
	/* ����� */ 
	if(ret < 0)
	{
		unregister_chrdev_region(dev_nr, 1);
	}
	printk("tinitialized\r\n");
	return (ret);
}

static void __exit first_chardev_exit(void)
{
	/* ����cdev�ṹ���������Դ */ 
	cdev_del(&my_cdev);
	
	/* ע��ע�� */ 
	unregister_chrdev_region(dev_nr, 1);
	printk("Goodbye ");
}

module_init(first_chardev_init);
module_exit(first_chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ruiwen");
MODULE_DESCRIPTION("This the samlpe dev_test");

