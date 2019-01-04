DEBUG=y

ifeq($(DEBUG),y)
	DFLAGS:= -O -g -DDEBUGPRINT
else
	DFLAGS:=O2
endif

OUTPUT:=monitor

CFLAGS+=DFLAGS

ifneq($(KERNELRELEASE),)
	monitor-objs:=main.o code.o
	obj-m:=monitor.o
else
	PWD:=$(shell pwd)
endif

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

CFLAGS += $(DEBFLAGS) -I$(LDDINC)

TARGET = scullv

ifneq ($(KERNELRELEASE),)

scullv-objs := main.o mmap.o

obj-m	:= scullv.o

else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) LDDINC=$(PWD) modules

endif


install:
	install -d $(INSTALLDIR)
	install -c $(TARGET).o $(INSTALLDIR)


depend .depend dep:
	$(CC) $(CFLAGS) -M *.c > .depend

ifeq (.depend,$(wildcard .depend))
	include .depend
endif
