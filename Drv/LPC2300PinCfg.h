//修改自周立功相关文件
#ifndef __LPC2300PINCFG_H
#define __LPC2300PINCFG_H

/*********************************************************************************************************
                                            P0
*********************************************************************************************************/
//P0.00
#define    P0_00_GPIO        0x00                  //GPIO
#define    P0_00_RD1         0x01                  //CAN-1控制器接收引脚
#define    P0_00_TXD3        0x02                  //UART3发送引脚
#define    P0_00_SDA1        0x03                  //I2C-1数据线（开漏）

#define    P0_00_FNUC        P0_00_GPIO

//P0.01
#define    P0_01_GPIO        0x00                  //GPIO
#define    P0_01_TD1         0x01                  //CAN-1控制器发送引脚
#define    P0_01_RXD3        0x02                  //UART3接收引脚
#define    P0_01_SCL1        0x03                  //I2C-1时钟线（开漏）

#define    P0_01_FNUC        P0_01_GPIO

//P0.02
#define    P0_02_GPIO        0x00                  //GPIO
#define    P0_02_TXD0        0x01                  //UART0发送引脚
//...      Reserved          0x02                  //保留
//...      Reserved          0x03

#define    P0_02_FNUC        P0_02_GPIO

//P0.03
#define    P0_03_GPIO        0x00                  //GPIO
#define    P0_03_RXD0        0x01                  //UART0接收引脚
//...      Reserved          0x02                  //保留
//...      Reserved          0x03

#define    P0_03_FNUC        P0_03_GPIO

//P0.04
#define    P0_04_GPIO        0x00                  //GPIO
#define    P0_04_I2SRX_CLK   0x01                  //I2S总线接收时钟SCK，由主机驱动，从机接收
#define    P0_04_RD2         0x02                  //CAN-2接收线
#define    P0_04_CAP20       0x03                  //定时器2捕获输入，通道0

#define    P0_04_FNUC        P0_04_GPIO

//P0.05
#define    P0_05_GPIO        0x00                  //GPIO
#define    P0_05_I2SRX_WS    0x01                  //I2S总线接收字选择WS，由主机驱动，从机接收
#define    P0_05_TD2         0x02                  //CAN-2发送线
#define    P0_05_CAP21       0x03                  //定时器2捕获输入，通道1

#define    P0_05_FNUC        P0_05_GPIO

//P0.06
#define    P0_06_GPIO        0x00                  //GPIO
#define    P0_06_I2SRX_SDA   0x01                  //I2S总线接收数据线SD，由发送器驱动，接收器读取
#define    P0_06_SSEL1       0x02                  //SSP-1总线从机选择
#define    P0_06_MAT20       0x03                  //定时器2匹配输出，通道0

#define    P0_06_FNUC        P0_06_GPIO

//P0.07
#define    P0_07_GPIO        0x00                  //GPIO
#define    P0_07_I2STX_CLK   0x01                  //I2S总线发送时钟线SCK，由主机驱动，从机接收
#define    P0_07_SSCK1       0x02                  //SSP-1时钟线
#define    P0_07_MAT21       0x03                  //定时器2匹配输出，通道1

#define    P0_07_FNUC        P0_07_GPIO

//P0.08
#define    P0_08_GPIO        0x00                  //GPIO
#define    P0_08_I2STX_WS    0x01                  //I2S总线发送字选择WS，由主机驱动，从机接收
#define    P0_08_SMISO1      0x02                  //SSP-1主机输入从机输出数据线
#define    P0_08_MAT22       0x03                  //定时器2匹配输出，通道2

#define    P0_08_FNUC        P0_08_GPIO

//P0.09
#define    P0_09_GPIO        0x00                  //GPIO
#define    P0_09_I2STX_SDA   0x01                  //I2S总线发送数据线SD，由发送器驱动，接收器读取
#define    P0_09_SMOSI1      0x02                  //SSP-1主机输出从机输入数据线
#define    P0_09_MAT23       0x03                  //定时器2匹配输出，通道3

#define    P0_09_FNUC        P0_09_GPIO

