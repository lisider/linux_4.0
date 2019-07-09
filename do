#########################################################################
# File Name: run.sh
# Author: Sues
# mail: sumory.kaka@foxmail.com
# Created Time: Sat 06 Jul 2019 06:34:59 PM PDT
# Version : 1.0
#########################################################################
#!/bin/bash
#
# suggest:
# 1. ./do env
# 2. ./do build
# 3. ./do run

LROOT=$PWD
ROOTFS_X86=_install_x86
ROOTFS_ARM32=_install_arm32
ROOTFS_ARM64=_install_arm64
CONSOLE_DEV_NODE=dev/console

if [ $# -lt 1 ]; then
	echo "Usage: $0 run [debug]"
	echo "Usage: $0 kill"
	echo "Usage: $0 gdb"
	echo "Usage: $0 build"
	echo "Usage: $0 rebuild"
	echo "Usage: $0 env"
	echo "Usage: $0 eclipse"
	echo "Usage: $0 build_cpio"
	echo "Usage: $0 cpio_rootfs"
	echo "Usage: $0 run_cpio"
fi




case $1 in
	build)
        export ARCH=arm
        export CROSS_COMPILE=arm-linux-gnueabi-
        mkdir _install_arm32/dev/
        cd _install_arm32/dev/
        sudo mknod console c 5 1
        cd ../..
        make vexpress_defconfig
        make bzImage
        make dtbs
        ;;

	rebuild)
        export ARCH=arm
        export CROSS_COMPILE=arm-linux-gnueabi-
        make bzImage
        ;;

	build_cpio)
        export ARCH=arm
        export CROSS_COMPILE=arm-linux-gnueabi-
        make vexpress_cpio_defconfig
        make bzImage
        make dtbs
        ;;
	run)


        if [ ! -c $LROOT/$ROOTFS_ARM32/$CONSOLE_DEV_NODE ]; then
        	echo "please create console device node first, and recompile kernel"
        	exit 1
        fi

        #if [ $# -eq 2 ] && [[ "debug" == $2  ]]; then
        if [ $# -eq 2 ] ; then
        	echo "Enable GDB debug mode"
        	DBG="-s -S"
        fi

        qemu-system-arm -M vexpress-a9 -m 512M -kernel arch/arm/boot/zImage \
        		-dtb arch/arm/boot/dts/vexpress-v2p-ca9.dtb -nographic \
        		-append "rdinit=/linuxrc console=ttyAMA0 loglevel=8" \
        		$DBG
        ;;
	run_cpio)


        if [ ! -c $LROOT/$ROOTFS_ARM32/$CONSOLE_DEV_NODE ]; then
        	echo "please create console device node first, and recompile kernel"
        	exit 1
        fi

        #if [ $# -eq 2 ] && [[ "debug" == $2  ]]; then
        if [ $# -eq 2 ] ; then
        	echo "Enable GDB debug mode"
        	DBG="-s -S"
        fi

        qemu-system-arm -M vexpress-a9 -m 512M -kernel arch/arm/boot/zImage \
        		-initrd ./initrd.cpio \
        		-dtb arch/arm/boot/dts/vexpress-v2p-ca9.dtb -nographic \
        		-append "rdinit=/linuxrc console=ttyAMA0 loglevel=8" \
        		$DBG
#-append "root=/dev/sda1"
        ;;
	kill)
        sudo kill -9 `ps -ef |grep qemu-system-arm |grep -v grep |head -1 | awk  '{print $2}'`
        ;;
	gdb)
        #if [ $# -eq 2 ] && [ $2 == "start_kernel" ]; then
        if [ $# -eq 2 ] ; then
        	echo "Enable GDB debug mode"
        	DBG_FILE="gdb_start_kernel.init"
        fi
        arm-none-eabi-gdb --tui gdb -x $DBG_FILE
        ;;
	env)
        sudo apt-get install qemu libncurses5-dev gcc-arm-linux-gnueabi build-essential gdb-arm-none-eabi gcc-aarch64-linux-gnu eclipse-cdt libdw-dev systemtap systemtap-runtime
        ;;
	eclipse)
        eclipse &
        #eclipse 怎么新建工程,参见 <<奔跑吧-linux内核-qemu调试内核-样章.pdf>>
        ;;

	cpio_rootfs)
        rm initrd.cpio
        cd _install_arm32
        find . | cpio -o -H newc --file ../initrd.cpio
        cd ..
        ;;

esac



