cmdline 中的类别分为两种
	1.内核参数 // 根据值做 模块的初始化,对应一系列函数
		early_param                    	// 在  parse_args "early options"  时解析
			obs_kernel_param
		__setup 						// 在  parse_args "Booting kernel" 时解析
			obs_kernel_param
	2.模块参数 // 根据值做 变量的初始化,对应一个值, 传值sample 为 file.value=5  file 为 模块文件名file.c中的前缀,value 为file.c 中的变量 value , 5 为值
		module_param 					// 在  parse_args "Booting kernel" 时解析.
			kernel_param_ops


解析顺序
	1.  parse_args "early options"
			early_param
	2.  parse_args "Booting kernel"
			module_param
			__setup
	3.  parse_args initcall_level_names[level]
			module_param


解析方法

	1.
	parse_args "early options" 即 do_early_param
		解析的是 __setup_start 到 __setup_end 的 .init.data 段,即 early_param 定义的函数相关结构体    (还会解析一种情况  cmdline中的console且匹配到了__setup的earlycon)

	2.
	parse_args "Booting kernel"
		解析的是 __start___param 到 __stop___param ,且level 为 -1 的 __param 段,即 module_param 定义的 变量相关结构体 ,用来匹配的 param 为 module_name.value_name ,例如 node-platform.num , module_param 的level 都是 -1
		如果没有解析到会调用 obsolete_checksetup 解析 __setup_start  到 __setup_end 的 .init.data 段,即 __setup 定义的函数相关结构体

	3.
	parse_args initcall_level_names[level] ,level 范围 为 0 ... 7
		解析的是 __start___param 到 __stop___param ,且level 为 level 的 __param 段,即 module_param 定义的 变量相关结构体 ,此次解析不会匹配到任何 module_param ,因为 module_param level 等于 -1
		如果没有解析到会调用 repair_env_string 解析 __setup_start  到 __setup_end 的 .init.data 段,即 __setup 定义的函数相关结构体
		repair_env_string 并不会做什么实际的事情




// module_param 相关函数
module_param_cb
param_ops_int
kernel_param_ops
