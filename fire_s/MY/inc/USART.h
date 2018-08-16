#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x_it.h"

typedef struct
{
	u16 rdPtr;    // 读指针
	u8 rdEnabled;    // 能读否
	u16 rdNum;    // 需读字节数 0 - 无限
	u8 rdEnded;    //0- 未完成 1- 完成
	uint16_t rdBuff[300];    // 读缓存   
	u16 wrPtr;    // 写指针   
	u8 wrEnabled;    // 能写否
	u16 wrNum;    // 需写字节数 0 - 无限
	u8 wrEnded;    //0- 未完成 1- 完成
	uint16_t wrBuff[50];    // 写缓存 
} USART_DATA;

typedef struct
{
  USART_TypeDef *USARTPORT; // UASRT1-5
	u8 Enabled;  // 0 - 不用 1- 用
	USART_InitTypeDef Config;
} USART_CONFIG;


// Type of the Board
#define NOBOARD 0
#define IOBOARD 1          //回路板  有240点位
#define POWERBOARD 2       //电源板子
#define FIREBOARD 3        //灭火板 每板2路
#define MULTILINEBOARD 4   //多线盘  每板12路
#define UNKNOWNBOARD 0xff


typedef struct
{
  char Name[2]; // Name of the board defalu is the NO. "1"--"70"
	u8 Type;  // the type of Board
	FunctionalState State;  // the state of Board to Flash 回路板（短路 通讯故障 掉线 使能）  
} INTERNAL_BOARD;

typedef struct
{
  u8 bNum;
	u8 nodeNum;
	u8 stat;
} IOBOARDNEWSTAT;


typedef struct
{
  u8 bNum;
	u8 stat1;
	u8 stat2;
} MLBOARDNEWSTAT;

typedef struct
{
  u8 mNum;  //主机号 1-31 目前1-2
	u8 bNum;  //板子编号 1-70
	u8 dNum;  //点位编号 1-240
	u8 dType; //点位类型
	u8 eventType; // 事件类型 1-8
	u8 ioType; //类型，可转汉字  250:表示火警  可扩展到100种
	u8 year;
	u8 mon;
	u8 day;
	u8 hour;
	u8 min;
	u8 sec;
	u8 stat;  //bit0: 是否发送给主机过
} IOSTAT;   //回路板 可能有5000  14B

typedef struct
{
  u8 type;  //类型 故障 报警 火警
	u8 bNum;  //板子编号 1-70
	u8 dNum;  //点位编号 1-240
	u8 eventType; // 事件类型 1-8
	u8 ioType; //类型，可转汉字  250:表示火警  可扩展到100种
} VOICEIO;   //回路板 可能有5000  14B

typedef struct
{
	u16 validConf;       //设定值为 0x55AA表示已有配置 2B
	USART_CONFIG usart1_config,usart2_config,usart3_config,usart4_config,usart5_config; //122B  size=124
} CONFIGURATION0;

typedef struct
{
	u16 serialTW;        // 2B
	u16 machineID;       // 4B
	u8 machineVersion;   // 5B
	u8 mNum;             // 6B
	u8 mRunningStat;     // 7B    
	u8 mAutomation;      // 8B
  u8 password[10],suPassword[10]; //28B
	u8 enPrint;         // 29B
	u8 isFullFireB;				//30
	u8 isFullErrB;				//31
	u8 isFullIOB;					//32
	u8 isFullPingBiB;			//33
	u8 isFullActB;				//34
	u8 voiceConf1[30];    //64
	u8 voiceConf2[30];		//94
	u8 maxMac;            //95  //丛机数量
	
} CONFIGURATION1;

typedef struct
{
  u8 type;          //节点类型
	u8 stat; 					//屏蔽状态 stat.7 0 未屏蔽 1 屏蔽   stat.10 灵敏度 0-3
	u16 area;         //区号 4位BCD码
	u8 addr[16];  		//位置，8个汉字
	u8 year;
	u8 mon;
	u8 day;
	u8 hour;
	u8 min;
	u8 sec;
} NODESTRUCT;   //节点结构 22B*240=5280B/128= 41.25 42BLOCK

void Init5USART(u8 num);  //0 for all 5 USART 1-5 for single USART
void IBSendCmd(u8 bNum, u8 type, u8 step);
void DealFinishRead(u8 port);
void DealFinishSend(u8 port);
void DealPowerStat(u8 bNum, u8 status);
void DealIOStat(u8 bNum, USART_DATA *usart_data);
void DealMLStat(u8 bNum, USART_DATA *usart_data);
void DealFireStat(u8 bNum, USART_DATA *usart_data);
void DealUSART(u8 port,USART_DATA *usart_data, uint16_t rddata,u16 sr);
void SendUSART(u8 port,USART_TypeDef* USARTx,USART_DATA *usart_data);
void moveNextIB(u8 cont, u8 confByte);
void testUSART1(void);
void testUSART3(void);
void testUSART4(void);
void testUSART5(void);

#endif /* _USART_H_ */

