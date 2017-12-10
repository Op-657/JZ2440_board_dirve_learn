#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/workqueue.h>


#define MY_WORK_QUEUE_NAME			"my_work_queue"

/* 定义工作队列结构体, 即工作的链表头, 所有的工作都将挂在该队列上 */
struct workqueue_struct * my_wq;

/* 自定义结构体参数 */
typedef struct {
	struct work_struct my_work;
	int x;
}my_work_t;


my_work_t *work1, *work2;

static void my_work_handler(struct work_struct *data)
{
	my_work_t  *my_work = (my_work_t *)data;
	printk("my_work_handler runing\r\n");
	printk("my_work.x:%d\n", my_work->x);
	kfree((void *)(data));
}


static int __init test_init(void)
{
	int ret = 0;

	printk("inti runing\r\n");

	/* 创建工作队列链表 */
	my_wq = create_workqueue(MY_WORK_QUEUE_NAME);
	if(my_wq)
	{

		work1 = (my_work_t *)kmalloc(sizeof(my_work_t), GFP_KERNEL);
		if(work1)
		{

			/* 初始化工作 */
			INIT_WORK((struct work_struct *)work1, my_work_handler);
			work1->x = 1;
			/* 将工作挂载到队列链表中 */
			ret = queue_work(my_wq, (struct work_struct *)work1);
		}


		work2 = (my_work_t *)kmalloc(sizeof(my_work_t), GFP_KERNEL);
		if(work2)
		{

			INIT_WORK((struct work_struct *)work2, my_work_handler);
			work2->x = 2 ;
			ret = queue_work(my_wq, (struct work_struct *)work2);
		}
		return ret;
	}
	else
	{
		return -1;
	}
}


static void __exit test_exit(void)
{
	destroy_workqueue(my_wq);
	printk("bye see again\r\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
