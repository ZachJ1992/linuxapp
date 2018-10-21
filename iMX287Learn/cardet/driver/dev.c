//功能布局：中断设备只负责接收中断，并用时间标定信号边沿。产生的数据是各个通道标定的时间，此数据可以通过read函数读取到用户空间。
#include "header.h"

extern irqreturn_t isr_ch0_xphandle(unsigned int irq,void *dev_id);
extern irqreturn_t isr_ch0_yphandle(unsigned int irq,void *dev_id);

#define CARDET_GPIO_IRQ    gpio_to_irq(CH0_GPIO_No)   //中断号
#define DEVICE_NAME        "cardet_irq"

static int major;   //主设备号
static int minor;   //次设备号

struct cdev *pcdev;     //cdev数据结构
static dev_t devno;     //设备编号
static struct class *pcardet_class;

char const irq_types[5]={
    IRQ_TYPE_EDGE_RISING,
    IRQ_TYPE_EDGE_FALLING,
    IRQ_TYPE_EDGE_BOTH,
    IRQ_TYPE_LEVEL_HIGH,
    IRQ_TYPE_LEVEL_LOW
};

struct _IOTABLE _iotable[][2] = {
    { {CH0_XP_GPIO_No,"CH0_XP_GPIO_No",gpio_to_irq,IRQ_TYPE_EDGE_FALLING,isr_ch0_xphandle,"CH0 X Point"},
      {CH0_YP_GPIO_No,"CH0_YP_GPIO_No",gpio_to_irq,IRQ_TYPE_EDGE_FALLING,isr_ch0_yphandle,"CH0 Y Point"},
    },
};

struct _TIMESIGN _timesign[CHANNEL_MAX];

static int cardet_irq_open(struct inode *inode,struct file *file){
    //try_module_get()用于增加模块引用计数，设备每被打开 1 次，模块引用计数加 1
    try_module_get(THIS_MODULE);
    printk(KERN_INFO DEVICE_NAME" opend!\n");
    return 0;
}

static ssize_t cardet_irq_read(struct file *file,char *buf,size_t count,loff_t *f_pos){
    //printk(KERN_INFO DEVICE_NAME" read method.\n");
    //将驱动层数据拷贝到用户层
    if( copy_to_user((void*)buf,&_timesign[0],count) ){
        printk("copy_to_user() err \n");
        return -EFAULT;
    }
    return count;
}

static ssize_t cardet_irq_write(struct file *file,const char *buf,size_t count,loff_t *f_pos){
    struct timeval tv;    
    do_gettimeofday(&tv);
    //printk(KERN_EMERG DEVICE_NAME" cardet_irq_write tv.tv_sec=%ld,tv.tv_usec=%ld\n",tv.tv_sec,tv.tv_usec);
    
    //将用户层数据拷贝到驱动层
    if( copy_from_user(&_timesign[0],(void*)buf,count) ){
        printk(KERN_INFO "copy_from_user() err\n");
        return -EFAULT;
    }
    return count;
}

static int cardet_irq_release(struct inode *inode,struct file *file){
    printk(KERN_INFO DEVICE_NAME" closed!\n");
    //module_put()用于递减模块引用计数，设备被关闭 1 次，模块引用计数减 1；当引用计数为 0 时，模块可以被卸载
    module_put(THIS_MODULE);
    return 0;
}

struct file_operations cardet_irq_fops = {
    .owner      =   THIS_MODULE,
    .read       =   cardet_irq_read,
    .write      =   cardet_irq_write,
    .open       =   cardet_irq_open,
    .release    =   cardet_irq_release,
};


