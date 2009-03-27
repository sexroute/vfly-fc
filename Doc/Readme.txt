LPC2300系列ARM7芯片项目文件，包含启动文件及驱动程序
模块说明：
	核心启动管理模块
	驱动模块
	工具函数模块
	用户函数模块
	项目文档
文件说明：
M2300根目录
│  M2300.IAB
│  M2300.IAD
│  M2300.IMB
│  M2300.IMD
│  M2300.mcp	ADS项目文件
│  M2300.PFI
│  M2300.PO
│  M2300.PR	SourceInsight3项目文件
│  M2300.PRI
│  M2300.PS
│  M2300.SearchResults
│  M2300.WK3
│
├─Core	核心模块目录
│      IRQ.s	IRQ启动汇编文件
│      IRQHander.c	IRQ管理模块
│      IRQHander.h
│      mem_a.scf	储存空间分配文件
│      mem_b.scf
│      mem_c.scf
│      Startup.s	启动文件
│      target.c	目标初始化文件
│      target.h
│
├─Doc		文档目录
│      Readme-old.txt
│      Readme.txt
│      vFly飞控下位机程序开发规范.txt
│      类库开发参考准则.pdf
│
├─Drv		驱动程序目录
│      74LV138.h	74LV138芯片操作头文件
│      AD.c		AD芯片驱动程序
│      AD.h
│      AT45DB642D.h	AT45DB642D芯片头文件
│      CAN.c		CAN总线驱动程序
│      CAN.h
│      DA.c		DA芯片驱动程序
│      DA.h
│      Drv.h		驱动程序统一头文件
│      Flash.c		Flash驱动程序
│      Flash.h
│      FPGA.c		FPGA模块驱动程序
│      FPGA.h
│      Gpio.c		Gpio驱动程序
│      Gpio.h
│      LPC2300PinCfg.c	引脚配置驱动程序
│      LPC2300PinCfg.h	引脚配置文件
│      SPI.c		SPI驱动程序
│      SPI.h
│      SSP.c		SSP驱动程序
│      SSP.h
│      SSP0.c		SSP0接口模式转换驱动程序
│      SSP0.h
│      Timer.c		时钟控制器驱动程序
│      Timer.h
│      UART.c		UART驱动程序
│      UART.h
│
├─Inc		定义文件目录
│      Config.h	类型及初始化定义文件(所有文件均需包含此文件)
│      LPC23xx.h	LPC23xx寄存器定义头文件
│
├─Tools	工具文件目录
│      Queue.c		通用列队数据存储驱动程序
│      Queue.h
│      tools.c		工具函数集
│      tools.h
│
└─User	用户文件目录
        main.c		C语言函数主入口
        Test.h		测试函数集

2008年7月3日
添加AD DA的电压值运算函数
添加AD的端口对应宏
2008年8月4日
添加类型转换宏(Config.h)
添加注释(FPGA.c)