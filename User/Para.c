/*
	Para.c
	vFly项目公共变量定义文件
	BUAA 2009-2-27
	张以成
*/
#include "Config.h"
#include "FCFrame.h"
#include "INSFrame.h"
//#include "PWM.h"
//#include "FCA.h"
//#include "FCA2.h"
//#include "Init.h"
#include "Main.h"
//==================================================
//FCA2.c
//FCA算法输出阵
volatile fp32 FCAOutData[16][3]={0};//CHANGE:与FCA协议0x50~5F个数同步
/*
00 位置控制输出量	3	ud, vd, wd
01 航向控制输出量	1	rd

02 算法舵量1	2	A1s; B1s; At；
03 算法舵量2	3	Am；Ay

04 遥控器舵量1	3	A1s，B1s，At
05 遥控器舵量2	2	Am，Ay

06 遥控器舵量3	2	转速控制器输出；陀螺仪输出；

07 导航系位置	3	直接测量：转换为平面坐标
08 导航系速度	3	直接测量
09 导航系姿态	3	直接测量
10 本体系角速率	3	直接测量
11 本体系位置1	3	需要转化：校正航向角；
12 本体系速度1	3	需要转化1（校正航向角）2（校正姿态角）
		
13 指向目标位置	3	算法参考量
14 指向目标速度	3	算法参考量
15 指向目标姿态		算法参考量
*/

//FCA悬停点阵
volatile fp32 FCAMedianData[6][3]={0};//CHANGE:与FCA协议0x60~6F个数同步
/*
00 悬停位置		3	x，y，z
01 悬停姿态		3	θ，φ，ψ
02 悬停速度		3	u, v, w
03 悬停角速度	3	p, q, r
04 悬停舵量1	3	A1s，B1s，At
05 悬停舵量2	2	Am Ay

*/

//FCA算法目标阵
volatile fp32 FCATarData[5][3]={0};//CHANGE:与FCA协议0x70~7F个数同步
/*
00 位置	3	本体系相对位置
01 速度	3	本体系速度（纠正航向角）
02 姿态	3	导航系姿态
03 混控比例1
04 混控比例2
*/

//FCA系数阵
volatile fp32 FCAKData[14][3]={0};//CHANGE:与FCA协议0x80~0x8F同步
/*
00 水平位置通道x1	3	比例；（微分；）积分； kpx, kix
01 水平位置通道x2	3	区域直径，区域衰减；速度限值xmax, kxmax, umax
02 水平位置通道y1	3	比例；（微分；）积分；kpy, kiy
03 水平位置通道y2	3	区域直径，区域衰减；速度限值ymax, kymax, vmax
04 水平速度姿态x1	3	比例2个，微分1个kpu, kpp; kdp
05 水平速度姿态x2	3	（积分1个）；耦合参数2个；kiu, kou, kop
06 水平速度姿态y1	3	比例2个，微分1个kpv, kpr, kdr
07 水平速度姿态y2	3	（积分1个）；耦合参数2个；kiv, kov, kor

08 垂直航向通道1	2	比例，积分；kpay, kiay
09 垂直航向通道2	3	区域直径，区域衰减；速度限值aymax, kaymax, armax
10 垂直航向速率3	2	比例，微分kpar, kdar
11 垂直高度通道1	2	比例；积分；kph, kih
12 垂直高度通道2	3	区域直径，区域衰减；速度限值hmax, khmax, wmax
13 垂直高度通道3	2	比例，微分kpw, kdw
*/

//==================================================
//FCA.c
//FCA数据帧
volatile fp32 FCAData[19][3]={0};//CHANGE:与FCA协议个数同步
/*
00:姿态中位*3 
01:速度中位*3 
02:角速度中位*3 
03:控制量中位*3 
04:算法控制量*3 
05:遥控控制量*3 
06:输出控制量*3 
07:控制系数K1-3 
08:控制系数K4-6 
09:控制系数K7-9 
10:控制系数K10-12 
11:混控比例*3 
12:目标位置X+目标位置Y 

13:速度*3 (本体系)*
14:角速度*3 int16 AngVeloBodyX;//0.01°/s int16 AngVeloBodyY;//0.01°/s int16 AngVeloBodyZ;//0.01°/s
15:角度*3 int16 Pitch;//0.1°int16 Roll;//0.1°int16 Yaw;//0.1°
16:位置*3 int32 Lo;// 1e-7 degree int32 La;// 1e-7 degree int32 Height;// millmeter
17:角度增量*3 *
18:角速度增量*3 *
*/

//==================================================
//FCFrame.c
//FC统计数据
volatile uint32 FCBlockError=0;
//解析后的帧头
volatile FCFrameStruct FCFrameObj;
//On BlockNum=0
//收集中位数据的标志位
volatile uint8 CollectMedianDataState;
volatile uint8 CollectMedianPosYaw;
//For F
//回发标志位,0-255代表最多256个协,0不使用.
volatile uint8 DataSendFlag[256]={0};//0-2位分别代表单次回发，连续回发，数据记录
//For 1
//需要下载的数据长度
volatile uint32 DataToDownload=0;

//==================================================
//INSFrame.c
//INS统计数据
volatile uint32 INSFrameError=0;
//INS帧对象
volatile INSFrameStruct INSFrameObj;

//==================================================
//Main.c
volatile uint8 RunState=0;//运行状态
volatile uint8 Do=DoNothing;//执行操作
volatile uint8 DataSendDF=4;//数据发送分频
volatile uint8 DataRecDF=4;//数据记录分频
volatile uint8 ErrorNum=0;//错误计数
volatile uint8 LastErrorNo=0;//最后一个发生的错误编号
volatile uint8 LastErrorPara[3];//最后一个发生的错误参数
volatile uint32 MainLoopRunCounter=0;//主循环计数器
volatile uint8 PreAutoMode=MODETarget0;//预定的自驾模式


//==================================================
//PWM.c
volatile float PWMIn[9]={0};//-1~-1
volatile float PWMOut[9]={0};//-1~1
