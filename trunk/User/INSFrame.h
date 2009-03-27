#ifndef __INS_H
#define __INS_H

#define INSFrameS0 0xA5
#define INSFrameS1 0x5A
#define INSFrameS3 0xF1
#define INSFrameLength 54

//定义INS帧结构(修改自蒲立的相关文件,__packed表示取消对齐)
__packed struct _INSFrameStruct
{
	uint8 S1;
	uint8 S2;
	uint8 Length;
	uint8 Command;
	uint8 Status;
	
	int16 Pitch;//0.1°
	int16 Roll;//0.1°
	int16 Yaw;//0.1°
	int16 SpeedN;//0.01m/s
	int16 SpeedE;//0.01m/s
	int16 SpeedD;//0.01m/s
	
	int16 SpeedBodyX;//0.01m/s
	int16 SpeedBodyY;//0.01m/s
	int16 SpeedBodyZ;//0.01m/s
	
	int32 Lo;// 1e-7 degree
	int32 La;// 1e-7 degree
	int32 Height;// millmeter

	uint8 SatNumber;
	int16 GPSPrecision;//0.01m
	uint8 GPSHour;
	uint8 GPSMinute;
	uint8 GPSSecond;
	
	int16 AcclBodyX;//0.001m/s^2
	int16 AcclBodyY;//0.001m/s^2
	int16 AcclBodyZ;//0.001m/s^2
	int16 AngVeloBodyX;//0.01°/s
	int16 AngVeloBodyY;//0.01°/s
	int16 AngVeloBodyZ;//0.01°/s
	
	uint8 CheckSum;
};
typedef struct _INSFrameStruct INSFrameStruct;

void INS(void);

#endif
