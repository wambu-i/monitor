#include <linux/fs.h>
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
    return result;
}

void monitor_cleanup(void) {
    /*
    * Frees device drives allocated during initialization. 
    * void unregister_chrdev_region(dev_t first, unsigned int count);
    */ 
    unregister_chrdev_region(0, MONITOR_DEVICES);

}