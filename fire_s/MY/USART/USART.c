#include "USART.h"
#include "delay.h"
#include "rtc_time.h"
#include "GloablVar.h"
#include "string.h"

extern IOSTAT  ioStat[];    //共5类 报警
extern IOSTAT  fireStat[] ;   //共5类 火警
extern IOSTAT  pinbiStat[] ;   //共5类 屏蔽
extern IOSTAT  errStat[] ;   //共5类 故障
extern IOSTAT  actStat[];   //共5类 联动
extern VOICEIO voiceIO[100],recoverEvt[100];

extern u8 ioInit[20][240];

void FSMC_NOR_WriteBuffer(u8* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite);
void FSMC_NOR_ReadBuffer(u8* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead);

	
void Init5USART(u8 num) {   //Init USART 0: all 5 ports; 1-5: single port
		GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO 
//	                       | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5 | RCC_APB1Periph_UART4 | RCC_APB1Periph_USART2 
//	                        | RCC_APB1Periph_USART3 | RCC_APB1Periph_TIM2,ENABLE);
	if (myConf.usart1_config.Enabled == 1 && (num == 0 || num == 1)) {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//USART1,GPIOA
 	    USART_DeInit(USART1);
	    //USART1_TX   PA.9
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
   
      //USART1_RX	  PA.10
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
      GPIO_Init(GPIOA, &GPIO_InitStructure);    
	
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);
      USART_InitStructure.USART_BaudRate = myConf.usart1_config.Config.USART_BaudRate;
      USART_InitStructure.USART_WordLength = myConf.usart1_config.Config.USART_WordLength;
      USART_InitStructure.USART_StopBits = myConf.usart1_config.Config.USART_StopBits;
      USART_InitStructure.USART_Parity = myConf.usart1_config.Config.USART_Parity;
      USART_InitStructure.USART_HardwareFlowControl = myConf.usart1_config.Config.USART_HardwareFlowControl;
      USART_InitStructure.USART_Mode = myConf.usart1_config.Config.USART_Mode;

      USART_Init(USART1, &USART_InitStructure);
//	    USART_ITConfig(USART1, USART_FLAG_TC, ENABLE);
//	    USART_ITConfig(USART1, USART_FLAG_TXE, ENABLE);
	    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
      USART_Cmd(USART1, ENABLE);
		}
		
		if (myConf.usart2_config.Enabled == 1 && (num == 0 || num == 2)) {
		  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//PORTA
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //USART2
 	    USART_DeInit(USART2);
	    //USART2_TX   PA.2
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
   
      //USART2_RX	  PA.3
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
      GPIO_Init(GPIOA, &GPIO_InitStructure);     
	
      NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);
      USART_InitStructure.USART_BaudRate = myConf.usart2_config.Config.USART_BaudRate;
      USART_InitStructure.USART_WordLength = myConf.usart2_config.Config.USART_WordLength;
      USART_InitStructure.USART_StopBits = myConf.usart2_config.Config.USART_StopBits;
      USART_InitStructure.USART_Parity = myConf.usart2_config.Config.USART_Parity;
      USART_InitStructure.USART_HardwareFlowControl = myConf.usart2_config.Config.USART_HardwareFlowControl;
      USART_InitStructure.USART_Mode = myConf.usart2_config.Config.USART_Mode;

      USART_Init(USART2, &USART_InitStructure);
	    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
      USART_Cmd(USART2, ENABLE);			
		}
		
		if (myConf.usart3_config.Enabled == 1 && (num == 0 || num == 3)) {
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//PORTB
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //USART3
 	    USART_DeInit(USART3);
	    //USART3_TX   PB.10
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
   
      //USART3_RX	  PB.11
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
      GPIO_Init(GPIOB, &GPIO_InitStructure);    
	
      NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);
      USART_InitStructure.USART_BaudRate = myConf.usart3_config.Config.USART_BaudRate;
      
      USART_InitStructure.USART_StopBits = myConf.usart3_config.Config.USART_StopBits;
			USART_InitStructure.USART_WordLength = myConf.usart3_config.Config.USART_WordLength;
			USART_InitStructure.USART_Parity = myConf.usart3_config.Config.USART_Parity;
			USART_InitStructure.USART_HardwareFlowControl = myConf.usart3_config.Config.USART_HardwareFlowControl;
      USART_InitStructure.USART_Mode = myConf.usart3_config.Config.USART_Mode;

      USART_Init(USART3, &USART_InitStructure);
	    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
      USART_Cmd(USART3, ENABLE); 			
		}
		
		if (myConf.usart4_config.Enabled == 1 && (num == 0 || num == 4)) {
		  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//PORTC
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); //UART4
 	    USART_DeInit(UART4);
	    //UART4_TX   PC.10
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
   
      //UART4_RX	  PC.11
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
      GPIO_Init(GPIOC, &GPIO_InitStructure);    
	
      NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);
      USART_InitStructure.USART_BaudRate = myConf.usart4_config.Config.USART_BaudRate;
      USART_InitStructure.USART_WordLength = myConf.usart4_config.Config.USART_WordLength;
      USART_InitStructure.USART_StopBits = myConf.usart4_config.Config.USART_StopBits;
      USART_InitStructure.USART_Parity = myConf.usart4_config.Config.USART_Parity;
      USART_InitStructure.USART_HardwareFlowControl = myConf.usart4_config.Config.USART_HardwareFlowControl;
      USART_InitStructure.USART_Mode = myConf.usart4_config.Config.USART_Mode;

      USART_Init(UART4, &USART_InitStructure);
	    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
      USART_Cmd(UART4, ENABLE); 			
		}
		
		if (myConf.usart5_config.Enabled == 1 && (num == 0 || num == 5)) {
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	//PORTC,PORTD
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE); //UART5
 	    USART_DeInit(UART5);
	    //UART5_TX   PC.12
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
   
      //UART5_RX	  PD.2
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
      GPIO_Init(GPIOD, &GPIO_InitStructure);    
	
      NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStructure);
      USART_InitStructure.USART_BaudRate = myConf.usart5_config.Config.USART_BaudRate;
      USART_InitStructure.USART_WordLength = myConf.usart5_config.Config.USART_WordLength;
      USART_InitStructure.USART_StopBits = myConf.usart5_config.Config.USART_StopBits;
      USART_InitStructure.USART_Parity = myConf.usart5_config.Config.USART_Parity;
      USART_InitStructure.USART_HardwareFlowControl = myConf.usart5_config.Config.USART_HardwareFlowControl;
      USART_InitStructure.USART_Mode = myConf.usart5_config.Config.USART_Mode;

      USART_Init(UART5, &USART_InitStructure);
	    USART_ITConfig(UART5, USART_IT_RXNE| USART_FLAG_TXE, ENABLE);
      USART_Cmd(UART5, ENABLE); 			
		}
		
}