//P0.10
#define    P0_10_GPIO        0x00                  //GPIO
#define    P0_10_TXD2        0x01                  //UART2发送引脚
#define    P0_10_SDA2        0x02                  //I2C-2数据线（非开漏引脚）
#define    P0_10_MAT30       0x03                  //定时器3匹配输出，通道0

#define    P0_10_FNUC        P0_10_GPIO

//P0.11
#define    P0_11_GPIO        0x00                  //GPIO
#define    P0_11_RXD2        0x01                  //UART2接收引脚
#define    P0_11_SCL2        0x02                  //I2C-2时钟线（非开漏引脚）
#define    P0_11_MAT31       0x03                  //定时器3匹配输出，通道1

#define    P0_11_FNUC        P0_11_GPIO

//P0.12
#define    P0_12_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P0_12_SMISO1      0x02                  //SSP-1主机输入从机输出数据线
#define    P0_12_AD06        0x03                  //ADC-0，输入6

#define    P0_12_FNUC        P0_12_GPIO

//P0.13
#define    P0_13_GPIO        0x00                  //GPIO
#define    P0_13_U2UP_LED    0x01                  //USB2 Good Link LED指示引脚
#define    P0_13_SMOSI1      0x02                  //SSP-1主机输出从机输入数据线
#define    P0_13_AD07        0x03                  //ADC-0，输入7

#define    P0_13_FNUC        P0_13_GPIO

//P0.14
#define    P0_14_GPIO        0x00                  //GPIO
#define    P0_14_U2CONNECT   0x01                  //USB2软连接控制
//...      Reserved          0x02                  //保留
#define    P0_14_SSEL1       0x03                  //SSP-1从机选择

#define    P0_14_FNUC        P0_14_GPIO

//P0.15
#define    P0_15_GPIO        0x00                  //GPIO
#define    P0_15_TXD1        0x01                  //UART1发送引脚
#define    P0_15_SSCK0       0x02                  //SSP-0时钟线
#define    P0_15_SCK0        0x03                 //SPI时钟线

#define    P0_15_FNUC        P0_15_GPIO

//P0.16
#define    P0_16_GPIO        0x00                  //GPIO
#define    P0_16_RXD1        0x01                  //UART1接收引脚
#define    P0_16_SSEL0       0x02                  //SSP-0从机选择
#define    P0_16_SEL0        0x03                  //SPI从机选择

#define    P0_16_FNUC        P0_16_GPIO

//P0.17
#define    P0_17_GPIO        0x00                  //GPIO
#define    P0_17_CTS1        0x01                  //UART1清零发送信号CTS
#define    P0_17_SMISO0      0x02                  //SSP-0主机输入从机输出数据线
#define    P0_17_MISO0       0x03                  //SPI主入从出数据线

#define    P0_17_FNUC        P0_17_GPIO

//P0.18
#define    P0_18_GPIO        0x00                  //GPIO
#define    P0_18_DCD1        0x01                  //UART1数据载波检测输入DCD
#define    P0_18_SMOSI0      0x02                  //SSP-0主机输出从机输入数据线
#define    P0_18_MOSI0       0x03                  //SPI主出从入数据线

#define    P0_18_FNUC        P0_18_GPIO

//P0.19
#define    P0_19_GPIO        0x00                  //GPIO
#define    P0_19_DSR1        0x01                  //UART1数据设置就绪DSR
#define    P0_19_MCICLK      0x02                  //SD、MMC接口时钟输出线
#define    P0_19_SDA1        0x03                  //I2C-1数据线（非开漏）

#define    P0_19_FNUC        P0_19_GPIO

//P0.20
#define    P0_20_GPIO        0x00                  //GPIO
#define    P0_20_DTR1        0x01                  //UART1终端就绪DTR
#define    P0_20_MCICMD      0x02                  //SD、MMC接口命令线
#define    P0_20_SCL1        0x03                  //I2C-1时钟线（非开漏）

#define    P0_20_FNUC        P0_20_GPIO

//P0.21
#define    P0_21_GPIO        0x00                  //GPIO
#define    P0_21_RI1         0x01                  //UART1振铃信号
#define    P0_21_MCIPWR      0x02                  //外部SD、MMC电源使能
#define    P0_21_RD1         0x03                  //CAN-1接收引脚

