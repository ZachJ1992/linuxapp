obj-m += devNodeReg.o
KDIR := /home/topeet/Android/iTop4412_Kernel_3.0
PWD ?= $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.mod.c *.order *.symvers
