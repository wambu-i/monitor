#ifndef MONITOR_H
#define MONITOR_H

#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#undef DEBUGPRINT
#ifdef DEBUGPRINT
    #ifdef __KERNEL__
        #define DMESGPRINT(message, args...) printk(KERN_DEBUG "monitor: " message, ##args)
    #else
        #define DMESGPRINT(message, args...) fprintf(stderr, message, ##args)
    #endif
#else
    #define DMESGPRINT(messages, args...)
#endif
#undef DEBUGPRINT

#ifndef MONITOR_MAJOR
    #define MONITOR_MAJOR 0
#endif

#define DEVICE "monitor"
#define MONITOR_DEVICES 3
#define GPIO_DEVICES 3

/* Structures */

/* Per GPIO device structure.
* @lock - Spinlock structure to control access to the device.
* @next_device - The next character device.
* @device - the internal representation of the driver in the form of major and minor number.
* @monitor_dev - the character device structure 
*/
struct monitor_device {
    spinlock_t lock;
    struct monitor_device *next_device;
    dev_t device;
    struct cdev monitor_cdev;
    int state; 
};

struct file_operations monitor_fops = {
    .owner = THIS_MODULE
};

struct monitor_device *monitor_devices;
struct class *monitor_class;

/* Variables */

extern int monitor_major;
#endif