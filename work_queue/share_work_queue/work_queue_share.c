#include <linux/module.h>
#include <linux/init.h>
#include <linux/workqueue.h>

/* 定义工作结构体, 每一个结构体表示一个工作, 当初始化完成后挂在在共享队列链表上等待调度 */
static struct work_struct work;

/* 当调度时, 执行的功能函数 */
static void work_handler(struct work_struct *data)
{
	printk("work handler function runing\r\n");
	printk("***************************\r\n");
}


/* 模块初始化 */
static int __init test_init(void)
{
	/* 初始化结构体 */
	INIT_WORK(&work, work_handler);

	schedule_work(&work);
	return 0;
}



static void __exit test_exit(void)
{
	printk("%s\r\n", __FUNCTION__);
}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
