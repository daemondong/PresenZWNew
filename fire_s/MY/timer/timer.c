/*-------------------------------------------------------------------------------
文件名称：timer.c
文件描述：通用定时器配置        
备    注：无
---------------------------------------------------------------------------------*/
#include "timer.h"
#include "USART.h"
#include "GloablVar.h"

void LCDFlash_Off(void);
void LCDFlash_On(void);


#define LEDFLASH 5
#define LCDFLASH 7
u8 tim2_1000ms;
u8 tim2_8s;
u8 tim2_100ms;


/*-------------------------------------------------------------------------------
程序名称：TIM2_Base_Init
程序描述：定时器TIM2通用定时功能
输入参数：定时器重装值
返回参数：无
备    注：无
---------------------------------------------------------------------------------*/
void TIM2_Base_Init(u16 Count)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = Count-1;  //重装值，
	TIM_TimeBaseStructure.TIM_Prescaler =720-1; //分频系数，72M/7200=100KHz,其他依此类推
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);     //把上述数值写入对应寄存器
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);            //使能或者失能指定的TIM中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;            //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                 //把上述数值写入对应寄存

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
							 
}

extern u32 secCount;
extern IOSTAT  errStat[1200] ;   //共5类 故障

void TIM2_Int(void) {
	
	if (timeout2 != 0xff && timeout2 > 0) { 
		timeout2 -- ; 
		if (timeout2==0) {
			timeout2=0xff;
			RESETBIT(is2Sended);
		}
	}
	
	if (keyVoice>0) {
		keyVoice--;
		if (keyVoice==0) KEYVOICE_OFF;
	}
	
	if (ISSET(hSEnable)) {
		hSound++;
		if (hSound<HSOUNDDELAY) SOUND_ON;
    else SOUND_OFF;
    if (hSound> 2* HSOUNDDELAY) hSound=0;
		SILENTOFF;
	}
	else if (ISSET(mSEnable)) {
		mSound++;
		if (mSound<MSOUNDDELAY) SOUND_ON;
    else SOUND_OFF;
    if (mSound> 2* MSOUNDDELAY) mSound=0; 		
		SILENTOFF;
	}
	else if (ISSET(lSEnable)) {
		lSound++;
		if (lSound<LSOUNDDELAY) SOUND_ON;
    else SOUND_OFF;
    if (lSound> 2* LSOUNDDELAY) lSound=0; 		
		SILENTOFF;
	}
	else SOUND_OFF;

	tim2_100ms++;

  if (tim2_100ms % 2 == 0 ) SETBIT(Scankey);
	
	if (tim2_100ms>=10) {
		if (stopkeytime>0) stopkeytime--;
		tim2_100ms=0;
		tim2_1000ms++;
		//	if (tim2_1000ms>=LEDFLASH) SYSLEDOFF;
		if (tim2_1000ms>=LCDFLASH) LCDFlash_Off();
		if (tim2_1000ms>=10) {
			tim2_1000ms=0;
//			SYSLEDON;
			LCDFlash_On();
			secCount ++;
			SETBIT(ifGetTime);
			loopIO = 1;
			SETBIT(loopU3);

			tim2_8s++;
			if (tim2_8s==4) { flashstat=1; flashLine=0x0F; }
			if (tim2_8s>=8) { tim2_8s =0 ;flashstat=0; flashLine=0x0F;
/*				if (ISRESET(IsWindow)) {
					SETBIT(IsWindow);
					windowNum=0x22;
					windowSelect=0x10;
					RESETBIT(IsWindowShowed);
				}
*/			}
			
			if (tim2_8s == 0 && isComputer == 0) testUSART1();
			
			
		if (bonTime>0) {
				bonTime--;
//				if (bonTime==0) LCD_BOFF;
			}
			if (setupTime>0) {
				setupTime--;
//				if (setupTime==0) Back_to_Screen();
			}
			if (clearLastTime>0) {
				clearLastTime--;
				if (clearLastTime==0) dispLine=4;
			}
						
			if (windowTime>0) {
				windowTime--;
				if (windowTime==0) RESETBIT(IsWindow);
			}
			if (selfTesting>0) {
				selfTesting--;
				if (selfTesting==0) { 
					LedNew05=storeLed05;LedNew06=storeLed06;
					KEYVOICE_OFF; SOUND_OFF;
					dispLine=4;
					nowLine[4] =7;
					if (MenuDeep>0) MenuDeep--;
//					SetOneLineMenu(1);
				}
			}
		}
	}
	
	if (selfTesting>0) { SOUND_ON; KEYVOICE_ON;}
}

char chrHex(u8 n)
{
	if (n<=9) return (n+'0');
	else return (n-10+'A');
}
void strHex(u8 *strp, u8 n)
{
	*strp = chrHex(n / 16);
	*(strp+1) = chrHex(n % 16);
}


/*----------------------德飞莱 技术论坛：www.doflye.net--------------------------*/
