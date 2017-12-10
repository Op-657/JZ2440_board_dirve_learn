#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>

#include <linux/delay.h>


/* 定义tasklet工作结构体 */
struct tasklet_struct my_tasklet;

/* 工作队列绑定的函数 */
void my_tasklet_function(unsigned long data);

/* 向工作对列绑定的函数传递参数 */
char my_tasklet_data[]="my_tasklet_function was called.";

/* 静态声明tasklet 工作 */
DECLARE_TASKLET(my_tasklet, my_tasklet_function,(unsigned long)my_tasklet_data);

void my_tasklet_function(unsigned long data)
{
	printk("%s\r\n",(char *)data);
	udelay(2000);
}

int __init init_module(void)
{
	printk("module insmod\r\n");
	/* 调度工作, 即将工作放在队列链表中等待调度 */
	tasklet_schedule(&my_tasklet);

	return 0;
}

void __exit cleanup_module(void)
{
	printk("module remove\r\n");
	tasklet_kill(&my_tasklet);
}

MODULE_LICENSE("GPL");

