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

ifeq (.depend,$(wildcard .depend))
	include .depend
endif