void DealGetData(u8 cmd, USART_DATA *ud){
	u16 i,num;
	
	switch(cmd) {
		case 33:
			num=ud->rdBuff[2]*256+ud->rdBuff[3];
			for (i=0;i<num;i++) {
				SeterrStat(USART3MainMac, ud->rdBuff[4+i*4], ud->rdBuff[4+i*4+1], ud->rdBuff[4+i*4+2], ud->rdBuff[4+i*4+3]);
			}
			break;
	}
}

void sendU3(u16 *buf,u8 len, u8 type) {
	u8 i,val;
	
	U3_SEND;
	for (i=0,val=0;i<len;i++) {
		val=val ^ buf[i];
		USART_SendData(USART3, buf[i]);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)== RESET);
	}
	if (type==1) {
		USART_SendData(USART3, val);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)== RESET);
	}
}


void DealUSART(u8 port,USART_DATA *usart_data, uint16_t rddata,u16 sr) {
	if (usart_data->rdEnabled && usart_data->rdEnded == 0) {
		if (usart_data->rdPtr==0 && rddata ==0) return;
		if (port==3) usart_data->rdBuff[5+usart_data->rdPtr] = sr;
	  usart_data->rdBuff[usart_data->rdPtr++]= rddata;
		if (port==2) timeout2=NORMALTIMEOUT;
		
		//IO Board Query Data
		if (port == 2 && usart_data->rdPtr == 2 && usart_data->rdBuff[0] == 4 
				&& InterBoard[ptrIB].Type == IOBOARD && (usart_data->rdBuff[1] & 0x80) == 0x00) {
			if ((usart_data->rdBuff[1] & 0x02) == 0x02) usart_data->rdNum += 240;
			if ((usart_data->rdBuff[1] & 0x04) == 0x04) usart_data->rdNum += 12;
		}
		
		if (port == 2 && usart_data->rdBuff[0] == 0xAA) timeout2=0; 
		//IO Board Output Metrics
//		if (port == 2 && usart_data->rdPtr == 1 && InterBoard[ptrIB].Type == IOBOARD && usart_data->wrBuff[0]==5) {
//				testByte0=usart_data->wrBuff[0];
//				testByte1=usart_data->rdBuff[0];
//				timeout2=0;
//		}			
	  
		//Test TuWen or Computer
		if (port == 1 && usart_data->rdPtr == 2 && usart_data->rdBuff[0] == 241 & isComputer == 0) {
			usart_data->rdNum = 3;
			if (usart_data->rdBuff[1] == 0xAA) isComputer = 2; 
			if (usart_data->rdBuff[1] == 0x55) isComputer = 3;
			if (usart_data->rdBuff[1] == 0x88) {
				isComputer = 1;
				usart_data->rdNum = 5;
			}
		}			

		//Big Data for USART1
		if (port == 1 && usart_data->rdPtr == 3 && usart_data->rdBuff[0] == 221 && usart_data->rdBuff[1] == 5) {
			usart_data->rdNum = usart_data->rdBuff[2] * 128 + 6;
		}			

		if (usart_data->rdNum > 0 && usart_data->rdPtr >= usart_data->rdNum) {
		  usart_data->rdEnded = 1;
			ifDealUSART[port] |= 0x01;
		}

		if (usart_data->rdNum == 0 && port == 1 && isComputer == 1 && rddata == 0xFF) {
		  usart_data->rdEnded = 1;
			ifDealUSART[port] |= 0x01;
		}
		if (rddata > 0x100 && port == 3 && myConf1.mNum>1) {
			usart_data->rdBuff[0] = rddata;
		  usart_data->rdEnded = 1;
			ifDealUSART[port] |= 0x01;
		}

		if (usart_data->rdPtr > 3 && port == 3 && rddata == 0xFF && myConf1.mNum>1) {
		  usart_data->rdEnded = 1;
			ifDealUSART[port] |= 0x01;
		}
		
		if (port == 3 && myConf1.mNum==1) {
			if (sendStep==1) {
				if (rddata==startQmNum)	{ USART3MainStat=1; USART3MainMac=startQmNum; sendStep=2; }
				else { USART3MainStat=0; USART3MainMac=0; sendStep=0; }
				usart_data->rdEnded = 1;
				ifDealUSART[port] |= 0x01; 
			}
			else if (sendStep==2) {
				if (USART3MainCmd==1 && usart_data->rdPtr>=5) {
					sendStep=3;
					usart_data->rdEnded = 1;
					ifDealUSART[port] |= 0x01; 
				}
			}
			else if (sendStep==4) {
				if ((USART3MainCmd==2 || USART3MainCmd==33) && usart_data->rdPtr >= 5+4*(usart_data->rdBuff[2]*256+usart_data->rdBuff[3])) {
					usart_data->rdEnded = 1;
					ifDealUSART[port] |= 0x01; 
				}
			}
		}
		
	}
}

void SendUSART(u8 port,USART_TypeDef* USARTx,USART_DATA *usart_data) {
//	USART_ClearITPendingBit(USARTx,USART_IT_TXE);
	USART_ClearFlag(USARTx,USART_FLAG_TC);
	if (usart_data->wrEnabled && usart_data->wrEnded == 0) {
		USART_SendData(USARTx, usart_data->wrBuff[usart_data->wrPtr++]);
	  if (usart_data->wrNum > 0 && usart_data->wrPtr >= usart_data->wrNum) {
			USART_ITConfig(USARTx, USART_FLAG_TXE, DISABLE);
		  usart_data->wrEnded = 1;
			ifDealUSART[port] |= 0x02;
//			DealFinishSend(port);
		}
	}
	else USART_ITConfig(USARTx, USART_FLAG_TXE, DISABLE);
}


