#include <linux/module.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/types.h>
#include <asm/irq.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/phy.h>
#include <linux/gpio.h>

//静态映射
//#define GPMCON     *(volatile unsigned long *)0x7F008820
//#define GPMDAT     *(volatile unsigned long *)0x7F008824

//动态映射 
#define GPIO_OE     		(0x4804C000 + 0x134) //input/output
#define GPIO_DATAOUT		(0x4804C000 + 0x13C)
#define GPIO_SETDATAOUT		(0x4804C000 + 0x194)

/* Convert GPIO signal to GPIO pin number */
//#define GPIO_TO_PIN(bank, gpio) (32 * (bank) + (gpio))
 
static volatile unsigned long *OE, *DATAOUT,*SETDATAOUT;
static int major;

static int userio_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);

	offset = offset + 0 * 1024 * 1024;

	/* we do not want to have this area swapped out, lock it */
	vma->vm_flags |= VM_LOCKED; 

	if(remap_pfn_range(vma,vma->vm_start,offset,size,PAGE_SHARED)){
		printk("remap page range failed\n");
		return (- ENXIO); 	
	}

	return 0;
}

static int userio_open(struct inode *inode, struct file *file)
{
	return 0;
}
static int userio_release(struct inode *inode, struct file *file)
{
	return 0;
}
static ssize_t userio_read(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
	return 0;
}
static ssize_t userio_write(struct file *file,const char __user *ubuf,size_t count, loff_t *ppos)
{
	return 0;
}

//自定义file_operations结构体，并且填充
static const struct file_operations test_fops = {
	//该成员根本不是一个操作，他是一个指向拥有这个结构的模块的指针，
	//该成员用来在它的操作还在被使用时阻止模块被卸载，几乎在所有的时间中，它被简单初始化为THIS_MODULE 
	.owner = THIS_MODULE, 

	.mmap		= userio_mmap,
	.open 		= userio_open,  	//打开相关设备的函数指针 
	.release	= userio_release,       //关闭相关设备的函数指针 
	.read 		= userio_read,
	.write 		= userio_write,
};

static int __init userio_init(void)
{

	if ((major = register_chrdev(0, "userio", &test_fops)) < 0){
		printk("userio: unable to register character device\n");
		return ( - EIO) ;
	}
	printk("userio: device major = %d\n", major);


	OE = ioremap(GPIO_OE, 4);
	DATAOUT = ioremap(GPIO_DATAOUT, 4);
	SETDATAOUT = ioremap(GPIO_SETDATAOUT, 4);

	*OE &= 0xFBFFFFFF;//set output
	*SETDATAOUT |= 0x04000000;
	*DATAOUT &= ~0x04000000;
	return 0;
}
 
static void __exit userio_exit(void)
{
	iounmap(OE);
	iounmap(DATAOUT);
	iounmap(SETDATAOUT);
	unregister_chrdev(major, "userio");
}
 
module_init(userio_init);
module_exit(userio_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("wgco");
