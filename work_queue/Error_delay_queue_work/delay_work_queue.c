#include <linux/kernel.h>
#include <stdio.h>
#include <linux/module.h>
#include <linux/workqueue.h>


#define MY_WORK_QUEUE_NAME			"delay_work_queue"

struct workqueue_struct * my_wq;

typedef struct {
	struct delayed_work delay_work;
	int x;
}my_delay_work_t;


my_delay_work_t my_delay_work1, my_delay_work2;


static void my_wq_function(struct work_struct *data)
{
	struct delayed_work *dw = (struct delay_work *)to_delayed_work(data);
	my_delay_work_t *my_delay_work = container_of(dw, my_delay_work_t, delay_work);

	printk("my_work_handler runing\r\n");
	printk("my_delay_work.x:%d\n", my_delay_work->x);

}


static int __init test_init(void)
{

	printk("inti runing\r\n");
	my_delay_work1.x = 1;
	my_delay_work2.x = 2;

	INIT_DELAY_WORK(&my_delay_work1.delay_work, my_wq_function);
	INIT_DELAY_WORK(&my_delay_work2.delay_work, my_wq_function);

	schedule_delayed_work(&my_delay_work1.delay_work, HZ*2);

	schedule_delayed_work(&my_delay_work2.delay_work, HZ*8);

	return 0;
}



static void __exit test_exit(void)
{
	printk("bye see again\r\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
