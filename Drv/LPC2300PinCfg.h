//�޸�������������ļ�
#ifndef __LPC2300PINCFG_H
#define __LPC2300PINCFG_H

/*********************************************************************************************************
                                            P0
*********************************************************************************************************/
//P0.00
#define    P0_00_GPIO        0x00                  //GPIO
#define    P0_00_RD1         0x01                  //CAN-1��������������
#define    P0_00_TXD3        0x02                  //UART3��������
#define    P0_00_SDA1        0x03                  //I2C-1�����ߣ���©��

#define    P0_00_FNUC        P0_00_GPIO

//P0.01
#define    P0_01_GPIO        0x00                  //GPIO
#define    P0_01_TD1         0x01                  //CAN-1��������������
#define    P0_01_RXD3        0x02                  //UART3��������
#define    P0_01_SCL1        0x03                  //I2C-1ʱ���ߣ���©��

#define    P0_01_FNUC        P0_01_GPIO

//P0.02
#define    P0_02_GPIO        0x00                  //GPIO
#define    P0_02_TXD0        0x01                  //UART0��������
//...      Reserved          0x02                  //����
//...      Reserved          0x03

#define    P0_02_FNUC        P0_02_GPIO

//P0.03
#define    P0_03_GPIO        0x00                  //GPIO
#define    P0_03_RXD0        0x01                  //UART0��������
//...      Reserved          0x02                  //����
//...      Reserved          0x03

#define    P0_03_FNUC        P0_03_GPIO

//P0.04
#define    P0_04_GPIO        0x00                  //GPIO
#define    P0_04_I2SRX_CLK   0x01                  //I2S���߽���ʱ��SCK���������������ӻ�����
#define    P0_04_RD2         0x02                  //CAN-2������
#define    P0_04_CAP20       0x03                  //��ʱ��2�������룬ͨ��0

#define    P0_04_FNUC        P0_04_GPIO

//P0.05
#define    P0_05_GPIO        0x00                  //GPIO
#define    P0_05_I2SRX_WS    0x01                  //I2S���߽�����ѡ��WS���������������ӻ�����
#define    P0_05_TD2         0x02                  //CAN-2������
#define    P0_05_CAP21       0x03                  //��ʱ��2�������룬ͨ��1

#define    P0_05_FNUC        P0_05_GPIO

//P0.06
#define    P0_06_GPIO        0x00                  //GPIO
#define    P0_06_I2SRX_SDA   0x01                  //I2S���߽���������SD���ɷ�������������������ȡ
#define    P0_06_SSEL1       0x02                  //SSP-1���ߴӻ�ѡ��
#define    P0_06_MAT20       0x03                  //��ʱ��2ƥ�������ͨ��0

#define    P0_06_FNUC        P0_06_GPIO

//P0.07
#define    P0_07_GPIO        0x00                  //GPIO
#define    P0_07_I2STX_CLK   0x01                  //I2S���߷���ʱ����SCK���������������ӻ�����
#define    P0_07_SSCK1       0x02                  //SSP-1ʱ����
#define    P0_07_MAT21       0x03                  //��ʱ��2ƥ�������ͨ��1

#define    P0_07_FNUC        P0_07_GPIO

//P0.08
#define    P0_08_GPIO        0x00                  //GPIO
#define    P0_08_I2STX_WS    0x01                  //I2S���߷�����ѡ��WS���������������ӻ�����
#define    P0_08_SMISO1      0x02                  //SSP-1��������ӻ����������
#define    P0_08_MAT22       0x03                  //��ʱ��2ƥ�������ͨ��2

#define    P0_08_FNUC        P0_08_GPIO

//P0.09
#define    P0_09_GPIO        0x00                  //GPIO
#define    P0_09_I2STX_SDA   0x01                  //I2S���߷���������SD���ɷ�������������������ȡ
#define    P0_09_SMOSI1      0x02                  //SSP-1��������ӻ�����������
#define    P0_09_MAT23       0x03                  //��ʱ��2ƥ�������ͨ��3

#define    P0_09_FNUC        P0_09_GPIO

//P0.10
#define    P0_10_GPIO        0x00                  //GPIO
#define    P0_10_TXD2        0x01                  //UART2��������
#define    P0_10_SDA2        0x02                  //I2C-2�����ߣ��ǿ�©���ţ�
#define    P0_10_MAT30       0x03                  //��ʱ��3ƥ�������ͨ��0

