#ifdef   OS_GLOBALS
   
#define  OS_EXT  
   
#else  
   
#define  OS_EXT extern  
   
#endif

#include <time.h>

// State of CPU
#define STOPPED 0 
#define INITING 1
#define DEBUGING 2
#define RUNNING 3
#define RESETTING 4

// TimeOut for USART
#define NORMALTIMEOUT 10  //100ms 通讯超时
#define LONGTIMEOUT 50  //500ms 发送命令通讯超时

// Max of board
#define MAXNUMBOARD 130


#define MAXDISPLINE 4

//List of Key Value
#ifdef  OLD_KEY
	#define KEYUP 0x18
	#define KEYDOWN 0x1B
	#define KEYLEFT 0x19
	#define KEYRIGHT 0x1C
	#define KEYTAB 0x1A
	#define KEYSILENT 0x10
	#define KEYRESET 0x08
	#define KEYVOICE 0x00
	#define KEY01 0x11
	#define KEY02 0x12
	#define KEY03 0x13
	#define KEY04 0x09
	#define KEY05 0x0A
	#define KEY06 0x0B
	#define KEY07 0x01
	#define KEY08 0x02
	#define KEY09 0x03
	#define KEYESC 0x14
	#define KEY00 0x0C
	#define KEYOK 0x04
#else
//List of Key Value
	#define KEYUP 0x04
	#define KEYDOWN 0x00
	#define KEYLEFT 0x03
	#define KEYRIGHT 0x01
	#define KEYTAB 0x02
	#define KEYSILENT 0x1C
	#define KEYRESET 0x14
	#define KEYVOICE 0x0C
	#define KEY01 0x1B
	#define KEY02 0x1A
	#define KEY03 0x19
	#define KEY04 0x13
	#define KEY05 0x12
	#define KEY06 0x11
	#define KEY07 0x0B
	#define KEY08 0x0A
	#define KEY09 0x09
	#define KEYESC 0x18
	#define KEY00 0x08
	#define KEYOK 0x10
#endif
//常数设置
#define MAXMACHINENUM 10
#define MAXIOBOARD 1		//最大接口板数
#define STOPKEYTIME 4		//连续按键需大于400ms
#define SELFTESTTIME 3		//自检时间 3s

// Public Functions
void FSMC_NOR_EraseChip(void);
void FSMC_NOR_EraseBlock(u32 BlockAddr);
void FSMC_NOR_Write(u32 WriteAddr, u8 Data);
void FSMC_NOR_ReadBuffer(u8* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead);
void FSMC_NOR_WriteBuffer(u8* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead);
void strHex(u8 *strp, u8 n);
void ShowString8 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod);
void showMessage(u8 tt, u8 tl);
void ShowString816 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod);
void ShowStringPosi8 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod, u8 posi);
u8 get_dat(void);
char ZLG7289_Key(void);
void Time_SetCalendarTime(struct tm t);
u16 str2Dec(u8 *str,u8 len);
u16 str2BCD(u8 *str,u8 len);
void SetOneLineMenu(u8 tt);
void SetTwoChoiceMenu(u8 tt,u8 nc);
void printRec(u8 tt, IOSTAT *rec);
char *dotType(u8 dt,u8 t,u8 type);
char *typeString(u8 dt,u8 t);
char *typeFullString(u8 dt,u8 t);
void sendU3(u16 *buf,u8 len,u8 type);

void Back_to_Screen(void);
void WriteConf(void);
void EraseFlashBlocks(u32 Startaddr,u16 num);
u8 AddFlashRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem, u16 MaxBlocks);
u8 GetLastFlashRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem, u16 MaxBlocks, u8 isFull);
u8 GetPreFlashRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem, u16 MaxBlocks);
u8 GetNextFlashRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem, u16 MaxBlocks);
void ReadFlashRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem, u16 RecordNum);
void WriteFlashFixRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem,u16 RecordNum);
void ReadFlashFixRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem,u16 RecordNum);
void cpChinese2Str(u8 *ptr, u8 *cptr, u8 ll);
void ZLG7289_Download(u8 mod, char x, u8 dp, char dat);
void ShowStringPosi16 (u8 ll, u8 rr, u8 *Char, u8 len, u8 posi);
void ShowString16 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod);
void Disp_ClearLine(u8 ll,u8 nl,u8 rr, u8 nr);
void Disp_Line(u8 LineType,u8 LineNum);

