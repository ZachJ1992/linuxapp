#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>

#define KEY_GPIO        68  //GPIO2_4
#define KEY_GPIO_IRQ    gpio_to_irq(KEY_GPIO)   //中断号
#define DEVICE_NAME     "key_irq"

static int major;   //主设备号
static int minor;   //次设备号

struct cdev *key_irq;   //cdev数据结构
static dev_t devno;     //设备编号
static struct class *key_irq_class;

char const irq_types[5]={
    IRQ_TYPE_EDGE_RISING,
    IRQ_TYPE_EDGE_FALLING,
    IRQ_TYPE_EDGE_BOTH,
    IRQ_TYPE_LEVEL_HIGH,
    IRQ_TYPE_LEVEL_LOW
};

static int key_irq_open(struct inode *inode,struct file *file){
    //try_module_get()用于增加模块引用计数，设备每被打开 1 次，模块引用计数加 1
    try_module_get(THIS_MODULE);
    printk(KERN_INFO DEVICE_NAME" opend!\n");
    return 0;
}

static int key_irq_release(struct inode *inode,struct file *file){
    printk(KERN_INFO DEVICE_NAME" closed!\n");
    //module_put()用于递减模块引用计数，设备被关闭 1 次，模块引用计数减 1；当引用计数为 0 时，模块可以被卸载
    module_put(THIS_MODULE);
    return 0;
}

static irqreturn_t key_irq_irq_handler(unsigned int irq,void *dev_id){
    printk("KEY IRQ HAPPENED!\n");
    return IRQ_HANDLED;
}

struct file_operations key_irq_fops = {
    .owner      =   THIS_MODULE,
    .open       =   key_irq_open,
    .release    =   key_irq_release,
};

static int __init key_irq_init(void){
    int ret;

    gpio_free(KEY_GPIO);    //释放GPIO端口    
    ret = gpio_request_one(KEY_GPIO,GPIOF_IN,"KEY IRQ");    //申请IO
    if( ret < 0 ){
        printk(KERN_ERR"Failed to request GPIO for KEY\n");
    }

    gpio_direction_input(KEY_GPIO); //设置GPIO为输入
    if( request_irq(KEY_GPIO_IRQ,key_irq_irq_handler,IRQF_DISABLED,"key_irq irq",NULL) ){
        //申请中断
        printk(KERN_WARNING DEVICE_NAME":Can't get IRQ:%d!\n",KEY_GPIO_IRQ);
    }
    
    set_irq_type(KEY_GPIO_IRQ,irq_types[0]);
    disable_irq(KEY_GPIO_IRQ);
    enable_irq(KEY_GPIO_IRQ);

    ret = alloc_chrdev_region(&devno,minor,1,DEVICE_NAME);  //从系统获取主设备号
    major = MAJOR(devno);
    if( ret < 0 ){
        printk(KERN_ERR"cannot get major %d\n",major);
        return -1;
    }

    key_irq = cdev_alloc(); //分配key_irq结构
    if( key_irq != NULL ){
        cdev_init(key_irq,&key_irq_fops);   //初始化key_irq结构
        key_irq->owner = THIS_MODULE;
        if( cdev_add(key_irq,devno,1) != 0 ){
            printk(KERN_ERR"add cdev error!\n");
            goto error;
        }
    }else{
        printk(KERN_ERR"cdev_alloc_error!\n");
        return -1;
    }

    //要编写一个能用udev管理的设备驱动，需要在驱动代码中调用class_create()为设备创建一个class类，再调用device_create()为每个设备创建对应的设备
    key_irq_class = class_create(THIS_MODULE,"key_irq_class");
    if( IS_ERR(key_irq_class) ){
        printk(KERN_INFO"create class error\n");
        return -1;
    }

    device_create(key_irq_class,NULL,devno,NULL,DEVICE_NAME);
    printk(KERN_EMERG"key_irq_init()\n");
    return 0;

error:
    unregister_chrdev_region(devno,1);  //释放已经获得的设备号
    return ret;    
}
static void __exit key_irq_exit(void){
    gpio_free(KEY_GPIO);
    disable_irq(KEY_GPIO_IRQ);
    free_irq(KEY_GPIO_IRQ,NULL);
    cdev_del(key_irq);  //移除字符设备
    unregister_chrdev_region(devno,1);  //释放设备号
    device_destroy(key_irq_class,devno);
    class_destroy(key_irq_class);

    printk(KERN_EMERG"I'll be leaving, bye!\n");
}
module_init(key_irq_init);
module_exit(key_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wgco");
