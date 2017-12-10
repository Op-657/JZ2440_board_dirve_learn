### linux中的模块编程

#### 模块加载

内核的组件中包含许多不同的模块， 将模块加载到内核有两中方式， 一种时在编译内核的时候静态加载， 另外一种为内核运行时通过**insmod**命令动态加载

#### 模块编译

模块的终极目标都是加载进内核， 实现特定的功能。 但是内核版本多样， 每个版本之间会有差异， 所以模块要能正确编译并运行， 应确保具备正确的编译器版本， 及待运行内核树。

#### Makefile

```Makefile
KERN_DIR = /work/system/linux-2.6.22.6
all:
	make -C $(KERN_DIR) M=`pwd` modules 
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
obj-m	:= Basic_module_model.o

//韦东山老师提供Makefile模板
```

* KERN_DIR： 表示内核书目录， 即编译依赖的内核版本环境
* *目标*  := 依赖 -----> obj-m := Basic_module_model.o
* \` pwd \`: 表示到模块源文件目录

当文件被更新或目标不存在时， 执行：

```shell
make -C $(KERN_DIR) M=`pwd` modules 
```

该命令首先改变目录到 -C 指定目录（/work/system/linux-2.6.22.6），其中保存有内核顶层的Makefile。 M=选项让该Makefile在构造module目标之前返回到模块源代码目录， 然后modules目标指向obj-m变量中设定的模块

#### 模块卸载

> rmmod	Module_Name

#### 模块参数

在insmod装载模块的时候， 可以按照**一定的格式**传递参数给模块， 以增加模块的灵活性。



