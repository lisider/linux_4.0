1.
parse_early_param
    有 console= 且 有 param_early(earlycon)
    如果 console=null ,则修复
    如果 console=something ,则退出

    choen 中 有 earlyprintk 也不会调用 earlyprintk 注册的函数
2.
parse_args Booting kernel
    console='ttyAMA0'
    console_setup
        填充 console_cmdline
3.
console_init // 注册的 console_initcall(xxx);
    con_init // console_initcall(con_init);
    里面将 register_console(&vt_console_driver);
        将 command line 即 dts 中的 console=xxx 中的 xxx 与 vt_console_driver.name 比较,如果不同,则退出
    hvc_console_init
       里面 调用 register_console(&hvc_console);
            将 hvc_console 即 dts 中的 console=xxx 中的 xxx 与 hvc_console.name 比较,如果不同,则退出

4.
level 6 
AMBA PL011 UART driver
    probe 里面调用 register_console(amba_console);
        将 command line 即 dts 中的 console=xxx 中的 xxx 与 vt_console_driver.name 比较,相同,继续

