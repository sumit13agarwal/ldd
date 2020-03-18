obj-m += pen.o

KERNEL_SRC = /usr/src/linux-headers-$(shell uname -r)

all:
	$(MAKE) -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
clean:
	$(MAKE) -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) clean
