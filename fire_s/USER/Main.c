#include "Main.h"

#define  OS_GLOBALS
#include "GloablVar.h"

#include "rtc_time.h"
extern IOSTAT  ioStat[5000];   //共5类 故障 火警 联动 报警 屏蔽
extern u8 ioInit[20][240];

void FSMC_SRAM_Init(void);
void Test_RAM(void);
void Test_NORFlash(void);
void LCD_Test(void);
void TIM2_Base_Init(u16 Count);
void Disp_Screen(void);
void StartRcv(USART_DATA *usart_data, u8 n);
void initIOBoard(u8 initIOB);
void resetIOBoard(u8 initIOB);



void IOPORT_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  //打开PB口时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);
	//PB5,PE5引脚设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_3 | GPIO_Pin_4;
	//端口速度
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	//端口模式，此为输出推挽模式
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//初始化对应的端口
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  //14 按键音
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // 控制485收发
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	//端口速度
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  // 控制USART3收发
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOF, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

void ZLG7289_DIO(u8 tt)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  if (tt == 1) {
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	else {
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
  }
}

void WriteConf(void) {
	FSMC_NOR_EraseBlock(STARTBLOCK);
	FSMC_NOR_WriteBuffer((u8 *)&myConf, STARTBLOCK, sizeof(CONFIGURATION0));
	FSMC_NOR_EraseBlock(STARTBLOCK+128);
	FSMC_NOR_WriteBuffer((u8 *)&myConf1, STARTBLOCK+128, sizeof(CONFIGURATION1));
}

void GetMyConf(void) {
	
	FSMC_NOR_ReadBuffer((u8 *)&myConf, STARTBLOCK, sizeof(CONFIGURATION0));
	FSMC_NOR_ReadBuffer((u8 *)&myConf1, STARTBLOCK+128, sizeof(CONFIGURATION1));
	if (myConf.validConf != 0x55AA) {
	
  myConf.validConf = 0x55AA;
	// USART1 Configuration Computer
	myConf.usart1_config.USARTPORT = USART1;
	myConf.usart1_config.Enabled = 1;
	myConf.usart1_config.Config.USART_BaudRate = 19200;
	myConf.usart1_config.Config.USART_StopBits = USART_StopBits_1;
	myConf.usart1_config.Config.USART_Parity = USART_Parity_No;
	myConf.usart1_config.Config.USART_WordLength = USART_WordLength_8b;
	myConf.usart1_config.Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	myConf.usart1_config.Config.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

// USART2 Configuration Internal
	myConf.usart2_config.USARTPORT = USART2;
	myConf.usart2_config.Enabled = 1;
	myConf.usart2_config.Config.USART_BaudRate = 19200;
	myConf.usart2_config.Config.USART_StopBits = USART_StopBits_1;
	myConf.usart2_config.Config.USART_Parity = USART_Parity_No;
	myConf.usart2_config.Config.USART_WordLength = USART_WordLength_9b;
	myConf.usart2_config.Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	myConf.usart2_config.Config.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

// USART3 Configuration Primary and Secondary
	myConf.usart3_config.USARTPORT = USART3;
	myConf.usart3_config.Enabled = 1;
	myConf.usart3_config.Config.USART_BaudRate = 9600;
	myConf.usart3_config.Config.USART_StopBits = USART_StopBits_1;
	myConf.usart3_config.Config.USART_Parity = USART_Parity_No;
	myConf.usart3_config.Config.USART_WordLength = USART_WordLength_9b;
	myConf.usart3_config.Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	myConf.usart3_config.Config.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

// USART4 Configuration Printer
	myConf.usart4_config.USARTPORT = UART4;
	myConf.usart4_config.Enabled = 1;
	myConf.usart4_config.Config.USART_BaudRate = 9600;
	myConf.usart4_config.Config.USART_StopBits = USART_StopBits_1;
	myConf.usart4_config.Config.USART_Parity = USART_Parity_No;
	myConf.usart4_config.Config.USART_WordLength = USART_WordLength_8b;
	myConf.usart4_config.Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	myConf.usart4_config.Config.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

// USART5 Configuration  Lou Xian
	myConf.usart5_config.USARTPORT = UART5;
	myConf.usart5_config.Enabled = 1;
	myConf.usart5_config.Config.USART_BaudRate = 9600;
	myConf.usart5_config.Config.USART_StopBits = USART_StopBits_1;
	myConf.usart5_config.Config.USART_Parity = USART_Parity_No;
	myConf.usart5_config.Config.USART_WordLength = USART_WordLength_8b;
	myConf.usart5_config.Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	myConf.usart5_config.Config.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  myConf1.machineID = 0x1234;
	myConf1.machineVersion = 0x10;

	myConf1.password[0]=KEY01;
	myConf1.password[1]=KEY02;
	myConf1.password[2]=KEY03;
	myConf1.password[3]=KEY04;
	myConf1.password[4]=0;
	myConf1.password[5]=0;
	myConf1.password[6]=0;
	myConf1.password[7]=0;
	myConf1.password[8]=0;
	myConf1.password[9]=0;
	myConf1.suPassword[0]=KEY05;
	myConf1.suPassword[1]=KEY06;
	myConf1.suPassword[2]=KEY07;
	myConf1.suPassword[3]=KEY08;
	myConf1.suPassword[4]=0;
	myConf1.suPassword[5]=0;
	myConf1.suPassword[6]=0;
	myConf1.suPassword[7]=0;
	myConf1.suPassword[8]=0;
	myConf1.suPassword[9]=0;
	
	myConf1.mNum = 0;						//单机运行
	myConf1.maxMac = 0;					//从机数量 0
	myConf1.mRunningStat = 0;   //调试态 0 运行态 1  探测节点用
	myConf1.mAutomation = 0;    //手动 0 自动 1  是否触发声光
	myConf1.enPrint = 0;
	myConf1.isFullFireB = 0;
	myConf1.isFullErrB = 0;
	myConf1.isFullIOB = 0;
	myConf1.isFullPingBiB = 0;
	myConf1.isFullActB = 0;
	WriteConf();
	}

}

