/*
	74LV138.h
	74LV138ƬѡоƬ���������
	�����޸�ֻҪ�ı�A0-2����Ŷ��弴��
	BUAA 2008-7-16
	���Գ�
*/
#ifndef __74LV128_H
#define __74LV128_H

//�������Ŷ���
#define A0 P0_23
#define A1 P0_24
#define A2 P0_29

#define SelectY0 IO0DIR|=0x61800000;IO0CLR=0x21800000;//GpioClrLOW(A0);GpioClrLOW(A1);GpioClrLOW(A2)//ǿ�Ƶ������Ż�
#define SelectY1 GpioSetLOW(A0);GpioClrLOW(A1);GpioClrLOW(A2)
#define SelectY2 GpioClrLOW(A0);GpioSetLOW(A1);GpioClrLOW(A2)
#define SelectY3 GpioSetLOW(A0);GpioSetLOW(A1);GpioClrLOW(A2)
#define SelectY4 GpioClrLOW(A0);GpioClrLOW(A1);GpioSetLOW(A2)
#define SelectY5 GpioSetLOW(A0);GpioClrLOW(A1);GpioSetLOW(A2)
#define SelectY6 IO0CLR=0x800000;IO0SET=0x21000000;//GpioClrLOW(A0);GpioSetLOW(A1);GpioSetLOW(A2)//ǿ�Ƶ������Ż�
#define SelectY7 GpioSetLOW(A0);GpioSetLOW(A1);GpioSetLOW(A2)

#endif

