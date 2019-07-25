#include <linux/types.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/init.h>
#include "monitor.h"

#define AUTHOR "Wambui Karuga <wambui@karuga.xyz"
#define DESCRIPTION "SIM Detection Module"
#define LICENSE "GPL v2"


int monitor_major = MONITOR_MAJOR;
int monitor_minor = 0;

/* static unsigned sim_detect_gpio[GPIO_DEVICES] = {
    339, 340, 341
};

static unsigned sim_select_gpio[GPIO_DEVICES] = {
    342, 344, 345
};

static unsigned modem_enable_gpio[GPIO_DEVICES] = {
    366, 367, 368
}; */

static int initialize_monitor_device(struct monitor_device *device, int index) {
    int result;
    int dev_no;
    struct device *monitor;
    /*
    * Character devices have to be allocated and registered before use.
    * void cdev_init(struct cdev *cdev, struct file_operations *fops); 
    */
    dev_no = MKDEV(monitor_major, monitor_minor + index);
    cdev_init(&device->monitor_cdev, &monitor_fops);
    device->monitor_cdev.owner = THIS_MODULE;
    device->monitor_cdev.ops = &monitor_fops;
    /*
    * Let the kernel know about our device.
    * cdev_add(struct cdev *dev, dev_t num, unsigned int count);
    */ 

    result = cdev_add(&device->monitor_cdev, dev_no, 1);

    /*
    * Registration can fail with an result less than 0.
    */ 
    if (result) {
        printk(KERN_ALERT "Failure adding monitor device %d with error code %d", index, result);
        return result;
    }

    /* DMESGPRINT("Successfully created new device class.\n"); */

    /*
    Device creation in sysfs
    struct device * device_create (	struct class *class, struct device *parent, dev_t devt, const char *  	fmt, ...);
    */

    monitor = device_create(monitor_class, NULL, MKDEV(monitor_major, monitor_minor + index), NULL, DEVICE);
    if (IS_ERR(monitor)) {
        class_destroy(monitor_class);
        unregister_chrdev_region(0, MONITOR_DEVICES);
        /* DMESGPRINT("Failure to register device class"); */
        printk(KERN_ALERT "Failure to create device class.\n");
        return PTR_ERR(monitor);
    }

    printk(KERN_INFO "Successfully created device class!\n");

    return 0;
}

static int __init monitor_init(void) {
    int result;
    int devices;
    int i;
    dev_t device;

    /*
    * Request a major number for the kernel and initialized the number of devices we want.
    * There should be one device for each GPIO.
    * int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);
    */
    device = MKDEV(monitor_major, monitor_minor);
    if (monitor_major) {
        result = register_chrdev_region(device, MONITOR_DEVICES, DEVICE);
    }
    else {
        result = alloc_chrdev_region(&device, monitor_minor, MONITOR_DEVICES, DEVICE);
        monitor_major = MAJOR(device);
    }
    if (result < 0) {
        DMESGPRINT("Could not allocate requested devices");
        return result;
    }
    /* DMESGPRINT("Major number allocated is %d", monitor_major); */
    printk(KERN_INFO "Major number allocated is %d", monitor_major);

    monitor_class = class_create(THIS_MODULE, DEVICE);

    if (IS_ERR(monitor_class)) {
        class_destroy(monitor_class);
        unregister_chrdev_region(0, MONITOR_DEVICES);
        /* DMESGPRINT("Failure to register device class"); */
        printk(KERN_ALERT "Failure to register device class.\n");
        return PTR_ERR(monitor_class);
    }

    printk(KERN_INFO "Successfully registered new device class.\n");

    monitor_devices = kmalloc(sizeof(struct monitor_device) * MONITOR_DEVICES, GFP_KERNEL);

    if (!monitor_devices) {
        result = -ENOMEM;
        goto allocation_failure;
    }

    memset(monitor_devices, 0, MONITOR_DEVICES * sizeof(struct monitor_device));
    for (i = 0; i < MONITOR_DEVICES; i++) {
        /*
        * Allocate memory for each device structure and append it to the global array
        * void *kmalloc(size_t size, int flags);
        */
        devices = initialize_monitor_device(&monitor_devices[i], i);
    }

    return 0;

    allocation_failure:
        unregister_chrdev_region(0, MONITOR_DEVICES);
        return result;
    }

    static void __exit monitor_cleanup(void) {
    int i;

    for (i = 0; i < MONITOR_DEVICES; i++) {
        ;
    }

    for (i = 0; i < MONITOR_DEVICES; i++) {
        ;
    }
    /*
    * Frees device drives allocated during initialization. 
    * void unregister_chrdev_region(dev_t first, unsigned int count);
    */ 
    unregister_chrdev_region(0, MONITOR_DEVICES);
}


module_init(monitor_init);
module_exit(monitor_cleanup);

MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_LICENSE(LICENSE);