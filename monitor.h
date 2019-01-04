#ifndef MONITOR_H
#define MONITOR_H

#include <linux/kernel.h>

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

/* Function */
int monitor_init();
void monitor_cleanup(void);

/* Variables */

extern int monitor_major;
#endif