u8 SetioStat(u8 mn,u8 bn, u8 dn, u8 it, u8 et);
u8 SeterrStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et);
u8 SetfireStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et);
u8 SetpinbiStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et);
u8 SetactStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et);

u8 DelerrStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et);
u8 DelioStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et);
u8 DelfireStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et);
u8 DelpinbiStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et);
u8 DelactStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et);

void StartRcv(USART_DATA *usart_data, u8 n);
void StartSend(USART_DATA *usart_data, u8 n);
u8 DealGetStat(void);
void AddRecoverEvt(u8 tt,u8 bn,u8 dn,u8 it,u8 et);

void Disp_DrawRect(u8 x, u8 y, u8 x1, u8 y1, u8 m);
void ShowWindow(u8 wn,u8 m);


//位定义
OS_EXT u8 BitStat01;
#define hSEnable_BYTE BitStat01
#define hSEnable_BIT 0
#define mSEnable_BYTE BitStat01
#define mSEnable_BIT 1
#define lSEnable_BYTE BitStat01
#define lSEnable_BIT 2
#define loopU3_BYTE BitStat01
#define loopU3_BIT 3
#define Scankey_BYTE BitStat01
#define Scankey_BIT 4
#define voiceStat_BYTE BitStat01
#define voiceStat_BIT 5
#define ifGetTime_BYTE BitStat01
#define ifGetTime_BIT 6
#define needAction_BYTE BitStat01
#define needAction_BIT 7

OS_EXT u8 BitStat02;
#define needConfirm_BYTE BitStat02
#define needConfirm_BIT 0
#define is2Sended_BYTE BitStat02
#define is2Sended_BIT 1
#define queryPowerVlot_BYTE BitStat02
#define queryPowerVlot_BIT 2
#define U3StatDealing_BYTE BitStat02
#define U3StatDealing_BIT 3
#define IsWindow_BYTE BitStat02
#define IsWindow_BIT 4
#define IsWindowShowed_BYTE BitStat02
#define IsWindowShowed_BIT 5




#define SETBIT(x) x##_BYTE |= (1<<x##_BIT)
#define REVBIT(x) x##_BYTE ^= (1<<x##_BIT)
#define RESETBIT(x) x##_BYTE &= ~(1<<x##_BIT) 
#define ISSET(x) (x##_BYTE & (1<<x##_BIT))>0 
#define ISRESET(x) (x##_BYTE & (1<<x##_BIT))==0 


OS_EXT USART_DATA usart1_data,usart2_data,usart3_data,usart4_data,usart5_data;
OS_EXT u8 timeout1,timeout2,timeout3,timeout4,timeout5,timesSend;
OS_EXT INTERNAL_BOARD InterBoard[MAXNUMBOARD];
OS_EXT u8 ptrIB,testByte0,testByte1;
OS_EXT volatile u8 stepIBSend;
OS_EXT u8 ifDealUSART[6],tmpPassword[10];
OS_EXT volatile u8 initIOB, initOver, loopIO;
OS_EXT u8 USART3Status,myStat1,myStat2;			//从机通讯状态及工作状态
OS_EXT u8 USART3MainCmd,USART3MainMac,USART3MainStat,StatStep;			//主机命令
OS_EXT u16 GetStat;
OS_EXT u8 selfTesting;
OS_EXT NODESTRUCT oneNode;




//报警音
OS_EXT u8 hSound,mSound,lSound;
//OS_EXT u8 hSEnable,mSEnable,lSEnable;

//Power Board
OS_EXT u8 Led05, LedNew05, storeLed05;
OS_EXT u8 Led06, LedNew06, storeLed06;

//Power Board
OS_EXT u8 oldStatPB;
OS_EXT u8 newStatPB;