#define    P0_21_FNUC        P0_21_GPIO

//P0.22
#define    P0_22_GPIO        0x00                  //GPIO
#define    P0_22_RTS1        0x01                  //UART1请求发送引脚
#define    P0_22_MCIDAT0     0x02                  //SD、MMC接口数据线
#define    P0_22_TD1         0x03                  //CAN-1发送线

#define    P0_22_FNUC        P0_22_GPIO

//P0.23
#define    P0_23_GPIO        0x00                  //GPIO
#define    P0_23_AD00        0x01                  //ADC-0，通道0
#define    P0_23_I2SRX_CLK   0x02                  //I2S接收时钟SCK，由主机驱动，从机接收
#define    P0_23_CAP30       0x03                  //定时器3捕获输入，通道0

#define    P0_23_FNUC        P0_23_GPIO

//P0.24
#define    P0_24_GPIO        0x00                  //GPIO
#define    P0_24_AD01        0x01                  //ADC-0，通道1
#define    P0_24_I2SRX_WS    0x02                  //I2S接收字选择WS，由主机驱动，从机接收
#define    P0_24_CAP31       0x03                  //定时器3捕获输入，通道1

#define    P0_24_FNUC        P0_24_GPIO

//P0.25
#define    P0_25_GPIO        0x00                  //GPIO
#define    P0_25_AD02        0x01                  //ADC-0，通道2
#define    P0_25_I2SRX_SDA   0x02                  //I2S接收数据SD，由发送器驱动，接收器读取
#define    P0_25_TXD3        0x03                  //UART3发送引脚

#define    P0_25_FNUC        P0_25_GPIO

//P0.26
#define    P0_26_GPIO        0x00                  //GPIO
#define    P0_26_AD03        0x01                  //ADC-0，通道3
#define    P0_26_AOUT        0x02                  //DAC输出
#define    P0_26_RXD3        0x03                  //UART-3接收引脚

#define    P0_26_FNUC        P0_26_GPIO

//P0.31（仅LPC2378具有，在LPC2364/6/8中为保留位）
#define    P0_31_GPIO        0x00                  //GPIO
#define    P0_31_U2D_ANODE   0x01                  //USB2双向D+线
//...      Reserved          0x02                  //保留
//...      Reserved          0x03

#define    P0_31_FNUC        P0_31_GPIO

/*********************************************************************************************************
                                            P1
*********************************************************************************************************/
//P1.19
#define    P1_19_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
//...      Reserved          0x02
#define    P1_19_CAP11       0x03                  //定时器1捕获输入，通道1

#define    P1_19_FNUC        P1_19_GPIO

//P1.20
#define    P1_20_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P1_20_PWM12       0x02                  //PWM-1输出，通道2
#define    P1_20_SSCK0        0x03                  //SSP-0时钟SCK

#define    P1_20_FNUC        P1_20_GPIO

//P1.21
#define    P1_21_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P1_21_PWM13       0x02                  //PWM-1输出，通道3
#define    P1_21_SSEL0       0x03                  //SSP-0从机选择

#define    P1_21_FNUC        P1_21_GPIO

//P1.22
#define    P1_22_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
//...      Reserved          0x02
#define    P1_22_MAT10       0x03                  //定时器1匹配输出，通道0

#define    P1_22_FNUC        P1_22_GPIO

//P1.23
#define    P1_23_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P1_23_PWM14       0x02                  //PWM-1输出，通道4
#define    P1_23_SMISO0      0x03                  //SSP-0主机输入，从机输出

#define    P1_23_FNUC        P1_23_GPIO

//P1.24
#define    P1_24_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P1_24_PWM15       0x02                  //PWM-1输出，通道5
#define    P1_24_SMOSI0      0x03                  //SSP-0主机输出，从机输入

#define    P1_24_FNUC        P1_24_GPIO

//P1.25
#define    P1_25_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
//...      Reserved          0x02
#define    P1_25_MAT11       0x03                  //定时器1匹配输出，通道1

#define    P1_25_FNUC        P1_25_GPIO

//P1.26
#define    P1_26_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P1_26_PWM16       0x02                  //PWM-1输出，通道6
#define    P1_26_CAP00       0x03                  //定时器0捕获输入，通道0