void StartRcv(USART_DATA *usart_data, u8 n) {
	usart_data->rdPtr = 0;
	usart_data->rdEnabled = 1;
	usart_data->rdEnded = 0;
	usart_data->rdNum = n;
}

void StartSend(USART_DATA *usart_data, u8 n) {
	usart_data->wrPtr = 0;
	usart_data->wrEnabled = 1;
	usart_data->wrEnded = 0;
	usart_data->wrNum = n;
}

u8 IsValidRcvData(USART_DATA *usart_data) {
//	u8 i,xorval = 0;
	
//	for (i=0;i<usart_data->rdNum-1;i++) xorval ^= usart_data->rdBuff[i];
//	if (xorval==usart_data->rdBuff[i]) return 1;
	return 1;
}

void moveNextIB(u8 cont, u8 confByte) {  //Find next Internal Borad to send command cont=0 means need confirm byte send 
	if (cont == 1) {
		while (InterBoard[++ptrIB].State != ENABLE)
			if (ptrIB == MAXNUMBOARD - 1) ptrIB = 0;

		stepIBSend = 0;
		if (ptrIB>1 || loopIO == 1|| stateCPU == INITING || stateCPU == RESETTING) {
      if (loopIO == 1) loopIO=0;			
			if ( ptrIB<=20 || stateCPU == RUNNING) {
				IBSendCmd(ptrIB,InterBoard[ptrIB].Type, stepIBSend); 
				timeout2=NORMALTIMEOUT;
      }				
		}
		else timeout2=0;
  }
  else {
		SETBIT(needConfirm);
		U2_SEND;
		USART_SendData(USART2, confByte);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
		U2_RCV;
	}
}

extern u32 readBlock;

