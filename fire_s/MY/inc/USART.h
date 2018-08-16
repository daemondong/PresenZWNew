#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x_it.h"

typedef struct
{
	u16 rdPtr;    // ��ָ��
	u8 rdEnabled;    // �ܶ���
	u16 rdNum;    // ����ֽ��� 0 - ����
	u8 rdEnded;    //0- δ��� 1- ���
	uint16_t rdBuff[300];    // ������   
	u16 wrPtr;    // дָ��   
	u8 wrEnabled;    // ��д��
	u16 wrNum;    // ��д�ֽ��� 0 - ����
	u8 wrEnded;    //0- δ��� 1- ���
	uint16_t wrBuff[50];    // д���� 
} USART_DATA;

typedef struct
{
  USART_TypeDef *USARTPORT; // UASRT1-5
	u8 Enabled;  // 0 - ���� 1- ��
	USART_InitTypeDef Config;
} USART_CONFIG;


// Type of the Board
#define NOBOARD 0
#define IOBOARD 1          //��·��  ��240��λ
#define POWERBOARD 2       //��Դ����
#define FIREBOARD 3        //���� ÿ��2·
#define MULTILINEBOARD 4   //������  ÿ��12·
#define UNKNOWNBOARD 0xff


typedef struct
{
  char Name[2]; // Name of the board defalu is the NO. "1"--"70"
	u8 Type;  // the type of Board
	FunctionalState State;  // the state of Board to Flash ��·�壨��· ͨѶ���� ���� ʹ�ܣ�  
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
  u8 mNum;  //������ 1-31 Ŀǰ1-2
	u8 bNum;  //���ӱ�� 1-70
	u8 dNum;  //��λ��� 1-240
	u8 dType; //��λ����
	u8 eventType; // �¼����� 1-8
	u8 ioType; //���ͣ���ת����  250:��ʾ��  ����չ��100��
	u8 year;
	u8 mon;
	u8 day;
	u8 hour;
	u8 min;
	u8 sec;
	u8 stat;  //bit0: �Ƿ��͸�������
} IOSTAT;   //��·�� ������5000  14B

typedef struct
{
  u8 type;  //���� ���� ���� ��
	u8 bNum;  //���ӱ�� 1-70
	u8 dNum;  //��λ��� 1-240
	u8 eventType; // �¼����� 1-8
	u8 ioType; //���ͣ���ת����  250:��ʾ��  ����չ��100��
} VOICEIO;   //��·�� ������5000  14B

typedef struct
{
	u16 validConf;       //�趨ֵΪ 0x55AA��ʾ�������� 2B
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
	u8 maxMac;            //95  //�Ի�����
	
} CONFIGURATION1;

typedef struct
{
  u8 type;          //�ڵ�����
	u8 stat; 					//����״̬ stat.7 0 δ���� 1 ����   stat.10 ������ 0-3
	u16 area;         //���� 4λBCD��
	u8 addr[16];  		//λ�ã�8������
	u8 year;
	u8 mon;
	u8 day;
	u8 hour;
	u8 min;
	u8 sec;
} NODESTRUCT;   //�ڵ�ṹ 22B*240=5280B/128= 41.25 42BLOCK

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

