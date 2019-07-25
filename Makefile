DEBUG:=y
INCLUDE:=/home/wambui/openWRT/monitor/

ifeq ($(DEBUG),y)
	DEBFLAGS = -O -g -DDEBUGPRINT
else
	DEBFLAGS = -O2
endif

ifneq ($(KERNELRELEASE),)
	obj-m    := monitor.o
	ccflags-y	:= -I$(INCLUDE) 
	ccflags-y	+=	$(DEBFLAGS)

	CONFIG_MONITOR_DEBUG	:= -DDEBUG
	monitor-objs := code.o

else
	KDIR	:= /lib/modules/$(shell uname -r)/build
	MDIR	:= /lib/modules/$(shell uname -r)
	PWD		:= $(shell pwd)
	KREL	:= $(shell cd ${KDIR} && make -s kernelrelease)

	export CONFIG_MONITOR := m

	all:
		$(MAKE) -C $(KDIR)  M=$(PWD) modules

	clean:
		$(MAKE) -C $(KDIR) M=$(PWD) clean

	help:
		$(MAKE) -C $(KDIR) M=$(PWD) help

	install: monitor.ko
		rm -f ${MDIR}/kernel/monitor/monitor.ko
		install -m644 -b -D monitor.ko ${MDIR}/kernel/monitor/monitor.ko
		depmod -aq
	
	uninstall:
		rm -rf ${MDIR}/kernel/monitor
		depmod -aq
	
	.PHONY all clean

endif
