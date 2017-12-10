#include <linux/init.h>
#include <linux/module.h> 
#include <linux/kernel.h> 
/* 模块装载时调用 , __init 表示只在装载时调用一次， 之后在内核中移除 */ 
static int __init hello_init(void)
{
	printk(KERN_ALERT "hello world\r\n");
	return 0;
}

/* 模块卸载时调用， __exit 表示只在卸载时调用一次， 之后在内核中移除 */ 
static void __exit hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\r\n");
}

/* 用于指定模块初始化和清理模块的函数的宏 */ 
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
