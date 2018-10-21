#include <linux/init.h>
#include <linux/module.h>

#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
#include <mach/gpio-exynos4.h>

#define DRIVER_NAME "WgCoTest_ctl"
#define DEVICE_NAME "ledstest_ctl"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("TOPEET");

static long leds_ioctl( struct file *files, unsigned int cmd, unsigned long arg){
	printk(KERN_EMERG "leds_ioctl(): cmd is %d,arg is %ld\n",cmd,arg);
	
	if(cmd>=1){
		cmd=1;	
	}else{
		cmd=0;
	}

	if(arg>=1){
		arg=1;
	}else{
		arg=0;
	}
	
	gpio_set_value(EXYNOS4_GPL2(0),cmd);
	
	return 0;
}

static int leds_release(struct inode *inode, struct file *file){
	printk(KERN_EMERG "leds release()\n");
	return 0;
}

static int leds_open(struct inode *inode, struct file *file){
	printk(KERN_EMERG "leds open()\n");
	return 0;
}

static struct file_operations leds_ops = {
	.owner = THIS_MODULE,
	.open = leds_open,
	.release = leds_release,
	.unlocked_ioctl = leds_ioctl,
};

static  struct miscdevice leds_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &leds_ops,
};

static int leds_probe(struct platform_device *pdv){
	int ret;
	
	printk(KERN_EMERG "leds_probe()\n");
	
	ret = gpio_request(EXYNOS4_GPL2(0),"LEDS");
	if(ret < 0){
		printk(KERN_EMERG "gpio_request EXYNOS4_GPL2(0) failed!\n");
		return ret;
	}
	printk(KERN_EMERG "gpio_request EXYNOS4_GPL2(0) success!\n");
	
	s3c_gpio_cfgpin(EXYNOS4_GPL2(0),S3C_GPIO_OUTPUT);
	gpio_set_value(EXYNOS4_GPL2(0),0);
	misc_register(&leds_dev);
	
	return 0;
}

static int leds_remove(struct platform_device *pdv){
	printk(KERN_EMERG "leds_remove()\n");
	misc_deregister(&leds_dev);
	return 0;
}

static void leds_shutdown(struct platform_device *pdv){
	printk(KERN_EMERG "leds_shutdown()\n");
}

static int leds_suspend(struct platform_device *pdv,pm_message_t pmt){
	printk(KERN_EMERG "leds_suspend()\n");
	return 0;
}

static int leds_resume(struct platform_device *pdv){
	printk(KERN_EMERG "leds_resume()\n");
	return 0;
}

struct platform_driver leds_driver = {
	.probe = leds_probe,
	.remove = leds_remove,
	.shutdown = leds_shutdown,
	.suspend = leds_suspend,
	.resume = leds_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};

static int leds_init(void){
	int ret;
	
	printk(KERN_EMERG "leds_init()\n");
	ret = platform_driver_register(&leds_driver);
	
	printk(KERN_EMERG "DriverState is %d\n",ret);
	return 0;
}

static void leds_exit(void){
	printk(KERN_EMERG "leds_exit()\n");
	platform_driver_unregister(&leds_driver);	
}

module_init(leds_init);
module_exit(leds_exit);

