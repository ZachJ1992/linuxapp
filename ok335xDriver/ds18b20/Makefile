#编译方式有两种 1. make -C 用户目录/kernel-3.2 M=`pwd` modules
#               2. make
#
#               # Makefile2.6
ifneq ($(KERNELRELEASE),)
	#kbuild syntax. dependency relationshsip of files and target modules are listed here.
obj-m := ds18b20.o
#cardet_isr-objs := ds18b20.o
else
	PWD := $(shell pwd)
	KVER = 3.2
KDIR := ~/onwork/github/kernel_00/OK335xD/kernel-3.2 
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.order *.symvers
endif
