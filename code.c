#include <linux/types.h>
#include <linux/errno.h>
#include <monitor.h>


int monitor_init(void) {
    int result;
    dev_t device;
    /*
    * Request a major number for the kernel and initialized the number of devices we want.
    * There should be one device for each GPIO.
    * int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);
    */
    result = alloc_chrdev_region(&device, 0, MONITOR_DEVICES, DEVICE);
    if (result < 0) {
        DMESGPRINT("Could not allocate requested devices");
        return result;
    }
    /* The major indicates the driver used by the kernel for a device. */
    monitor_major = MAJOR(result);
    DMESGPRINT("Major number allocated is %d", monitor_major);
    return 0;
}

void monitor_cleanup(void) {
    /*
    * Frees device drives allocated during initialization. 
    * void unregister_chrdev_region(dev_t first, unsigned int count);
    */ 
    unregister_chrdev_region(0, MONITOR_DEVICES);

}

void initialize_monitor_device(struct monitor_device *device, int index) {
    int result;
    int dev_no = MKDEV(monitor_major, index);
    /*
    * Character devices have to be allocated and registered before use.
    * void cdev_init(struct cdev *cdev, struct file_operations *fops); 
    */
    cdev_init(@device->monitor_cdev, &monitor_fops);
    device->monitor_cdev.owner = THIS_MODULE;
    device->monitor_cdev.ops = monitor_fops;
    /*
    * Let the kernel know about our device.
    * cdev_add(struct cdev *dev, dev_t num, unsigned int count);
    */ 

    result = cdev_add(&device->monitor_cdev, dev_no, 1);

    /*
    * Registration can fail with an result less than 0.
    */ 
    if (result):
        printk(KERN_ALERT, "Failure adding monitor device %d with error code %d", index, result);
}