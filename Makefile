obj-m += ioctl_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

ioctl : ioctl.c
	cc -o ioctl ioctl.c

C: 
	rm *.ko
