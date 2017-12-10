#include <linux/init.h>
#include <linux/module.h> 
#include <linux/kernel.h> 
/* ģ��װ��ʱ���� , __init ��ʾֻ��װ��ʱ����һ�Σ� ֮�����ں����Ƴ� */ 
static int __init hello_init(void)
{
	printk(KERN_ALERT "hello world\r\n");
	return 0;
}

/* ģ��ж��ʱ���ã� __exit ��ʾֻ��ж��ʱ����һ�Σ� ֮�����ں����Ƴ� */ 
static void __exit hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\r\n");
}

/* ����ָ��ģ���ʼ��������ģ��ĺ����ĺ� */ 
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