void DealFinishRead(u8 port) {
	u16 i;
	u8 j,WriteReadStatus;
	u8 rcvbuff[128];
	u32 writeBlock;
	
	switch (port)
	{
		case 1: //USART1 for TuWen or Computer
			USART_ClearFlag(USART1, USART_FLAG_TC);
			if (isComputer == 1) {
				if (usart1_data.rdBuff[0] == 241) {   //Start Computer
				  USART_SendData(USART1, 0xAA);
				  Delay_us(100);
//				  usart1_config.Config.USART_BaudRate = 38400;
//				  Init5USART(1);
//					USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//					USART_Cmd(USART1, ENABLE);
					StartRcv(&usart1_data,0);
				}
				if (usart1_data.rdBuff[0] == 221) {		//Deal Computer command
//					StartRcv(&usart1_data,0);
					switch (usart1_data.rdBuff[1])
					{
						case 1 :  //Get Version
					    StartRcv(&usart1_data,0);
							usart1_data.wrBuff[0] = 231;
							usart1_data.wrBuff[1] = 1;
							usart1_data.wrBuff[2] = myConf1.machineID >> 8;
							usart1_data.wrBuff[3] = myConf1.machineID & 0xFF;
							usart1_data.wrBuff[4] = myConf1.machineVersion;
							usart1_data.wrBuff[5] = usart1_data.wrBuff[0] ^ usart1_data.wrBuff[1] ^ usart1_data.wrBuff[2] 
						                        ^ usart1_data.wrBuff[3] ^ usart1_data.wrBuff[4];
							StartSend(&usart1_data,6);
							USART_ITConfig(USART1, USART_IT_TC, ENABLE);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
							break;

						case 2 :  //Clear All Flash
							FSMC_NOR_EraseChip();
					    StartRcv(&usart1_data,0);
							usart1_data.wrBuff[0] = 231;
							usart1_data.wrBuff[1] = 2;
							usart1_data.wrBuff[2] = 0xAA;
							usart1_data.wrBuff[3] = usart1_data.wrBuff[0] ^ usart1_data.wrBuff[1] ^ usart1_data.wrBuff[2]; 
							StartSend(&usart1_data,4);
							USART_ITConfig(USART1, USART_IT_TC, ENABLE);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
							break;

						case 3 :  //Clear Program Flash
							usart1_data.wrBuff[0] = 231;
							usart1_data.wrBuff[1] = 3;
							usart1_data.wrBuff[2] = 0xAA;
							usart1_data.wrBuff[3] = usart1_data.wrBuff[0] ^ usart1_data.wrBuff[1] ^ usart1_data.wrBuff[2]; 
							StartSend(&usart1_data,4);
							USART_ClearITPendingBit(USART1,USART_IT_TXE);
							USART_ClearFlag(USART1,USART_FLAG_TC);
						  USART_ITConfig(USART1, USART_FLAG_TXE, DISABLE);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
						  Delay_us(20);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
						  Delay_us(20);
						  for (i=256;i<512;i++) {
								FSMC_NOR_EraseBlock(1024 * i);
								FSMC_NOR_EraseBlock(1024 * i + 128);
								FSMC_NOR_EraseBlock(1024 * i + 256);
								FSMC_NOR_EraseBlock(1024 * i + 384);
								FSMC_NOR_EraseBlock(1024 * i + 512);
								FSMC_NOR_EraseBlock(1024 * i + 640);
								FSMC_NOR_EraseBlock(1024 * i + 768);
								FSMC_NOR_EraseBlock(1024 * i + 896);
								USART_SendData(USART1, 0x31);
						    Delay_us(20);
							}								
						  
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
						  Delay_us(20);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
						  Delay_us(20);
					    StartRcv(&usart1_data,0);
							break;

						case 4 :  //Clear Char DB Flash
							usart1_data.wrBuff[0] = 231;
							usart1_data.wrBuff[1] = 4;
							usart1_data.wrBuff[2] = 0xAA;
							usart1_data.wrBuff[3] = usart1_data.wrBuff[0] ^ usart1_data.wrBuff[1] ^ usart1_data.wrBuff[2]; 
							USART_ClearITPendingBit(USART1,USART_IT_TXE);
							USART_ClearFlag(USART1,USART_FLAG_TC);
						  USART_ITConfig(USART1, USART_FLAG_TXE, DISABLE);

//							USART_ITConfig(USART1, USART_IT_TC, ENABLE);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
						  Delay_us(20);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
						  Delay_us(20);
						  for (i=0;i<256;i++) {
								FSMC_NOR_EraseBlock(1024 * i);
								FSMC_NOR_EraseBlock(1024 * i + 128);
								FSMC_NOR_EraseBlock(1024 * i + 256);
								FSMC_NOR_EraseBlock(1024 * i + 384);
								FSMC_NOR_EraseBlock(1024 * i + 512);
								FSMC_NOR_EraseBlock(1024 * i + 640);
								FSMC_NOR_EraseBlock(1024 * i + 768);
								FSMC_NOR_EraseBlock(1024 * i + 896);
								USART_SendData(USART1, 0x31);
								Delay_us(20);
							}								
						  
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
						  Delay_us(20);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
						  Delay_us(20);
					    StartRcv(&usart1_data,0);
							break;

						case 5 :  //Write a bolck bytes to Flash the most the 2 block 256B
							for (j=0 ; j < usart1_data.rdBuff[2] ; j++) {
								readBlock = (((u32)usart1_data.rdBuff[3])*256 + usart1_data.rdBuff[4])/2;
								writeBlock = (((u32)usart1_data.rdBuff[3])*256 + usart1_data.rdBuff[4])*128 + j * 128;
								FSMC_NOR_EraseBlock(writeBlock);
								for (i=0;i<0x80;i++)
									FSMC_NOR_Write(writeBlock+i, usart1_data.rdBuff[5+j*128+i] & 0xff);

								FSMC_NOR_ReadBuffer(rcvbuff, writeBlock, 0x80); 

								/* Read back NOR memory and check content correctness */   
								for (i = 0x00; (i < 0x80) && (WriteReadStatus == 0); i++)
								{
										if ((rcvbuff[i] & 0xff) != (usart1_data.rdBuff[5+j*128+i] & 0xff) )
										{
											WriteReadStatus = i + 1;
								    }
							  }
							}
//							strHex(keyShow,usart1_data.rdBuff[3]);
//							ShowString8 (30, 5, keyShow,2 ,0);
//							strHex(keyShow,usart1_data.rdBuff[4]);
//							ShowString8 (30, 21, keyShow,2 ,0);
							StartRcv(&usart1_data,0);
							usart1_data.wrBuff[0] = 231;
							usart1_data.wrBuff[1] = 5;
							usart1_data.wrBuff[2] = 0xAA;
							usart1_data.wrBuff[3] = usart1_data.wrBuff[0] ^ usart1_data.wrBuff[1] ^ usart1_data.wrBuff[2]; 
							StartSend(&usart1_data,4);
							USART_ITConfig(USART1, USART_IT_TC, ENABLE);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
							break;

						case 6 :  //Read a bolck bytes from Flash the most the 2 block 256B
							break;
						
						case 7 :  //Set Date&Time
							machineDate.tm_year = usart1_data.rdBuff[2] + 2000;
						  machineDate.tm_mon = usart1_data.rdBuff[3] - 1;
						  machineDate.tm_mday = usart1_data.rdBuff[4];
						  machineDate.tm_hour = usart1_data.rdBuff[5];
						  machineDate.tm_min = usart1_data.rdBuff[6];
						  machineDate.tm_sec = usart1_data.rdBuff[7];
						  Time_SetCalendarTime(machineDate);
					    StartRcv(&usart1_data,0);
						  usart1_data.wrBuff[0] = 231;
							usart1_data.wrBuff[1] = 7;
							usart1_data.wrBuff[2] = 0xAA;
							usart1_data.wrBuff[3] = usart1_data.wrBuff[0] ^ usart1_data.wrBuff[1] ^ usart1_data.wrBuff[2]; 
							StartSend(&usart1_data,4);
							USART_ITConfig(USART1, USART_IT_TC, ENABLE);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
							break;

						case 8 :  //Set Date&Time
							machineDate = Time_GetCalendarTime();
					    StartRcv(&usart1_data,0);
						  usart1_data.wrBuff[0] = 231;
							usart1_data.wrBuff[1] = 8;
							usart1_data.wrBuff[2] = machineDate.tm_year - 2000;
						  usart1_data.wrBuff[3] = machineDate.tm_mon + 1;
						  usart1_data.wrBuff[4] = machineDate.tm_mday;
						  usart1_data.wrBuff[5] = machineDate.tm_hour;
						  usart1_data.wrBuff[6] = machineDate.tm_min;
						  usart1_data.wrBuff[7] = machineDate.tm_sec;
							usart1_data.wrBuff[8] = usart1_data.wrBuff[0] ^ usart1_data.wrBuff[1] ^ usart1_data.wrBuff[2]
																		^ usart1_data.wrBuff[3] ^ usart1_data.wrBuff[4] ^ usart1_data.wrBuff[5]
																		^ usart1_data.wrBuff[6] ^ usart1_data.wrBuff[7];
							StartSend(&usart1_data,9);
							USART_ITConfig(USART1, USART_IT_TC, ENABLE);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
							break;

						case 9 :  //Test RAM
							break;
						
						case 10 :  //Test Printer
							break;
						
						case 11 :  //Test LED
					    StartRcv(&usart1_data,0);
						  usart1_data.wrBuff[0] = 231;
							usart1_data.wrBuff[1] = 11;
							usart1_data.wrBuff[2] = 0xAA;
							usart1_data.wrBuff[3] = usart1_data.wrBuff[0] ^ usart1_data.wrBuff[1] ^ usart1_data.wrBuff[2];
							StartSend(&usart1_data,4);
							USART_ITConfig(USART1, USART_IT_TC, ENABLE);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
							break;
						
						case 12 :  //Set Main Board
							GPIO_WriteBit(GPIOE, GPIO_Pin_3,(BitAction)((usart1_data.rdBuff[2] & 0x04) >> 2));
					    StartRcv(&usart1_data,0);
						  usart1_data.wrBuff[0] = 231;
							usart1_data.wrBuff[1] = 12;
							usart1_data.wrBuff[2] = 0xAA;
							usart1_data.wrBuff[3] = usart1_data.wrBuff[0] ^ usart1_data.wrBuff[1] ^ usart1_data.wrBuff[2];
							StartSend(&usart1_data,4);
							USART_ITConfig(USART1, USART_IT_TC, ENABLE);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
							break;
						
						case 13 :  //Set Machine ID
							myConf1.machineID = usart1_data.rdBuff[2] * 256 + usart1_data.rdBuff[3];
					    WriteConf();
						  StartRcv(&usart1_data,0);
						  usart1_data.wrBuff[0] = 231;
							usart1_data.wrBuff[1] = 13;
							usart1_data.wrBuff[2] = 0xAA;
							usart1_data.wrBuff[3] = usart1_data.wrBuff[0] ^ usart1_data.wrBuff[1] ^ usart1_data.wrBuff[2];
							StartSend(&usart1_data,4);
							USART_ITConfig(USART1, USART_IT_TC, ENABLE);
							USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
							break;

						case 14 :  //Print Test Paper
							break;
						
						case 15 :  //Stop Computer Controll
							break;
						
						case 16 :  //Query Stat of Main Board
							break;
						
						case 17 :  //Query Stat of Main Board
							break;
						
						case 18 :  //Query Stat of Main Board
							break;
						
						case 19 :  //Query Stat of Main Board
							break;
						
						case 20 :  //Query Stat of Main Board
							break;
						

					}
				}
			}
			break;
		
		case 2: //USART2 for internal board
			USART_ClearFlag(USART2, USART_FLAG_TC);
		  if ((stateCPU == INITING || stateCPU == RESETTING) && usart2_data.rdBuff[0] == 0xAA) 
				initOver = 1;
			if (stepIBSend == 0 && ((usart2_data.rdBuff[0] & 0xff) == ptrIB)) {
				timeout2 = 0;
				DelerrStat(myConf1.mNum, ptrIB, 0, 0, 211);
//				if (stateCPU==RUNNING)
        if (stateCPU == INITING || stateCPU == RESETTING) initIOB = ptrIB;
        else {				
					stepIBSend++;
					IBSendCmd(ptrIB, InterBoard[ptrIB].Type, stepIBSend);
				}
			}
			else if (stepIBSend > 0) {
				DelerrStat(myConf1.mNum,ptrIB, 0, 0, 211);
				timeout2 = 0;
				switch (InterBoard[ptrIB].Type)
				{
					case POWERBOARD:
						if (stepIBSend == 1 && IsValidRcvData(&usart2_data)) {   // Get power status or Volt
							timeout2 = 0;
							if (ISSET(queryPowerVlot)) {
								mainVlot=usart2_data.rdBuff[1];
								backVlot=usart2_data.rdBuff[2];
								dispLine=4;
								RESETBIT(queryPowerVlot);
							}
							else DealPowerStat(ptrIB, usart2_data.rdBuff[1]);
						}
						moveNextIB(1,0);
						break;
					
					case IOBOARD:
						if (stepIBSend == 1 && IsValidRcvData(&usart2_data)) {   // Get IO status
							timeout2 = 0;
							moveNextIB(0,0xAA);
							if (usart2_data.rdNum > 240 || ((usart2_data.rdBuff[1] & 0x01) !=0)) DealIOStat(ptrIB, &usart2_data);
						}
						moveNextIB(1,0);
						break;

					case MULTILINEBOARD:
						if (stepIBSend == 1 && IsValidRcvData(&usart2_data)) {   // Get Multi Line status
							timeout2 = 0;
							DealMLStat(ptrIB, &usart2_data);
						}
						moveNextIB(1,0);
						break;

					case FIREBOARD:
						if (stepIBSend == 1 && IsValidRcvData(&usart2_data)) {   // Get Fire status
							timeout2 = 0;
							DealFireStat(ptrIB, &usart2_data);
						}
						moveNextIB(1,0);
						break;
				}
			}
			break;

		case 3: //USART3 主从机
			USART_ClearFlag(USART3, USART_FLAG_TC);
		  if (usart3_data.rdBuff[0]>0x100 && myConf1.mNum>1) {
				if  (usart3_data.rdBuff[0] == (0x100 | myConf1.mNum)) {   //从机收到地址
					USART3Status=1;
					U3_SEND;
					USART_SendData(USART3, myConf1.mNum);
					while(USART_GetFlagStatus(USART3, USART_FLAG_TC)== RESET);
				}
				else 
					USART3Status=0;
				StartRcv(&usart3_data,0);
				U3_RCV;
			}
			else if (USART3Status==1 && myConf1.mNum>1) { //从机收到数据	
				if (usart3_data.rdBuff[0]==219) {
					switch (usart3_data.rdBuff[1]) {
						case 1:
							usart3_data.wrBuff[0]=225;
							usart3_data.wrBuff[1]=1;
							usart3_data.wrBuff[2]=myStat1;
							usart3_data.wrBuff[3]=myStat2;
							sendU3(usart3_data.wrBuff,4,1);
							break;

						case 2:																		//新火警
							usart3_data.wrBuff[0]=225;
							usart3_data.wrBuff[1]=2;
							for (i=0,j=0;i<totFire;i++)
								if ((fireStat[i].stat & 0x01) ==0) {
									fireStat[i].stat |= 0x01;
									usart3_data.wrBuff[4+j*4]=fireStat[i].bNum;
									usart3_data.wrBuff[4+j*4+1]=fireStat[i].dNum;
									usart3_data.wrBuff[4+j*4+2]=fireStat[i].dType;
									usart3_data.wrBuff[4+j*4+3]=fireStat[i].eventType;
									j++;
								}
							usart3_data.wrBuff[2]=j/256;
							usart3_data.wrBuff[3]=j%256;
							sendU3(usart3_data.wrBuff,4+j*4,1);
							break;


						case 33:																		//新故障
							usart3_data.wrBuff[0]=225;
							usart3_data.wrBuff[1]=33;
							for (i=1,j=0;i<=totErr;i++)
								if ((errStat[i].stat & 0x01) ==0) {
									errStat[i].stat |= 0x01;
									usart3_data.wrBuff[4+j*4]=errStat[i].bNum;
									usart3_data.wrBuff[4+j*4+1]=errStat[i].dNum;
									usart3_data.wrBuff[4+j*4+2]=errStat[i].dType;
									usart3_data.wrBuff[4+j*4+3]=errStat[i].eventType;
									j++;
								}
							usart3_data.wrBuff[2]=j/256;
							usart3_data.wrBuff[3]=j%256;
							sendU3(usart3_data.wrBuff,4+j*4,1);
							break;

						case 34:																		//恢复新故障
							usart3_data.wrBuff[0]=225;
							usart3_data.wrBuff[1]=34;
							for (i=0,j=0;i<totRecoverEvt;)
								if (recoverEvt[i].type==1) {
									usart3_data.wrBuff[4+j*4]=recoverEvt[i].bNum;
									usart3_data.wrBuff[4+j*4+1]=recoverEvt[i].dNum;
									usart3_data.wrBuff[4+j*4+2]=recoverEvt[i].ioType;
									usart3_data.wrBuff[4+j*4+3]=recoverEvt[i].eventType;
									j++;
									totRecoverEvt--;
									if (i<totRecoverEvt) recoverEvt[i]=recoverEvt[i+1]; 
								}
								else i++;
							usart3_data.wrBuff[2]=j/256;
							usart3_data.wrBuff[3]=j%256;
							sendU3(usart3_data.wrBuff,4+j*4,1);
							break;
					}
				}
				StartRcv(&usart3_data,0);
				U3_RCV;
			}
			else if (myConf1.mNum==1) {									//主机收到数据
				if (sendStep==2) {						//从机地址已确认
							
							usart3_data.wrBuff[0]=219;
							usart3_data.wrBuff[1]=1;
							USART3MainCmd=1;
							usart3_data.wrBuff[2]=0x55;
							usart3_data.wrBuff[3]=0xFF;
							sendU3(usart3_data.wrBuff,4,0);
							StartRcv(&usart3_data,0);
							U3_RCV;
				}
				else if (sendStep==3) {
					GetStat=(usart3_data.rdBuff[3] << 8) | usart3_data.rdBuff[2];
					StatStep=0;
					RESETBIT(U3StatDealing);
				}
				else if (sendStep==4) {
					DealGetData(USART3MainCmd,&usart3_data);
					RESETBIT(U3StatDealing);
				}
				
			}
			else {
				StartRcv(&usart3_data,0);
				USART3Status=0;
				U3_RCV;
			}
			break;
	}
}