void Init_USART(void){
	
	Init5USART(0); // Init all 5 USART

}

void SetDeault_InterBoard(void)
{
	sprintf (InterBoard[1].Name, "1");
	InterBoard[1].Type = IOBOARD;
	InterBoard[1].State = ENABLE;

	sprintf (InterBoard[21].Name, "1");
	InterBoard[21].Type = POWERBOARD;
	InterBoard[21].State = ENABLE;

	sprintf (InterBoard[31].Name, "1");
	InterBoard[31].Type = FIREBOARD;
	InterBoard[31].State = DISABLE;
	sprintf (InterBoard[32].Name, "2");
	InterBoard[32].Type = FIREBOARD;
	InterBoard[32].State = DISABLE;

	sprintf (InterBoard[41].Name, "1");
	InterBoard[41].Type = MULTILINEBOARD;
	InterBoard[41].State = DISABLE;
	
}

extern unsigned char GUI_F8x8_acFont[][8];
extern unsigned char GUI_F8x16_acFont[][16];
void init_main(){
	u8 i,j,k,tmpC[16];
	
	
//  横向字库转纵向
	for (i=0;i<128;i++) {
		for (j=0;j<8;j++) {
			tmpC[j]=0;
			for (k=0;k<8;k++)
				if ((GUI_F8x8_acFont[i][k] & (1<< j)) >0) tmpC[j] |= (1 << k);
		}
		for (j=0;j<8;j++) 
			GUI_F8x8_acFont[i][j]=tmpC[j];
	}

	for (i=0;i<95;i++) {
		for (j=0;j<16;j++) {
			tmpC[j]=0;
			for (k=0;k<8;k++)
				if ((GUI_F8x16_acFont[i][k+8*(j/8)] & (1<< j%8)) >0) tmpC[j] |= (1 << k);
		}
		for (j=0;j<16;j++) 
			GUI_F8x16_acFont[i][j]=tmpC[j];
	}

	GetMyConf();
	Init_USART();
	stateCPU = RESETTING;
	isComputer = 0;
	machineDate = Time_GetCalendarTime();

	for (i=0;i<10;i++) {typeTot[i]=0;typePingbi[i]=0;}

//	for (i=0;i<MAXIOBOARD;i++)
	for (j=0;j<240;j++) {									//单回路板读入点配置
		ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),j);
		if (oneNode.type>0 && oneNode.type <=8) {
			typeTot[0]++;
			typeTot[oneNode.type]++;
			if ((oneNode.stat & 0x80)==0) ioInit[0][j]=oneNode.type;
			else { ioInit[0][j]=0x00; typePingbi[oneNode.type]++;}
			if ((oneNode.stat & 0x80)==0x80) SetpinbiStat(myConf1.mNum, 1, j+1, 1, 1);
		}
  }		
	
	moveNextIB(1,0);
	i = ptrIB;
	while ((stateCPU == INITING || stateCPU == RESETTING) && i < 21) {
		for (j=1; j<=5 ; j++){
			if ((ifDealUSART[j] & 0x01) == 0x01) { ifDealUSART[j] &= 0xFE;DealFinishRead(j); }
			if ((ifDealUSART[j] & 0x02) == 0x02) { ifDealUSART[j] &= 0xFD;DealFinishSend(j); }
		}
		
		if (timeout2 == 0xFF) {       //通讯故障
			initIOB=0;
			moveNextIB(1,0);
			i = ptrIB;
		}
		if (i<21 && initIOB >= 1 && initIOB <= 20 && stateCPU == INITING) {    //回路板复位
			initOver = 0;
			initIOBoard(initIOB);
			timeout2=NORMALTIMEOUT;
			while (initOver == 0 && timeout2 != 0xFF)
				if ((ifDealUSART[2] & 0x01) == 0x01) { ifDealUSART[2] &= 0xFE;DealFinishRead(2); }
			if (timeout2 == 0xFF) {       //通讯故障
			}
			initIOB=0;
			moveNextIB(1,0);
			i = ptrIB;
		}

		if (i<21 && initIOB >= 1 && initIOB <= 20 && stateCPU == RESETTING) {    //回路板初始化
			initOver = 0;
			resetIOBoard(initIOB);
			timeout2=NORMALTIMEOUT;
			while (initOver == 0 && timeout2 != 0xFF)
				if ((ifDealUSART[2] & 0x01) == 0x01) { ifDealUSART[2] &= 0xFE;DealFinishRead(2); }
			if (timeout2 == 0xFF) {       //通讯故障
			}
			initIOB=0;
			moveNextIB(1,0);
			i = ptrIB;
		}

		if (i>20 && stateCPU == RESETTING) {
			Delay_ms(400);
			ptrIB=0;
			moveNextIB(1,0);
			i = ptrIB;
			stateCPU = INITING;
		}
	}
	Delay_ms(400);
	stateCPU = RUNNING;
}