#define    P0_10_FNUC        P0_10_GPIO

//P0.11
#define    P0_11_GPIO        0x00                  //GPIO
#define    P0_11_RXD2        0x01                  //UART2��������
#define    P0_11_SCL2        0x02                  //I2C-2ʱ���ߣ��ǿ�©���ţ�
#define    P0_11_MAT31       0x03                  //��ʱ��3ƥ�������ͨ��1

#define    P0_11_FNUC        P0_11_GPIO

//P0.12
#define    P0_12_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P0_12_SMISO1      0x02                  //SSP-1��������ӻ����������
#define    P0_12_AD06        0x03                  //ADC-0������6

#define    P0_12_FNUC        P0_12_GPIO

//P0.13
#define    P0_13_GPIO        0x00                  //GPIO
#define    P0_13_U2UP_LED    0x01                  //USB2 Good Link LEDָʾ����
#define    P0_13_SMOSI1      0x02                  //SSP-1��������ӻ�����������
#define    P0_13_AD07        0x03                  //ADC-0������7

#define    P0_13_FNUC        P0_13_GPIO

//P0.14
#define    P0_14_GPIO        0x00                  //GPIO
#define    P0_14_U2CONNECT   0x01                  //USB2�����ӿ���
//...      Reserved          0x02                  //����
#define    P0_14_SSEL1       0x03                  //SSP-1�ӻ�ѡ��

#define    P0_14_FNUC        P0_14_GPIO

//P0.15
#define    P0_15_GPIO        0x00                  //GPIO
#define    P0_15_TXD1        0x01                  //UART1��������
#define    P0_15_SSCK0       0x02                  //SSP-0ʱ����
#define    P0_15_SCK0        0x03                 //SPIʱ����

#define    P0_15_FNUC        P0_15_GPIO

//P0.16
#define    P0_16_GPIO        0x00                  //GPIO
#define    P0_16_RXD1        0x01                  //UART1��������
#define    P0_16_SSEL0       0x02                  //SSP-0�ӻ�ѡ��
#define    P0_16_SEL0        0x03                  //SPI�ӻ�ѡ��

#define    P0_16_FNUC        P0_16_GPIO

//P0.17
#define    P0_17_GPIO        0x00                  //GPIO
#define    P0_17_CTS1        0x01                  //UART1���㷢���ź�CTS
#define    P0_17_SMISO0      0x02                  //SSP-0��������ӻ����������
#define    P0_17_MISO0       0x03                  //SPI����ӳ�������

#define    P0_17_FNUC        P0_17_GPIO

//P0.18
#define    P0_18_GPIO        0x00                  //GPIO
#define    P0_18_DCD1        0x01                  //UART1�����ز��������DCD
#define    P0_18_SMOSI0      0x02                  //SSP-0��������ӻ�����������
#define    P0_18_MOSI0       0x03                  //SPI��������������

#define    P0_18_FNUC        P0_18_GPIO

//P0.19
#define    P0_19_GPIO        0x00                  //GPIO
#define    P0_19_DSR1        0x01                  //UART1�������þ���DSR
#define    P0_19_MCICLK      0x02                  //SD��MMC�ӿ�ʱ�������
#define    P0_19_SDA1        0x03                  //I2C-1�����ߣ��ǿ�©��

#define    P0_19_FNUC        P0_19_GPIO

//P0.20
#define    P0_20_GPIO        0x00                  //GPIO
#define    P0_20_DTR1        0x01                  //UART1�ն˾���DTR
#define    P0_20_MCICMD      0x02                  //SD��MMC�ӿ�������
#define    P0_20_SCL1        0x03                  //I2C-1ʱ���ߣ��ǿ�©��

#define    P0_20_FNUC        P0_20_GPIO

//P0.21
#define    P0_21_GPIO        0x00                  //GPIO
#define    P0_21_RI1         0x01                  //UART1�����ź�
#define    P0_21_MCIPWR      0x02                  //�ⲿSD��MMC��Դʹ��
#define    P0_21_RD1         0x03                  //CAN-1��������

#define    P0_21_FNUC        P0_21_GPIO

//P0.22
#define    P0_22_GPIO        0x00                  //GPIO
#define    P0_22_RTS1        0x01                  //UART1����������
#define    P0_22_MCIDAT0     0x02                  //SD��MMC�ӿ�������
#define    P0_22_TD1         0x03                  //CAN-1������

#define    P0_22_FNUC        P0_22_GPIO

//P0.23
#define    P0_23_GPIO        0x00                  //GPIO
#define    P0_23_AD00        0x01                  //ADC-0��ͨ��0
#define    P0_23_I2SRX_CLK   0x02                  //I2S����ʱ��SCK���������������ӻ�����
#define    P0_23_CAP30       0x03                  //��ʱ��3�������룬ͨ��0

#define    P0_23_FNUC        P0_23_GPIO

//P0.24
#define    P0_24_GPIO        0x00                  //GPIO
#define    P0_24_AD01        0x01                  //ADC-0��ͨ��1
#define    P0_24_I2SRX_WS    0x02                  //I2S������ѡ��WS���������������ӻ�����
#define    P0_24_CAP31       0x03                  //��ʱ��3�������룬ͨ��1

#define    P0_24_FNUC        P0_24_GPIO

//P0.25
#define    P0_25_GPIO        0x00                  //GPIO
#define    P0_25_AD02        0x01                  //ADC-0��ͨ��2
#define    P0_25_I2SRX_SDA   0x02                  //I2S��������SD���ɷ�������������������ȡ
#define    P0_25_TXD3        0x03                  //UART3��������

#define    P0_25_FNUC        P0_25_GPIO

//P0.26
#define    P0_26_GPIO        0x00                  //GPIO
#define    P0_26_AD03        0x01                  //ADC-0��ͨ��3
#define    P0_26_AOUT        0x02                  //DAC���
#define    P0_26_RXD3        0x03                  //UART-3��������

#define    P0_26_FNUC        P0_26_GPIO

//P0.31����LPC2378���У���LPC2364/6/8��Ϊ����λ��
#define    P0_31_GPIO        0x00                  //GPIO
#define    P0_31_U2D_ANODE   0x01                  //USB2˫��D+��
//...      Reserved          0x02                  //����
//...      Reserved          0x03

#define    P0_31_FNUC        P0_31_GPIO

/*********************************************************************************************************
                                            P1
*********************************************************************************************************/
//P1.19
#define    P1_19_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
//...      Reserved          0x02
#define    P1_19_CAP11       0x03                  //��ʱ��1�������룬ͨ��1

#define    P1_19_FNUC        P1_19_GPIO

//P1.20
#define    P1_20_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P1_20_PWM12       0x02                  //PWM-1�����ͨ��2
#define    P1_20_SSCK0        0x03                  //SSP-0ʱ��SCK

#define    P1_20_FNUC        P1_20_GPIO

//P1.21
#define    P1_21_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P1_21_PWM13       0x02                  //PWM-1�����ͨ��3
#define    P1_21_SSEL0       0x03                  //SSP-0�ӻ�ѡ��

#define    P1_21_FNUC        P1_21_GPIO

//P1.22
#define    P1_22_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
//...      Reserved          0x02
#define    P1_22_MAT10       0x03                  //��ʱ��1ƥ�������ͨ��0

#define    P1_22_FNUC        P1_22_GPIO

//P1.23
#define    P1_23_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P1_23_PWM14       0x02                  //PWM-1�����ͨ��4
#define    P1_23_SMISO0      0x03                  //SSP-0�������룬�ӻ����

#define    P1_23_FNUC        P1_23_GPIO

//P1.24
#define    P1_24_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P1_24_PWM15       0x02                  //PWM-1�����ͨ��5
#define    P1_24_SMOSI0      0x03                  //SSP-0����������ӻ�����

#define    P1_24_FNUC        P1_24_GPIO

//P1.25
#define    P1_25_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
//...      Reserved          0x02
#define    P1_25_MAT11       0x03                  //��ʱ��1ƥ�������ͨ��1

#define    P1_25_FNUC        P1_25_GPIO

//P1.26
#define    P1_26_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P1_26_PWM16       0x02                  //PWM-1�����ͨ��6
#define    P1_26_CAP00       0x03                  //��ʱ��0�������룬ͨ��0

#define    P1_26_FNUC        P1_26_GPIO

//P1.27
#define    P1_27_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
//...      Reserved          0x02
#define    P1_27_CAP01       0x03                  //��ʱ��0�������룬ͨ��1

#define    P1_27_FNUC        P1_27_GPIO

