#
# Makefile
#
# The Makefile of the sparrow driver
#
# Author: wowo<www.wowotech.net>
#
# This program is free software; you can redistribute  it and/or modify it
# under  the terms of  the GNU General  Public License as published by the
# Free Software Foundation;  either version 2 of the  License, or (at your
# option) any later version.
#

# Module name will be producted
MOD_NAME1 = spa_drv
MOD_NAME2 = spa_dev

# all the dependented object files, they will generate {MOD_NAME}.o
$(MOD_NAME1)-objs := sparrow_drv.o TS_test.o
$(MOD_NAME2)-objs := sparrow_dev.o

# {MOD_NAME}.o will generate {MOD_NAME}.ko
obj-m := $(MOD_NAME1).o $(MOD_NAME2).o


# 
# The kernel directory and the build options should passed 
# kernel's Makefile, you can change it according to your platform.
#
# NOTE: the kernel should built first!
#
KDIR := $(pwd)	# set to yours
CROSS_ARCH := ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi-

PWD=$(shell pwd) 

all: 
	make $(CROSS_ARCH) -C $(KDIR) M=$(PWD) modules

clean: 
	rm -rf *.o *.ko *.mod.c *.order *.symvers \.*.cmd \.tmp_versions