u32 readBlock;
void ShowChar16 (Uchar ll, Uchar rr, Uchar *Char);
u8 FSMC_NOR_Read(u32 ReadAddr);
void	Deal_Key(u8 k);



void main_loop()
{
  u8 i,j,newkey;
	u8 charShow[32];
	
	loopIO = 0;
  ptrIB = 0;
	if (stateCPU == RUNNING) moveNextIB(1,0);
	timeout2=NORMALTIMEOUT;
//	testUSART1();
//	testUSART4();
//	testUSART3();
//	testUSART5();
	readBlock=0xFFFF;
	oldScr = 0;
	newScr = 1;
	dispLine=0;
	newTab = 4;
	LedNew05=0;
	LedNew06=0;
	keyValue=0xFF;
	totRecoverEvt=0;
	ZLG7289_Download(2,5,0,LedNew05);
	ZLG7289_Download(2,6,0,LedNew06);
	RESETBIT(loopU3);
	RESETBIT(IsWindow);
	myStat1=0;
	myStat2=0;
	USART3MainStat=0;
	if (myConf1.mNum >1) {
//		USART_SetAddress (USART3, myConf1.mNum); // set address usart
//		USART_WakeUpConfig (USART3, USART_WakeUp_AddressMark); // wakeup mode
//		USART_ReceiverWakeUpCmd (USART3, ENABLE); // usart is mute
		StartRcv(&usart3_data,0);
		U3_RCV;  //从机工作于接收模式
	}
	if (myConf1.mNum == 1) {
		startQmNum=1;
	}
	else startQmNum=0;
	sendStep=0;
	windowNum=0;
	windowSelect=0;
//	StartRcv(&usart1_data,0);			
  while (1) {
		
		//Deal LCD
		if (ISSET(IsWindow)) {
			if (windowNum>0) {
				if (windowSelect /16 != windowSelect % 16) ShowWindow(windowNum,0);
			}
		}
		else if (ISSET(IsWindowShowed)) {
				SETBIT(IsWindow);
				Deal_Key(KEYESC);
		}
		else if (oldScr != newScr || oldTab != newTab || oldLine != newLine || newTime != 0 
			 || flashLine != 0 || updateLine != 0 || dispLine !=0)
			Disp_Screen();

		if (loopIO == 1 && ptrIB == 1) { loopIO =0;IBSendCmd(ptrIB,InterBoard[ptrIB].Type, stepIBSend); timeout2=NORMALTIMEOUT;}
		
		
		USART3MainStat=DealGetStat();          //处理从机状态

		if (ISSET(loopU3) && myConf1.mNum == 1 && USART3MainStat==0) { //主机查从机
			RESETBIT(loopU3);
			startQmNum++;
			if (startQmNum > myConf1.maxMac + 1) startQmNum=2;
			usart3_data.wrBuff[0] = 0x100|startQmNum;
			sendU3(usart3_data.wrBuff,1,0);
			sendStep=1;
			StartRcv(&usart3_data,0);
			U3_RCV;
			timeout3=NORMALTIMEOUT;
		}   

		for (i=1; i<=5 ; i++){
			if ((ifDealUSART[i] & 0x01) == 0x01) { ifDealUSART[i] &= 0xFE;DealFinishRead(i); }
			if ((ifDealUSART[i] & 0x02) == 0x02) { ifDealUSART[i] &= 0xFD;DealFinishSend(i); }
		}
		
		if (timeout2 == 0xFF && ISRESET(is2Sended)) {        //通讯故障
			SETBIT(is2Sended);
      timesSend ++ ;
      if (timesSend>5) {
				timesSend=0;
				if (SeterrStat(myConf1.mNum, ptrIB, 0, 0, 211) ==1 ) {  //通讯故障
						nowLine[4] = 4;
						dispLine=4;
					}
				moveNextIB(1,0);
			}
			else {
				IBSendCmd(ptrIB,InterBoard[ptrIB].Type, stepIBSend);
				timeout2=NORMALTIMEOUT;
			}
		}
		
		if (timeout2==0) timesSend=0;
		

		if (readBlock != 0xFFFF) {
			for (j=0 ;j<8;j++) {
				for(i=0;i<32;i++) {
					charShow[i]=FSMC_NOR_Read(readBlock*256+i+j*32);
				}

				ShowChar16 (5, j*16, charShow);
			}
			readBlock = 0xFFFF;
		}

		//Deal Key
		if (ISSET(Scankey) && stopkeytime==0) {
			RESETBIT(Scankey);
			newkey=0xFF;
			newkey = ZLG7289_Key();
	
	    if  (newkey != keyValue) { keyValue = newkey; keyCount=0; }
	    else  keyCount++;
		  if (keyCount > 1 ) {
			  keyCount = 0;
			  if (keyValue != 0xFF) {
//					KEYVOICE_ON;
					keyVoice=KEYVOICETIME;
//				  strHex(charShow,keyValue);
//			    ShowString8 (0, 0, charShow,2 ,0);
					Deal_Key(keyValue);
					keyValue = 0xFF;
					stopkeytime=STOPKEYTIME;
					if (nowLine[4]>6 || newScr>2) setupTime = SETUPTIMEOUT;       //处于设置状态,注意检查是否有其他显示用于设置状态
			  }
		  }
		}

		//Deal Date Time
    if (ISSET(ifGetTime)) { RESETBIT(ifGetTime); machineDate = Time_GetCalendarTime(); if (oldScr==1 && nowLine[4]!=100) newTime = 1;}
	  
	  
		//deal LED
		if (Led05 != LedNew05) { Led05 = LedNew05; ZLG7289_Download(2,5,0,LedNew05);}
	  if (Led06 != LedNew06) { Led06 = LedNew06; ZLG7289_Download(2,6,0,LedNew06);}
	}	
}


