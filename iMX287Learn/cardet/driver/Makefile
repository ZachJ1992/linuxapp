#编译方式有两种 1. make -C ~/linux-2.6.35.3 M=`pwd` modules
#               2. make

# Makefile2.6
ifneq ($(KERNELRELEASE),)
#kbuild syntax. dependency relationshsip of files and target modules are listed here.
obj-m := cardet_isr.o
cardet_isr-objs := dev.o ISR.o
else
PWD := $(shell pwd)
KVER = 2.6.35.3
KDIR := /home/wgco/linux-2.6.35.3 
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.order *.symvers
endif
