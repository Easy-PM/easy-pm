obj-m += kmv.o
load = $(shell lsmod | grep kmv | wc -l)
ccflags-y := -DDEBUG

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
ifeq ($(load), 1)
	sudo rmmod kmv
endif
	sudo insmod kmv.ko
