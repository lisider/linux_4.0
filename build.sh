#########################################################################
# File Name: build.sh
# Author: Sues
# mail: sumory.kaka@foxmail.com
# Created Time: Sun 07 Oct 2018 06:31:12 PM PDT
# Version : 1.0
#########################################################################
#!/bin/bash
CURRENT_SHELL=$(ps -ef | grep `echo $$` | grep -v grep | grep -v ps | awk -F" " '{print $8}' |grep -v "awk" | uniq)


if [ $CURRENT_SHELL != "bash" ];then
    echo use bash as your shell
    return -2
else
    echo using bash
fi

if [ $# -eq 0 ];then
    export ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-;make LOADADDR=0x60004000 uImage -j8 \
        && cp arch/arm/boot/uImage ../tftpboot
elif [ "$1" == "all" ];then
    export ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-;make LOADADDR=0x60004000 uImage -j8 && make modules && make dtbs \
        && cp arch/arm/boot/uImage ../tftpboot && cp arch/arm/boot/dts/vexpress-v2p-ca9.dtb ../tftpboot
elif [ "$1" == "dts" ];then
    export ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-;make dtbs \
        && cp arch/arm/boot/dts/vexpress-v2p-ca9.dtb ../tftpboot
elif [ "$1" == "modules" ] || [ $1 == "module"  ];then
    export ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-;make modules
elif [ "$1" == "export"  ] || [ $1 == "exports"   ];then
    export ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-;
	make export_report  > export_symbole.txt # 用来 导出 EXPORT 出来的符号
fi
