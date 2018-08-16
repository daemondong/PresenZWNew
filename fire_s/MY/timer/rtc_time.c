/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "rtc_time.h"
#include "delay.h"
/* Private function prototypes -----------------------------------------------*/
void Time_Set(u32 t);
/* Private functions ---------------------------------------------------------*/

u32 secCount;
/*******************************************************************************
* Function Name  : Time_ConvUnixToCalendar(time_t t)
* Input    : u32 t  Current UNIX Date
* Output   : None
* Return   : struct tm
*******************************************************************************/
struct tm Time_ConvUnixToCalendar(time_t t)
{
 struct tm *t_tm;
 t_tm = localtime(&t);
 t_tm->tm_year += 1900;
 return *t_tm;
}

/*******************************************************************************
* Function Name  : Time_ConvCalendarToUnix(struct tm t)
* Description    : Get UNIX Date
* Input    : struct tm t
* Output   : None
* Return   : time_t
*******************************************************************************/
time_t Time_ConvCalendarToUnix(struct tm t)
{
 t.tm_year -= 1900;  
 return mktime(&t);
}

/*******************************************************************************
* Function Name  : Time_GetUnixTime()
* Input    : None
* Output   : None
* Return   : time_t t
*******************************************************************************/
time_t Time_GetUnixTime(void)
{
 return (time_t)RTC_GetCounter();
// return (time_t) secCount;
}

/*******************************************************************************
* Function Name  : Time_GetCalendarTime()
* Input    : None
* Output   : None
* Return   : time_t t
*******************************************************************************/
struct tm Time_GetCalendarTime(void)
{
 time_t t_t;
 struct tm t_tm;

 t_t = (time_t)RTC_GetCounter();
// t_t = (time_t)secCount;
 t_tm = Time_ConvUnixToCalendar(t_t);
 t_tm.tm_year +=28;
 return t_tm;
}

/*******************************************************************************
* Function Name  : Time_SetUnixTime()
* Input    : time_t t
* Output   : None
* Return   : None
*******************************************************************************/
void Time_SetUnixTime(time_t t)
{
//  secCount = (u32) t;
	PWR_BackupAccessCmd(ENABLE);
  RTC_WaitForLastTask();
  RTC_SetCounter((u32)t);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);  
	RTC_WaitForLastTask();
  PWR_BackupAccessCmd(DISABLE);
  return;
}

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
//	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
//	RTC_SetCounter(seccount);	//设置RTC计数器的值

//	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	


/*******************************************************************************
* Function Name  : Time_SetCalendarTime()
* Input    : struct tm t
* Output   : None
* Return   : None
*******************************************************************************/
void Time_SetCalendarTime(struct tm t)
{
 t.tm_year -=28;
 Time_SetUnixTime(Time_ConvCalendarToUnix(t));
 return;
}

u8 RTC_Init(void)
{
	//检查是不是第一次配置时钟
	u8 temp=0;
 
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)		//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
		{	 			
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
		PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
		BKP_DeInit();	//复位备份区域 	
		RCC_LSEConfig(RCC_LSE_ON);	//设置外部低速晶振(LSE),使用外设低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
			{
			temp++;
			Delay_us(10000);
			}
		if(temp>=250)return 1;//初始化时钟失败,晶振有问题	    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();		//等待RTC寄存器同步  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//使能RTC秒中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();/// 允许配置	
		RTC_SetPrescaler(32767); //设置RTC预分频的值
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		//RTC_Set(2009,12,2,10,0,55);  //设置时间	
		RTC_ExitConfigMode(); //退出配置模式  
		BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//向指定的后备寄存器中写入用户程序数据
		}
	else//系统继续计时
		{

		RTC_WaitForSynchro();	//等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		}
	return 0; //ok

}		 				    
