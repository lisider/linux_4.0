#########################################################################
# File Name: build.sh
# Author: Sues
# mail: sumory.kaka@foxmail.com
# Created Time: Sun 07 Oct 2018 06:31:12 PM PDT
# Version : 1.0
#########################################################################
#!/bin/bash
export ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-;make LOADADDR=0x60004000 uImage -j8 && make modules && make dtbs \
    && cp arch/arm/boot/uImage ../tftpboot && cp arch/arm/boot/dts/vexpress-v2p-ca9.dtb ../tftpboot