//P1.28
#define    P1_28_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P1_28_PCAP10      0x02                  //PWM-1�������룬ͨ��0
#define    P1_28_MAT00       0x03                  //��ʱ��0ƥ�������ͨ��0

#define    P1_28_FNUC        P1_28_GPIO

//P1.29
#define    P1_29_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P1_29_PCAP11      0x02                  //PWM-1�������룬ͨ��1
#define    P1_29_MAT01       0x03                  //��ʱ��0ƥ�������ͨ��1

#define    P1_29_FNUC        P1_29_GPIO

//P1.31
#define    P1_31_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P1_31_SSCK1       0x02                  //SSP-1ʱ����
#define    P1_31_AD05       (0x03U)                //ADC-0���룬ͨ��5

#define    P1_31_FNUC        P1_31_GPIO

/*********************************************************************************************************
                                            P2
*********************************************************************************************************/
//P2.00
#define    P2_00_GPIO        0x00                  //GPIO
#define    P2_00_PWM11       0x01                  //PWM-1��ͨ��1
#define    P2_00_TXD1        0x02                  //UART1���ݷ�������
#define    P2_00_TRACECLK    0x03                  //����ʱ��

#define    P2_00_FNUC        P2_00_GPIO

//P2.01
#define    P2_01_GPIO        0x00                  //GPIO
#define    P2_01_PWM12       0x01                  //PWM-1��ͨ��2
#define    P2_01_RXD1        0x02                  //UART1���ݽ�������
#define    P2_01_PIPESTAT0   0x03                  //��ˮ��״̬��λ0

#define    P2_01_FNUC        P2_01_GPIO

//P2.02
#define    P2_02_GPIO        0x00                  //GPIO
#define    P2_02_PWM13       0x01                  //PWM-1��ͨ��3
#define    P2_02_CTS1        0x02                  //UART1���㷢����
#define    P2_02_PIPESTAT1   0x03                  //��ˮ��״̬��λ1

#define    P2_02_FNUC        P2_02_GPIO

//P2.03
#define    P2_03_GPIO        0x00                  //GPIO
#define    P2_03_PWM14       0x01                  //PWM-1��ͨ��4
#define    P2_03_DCD1        0x02                  //UART1�����ز������
#define    P2_03_PIPESTAT2   0x03                  //��ˮ��״̬��λ2

#define    P2_03_FNUC        P2_03_GPIO

//P2.04
#define    P2_04_GPIO        0x00                  //GPIO
#define    P2_04_PWM15       0x01                  //PWM-1��ͨ��5
#define    P2_04_DSR1        0x02                  //UART1�������þ�����
#define    P2_04_TRACESYNC   0x03                  //����ͬ��

#define    P2_04_FNUC        P2_04_GPIO

//P2.05
#define    P2_05_GPIO        0x00                  //GPIO
#define    P2_05_PWM16       0x01                  //PWM-1��ͨ��6
#define    P2_05_DTR1        0x02                  //UART1�����ն˾�����
#define    P2_05_TRACEPKT0   0x03                  //���ٰ���λ0

#define    P2_05_FNUC        P2_05_GPIO

//P2.06
#define    P2_06_GPIO        0x00                  //GPIO
#define    P2_06_PCAP10      0x01                  //PWM-1�������룬ͨ��0
#define    P2_06_RI1         0x02                  //UART1��������
#define    P2_06_TRACEPKT1   0x03                  //���ٰ���λ1

#define    P2_06_FNUC        P2_06_GPIO

//P2.07
#define    P2_07_GPIO        0x00                  //GPIO
#define    P2_07_RD2         0x01                  //CAN-2������
#define    P2_07_RTS1        0x02                  //UART1���������
#define    P2_07_TRACEPKT2   0x03                  //���ٰ���λ2

#define    P2_07_FNUC        P2_07_GPIO

//P2.08
#define    P2_08_GPIO        0x00                  //GPIO
#define    P2_08_TD2         0x01                  //CAN-2������
#define    P2_08_TXD2        0x02                  //UART2��������
#define    P2_08_TRACEPKT3   0x03                  //���ٰ���λ3

#define    P2_08_FNUC        P2_08_GPIO

//P2.09
#define    P2_09_GPIO        0x00                  //GPIO
#define    P2_09_U1CONNECT   0x01                  //USB1�����ӿ���
#define    P2_09_RXD2        0x02                  //UART2��������
#define    P2_09_EXTIN0      0x03                  //�ⲿ����������

#define    P2_09_FNUC        P2_09_GPIO

//P2.10��ISP��
#define    P2_10_GPIO        0x00                  //GPIO
#define    P2_10_EINT0       0x01                  //�ⲿ�ж�0����
//...      Reserved          0x02                  //����
//...      Reserved          0x03

#define    P2_10_FNUC        P2_10_GPIO

//P2.11
#define    P2_11_GPIO        0x00                  //GPIO
#define    P2_11_EINT1       0x01                  //�ⲿ�ж�1����
#define    P2_11_MCIDAT1     0x02                  //SD��MMC�ӿ�������1
#define    P2_11_I2STX_CLK   0x03                  //I2S����ʱ��SCK���������������ӻ�����

#define    P2_11_FNUC        P2_11_GPIO

//P2.12
#define    P2_12_GPIO        0x00                  //GPIO
#define    P2_12_EINT2       0x01                  //�ⲿ�ж�2����
#define    P2_12_MCIDAT2     0x02                  //SD��MMC�ӿ�������2
#define    P2_12_I2STX_WS    0x03                  //I2S������ѡ��WS���������������ӻ�����

#define    P2_12_FNUC        P2_12_GPIO

//P2.13
#define    P2_13_GPIO        0x00                  //GPIO
#define    P2_13_EINT3       0x01
#define    P2_13_MCIDAT3     0x02                  //SD��MMC�ӿ�������3
#define    P2_13_I2STX_SDA   0x03                  //I2S��������SD���������������ӻ�����

#define    P2_13_FNUC        P2_13_GPIO

/*********************************************************************************************************
                                            P3
*********************************************************************************************************/
//P3.23
#define    P3_23_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P3_23_CAP00       0x02                  //��ʱ��0�������룬ͨ��0
#define    P3_23_PCAP10      0x03                  //PWM-1�������룬ͨ��0

#define    P3_23_FNUC        P3_23_GPIO

//P3.24
#define    P3_24_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P3_24_CAP01       0x02                  //��ʱ��0�������룬ͨ��1
#define    P3_24_PWM11       0x03                  //PWM-1�����ͨ��1

#define    P3_24_FNUC        P3_24_GPIO

//P3.25
#define    P3_25_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P3_25_MAT00       0x02                  //��ʱ��0ƥ�������ͨ��0
#define    P3_25_PWM12       0x03                  //PWM-1�����ͨ��2

#define    P3_25_FNUC        P3_25_GPIO

//P3.26
#define    P3_26_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P3_26_MAT01       0x02                  //��ʱ��0ƥ�������ͨ��1
#define    P3_26_PWM13       0x03                  //PWM-1�����ͨ��3

#define    P3_26_FNUC        P3_26_GPIO


/*********************************************************************************************************
                                            P4
*********************************************************************************************************/
//P4.28
#define    P4_28_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P4_28_MAT20       0x02                  //��ʱ��2ƥ�������ͨ��0
#define    P4_28_TXD3        0x03                  //UART3��������

#define    P4_28_FNUC        P4_28_GPIO

//P4.29
#define    P4_29_GPIO        0x00                  //GPIO
//...      Reserved          0x01                  //����
#define    P4_29_MAT21       0x02                  //��ʱ��2ƥ�������ͨ��1
#define    P4_29_RXD3        0x03                  //UART3��������

#define    P4_29_FNUC        P4_29_GPIO

/*********************************************************************************************************
                                            �����ڲ���������������
*********************************************************************************************************/
#define    PIN_PULL_UP       0x00                  //�����ڲ�����
#define    PIN_NO_PULL       0x02                  //���żȲ�������Ҳ������
#define    PIN_PULL_DOWN     0x03                  //�����ڲ�����

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
                                            ETM ����ӿ�
*********************************************************************************************************/
#define    ETM_DISABLE       0x00                                       /*  ��ֹETM����ӿ�             */

/*********************************************************************************************************
** ��������: void PinInit(void)
** ��������: ���ų�ʼ��
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��������ʼ�����е��������ã�����������ӡ���������������
*********************************************************************************************************/
extern void PinInit(void);

#endif                                                                  /*  __LPC2300PINCFG_H           */

/*********************************************************************************************************
    End Of File
*********************************************************************************************************/
