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

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
//	PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
//	RTC_SetCounter(seccount);	//����RTC��������ֵ

//	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	


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
	//����ǲ��ǵ�һ������ʱ��
	u8 temp=0;
 
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)		//��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
		{	 			
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
		PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
		BKP_DeInit();	//��λ�������� 	
		RCC_LSEConfig(RCC_LSE_ON);	//�����ⲿ���پ���(LSE),ʹ��������پ���
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
			{
			temp++;
			Delay_us(10000);
			}
		if(temp>=250)return 1;//��ʼ��ʱ��ʧ��,����������	    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��  
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();		//�ȴ�RTC�Ĵ���ͬ��  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//ʹ��RTC���ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_EnterConfigMode();/// ��������	
		RTC_SetPrescaler(32767); //����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		//RTC_Set(2009,12,2,10,0,55);  //����ʱ��	
		RTC_ExitConfigMode(); //�˳�����ģʽ  
		BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//��ָ���ĺ󱸼Ĵ�����д���û���������
		}
	else//ϵͳ������ʱ
		{

		RTC_WaitForSynchro();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ��RTC���ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		}
	return 0; //ok

}		 				    
