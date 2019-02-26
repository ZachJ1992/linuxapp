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

///设备注册相关
#define D_MAJOR 0
#define D_MINOR 0
#define DEV_NAME "userio"

static int userio_major = D_MAJOR;
static int userio_minor = D_MINOR;

struct userio_device
{
	struct class    *sy_class;
	struct cdev     cdev;
};

static struct userio_device   dev;//若使用指针，记得给指针开辟空间。

///IO映射相关
#define GPIO1		0x4804C000
#define GPIO2		0x481AC000
#define GPIO3		0x481AE000

#define OE		0x134
#define DATAOUT		0x13C
#define SETDATAOUT	0x194

#define PIN(v)		(0x0000FFFF<<v)

#define TEST_NO		16

static volatile unsigned long *_gpio1_reg,*oe,*dataout,*setdataout;

static int userio_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long offset = vma->vm_pgoff;
	unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);

	//offset = offset + 0 * 1024 * 1024;

	/* we do not want to have this area swapped out, lock it */
	//vma->vm_flags |= VM_LOCKED; 
	if(remap_pfn_range(vma,vma->vm_start,offset,size,vma->vm_page_prot)){
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
static ssize_t userio_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return 0;
}
static ssize_t userio_write(struct file *file,const char __user *ubuf,size_t count, loff_t *ppos)
{
	if(ubuf[0]){
		*dataout |= PIN(TEST_NO);//0x04000000;
		//*(_gpio1_reg+DATAOUT) |= 0x04000000;//Data to set on output pins.
		printk("%s set gpio 1 \n",DEV_NAME);
	}else{
		*dataout &= ~PIN(TEST_NO);//~0x04000000;
		//*(_gpio1_reg+DATAOUT) &= ~0x04000000;//Data to set on output pins.
		printk("%s set gpio 0 \n",DEV_NAME);
	}
	return 0;
}


static struct file_operations fops={
	//该成员根本不是一个操作，他是一个指向拥有这个结构的模块的指针，
	//该成员用来在它的操作还在被使用时阻止模块被卸载，几乎在所有的时间中，它被简单初始化为THIS_MODULE 
	.owner = THIS_MODULE,

	.mmap		= userio_mmap,
	.open		= userio_open,
	.release	= userio_release,       //关闭相关设备的函数指针 
	.read		= userio_read,
	.write 		= userio_write,
}; 

static int __init userio_init(void)
{
	int result,err;
	dev_t devno = 0;
	int ret=0;

	if(userio_major){
        	devno = MKDEV(userio_major, userio_minor);
        	result = register_chrdev_region(devno, 1, DEV_NAME);
	}else{
		result = alloc_chrdev_region(&devno, userio_minor, 1, DEV_NAME);
		userio_major = MAJOR(devno);
	}

	if(result < 0){
        	printk(KERN_ERR "%s can't use major %d\n",DEV_NAME, userio_major);
	}
	printk("%s use major %d\n",DEV_NAME, userio_major);

	cdev_init(&dev.cdev,&fops);
	dev.cdev.owner = THIS_MODULE;
	err = cdev_add(&dev.cdev, devno, 1);

	if(err){
        	printk(KERN_NOTICE"ERROR %d add userio\n",err);
        	goto ERROR;
	}
	dev.sy_class   = class_create(THIS_MODULE, DEV_NAME);
    	
	device_create(dev.sy_class, NULL, MKDEV(userio_major, userio_minor), NULL, DEV_NAME);
	printk(KERN_NOTICE"Userio is ok!\n");

	//ret = gpio_request(W1GPIO,"gpio1_18");
	//if (ret < 0)
	//	pr_warning("w1-gpio Unable to request GPIO_W1_PULLUP_ENABLE\n");


	oe = ioremap(GPIO1 + 0x134, 4);
	setdataout = ioremap(GPIO1 + 0x194, 4);
	dataout = ioremap(GPIO1 + 0x13C, 4);

	*oe &= ~PIN(TEST_NO);//0xFBFFFFFF;//set output
	*setdataout |= PIN(TEST_NO);//0x04000000;
	*dataout &= ~PIN(TEST_NO);//0x04000000;

	//_gpio1_reg = (volatile unsigned long *)ioremap(GPIO1,4*1024);//gpiox寄存器大小4KB

	//
	//*(_gpio1_reg + OE) &= 0xFBFFFFFF; //set output
	//*(_gpio1_reg + SETDATAOUT) |= 0x04000000;//set the corresponding bit the GPIO_DATAOUT register.
	//*(_gpio1_reg + DATAOUT) &= ~0x04000000;//Data to set on output pins.
 
	return 0;
ERROR:
    printk(KERN_ERR"%s driver installed failure.\n",DEV_NAME);
    cdev_del(&dev.cdev);
    unregister_chrdev_region(devno, 1);
    return err;
}

static void __exit userio_exit(void)
{
	dev_t devno = MKDEV(userio_major, 0);

	//解除映射
	iounmap(_gpio1_reg);

	cdev_del(&dev.cdev);
	device_destroy(dev.sy_class,devno);
	class_destroy(dev.sy_class);

	unregister_chrdev_region(devno, 1);
	printk(KERN_NOTICE"bye userio!\n");
}

module_init(userio_init);
module_exit(userio_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("wgco");
