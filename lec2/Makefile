obj-m += scull.o

KERNEL_DIR = /usr/src/linux-headers-$(shell uname -r)

all:
	$(MAKE) -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /usr/lib/modules/$(shell uname -r)/modules M=$(PWD) clean