void DealFinishSend(u8 port) {
	if (port ==2 ) U2_RCV;
	if (port == 2 && stepIBSend == 1 && InterBoard[ptrIB].Type == IOBOARD && ISSET(needConfirm)) {
		RESETBIT(needConfirm);
//		moveNextIB(1,0);
	}
}


void IBSendCmd(u8 bNum, u8 type, u8 step) {
	
u8 i,j;
	
	if (step == 0) {  // send board number
		StartRcv(&usart2_data,1);
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		USART_ITConfig(USART2, USART_IT_TC, DISABLE);
		U2_SEND;
//		USART_ITConfig(USART2, USART_IT_TC, ENABLE);
		USART_ClearITPendingBit(USART2,USART_IT_TXE);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		USART_ClearFlag(USART2,USART_FLAG_TXE);
		USART_SendData(USART2, 0x100 | bNum);
//		Delay_us(500);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {}
		U2_RCV;
//		U2_SEND;
		return;
	}
	
	switch (type)
	{
		case POWERBOARD:
			switch (step)
			{
				case 1 :  // query power board status
					if (ISSET(queryPowerVlot)) {
						StartRcv(&usart2_data,4);
						usart2_data.wrBuff[0] = 3;   // Cmd: Query power
						usart2_data.wrBuff[1] = 3 ^ bNum;   // Cmd: Query power
						StartSend(&usart2_data,2);
						U2_SEND;
						USART_ClearFlag(USART2, USART_FLAG_TC);
//					USART_ITConfig(USART2, USART_IT_TC, ENABLE);
						USART_SendData(USART2, usart2_data.wrBuff[usart2_data.wrPtr++]);
						while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
						USART_SendData(USART2, usart2_data.wrBuff[usart2_data.wrPtr++]);
						while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
						U2_RCV;
					}
					else {
						StartRcv(&usart2_data,3);
						usart2_data.wrBuff[0] = 2;   // Cmd: Query power
						usart2_data.wrBuff[1] = 2 ^ bNum;   // Cmd: Query power
						StartSend(&usart2_data,2);
						U2_SEND;
						USART_ClearFlag(USART2, USART_FLAG_TC);
//					USART_ITConfig(USART2, USART_IT_TC, ENABLE);
						USART_SendData(USART2, usart2_data.wrBuff[usart2_data.wrPtr++]);
						while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
						USART_SendData(USART2, usart2_data.wrBuff[usart2_data.wrPtr++]);
						while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
						U2_RCV;
					}
					break;
			}
			break;

		case IOBOARD:
			switch (step)
			{
				case 1 :  // query IO board status
					if (ISSET(needAction)) {
						RESETBIT(needAction);
						timeout2=LONGTIMEOUT;
						StartRcv(&usart2_data,3);
						usart2_data.wrBuff[0] = 5;   // Cmd: Query IO
						j = 5 ^ bNum;
						for (i=0;i<30;i++) {
							if (i<2) usart2_data.wrBuff[i+1]=0xFF;
							else usart2_data.wrBuff[i+1]=0x00;
							j = (j ^ usart2_data.wrBuff[i+1]);
						}
						usart2_data.wrBuff[i+1] = j;
//					usart2_data.wrBuff[1] = 4 ^ 0;   // Cmd: Query IO
						StartSend(&usart2_data,32);
						U2_SEND;
						for (i=0;i<32;i++) {
							USART_SendData(USART2, usart2_data.wrBuff[usart2_data.wrPtr++]);
							while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
						}
						U2_RCV;
					}
					else {
						StartRcv(&usart2_data,3);
						usart2_data.wrBuff[0] = 4;   // Cmd: Query IO
						usart2_data.wrBuff[1] = 4 ^ bNum;   // Cmd: Query IO
						StartSend(&usart2_data,2);
						U2_SEND;
						USART_SendData(USART2, usart2_data.wrBuff[usart2_data.wrPtr++]);
						while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
						USART_SendData(USART2, usart2_data.wrBuff[usart2_data.wrPtr++]);
						while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
						U2_RCV;
					}
					break;
			}
			break;

		case MULTILINEBOARD:
			switch (step)
			{
				case 1 :  // query Multi Line board status
					StartRcv(&usart2_data,4);
					usart2_data.wrBuff[0] = 4;   // Cmd: Query Multi Line
					usart2_data.wrBuff[1] = 4 ^ bNum;   // Cmd: Query Multi Line
					StartSend(&usart2_data,2);
					USART_SendData(USART2, usart2_data.wrBuff[usart2_data.wrPtr++]);
					break;
			}
			break;

		case FIREBOARD:
			switch (step)
			{
				case 1 :  // query Fire board status
					StartRcv(&usart2_data,3);
					usart2_data.wrBuff[0] = 5;   // Cmd: Query Fire
					usart2_data.wrBuff[1] = 5 ^ bNum;   // Cmd: Query Fire
					StartSend(&usart2_data,2);
					USART_SendData(USART2, usart2_data.wrBuff[usart2_data.wrPtr++]);
					break;
			}
			break;
	}
}

