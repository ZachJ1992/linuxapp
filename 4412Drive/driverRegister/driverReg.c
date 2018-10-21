#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#define DRIVER_NAME "WgCoTest_Ctl"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("WgCo");

static int driverReg_probe(struct platform_device *pdv){
	printk(KERN_EMERG "driverReg_probe()\n");
	return 0;
}

static int driverReg_remove(struct platform_device *pdv){
	printk(KERN_EMERG "driverReg_remove()\n");
	return 0;
}

static void driverReg_shutdown(struct platform_device *pdv){
	printk(KERN_EMERG "driverReg_shutdown()\n");
}

static int driverReg_suspend(struct platform_device *pdv){
	printk(KERN_EMERG "driverReg_suspend()\n");
	return 0;
}

static int driverReg_resume(struct platform_device *pdv){
	printk(KERN_EMERG "driverReg_resume()\n");
	return 0;
}

struct platform_driver driverReg_driver = {
	.probe   = driverReg_probe,
	.remove  = driverReg_remove,
	.shutdown= driverReg_shutdown,
	.suspend = driverReg_suspend,
	.resume  = driverReg_resume,
	.driver  = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};

static int driverReg_init(void){
	int ret;
	
	printk(KERN_EMERG "driverReg_init()\n");
	ret = platform_driver_register(&driverReg_driver);
	
	printk(KERN_EMERG "DrivervState is: %d\n",ret);
	return 0;
}

static void driverReg_exit(void){
	printk(KERN_EMERG "driverReg_exit()\n");
	platform_driver_unregister(&driverReg_driver);	
}

module_init(driverReg_init);
module_exit(driverReg_exit);
