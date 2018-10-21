#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("WgCo");

static int hello_init(void){
	printk(KERN_EMERG "hello_init \n");
	return 0;
}

static void hello_exit(void){
	printk(KERN_EMERG "hello_exit \n");
}

module_init(hello_init);
module_exit(hello_exit);

//opration: 
//insmod helloworld.ko
//rmmod helloworld.ko