void testUSART1(void) {    //Check if tuwen or Computer
	StartRcv(&usart1_data,0);
	timeout1 = NORMALTIMEOUT;
	usart1_data.wrBuff[0] = 241;
	usart1_data.wrBuff[1] = myConf1.serialTW >> 8;
	usart1_data.wrBuff[2] = myConf1.serialTW & 0xFF;
	usart1_data.wrBuff[3] = usart1_data.wrBuff[0] ^ usart1_data.wrBuff[1] ^ usart1_data.wrBuff[2];
	usart1_data.wrBuff[4] = 0xFF;
	StartSend(&usart1_data,5);
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	USART_SendData(USART1, usart1_data.wrBuff[usart1_data.wrPtr++]);
}

void testUSART3(void) {    //Check if tuwen or Computer
	StartRcv(&usart3_data,0);
	timeout1 = NORMALTIMEOUT;
	usart3_data.wrBuff[0] = 01;
	usart3_data.wrBuff[1] = 0xAA;
	usart3_data.wrBuff[2] = 0x55;
	usart3_data.wrBuff[3] = 0xA0;
	usart3_data.wrBuff[4] = 0x05;
	StartSend(&usart3_data,5);
	U3_SEND;
	USART_ClearFlag(USART3, USART_FLAG_TC);
//	USART_ITConfig(USART3, USART_IT_TC, ENABLE);
  while (1) {
	USART_SendData(USART3, 0xAA);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)== RESET);
	USART_SendData(USART3, 0x55);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)== RESET);
	}
