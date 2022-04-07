PWD:=$(shell pwd)
KERNELPATH ?= /lib/modules/$(shell uname -r)/build
RANBO_PATH ?= $(PWD)/../../src

EXTRA_CFLAGS += -I$(RANBO_PATH)

CONFIG_RANBO = m

obj-$(CONFIG_RANBO) += main.o

all:
	$(MAKE) -C $(KERNELPATH) M=$(PWD) PWD=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELPATH) M=$(PWD) clean

.PHONY: all clean
