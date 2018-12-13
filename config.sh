#########################################################################
# File Name: ./build.sh
# Author: Sues
# mail: sumory.kaka@foxmail.com
# Created Time: Sun 07 Oct 2018 06:27:24 PM PDT
# Version : 1.0
#########################################################################
#!/bin/bash
export ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-;make vexpress_defconfig;