#define    P1_26_FNUC        P1_26_GPIO

//P1.27
#define    P1_27_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
//...      Reserved          0x02
#define    P1_27_CAP01       0x03                  //定时器0捕获输入，通道1

#define    P1_27_FNUC        P1_27_GPIO

//P1.28
#define    P1_28_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P1_28_PCAP10      0x02                  //PWM-1捕获输入，通道0
#define    P1_28_MAT00       0x03                  //定时器0匹配输出，通道0

#define    P1_28_FNUC        P1_28_GPIO

//P1.29
#define    P1_29_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P1_29_PCAP11      0x02                  //PWM-1捕获输入，通道1
#define    P1_29_MAT01       0x03                  //定时器0匹配输出，通道1

#define    P1_29_FNUC        P1_29_GPIO

//P1.31
#define    P1_31_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P1_31_SSCK1       0x02                  //SSP-1时钟线
#define    P1_31_AD05       (0x03U)                //ADC-0输入，通道5

#define    P1_31_FNUC        P1_31_GPIO

/*********************************************************************************************************
                                            P2
*********************************************************************************************************/
//P2.00
#define    P2_00_GPIO        0x00                  //GPIO
#define    P2_00_PWM11       0x01                  //PWM-1，通道1
#define    P2_00_TXD1        0x02                  //UART1数据发送引脚
#define    P2_00_TRACECLK    0x03                  //跟踪时钟

#define    P2_00_FNUC        P2_00_GPIO

//P2.01
#define    P2_01_GPIO        0x00                  //GPIO
#define    P2_01_PWM12       0x01                  //PWM-1，通道2
#define    P2_01_RXD1        0x02                  //UART1数据接收引脚
#define    P2_01_PIPESTAT0   0x03                  //流水线状态，位0

#define    P2_01_FNUC        P2_01_GPIO

//P2.02
#define    P2_02_GPIO        0x00                  //GPIO
#define    P2_02_PWM13       0x01                  //PWM-1，通道3
#define    P2_02_CTS1        0x02                  //UART1清零发送线
#define    P2_02_PIPESTAT1   0x03                  //流水线状态，位1

#define    P2_02_FNUC        P2_02_GPIO

//P2.03
#define    P2_03_GPIO        0x00                  //GPIO
#define    P2_03_PWM14       0x01                  //PWM-1，通道4
#define    P2_03_DCD1        0x02                  //UART1数据载波检测线
#define    P2_03_PIPESTAT2   0x03                  //流水线状态，位2

#define    P2_03_FNUC        P2_03_GPIO

//P2.04
#define    P2_04_GPIO        0x00                  //GPIO
#define    P2_04_PWM15       0x01                  //PWM-1，通道5
#define    P2_04_DSR1        0x02                  //UART1数据设置就绪线
#define    P2_04_TRACESYNC   0x03                  //跟踪同步

#define    P2_04_FNUC        P2_04_GPIO

//P2.05
#define    P2_05_GPIO        0x00                  //GPIO
#define    P2_05_PWM16       0x01                  //PWM-1，通道6
#define    P2_05_DTR1        0x02                  //UART1数据终端就绪线
#define    P2_05_TRACEPKT0   0x03                  //跟踪包，位0

#define    P2_05_FNUC        P2_05_GPIO

//P2.06
#define    P2_06_GPIO        0x00                  //GPIO
#define    P2_06_PCAP10      0x01                  //PWM-1捕获输入，通道0
#define    P2_06_RI1         0x02                  //UART1振铃输入
#define    P2_06_TRACEPKT1   0x03                  //跟踪包，位1

#define    P2_06_FNUC        P2_06_GPIO

//P2.07
#define    P2_07_GPIO        0x00                  //GPIO
#define    P2_07_RD2         0x01                  //CAN-2接收线
#define    P2_07_RTS1        0x02                  //UART1请求发送输出
#define    P2_07_TRACEPKT2   0x03                  //跟踪包，位2

#define    P2_07_FNUC        P2_07_GPIO

//P2.08
#define    P2_08_GPIO        0x00                  //GPIO
#define    P2_08_TD2         0x01                  //CAN-2发送线
#define    P2_08_TXD2        0x02                  //UART2发送引脚
#define    P2_08_TRACEPKT3   0x03                  //跟踪包，位3

