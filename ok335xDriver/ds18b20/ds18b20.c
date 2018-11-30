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

#define D_MAJOR 0
#define D_MINOR 0
#define DEV_NAME "ds18b20"

static int ds18b20_major = D_MAJOR;
static int ds18b20_minor = D_MINOR;

/* Convert GPIO signal to GPIO pin number */
#define GPIO_TO_PIN(bank, gpio) (32 * (bank) + (gpio))

#define W1GPIO GPIO_TO_PIN(1,18)

struct ds18b20_device
{
	struct class    *sy_class;
	struct cdev     cdev;
};

static struct ds18b20_device   dev;//若使用指针，记得给指针开辟空间。

static unsigned int ds18b20_reset(void)
{
	int err;
	gpio_direction_output(W1GPIO,0);

	gpio_set_value(W1GPIO,1);
	udelay(10);

	gpio_set_value(W1GPIO,0);
	udelay(600);

	//resistor pull-up,ds18b20 waits 15-60us
	gpio_set_value(W1GPIO,1);
	udelay(60);
	
	gpio_direction_input(W1GPIO);
	udelay(400);

	//...
	//这里不严格，应该还要判断DS18B20“presence pulse” 60-240us

	err = gpio_get_value(W1GPIO);
	return err;
}

static unsigned int ds18b20_write(unsigned char data)
{
	unsigned int i;
	gpio_direction_output(W1GPIO,0);

	for (i = 0; i < 8; i++){   //只能一位一位的读写
		gpio_set_value(W1GPIO,0);
		udelay(5);
		
		if(data & 0x01){
			gpio_set_value(W1GPIO,1);
		}else{
			//do nothing
		}
		udelay(60);
		
		//从最低位开始判断；每比较完一次便把数据向右移，获得新的最低位状态
		data >>= 1;
		
		gpio_set_value(W1GPIO,1);
		udelay(1);
	}
	
	return 0;
}

static unsigned int ds18b20_read(void)
{
	unsigned int i ;
	unsigned char data = 0x00;

	for (i =0; i < 8 ; i++){
		gpio_direction_output(W1GPIO,1);

		gpio_set_value(W1GPIO,1);
		udelay(1);

		gpio_set_value(W1GPIO,0);
		udelay(2);

		gpio_set_value(W1GPIO,1);
		gpio_direction_input(W1GPIO);

		data >>= 1;
		if(0 != gpio_get_value(W1GPIO))
			data |= 0x80;
		udelay(60);
	}
	return 0;
}

static ssize_t read_ds18b20(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	unsigned char Data[2] = {0x00, 0x00};
	unsigned long err;
	int flag;
	
	flag = ds18b20_reset();
	if(flag <= 0){
		printk("ds18b20 init fail!\n");
		return -1;
	}

	ds18b20_write(0xcc);
	ds18b20_write(0x44);

	Data[0] = ds18b20_read();
	Data[1] = ds18b20_read();

	ds18b20_reset();
	err = copy_to_user(buf, Data, sizeof(Data));

	return err? -EFAULT:count;
}

static int open_ds18b20(struct inode *inode, struct file *filp)
{
	int flag = 0;
	
	flag = ds18b20_reset();
	if(flag < 0){
		printk("open ds18b20 successful!\n");	
	}else{
		printk("open ds18b20 failed!\n");
	}
	return 0;
}

static int release_ds18b20(struct inode *inode, struct file *filp)
{
	return 0;
}

static struct file_operations fops={
	.owner = THIS_MODULE,
	.read  = read_ds18b20,
	.open  = open_ds18b20,
	.release = release_ds18b20,
}; 

static int __init ds18b20_init(void)
{
	int result,err;
	dev_t devno = 0;
	int ret=0;

	if(ds18b20_major){
        	devno = MKDEV(ds18b20_major, ds18b20_minor);
        	result = register_chrdev_region(devno, 1, DEV_NAME);
	}else{
		result = alloc_chrdev_region(&devno, ds18b20_minor, 1, DEV_NAME);
		ds18b20_major = MAJOR(devno);
	}

	if(result < 0){
        	printk(KERN_ERR "%s can't use major %d\n",DEV_NAME, ds18b20_major);
	}
	printk("%s use major %d\n",DEV_NAME, ds18b20_major);

	cdev_init(&dev.cdev,&fops);
	dev.cdev.owner = THIS_MODULE;
	err = cdev_add(&dev.cdev, devno, 1);

	if(err){
        	printk(KERN_NOTICE"ERROR %d add ds18b20\n",err);
        	goto ERROR;
	}
	dev.sy_class   = class_create(THIS_MODULE, DEV_NAME);
    	
	device_create(dev.sy_class, NULL, MKDEV(ds18b20_major, ds18b20_minor), NULL, DEV_NAME);
	printk(KERN_NOTICE"Ds18b20 is ok!\n");

	ret = gpio_request(W1GPIO,"gpio1_27");
	if (ret < 0)
		pr_warning("w1-gpio Unable to request GPIO_W1_PULLUP_ENABLE\n");
 
	return 0;
ERROR:
    printk(KERN_ERR"%s driver installed failure.\n",DEV_NAME);
    cdev_del(&dev.cdev);
    unregister_chrdev_region(devno, 1);
    return err;
}

static void __exit ds18b20_exit(void)
{
	dev_t devno = MKDEV(ds18b20_major, 0);

	cdev_del(&dev.cdev);
	device_destroy(dev.sy_class,devno);
	class_destroy(dev.sy_class);

	unregister_chrdev_region(devno, 1);
	printk(KERN_NOTICE"bye ds18b20!\n");
}

module_init(ds18b20_init);
module_exit(ds18b20_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("wgco");
