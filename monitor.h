#ifndef MONITOR_H
#define MONITOR_H

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

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

#define DEVICE "monitor"
#define MONITOR_DEVICES 3

/* Structures */

/* Per GPIO device structure.
* @lock - Spinlock structure to control access to the device.
* @next_device - The next character device.
* @device - the internal representation of the driver in the form of major and minor number.
* @monitor_dev - the character device structure 
*/
struct monitor_device {
    splinlock_t lock;
    struct monitor_device *next_device;
    dev_t device;
    struct cdev monitor_cdev; 
}

struct file_operations monitor_fops {
    .owner = THIS_MODULE
}
struct monitor_device *devices;
/* Function */
int monitor_init();
void monitor_cleanup(void);
void initialize_monitor_device(struct monitor_device *, int);

/* Variables */

extern int monitor_major;
#endif