//IO Board
OS_EXT u8 oldStatIOB[2][240];
OS_EXT u8 IOBSetting[2][240];
OS_EXT IOBOARDNEWSTAT newStatIOB[1000];
OS_EXT u16 wrPtrNewIO;
OS_EXT u16 rdPtrNewIO;

//ML Board
OS_EXT u8 oldStatMLB[30][2];
OS_EXT MLBOARDNEWSTAT newStatMLB[1000];
OS_EXT u16 wrPtrNewML;
OS_EXT u16 rdPtrNewML;

//Fire Board
OS_EXT u8 oldStatFireB[2];
OS_EXT u8 newStatFireB[2];
OS_EXT u8 mainVlot,backVlot;

//TuWen and Computer USART
OS_EXT 	u8 stateCPU;
OS_EXT u8 isComputer; // 0 - no test; 1 - Computer; 2 - TuWen OK; 3 - TuWen need data;
OS_EXT struct tm machineDate;
OS_EXT CONFIGURATION0 myConf;
OS_EXT CONFIGURATION1 myConf1;

// KeyBoard
OS_EXT u8 keyValue;
OS_EXT u8 keyCount;
OS_EXT u8 keyVoice,stopkeytime;

// LCD Screen
OS_EXT u8 windowNum,windowSelect,windowTc,windowTime;

OS_EXT u8 newTime;
OS_EXT u8 oldScr,newScr,bonTime,setupTime;
OS_EXT u8 oldTab,newTab;
OS_EXT u8 oldLine,newLine,updateLine,dispLine;
OS_EXT u8 OneLineMenuItem[10][15];     //菜单选项
OS_EXT u8 OneLineMenuSize[10];         //菜单选项长度
OS_EXT u8 MenuTitle[3][10],MenuTLen[3],MenuDeep;							//菜单标题
OS_EXT u8 OneLineNowMenu;							//当前所用菜单
OS_EXT u8 OneLineMenuMax;							//菜单选项个数
OS_EXT u8 OneLineMenuSelect;						//菜单当前项
OS_EXT u8 TwoChoiceNowMenu;							//当前所用两选一菜单
OS_EXT u8 TwoChoiceMenuSelect;						//两选一菜单菜单当前项
OS_EXT u8 MultiLineMenuSelect,subLine,subLine1;						//全屏菜单当前项
OS_EXT u8 nowLine[5],lineMod[5],oneLineMenu,setupSubMenu;
//LineMod:  0位：表示是否焦点 1位表示是否多行轮显  2位表示是否可获取焦点
OS_EXT u8 twoChoice, passwordIn; //两选一
OS_EXT u8 inputStr[50],inputChinese[16],inputPosi,inputMode,inputMax;
OS_EXT u8 inputMLStr[4][30],inputMLPosi[4],inputMLLine;
OS_EXT u8 flashLine, flashstat;
OS_EXT u16 totBJ,dispBJ,wrBJ;
OS_EXT u16 totFire,dispFire,wrFire;
OS_EXT u16 totPinbi,dispPinbi,wrPinbi;
OS_EXT u16 totErr,dispErr,wrErr;
OS_EXT u16 totAct,dispAct,wrAct;
OS_EXT u8 totVoice,wrVoice;
OS_EXT u8 totRecoverEvt;
//点位动作
//OS_EXT u8 voiceStat;
OS_EXT u8 actionBit[30];
// DateTime
OS_EXT u8 inputu8,clearLastTime;

//记录存储
OS_EXT u8 nowRecPosi,isFindRec,nowRecType;
OS_EXT u16 nowRecBlk,totRec,disRec;
OS_EXT IOSTAT nowRec;

//主从机设置
OS_EXT u8 printNow[MAXMACHINENUM];  //打印设置
OS_EXT u8 runnStat[MAXMACHINENUM];	 //运行状态设置
OS_EXT u8 startQmNum,sendStep;


//点位统计
OS_EXT u8 typeTot[10],typePingbi[10];

//时间设置
#define BACKONTIME 10       //背光10秒
#define SETUPTIMEOUT 30       //设置30秒无按键超时
#define KEYVOICETIME 20     //按键响200ms
#define HSOUNDDELAY 25			//2Hz
#define MSOUNDDELAY 50			//1Hz
#define LSOUNDDELAY 100   //0.5Hz 