#define    P2_08_FNUC        P2_08_GPIO

//P2.09
#define    P2_09_GPIO        0x00                  //GPIO
#define    P2_09_U1CONNECT   0x01                  //USB1软连接控制
#define    P2_09_RXD2        0x02                  //UART2接收引脚
#define    P2_09_EXTIN0      0x03                  //外部触发器输入

#define    P2_09_FNUC        P2_09_GPIO

//P2.10（ISP）
#define    P2_10_GPIO        0x00                  //GPIO
#define    P2_10_EINT0       0x01                  //外部中断0输入
//...      Reserved          0x02                  //保留
//...      Reserved          0x03

#define    P2_10_FNUC        P2_10_GPIO

//P2.11
#define    P2_11_GPIO        0x00                  //GPIO
#define    P2_11_EINT1       0x01                  //外部中断1输入
#define    P2_11_MCIDAT1     0x02                  //SD、MMC接口数据线1
#define    P2_11_I2STX_CLK   0x03                  //I2S发送时钟SCK，由主机驱动，从机接收

#define    P2_11_FNUC        P2_11_GPIO

//P2.12
#define    P2_12_GPIO        0x00                  //GPIO
#define    P2_12_EINT2       0x01                  //外部中断2输入
#define    P2_12_MCIDAT2     0x02                  //SD、MMC接口数据线2
#define    P2_12_I2STX_WS    0x03                  //I2S发送字选择WS，由主机驱动，从机接收

#define    P2_12_FNUC        P2_12_GPIO

//P2.13
#define    P2_13_GPIO        0x00                  //GPIO
#define    P2_13_EINT3       0x01
#define    P2_13_MCIDAT3     0x02                  //SD、MMC接口数据线3
#define    P2_13_I2STX_SDA   0x03                  //I2S发送数据SD，由主机驱动，从机接收

#define    P2_13_FNUC        P2_13_GPIO

/*********************************************************************************************************
                                            P3
*********************************************************************************************************/
//P3.23
#define    P3_23_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P3_23_CAP00       0x02                  //定时器0捕获输入，通道0
#define    P3_23_PCAP10      0x03                  //PWM-1捕获输入，通道0

#define    P3_23_FNUC        P3_23_GPIO

//P3.24
#define    P3_24_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P3_24_CAP01       0x02                  //定时器0捕获输入，通道1
#define    P3_24_PWM11       0x03                  //PWM-1输出，通道1

#define    P3_24_FNUC        P3_24_GPIO

//P3.25
#define    P3_25_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P3_25_MAT00       0x02                  //定时器0匹配输出，通道0
#define    P3_25_PWM12       0x03                  //PWM-1输出，通道2

#define    P3_25_FNUC        P3_25_GPIO

//P3.26
#define    P3_26_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P3_26_MAT01       0x02                  //定时器0匹配输出，通道1
#define    P3_26_PWM13       0x03                  //PWM-1输出，通道3

#define    P3_26_FNUC        P3_26_GPIO


/*********************************************************************************************************
                                            P4
*********************************************************************************************************/
//P4.28
#define    P4_28_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P4_28_MAT20       0x02                  //定时器2匹配输出，通道0
#define    P4_28_TXD3        0x03                  //UART3发送引脚

#define    P4_28_FNUC        P4_28_GPIO

//P4.29
#define    P4_29_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //保留
#define    P4_29_MAT21       0x02                  //定时器2匹配输出，通道1
#define    P4_29_RXD3        0x03                  //UART3接收引脚

#define    P4_29_FNUC        P4_29_GPIO

/*********************************************************************************************************
                                            引脚内部上下拉电阻配置
*********************************************************************************************************/
#define    PIN_PULL_UP       0x00                  //引脚内部上拉
#define    PIN_NO_PULL       0x02                  //引脚既不上拉，也不下拉
#define    PIN_PULL_DOWN     0x03                  //引脚内部下拉