//	U3_RCV;
//	USART_SendData(USART3, usart3_data.wrBuff[usart3_data.wrPtr++]);
}

void testUSART4(void) {    //Printer
//	u8 *buff;
	
//	buff = (u8 *)"我们";
	StartRcv(&usart4_data,0);
	timeout1 = NORMALTIMEOUT;
	usart4_data.wrBuff[0] = 0x0A;
//	usart4_data.wrBuff[1] = 'A';
//	usart4_data.wrBuff[2] = 'C';
//	usart4_data.wrBuff[3] = 'E';
//	usart4_data.wrBuff[4] = 'D';
//	usart4_data.wrBuff[1] = buff[0];
//	usart4_data.wrBuff[2] = buff[1];
//	usart4_data.wrBuff[3] = buff[2];
//	usart4_data.wrBuff[4] = buff[3];
//	strcpy((char *)usart4_data.wrBuff+1,(char *)buff);
//	sprintf((char *)usart4_data.wrBuff,"大家");
	usart4_data.wrBuff[5] = 0x0A;
	StartSend(&usart4_data,5);
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_ClearFlag(USART3, USART_FLAG_TC);
//	USART_ITConfig(UART4, USART_IT_TC, ENABLE);
//	while (1)
//		{
//		for (i=0;i<100;i++) {
//			USART_SendData(UART4, i);
//			while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
//			Delay_us(10);
//	USART_SendData(USART3, 0xAA);
//	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)== RESET);
//	USART_SendData(USART3, 0x55);
//	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)== RESET);
//		}
//	USART_SendData(UART4, 0xaa);
//	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
//	USART_SendData(UART4, 0xFF);
//	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
//	USART_SendData(UART4, 0x55);
//	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
//	USART_SendData(UART4, 0x00);
//	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	USART_SendData(UART4, usart4_data.wrBuff[0]);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	USART_SendData(UART4, usart4_data.wrBuff[1]);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	USART_SendData(UART4, usart4_data.wrBuff[2]);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	USART_SendData(UART4, usart4_data.wrBuff[3]);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	USART_SendData(UART4, usart4_data.wrBuff[4]);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	USART_SendData(UART4, usart4_data.wrBuff[5]);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	while(1);
}


void testUSART5(void) {    //Printer
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_ClearFlag(UART5, USART_FLAG_TC);
	U3_SEND;
	USART_ClearFlag(USART3, USART_FLAG_TC);
	while (1)
		{
//		for (i=0;i<100;i++) {
//			USART_SendData(UART5, i);
//			while(USART_GetFlagStatus(UART5, USART_FLAG_TC)== RESET);
//			Delay_us(10);
//		}
	USART_SendData(UART4, 0xaa);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	USART_SendData(UART4, 0x55);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	USART_SendData(UART5, 0xaa);
	while(USART_GetFlagStatus(UART5, USART_FLAG_TC)== RESET);
	USART_SendData(UART5, 0x00);
	while(USART_GetFlagStatus(UART5, USART_FLAG_TC)== RESET);
	USART_SendData(UART5, 0x55);
	while(USART_GetFlagStatus(UART5, USART_FLAG_TC)== RESET);
//	USART_SendData(UART5, 0x00);
//	while(USART_GetFlagStatus(UART5, USART_FLAG_TC)== RESET);
	USART_SendData(USART3, 0xAA);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)== RESET);
	USART_SendData(USART3, 0x55);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)== RESET);