static void cardet_io_init(void){
    int ch,po;
    int ret;
    
    for(ch=0;ch<CHANNEL_MAX;ch++){
        //每个通道有2个IO引脚需要配置
        for(po=0;po<2;po++){
            unsigned pinNo = _iotable[ch][po].pinNo;
            printk(KERN_INFO "ch=%d po=%d pinNo=%d\n",ch,po,pinNo);

            gpio_free(pinNo);    //释放GPIO端口    
            ret = gpio_request_one(pinNo,GPIOF_IN,_iotable[ch][po].pinDesc);    //申请IO
            if( ret < 0 ){
                printk(KERN_ERR"Failed to request GPIO for KEY\n");
            }

            gpio_direction_input(pinNo); //设置GPIO为输入
            
            int irqNo = gpio_to_irq(pinNo); //中断号
            printk(KERN_INFO "irqNo=%d\n",irqNo);            

            if( request_irq(_iotable[ch][po].irqNo(pinNo),_iotable[ch][po].isrHandle,IRQF_DISABLED,_iotable[ch][po].irqDesc,NULL) ){ //申请一个中断
                printk(KERN_WARNING DEVICE_NAME":Can't get IRQ:%d ch=%d po=%d!\n",_iotable[ch][po].irqNo(pinNo),ch,po);
            }
            
            //设置中断类型
            set_irq_type(_iotable[ch][po].irqNo(pinNo),_iotable[ch][po].irqType);
            disable_irq(_iotable[ch][po].irqNo(pinNo));
            enable_irq(_iotable[ch][po].irqNo(pinNo));
        }
    }
}

static int cardet_dev_init(void){
    int ret;

    ret = alloc_chrdev_region(&devno,minor,1,DEVICE_NAME);  //从系统获取主设备号
    major = MAJOR(devno); //保存获得的主设备号
    if( ret < 0 ){
        printk(KERN_ERR"cannot get major %d\n",major);
        return -1;
    }
    
    //2.6 内核用 cdev 数据结构来描述字符设备，cdev 在<linux/cdev.h>中定义
    pcdev = cdev_alloc(); //分配pcdev结构
    if( pcdev != NULL ){
        cdev_init(pcdev,&cardet_irq_fops);   //初始化pcdev结构
        pcdev->owner = THIS_MODULE;
        if( cdev_add(pcdev,devno,1) != 0 ){ //将 pcdev 添加到系统中了
            printk(KERN_ERR"add cdev error!\n");
            goto error;
        }
    }else{
        printk(KERN_ERR"cdev_alloc_error!\n");
        return -1;
    }

    //要编写一个能用udev管理的设备驱动，需要在驱动代码中调用class_create()为设备创建一个class类，再调用device_create()为每个设备创建对应的设备
    pcardet_class = class_create(THIS_MODULE,"pcardet_class");
    if( IS_ERR(pcardet_class) ){
        printk(KERN_INFO"create class error\n");
        return -1;
    }

    //device_create用于在 sysfs 系统中创建设备节点相关的文件 dev 等文件
    device_create(pcardet_class,NULL,devno,NULL,DEVICE_NAME);
    printk(KERN_EMERG"cardet_irq_init()\n");
    return 0;

error:
    unregister_chrdev_region(devno,1);  //释放已经获得的设备号
    return ret;    
}

static int __init cardet_irqdev_init(void){
    int ret;    

    cardet_io_init();
    ret = cardet_dev_init();
    return ret;
}
static void __exit cardet_irqdev_exit(void){
    int ch,po;
    for(ch=0;ch<CHANNEL_MAX;ch++){
        for(po=0;po<2;po++){
            int pinNo = _iotable[ch][po].pinNo;
            gpio_free(pinNo);
            disable_irq(_iotable[ch][po].irqNo(pinNo));
            free_irq(_iotable[ch][po].irqNo(pinNo),NULL);
        }
    }
    cdev_del(pcdev);  //移除字符设备
    unregister_chrdev_region(devno,1);  //释放设备号
    device_destroy(pcardet_class,devno);
    class_destroy(pcardet_class);

    printk(KERN_EMERG"I'll be leaving, bye!\n");
}
module_init(cardet_irqdev_init);
module_exit(cardet_irqdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wgco");
