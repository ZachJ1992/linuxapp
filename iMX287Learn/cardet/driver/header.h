#ifndef _HEADER_H_
#define _HEADER_H_
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/string.h>
#include <asm-generic/uaccess.h>

#include "config.h"

#define XPOINT_IDX  0   //X点索引
#define YPOINT_IDX  1   //Y点索引


struct _IOTABLE{
    unsigned pinNo;
    char *pinDesc;
    int (*irqNo)(unsigned);
    unsigned irqType;
    irqreturn_t (*isrHandle)(unsigned,void*);
    char *irqDesc;
};

#endif
