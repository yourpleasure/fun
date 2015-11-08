obj-m := main.o

KERNELDIR = /lib/modules/`uname -r`/build
PWD = $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules -Wall

clean:
	rm -rf main.o main.ko main.mod.c Module.symvers modules.order main.mod.o .*.cmd .tmp_versions
.PHONY: default clean