/*	USART_SendData(UART4, 0x0a);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	USART_SendData(UART4, 'A');
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	USART_SendData(UART4, 'C');
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	USART_SendData(UART4, 'E');
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	USART_SendData(UART4, 0x0a);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
	while(1);
*/  }
}

void initIOBoard(u8 initIOB) {
	u8 i,xorVal;
	
	U2_SEND;
	USART_SendData(USART2, 3);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
	USART_SendData(USART2, 1);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
	
	xorVal = 3 ^ 1 ^ initIOB;
	for (i=0;i<240;i++){
		USART_SendData(USART2, ioInit[initIOB-1][i]);
		xorVal ^= ioInit[initIOB-1][i];
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
	}
	
	USART_SendData(USART2, xorVal);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
	U2_RCV;
	StartRcv(&usart2_data,1);
	
}

void resetIOBoard(u8 initIOB) {
	
	U2_SEND;
	USART_SendData(USART2, 2);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
	USART_SendData(USART2, 2 ^ initIOB);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
	
	U2_RCV;
	StartRcv(&usart2_data,1);
	
}

void printRec(u8 tt, IOSTAT *rec) {
	u8 i,printBuff[256],tmpVal,prPosi,prLen=0;
	
	ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),rec->dNum-1);
	tmpVal=rec->dType;
	switch (tt) {
		case 1:  //故障
			cpChinese2Str(printBuff, (u8*)" 故障：", 7);
			prPosi=6;
			if (rec->mNum == 0) prPosi=0;   //cpChinese2Str(printBuff+7, (u8*)"单机  ", 6);
			else if (rec->mNum == 1) cpChinese2Str(printBuff+7, (u8*)"主机  ", 6);
			else { 
				sprintf((char *)(printBuff+7),"%02d",rec->mNum);
				cpChinese2Str(printBuff+9, (u8*)"从机", 4);
			}
			
			
			sprintf((char *)(printBuff + prPosi + 7),"%02d",rec->bNum);
			cpChinese2Str(printBuff+prPosi+9, (u8*)"号板", 4);
			if (rec->eventType<100) {
					sprintf((char *)(printBuff+prPosi+13),"%03d",rec->dNum);
					cpChinese2Str(printBuff+prPosi+16, (u8*)dotType(rec->eventType,tmpVal,0),8);
					printBuff[prPosi+24]=0x0A;
			
					sprintf((char *)(printBuff+prPosi+25),"   %02d/%02d/%02d %02d:%02d:%02d ",rec->year,rec->mon,rec->day,rec->hour,rec->min,rec->sec); //18+3=21
					cpChinese2Str(printBuff+prPosi+46, (u8*)typeFullString(tmpVal,rec->eventType),8);
					printBuff[prPosi+54]=0x0A;
			
					sprintf((char *)(printBuff+prPosi+55),"   位置: "); //9
					cpChinese2Str(printBuff+prPosi+64,oneNode.addr,12);
					printBuff[prPosi+76]=0x0A;
					sprintf((char *)(printBuff+prPosi+77),"-------------------------"); //25
					printBuff[prPosi+102]=0x0A;
					prLen = prPosi+103;
			}
			else if (rec->eventType>=200 && rec->eventType<208) {
					switch (rec->eventType-200) {
						case 2:
							cpChinese2Str(printBuff+prPosi+13, (u8*)"主电故障",8);
							break;

						case 3:
							cpChinese2Str(printBuff+prPosi+13, (u8*)"备电故障",8);
							break;

						case 4:
							cpChinese2Str(printBuff+prPosi+13, (u8*)"充电故障",8);
							break;

						case 5:
							cpChinese2Str(printBuff+prPosi+13, (u8*)"辅助输出短路",12);
							prPosi +=4;
							break;
					}
					printBuff[prPosi+21]=0x0A;
			
					sprintf((char *)(printBuff+prPosi+22),"   %02d/%02d/%02d %02d:%02d:%02d ",rec->year,rec->mon,rec->day,rec->hour,rec->min,rec->sec); //18+3=21
					printBuff[prPosi+43]=0x0A;
			
					sprintf((char *)(printBuff+prPosi+44),"-------------------------"); //25
					printBuff[prPosi+69]=0x0A;
					prLen = prPosi+70;
			}
			else if (rec->eventType==211 || rec->eventType==210) {
					switch (InterBoard[rec->bNum].Type) {
						case IOBOARD :
							if (rec->eventType==211) cpChinese2Str(printBuff+prPosi+13, (u8*)"回路板通讯故障",14);
							if (rec->eventType==210) cpChinese2Str(printBuff+prPosi+13, (u8*)"回路板短路故障",14);
							break;

						case POWERBOARD :
							cpChinese2Str(printBuff+prPosi+13,(u8*)"电源板通讯故障",14);
							break;

						case FIREBOARD :
							cpChinese2Str(printBuff+prPosi+13,(u8*)"灭火板通讯故障",14);
							break;

						case MULTILINEBOARD :
							cpChinese2Str(printBuff+prPosi+13, (u8*)"多线盘通讯故障",14);
							break;

						default :
							cpChinese2Str(printBuff+prPosi+13, (u8*)"未知板通讯故障",14);
							break;
					}
					printBuff[prPosi+27]=0x0A;
			
					sprintf((char *)(printBuff+prPosi+28),"   %02d/%02d/%02d %02d:%02d:%02d ",rec->year,rec->mon,rec->day,rec->hour,rec->min,rec->sec); //18+3=21
					printBuff[prPosi+49]=0x0A;
			
					sprintf((char *)(printBuff+prPosi+50),"-------------------------"); //25
					printBuff[prPosi+75]=0x0A;
					prLen = prPosi+76;
			}
			break;
	}
	if (prLen>0) {
		USART_ClearFlag(UART4, USART_FLAG_TC);
		for (i=0;i<prLen;i++) {
			USART_SendData(UART4, printBuff[i]);
			while(USART_GetFlagStatus(UART4, USART_FLAG_TC)== RESET);
		}
	}
}
