# Makefile for the monitor device driver.

DEBUG:=y
DFLAGS:= -O -g -DDEBUGPRINT
ifneq ($(KERNELRELEASE),)
	# call from kernel build system

	obj-$(CONFIG_MONITOR) += monitor.o

	monitor-objs := code.o

else
	# external module build

	EXTRA_FLAGS += -I$(PWD)

	KDIR	:= /lib/modules/$(shell uname -r)/build
	MDIR	:= /lib/modules/$(shell uname -r)
	PWD	:= $(shell pwd)
	KREL	:= $(shell cd ${KDIR} && make -s kernelrelease)

	export CONFIG_MONITOR := m

	all:
		$(MAKE) -C $(KDIR) $(EXTRA_FLAGS) M=$(PWD) modules

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

endif

.PHONY : all clean install uninstall
