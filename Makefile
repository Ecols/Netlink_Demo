KERNEL_DIR=/usr/src/linux-headers-3.19.0-25-generic
KMOD_NAME:=nlk
TARGET:=$(KMOD_NAME).ko

obj-m:=$(KMOD_NAME).o
$(KMOD_NAME)-y+=kinit.o knlk.o

USER_TARGET:=user
USER_SRCS:=user.c

all:kmod umk

kmod:
        $(VERB)make -C $(KERNEL_DIR) M=$(CURDIR) ARCH=x86_64 modules

umk:
        gcc $(USER_SRCS) -o $(USER_TARGET)

clean:
        $(VERB)rm -rf *.o *.ko *.mod.* *.order *.symvers .*.cmd .tmp_versions
        $(VERB)rm -rf $(USER_TARGET)
