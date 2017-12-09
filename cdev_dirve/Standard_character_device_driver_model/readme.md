### Linux2.6 标准字符设备驱动模型

Linux 2.6设备驱动使用struct cdev来描述一个字符设备：

```C
struct cdev{
  struct kobject kobj; /* 不了解 */
  struct module *owner; /* 所属的模块 */
  struct file_operations *ops; /* 文件操作结构体 */
  struct list_head list; 
  dev_t dev; /* 设备号 */
};
```

* 设备号

   在linux2.6 中，用dev_t类型（32位）来表示设备号，其中高12位表示主设备号， 低20为表示此设备号。高设备号表示该类设备对应的驱动程序， 低设备号表示使用该驱动程序的设备文件，在编程时，可使用以下的两个宏在主次设备号和（dev_t）类型之间转换， 宏包含在<linux/kdev.h>中

  |          转换          |              宏              |
  | :------------------: | :-------------------------: |
  | 主， 次设备号 ---> (dev_t) |  MKDEV(int major, int min)  |
  | (dev_t) ---> 主 次设备号  | MAJOR(dev_t)   MINOR(dev_t) |