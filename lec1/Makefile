obj-m += scull.o

KERNEL_DIR = /usr/src/linux-headers-$(shell uname -r)

all:
	$(MAKE) -C $(KERNEL_DIR) /usr/lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) /usr/lib/modules/$(shell uname -r)/build M=$(PWD) clean