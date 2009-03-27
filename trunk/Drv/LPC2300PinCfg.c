/*
	LPC23000PinCfg.c
	Pin配置管理器
	提供引脚功能统一配置功能（已在Mian中调用）。
	修改自周立功相关文件
	BUAA 2008-7-14
	张以成
*/

#include "Config.h"
#include "Drv.h"
#include "LPC2300PinCfg.h"

//引脚初始化函数
//需事先在 LPC2300PinCfg.h 头文件中选择好相应管脚的功能
//将初始化所有对用户开发的引脚的配置,完成引脚连接,上下拉电阻设置P0、P1口设置为低速访问模式以兼容老器件。
void PinInit (void)
{

    /*
     * 引脚连接配置
     */
    //P0.00 -- P0.15
	PINSEL0  = (P0_15_FNUC <<30)|(P0_14_FNUC <<28)|(P0_13_FNUC <<26)|(P0_12_FNUC <<24)|(P0_11_FNUC <<22)| 
	           (P0_10_FNUC <<20)|(P0_09_FNUC <<18)|(P0_08_FNUC <<16)|(P0_07_FNUC <<14)|(P0_06_FNUC <<12)| 
	           (P0_05_FNUC <<10)|(P0_04_FNUC << 8)|(P0_03_FNUC << 6)|(P0_02_FNUC << 4)|(P0_01_FNUC << 2)| P0_00_FNUC;

	//P0.16 -- P0.31
	PINSEL1  = //(PINSEL1 & ((0xf << 22) | (0xf << 26))) |
	           (P0_31_FNUC <<26)|
	           (P0_26_FNUC <<20)|(P0_25_FNUC <<18)|(P0_24_FNUC <<16)|(P0_23_FNUC <<14)|(P0_22_FNUC <<12)| 
	           (P0_21_FNUC <<10)|(P0_20_FNUC << 8)|(P0_19_FNUC << 6)|(P0_18_FNUC << 4)|(P0_17_FNUC << 2)| P0_16_FNUC;

	//P1.16 -- P1.31
	PINSEL3  = (PINSEL3 & (0x3f | (3 << 28))) |
	           (P1_31_FNUC <<30)|(P1_29_FNUC <<26)|(P1_28_FNUC <<24)|(P1_27_FNUC <<22)| 
	           (P1_26_FNUC <<20)|(P1_25_FNUC <<18)|(P1_24_FNUC <<16)|(P1_23_FNUC <<14)|(P1_22_FNUC <<12)| 
	           (P1_21_FNUC <<10)|(P1_20_FNUC << 8)|(P1_19_FNUC << 6);

	//P2.00 -- P2.15        
	PINSEL4  = (P2_13_FNUC <<26)|(P2_12_FNUC <<24)|(P2_11_FNUC <<22)| 
	           (P2_10_FNUC <<20)|(P2_09_FNUC <<18)|(P2_08_FNUC <<16)|(P2_07_FNUC <<14)|(P2_06_FNUC <<12)| 
	           (P2_05_FNUC <<10)|(P2_04_FNUC << 8)|(P2_03_FNUC << 6)|(P2_02_FNUC << 4)|(P2_01_FNUC << 2)| P2_00_FNUC;
	
	//P3.16 -- P3.31
	PINSEL7  = (P3_26_FNUC <<20)|(P3_25_FNUC <<18)|(P3_24_FNUC <<16)|(P3_23_FNUC <<14);

	//P4.16 -- P4.31
	PINSEL9  = (PINSEL9 & ~(0xf << 24)) |
	           ((P4_29_FNUC <<26) | (P4_28_FNUC <<24));

	PINSEL10 = ETM_DISABLE;           //禁止ETM接口功能
#if 1
	/*
	 * 引脚内部上下拉电阻配置
	 */
    //P0
    PINMODE0 = (P0_15_MODE <<30)|(P0_14_MODE <<28)|(P0_13_MODE <<26)|(P0_12_MODE <<24)|(P0_11_MODE <<22)| 
	           (P0_10_MODE <<20)|(P0_09_MODE <<18)|(P0_08_MODE <<16)|(P0_07_MODE <<14)|(P0_06_MODE <<12)| 
	           (P0_05_MODE <<10)|(P0_04_MODE << 8)|(P0_03_MODE << 6)|(P0_02_MODE << 4)|(P0_01_MODE << 2)| P0_00_MODE;
	
    PINMODE1 = (P0_31_MODE <<30)|
               (P0_26_MODE <<20)|(P0_25_MODE <<18)|(P0_24_MODE <<16)|(P0_23_MODE <<14)|(P0_22_MODE <<12)| 
	           (P0_21_MODE <<10)|(P0_20_MODE << 8)|(P0_19_MODE << 6)|(P0_18_MODE << 4)|(P0_17_MODE << 2)| P0_16_MODE;

	//P1        
    PINMODE3 = (P1_31_MODE <<30)|(P1_30_MODE <<28)|(P1_29_MODE <<26)|(P1_28_MODE <<24)|(P1_27_MODE <<22)| 
	           (P1_26_MODE <<20)|(P1_25_MODE <<18)|(P1_24_MODE <<16)|(P1_23_MODE <<14)|(P1_22_MODE <<12)| 
	           (P1_21_MODE <<10)|(P1_20_MODE << 8)|(P1_19_MODE << 6)|(P1_18_MODE << 4);

    //P2
    PINMODE4 = (P2_13_MODE <<26)|(P2_12_MODE <<24)|(P2_11_MODE <<22)| 
	           (P2_10_MODE <<20)|(P2_09_MODE <<18)|(P2_08_MODE <<16)|(P2_07_MODE <<14)|(P2_06_MODE <<12)| 
	           (P2_05_MODE <<10)|(P2_04_MODE << 8)|(P2_03_MODE << 6)|(P2_02_MODE << 4)|(P2_01_MODE << 2)| P2_00_MODE;

    //P3
    PINMODE7 = (P3_27_MODE <<22)| 
	           (P3_26_MODE <<20)|(P3_25_MODE <<18)|(P3_24_MODE <<16)|(P3_23_MODE <<14);
    
    //P4
    PINMODE9 = (P4_29_MODE <<26)|(P4_28_MODE <<24);
#endif

    /*
     * 引脚访问速度配置
     */
    SCS &= ~0x01;         //P0、P1默认为低速访问模式，以兼容老器件
    
    FIO0MASK =0x00;       //不屏蔽引脚高速功能
    FIO1MASK =0x00;
    FIO2MASK =0x00;
    FIO3MASK =0x00;
    FIO4MASK =0x00;
}

