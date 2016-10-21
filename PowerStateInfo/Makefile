obj-m += PowerStateInfo.o
mod = $(shell lsmod | grep  PowerStateInfo | wc -l)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
ifeq ($(mod), 1)
	sudo rmmod PowerStateInfo
endif
	sudo insmod PowerStateInfo.ko
