#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

//杂项设备，设备节点注册
#define DRIVER_NAME "WgCoTest_Ctl"
#define DEVICE_NAME "deviceNodeReg_dev"
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("WgCo");

static long devnodereg_ioctl( struct file *files, unsigned int cmd, unsigned long arg){
	printk(KERN_EMERG,"devnodereg_ioctl()\n");
	printk("cmd is %d,arg is %ld\n",cmd,arg);
	return 0;
}

static int devnodereg_release(struct inode *inode, struct file *file){
	printk(KERN_EMERG "devnodereg release()\n");
	return 0;
}

static int devnodereg_open(struct inode *inode, struct file *file){
	printk(KERN_EMERG "devnodereg open()\n");
	return 0;
}

static struct file_operations devnodereg_ops = {
	.owner = THIS_MODULE,
	.open = devnodereg_open,
	.release = devnodereg_release,
	.unlocked_ioctl = devnodereg_ioctl,
};

static  struct miscdevice devnodereg_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &devnodereg_ops,
};

static int devnodereg_probe(struct platform_device *pdv){
	printk(KERN_EMERG "devnodereg_probe()\n");
	misc_register(&devnodereg_dev);
	return 0;
}

static int devnodereg_remove(struct platform_device *pdv){
	printk(KERN_EMERG "devnodereg_remove()\n");
	misc_deregister(&devnodereg_dev);
	return 0;
}

static void devnodereg_shutdown(struct platform_device *pdv){
	printk(KERN_EMERG "devnodereg_shutdown()\n");
}

static int devnodereg_suspend(struct platform_device *pdv,pm_message_t pmt){
	printk(KERN_EMERG "devnodereg_suspend()\n");
	return 0;
}

static int devnodereg_resume(struct platform_device *pdv){
	printk(KERN_EMERG "devnodereg_resume()\n");
	return 0;
}

struct platform_driver devnodereg_driver = {
	.probe = devnodereg_probe,
	.remove = devnodereg_remove,
	.shutdown = devnodereg_shutdown,
	.suspend = devnodereg_suspend,
	.resume = devnodereg_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};

static int devnodereg_init(void){
	int ret;
	
	printk(KERN_EMERG "devnodereg_init()\n");
	ret = platform_driver_register(&devnodereg_driver);
	
	printk(KERN_EMERG "\tDriverState is %d\n",ret);
	return 0;
}

static void devnodereg_exit(void){
	printk(KERN_EMERG "devnodereg_exit()\n");
	platform_driver_unregister(&devnodereg_driver);	
}

module_init(devnodereg_init);
module_exit(devnodereg_exit);

