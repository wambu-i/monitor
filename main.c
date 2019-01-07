#include <linux/init.h>
#include "monitor.h"

#define AUTHOR "Wambui Karuga <wambui@karuga.xyz"
#define DESCRIPTION "SIM Detection Module"
#define LICENSE "GPL"

module_init(monitor_init);
module_cleanup(monitor_cleanup);

MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_LICENSE(LICENSE);