//P0
#define    P0_00_MODE        PIN_PULL_UP
#define    P0_01_MODE        PIN_PULL_UP
#define    P0_02_MODE        PIN_PULL_UP
#define    P0_03_MODE        PIN_PULL_UP
#define    P0_04_MODE        PIN_PULL_UP
#define    P0_05_MODE        PIN_PULL_UP
#define    P0_06_MODE        PIN_PULL_UP
#define    P0_07_MODE        PIN_PULL_UP
#define    P0_08_MODE        PIN_PULL_UP
#define    P0_09_MODE        PIN_PULL_UP
#define    P0_10_MODE        PIN_PULL_UP
#define    P0_11_MODE        PIN_PULL_UP
#define    P0_12_MODE        PIN_PULL_UP
#define    P0_13_MODE        PIN_PULL_UP
#define    P0_14_MODE        PIN_PULL_UP
#define    P0_15_MODE        PIN_PULL_UP

#define    P0_16_MODE        PIN_PULL_UP
#define    P0_17_MODE        PIN_PULL_UP
#define    P0_18_MODE        PIN_PULL_UP
#define    P0_19_MODE        PIN_PULL_UP
#define    P0_20_MODE        PIN_PULL_UP
#define    P0_21_MODE        PIN_PULL_UP
#define    P0_22_MODE        PIN_PULL_UP
#define    P0_23_MODE        PIN_PULL_UP
#define    P0_24_MODE        PIN_PULL_UP
#define    P0_25_MODE        PIN_PULL_UP
#define    P0_26_MODE        PIN_PULL_UP

#define    P0_31_MODE        PIN_PULL_UP

//P1
#define    P1_18_MODE        PIN_PULL_UP
#define    P1_19_MODE        PIN_PULL_UP
#define    P1_20_MODE        PIN_PULL_UP
#define    P1_21_MODE        PIN_PULL_UP
#define    P1_22_MODE        PIN_PULL_UP
#define    P1_23_MODE        PIN_PULL_UP
#define    P1_24_MODE        PIN_PULL_UP
#define    P1_25_MODE        PIN_PULL_UP
#define    P1_26_MODE        PIN_PULL_UP
#define    P1_27_MODE        PIN_PULL_UP     
#define    P1_28_MODE        PIN_PULL_UP     
#define    P1_29_MODE        PIN_PULL_UP     
#define    P1_30_MODE        PIN_PULL_UP     
#define    P1_31_MODE        PIN_PULL_UP     

//P2
#define    P2_00_MODE        PIN_PULL_UP
#define    P2_01_MODE        PIN_PULL_UP
#define    P2_02_MODE        PIN_PULL_UP
#define    P2_03_MODE        PIN_PULL_UP
#define    P2_04_MODE        PIN_PULL_UP
#define    P2_05_MODE        PIN_PULL_UP
#define    P2_06_MODE        PIN_PULL_UP
#define    P2_07_MODE        PIN_PULL_UP
#define    P2_08_MODE        PIN_PULL_UP
#define    P2_09_MODE        PIN_PULL_UP
#define    P2_10_MODE        PIN_PULL_UP
#define    P2_11_MODE        PIN_PULL_UP
#define    P2_12_MODE        PIN_PULL_UP
#define    P2_13_MODE        PIN_PULL_UP

//P3
#define    P3_23_MODE        PIN_PULL_UP
#define    P3_24_MODE        PIN_PULL_UP
#define    P3_25_MODE        PIN_PULL_UP
#define    P3_26_MODE        PIN_PULL_UP
#define    P3_27_MODE        PIN_PULL_UP    

//P4
#define    P4_28_MODE        PIN_PULL_UP     
#define    P4_29_MODE        PIN_PULL_UP     

/*********************************************************************************************************
                                            ETM 仿真接口
*********************************************************************************************************/
#define    ETM_DISABLE       0x00                                       /*  禁止ETM仿真接口             */

/*********************************************************************************************************
** 函数名称: void PinInit(void)
** 函数功能: 引脚初始化
** 入口参数: 无
** 返 回 值: 无
** 说    明: 函数将初始化所有的引脚配置，完成引脚连接、上下拉电阻设置
*********************************************************************************************************/
extern void PinInit(void);

#endif                                                                  /*  __LPC2300PINCFG_H           */

/*********************************************************************************************************
    End Of File
*********************************************************************************************************/