u8 RTC_Init(void);

void RCC_Init(void)  
{  
    ErrorStatus      HSEStartUpStatus;  
    RCC_DeInit();     
    RCC_HSEConfig(RCC_HSE_ON);   //设置外部高速晶振（HSE）  
    HSEStartUpStatus = RCC_WaitForHSEStartUp();     //等待HSE起振  
    if(HSEStartUpStatus == SUCCESS)     //SUCCESS：HSE晶振稳定且就绪  
    {  
        RCC_HCLKConfig(RCC_SYSCLK_Div1);  //设置AHB时钟（HCLK）,RCC_SYSCLK_Div1——AHB时钟 = 系统时钟  
        RCC_PCLK2Config(RCC_HCLK_Div1); //设置高速AHB时钟（PCLK2）,RCC_HCLK_Div1——APB2时钟 = HCLK  
        RCC_PCLK1Config(RCC_HCLK_Div2);  //设置低速AHB时钟（PCLK1）,RCC_HCLK_Div2——APB1时钟 = HCLK / 2  
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_2);      
            // 设置PLL时钟源及倍频系数,PLL的输入时钟 = HSE时钟频率；RCC_PLLMul_2——PLL输入时钟x 2  
        RCC_PLLCmd(ENABLE);//使能PLL     
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//检查指定的RCC标志位(PLL准备好标志)设置与否      
          
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);   
            //设置系统时钟（SYSCLK）,RCC_SYSCLKSource_PLLCLK——选择PLL作为系统时钟,11.0592x2  
        while(RCC_GetSYSCLKSource() != 0x08);     //PLL返回用作系统时钟的时钟源,0x08：PLL作为系统时钟        
    }  
//    RCC_GetClocksFreq(&RCC_ClocksStatus);  
}  

int main(void)
{
//	u16 aa = sizeof(USART_CONFIG);
	//	RCC_Init();
	SysTick_Init();
	
	while(RTC_Init());
	//RTC初始化	，一定要初始化成功
	FSMC_SRAM_Init();
	TIM2_Base_Init(1000);  //10ms中断
	IOPORT_Init();
	LCD_initial();
	
	
	LCD_BON;


		ZLG7289_Init(250);
//	LedRev05 = 0x03;
//	LedRev06 = 0x03;
//	Test_RAM();
	
//	while(1);
//  aa = sizeof(CONFIGURATION0);
//	myConf1.machineID = aa;
//  aa = sizeof(CONFIGURATION1);
//	myConf1.machineID = aa;
	SetDeault_InterBoard();
	init_main();
//	LCD_Test();
//	while(1);
	
	main_loop();
	return 0;
}