//#define SYSLEDON LedNew05 |= 0x01
//#define SYSLEDOFF LedNew05 &= 0xFE

//#define SYSLED1ON LedNew06 |= 0x01
//#define SYSLED1OFF LedNew06 &= 0xFE

#define U2_SEND GPIO_SetBits(GPIOE,GPIO_Pin_2)
#define U2_RCV GPIO_ResetBits(GPIOE,GPIO_Pin_2)
#define U3_SEND GPIO_SetBits(GPIOF,GPIO_Pin_7)
#define U3_RCV GPIO_ResetBits(GPIOF,GPIO_Pin_7)
#define LCD_BON GPIO_SetBits(GPIOE,GPIO_Pin_3)
#define LCD_BOFF GPIO_ResetBits(GPIOE,GPIO_Pin_3)
#define KEYVOICE_ON GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define KEYVOICE_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define SOUND_ON GPIO_SetBits(GPIOE,GPIO_Pin_4)
#define SOUND_OFF GPIO_ResetBits(GPIOE,GPIO_Pin_4)

#define LIGHT_REV GPIO_WriteBit(GPIOE, GPIO_Pin_3,(BitAction)(1-(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_3))))

#define GASERRORON LedNew05 |= 0x01
#define GASERROROFF LedNew05 &= 0xFE
#define PINBION LedNew05 |= 0x02
#define PINBIOFF LedNew05 &= 0xFD
//#define AUTOON LedNew05 |= 0x08
//#define AUTOOFF LedNew05 &= 0xF7
#define AUTOON LedNew06 |= 0x08
#define AUTOOFF LedNew06 &= 0xF7
#define DELAYON LedNew05 |= 0x10
#define DELAYOFF LedNew05 &= 0xEF
#define SILENTON LedNew05 |= 0x08
#define SILENTOFF LedNew05 &= 0xF7
#define ERRORON LedNew05 |= 0x40
#define ERROROFF LedNew05 &= 0xBF

//#define STARTON LedNew06 |= 0x01
//#define STARTOFF LedNew06 &= 0xFE
#define STARTON LedNew06 |= 0x02
#define STARTOFF LedNew06 &= 0xFD
#define RETURNON LedNew06 |= 0x02
#define RETURNOFF LedNew06 &= 0xFD
#define ALARMON LedNew06 |= 0x04
#define ALARMOFF LedNew06 &= 0xFB
//#define VOICELIGHTON LedNew06 |= 0x08
//#define VOICELIGHTOFF LedNew06 &= 0xF7
#define VOICELIGHTON LedNew06 |= 0x04
#define VOICELIGHTOFF LedNew06 &= 0xFB
#define MAINPOWERON LedNew06 |= 0x10
#define MAINPOWEROFF LedNew06 &= 0xEF
#define MAINPOWERERROR LedNew05 |= 0x10
#define MAINPOWERERROROFF LedNew05 &= 0xEF
#define BACKPOWERON LedNew06 |= 0x20
#define BACKPOWEROFF LedNew06 &= 0xDF
#define BACKPOWERERROR LedNew05 |= 0x20
#define BACKPOWERERROROFF LedNew05 &= 0xDF
#define FIREON LedNew06 |= 0x40
#define FIREOFF LedNew06 &= 0xBF


#define MAINPOWER(A) LedNew05 = A

#define STARTBLOCK 0x40000					       //存储基本配置 2*128

#define NODEBLOCK 0x40500					 		//存储点位配置 26B*240=6240B/128= 48 BLOCK

#define FIREBLOCK 0x42500		 					//存储火警记录 14B*1500=21000B/128= 164.06 165BLOCK 21120B
#define MAXFIREBLOCKS		150						//20480B

#define ERRBLOCK 0x47500		 					//存储故障记录 14B*1500=21000B/128= 164.06 165BLOCK
#define MAXERRBLOCKS		150
//#define MAXERRBLOCKS		40

#define IOBLOCK 0x4C500		 					//存储报警记录 14B*1500=21000B/128= 164.06 165BLOCK
#define MAXIOBLOCKS		150
//#define MAXERRBLOCKS		40

#define ENGLISH

