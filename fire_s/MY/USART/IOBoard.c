#include "USART.h"
#include "GloablVar.h"
#define TOTALRECORD 120

IOSTAT  ioStat[121];    //共5类 报警
IOSTAT  fireStat[121] ;   //共5类 火警
IOSTAT  pinbiStat[121] ;   //共5类 屏蔽
IOSTAT  errStat[121] ;   //共5类 故障
IOSTAT  actStat[1];   //共5类 联动
VOICEIO voiceIO[100],recoverEvt[100];
u8 ioInit[MAXIOBOARD][240]; //回路板初始化

u8 DealGetStat(void)
{
	if (GetStat==0) return 0;
	if (ISSET(U3StatDealing)) return 1;
	
	while ((GetStat & 0x01) == 0) {
		GetStat = GetStat >> 1;
		StatStep++;
	}

	GetStat= GetStat >> 1;
	StatStep++;
	SETBIT(U3StatDealing);
	sendStep=4;
	
	switch(StatStep) {
		case 1:                  //有恢复报警
			usart3_data.wrBuff[0]=219;
			usart3_data.wrBuff[1]=8;
			USART3MainCmd=34;
			usart3_data.wrBuff[2]=0x55;
			usart3_data.wrBuff[3]=0xFF;
			sendU3(usart3_data.wrBuff,4,0);
			StartRcv(&usart3_data,0);
			U3_RCV;
			break;
		case 2:                  //有新报警
			usart3_data.wrBuff[0]=219;
			usart3_data.wrBuff[1]=7;
			USART3MainCmd=34;
			usart3_data.wrBuff[2]=0x55;
			usart3_data.wrBuff[3]=0xFF;
			sendU3(usart3_data.wrBuff,4,0);
			StartRcv(&usart3_data,0);
			U3_RCV;
			break;
		case 3:                  //有恢复反馈
			usart3_data.wrBuff[0]=219;
			usart3_data.wrBuff[1]=6;
			USART3MainCmd=34;
			usart3_data.wrBuff[2]=0x55;
			usart3_data.wrBuff[3]=0xFF;
			sendU3(usart3_data.wrBuff,4,0);
			StartRcv(&usart3_data,0);
			U3_RCV;
			break;
		case 4:                  //有新反馈
			usart3_data.wrBuff[0]=219;
			usart3_data.wrBuff[1]=5;
			USART3MainCmd=34;
			usart3_data.wrBuff[2]=0x55;
			usart3_data.wrBuff[3]=0xFF;
			sendU3(usart3_data.wrBuff,4,0);
			StartRcv(&usart3_data,0);
			U3_RCV;
			break;
		case 5:                  //有恢复输出
			usart3_data.wrBuff[0]=219;
			usart3_data.wrBuff[1]=4;
			USART3MainCmd=34;
			usart3_data.wrBuff[2]=0x55;
			usart3_data.wrBuff[3]=0xFF;
			sendU3(usart3_data.wrBuff,4,0);
			StartRcv(&usart3_data,0);
			U3_RCV;
			break;
		case 6:                  //有新输出
			usart3_data.wrBuff[0]=219;
			usart3_data.wrBuff[1]=3;
			USART3MainCmd=34;
			usart3_data.wrBuff[2]=0x55;
			usart3_data.wrBuff[3]=0xFF;
			sendU3(usart3_data.wrBuff,4,0);
			StartRcv(&usart3_data,0);
			U3_RCV;
			break;
		case 7:                  //有新火警
			usart3_data.wrBuff[0]=219;
			usart3_data.wrBuff[1]=2;
			USART3MainCmd=34;
			usart3_data.wrBuff[2]=0x55;
			usart3_data.wrBuff[3]=0xFF;
			sendU3(usart3_data.wrBuff,4,0);
			StartRcv(&usart3_data,0);
			U3_RCV;
			break;
		case 8:                  //保留
			break;
		case 9:                  //有恢复故障
			usart3_data.wrBuff[0]=219;
			usart3_data.wrBuff[1]=34;
			USART3MainCmd=34;
			usart3_data.wrBuff[2]=0x55;
			usart3_data.wrBuff[3]=0xFF;
			sendU3(usart3_data.wrBuff,4,0);
			StartRcv(&usart3_data,0);
			U3_RCV;
			break;
		case 10:                  //有新故障
			usart3_data.wrBuff[0]=219;
			usart3_data.wrBuff[1]=33;
			USART3MainCmd=33;
			usart3_data.wrBuff[2]=0x55;
			usart3_data.wrBuff[3]=0xFF;
			sendU3(usart3_data.wrBuff,4,0);
			StartRcv(&usart3_data,0);
			U3_RCV;
			break;
		case 11:                  //或控制分区状态有变化
			break;
		case 12:                  //与控制分区状态有变化
			break;
		case 13:                  //曾经复位
			break;
		case 14:                  //正在自检
			break;
		case 15:                  //消火栓泵反馈有变化
			break;
		case 16:                  //保留
			break;
	}
	
	return 1;
}


void AddVoiceIO(u8 tt,u8 bn, u8 dn, u8 it, u8 et)
{
u16 iptr;	
	
	if (totVoice>0) {
		for (iptr=0;iptr<totVoice;iptr++) {
			if ( voiceIO[iptr].type == tt && voiceIO[iptr].bNum == bn && voiceIO[iptr].dNum == dn && voiceIO[iptr].ioType == it && voiceIO[iptr].eventType == et )
				return;
		}
		wrVoice++;
	}
	else wrVoice=0;
	totVoice++;
	if (wrVoice>=100) { totVoice=100;wrVoice=0;}
	if (totVoice>100) totVoice=100;
	voiceIO[wrVoice].type = tt;
	voiceIO[wrVoice].bNum = bn;
	voiceIO[wrVoice].dNum = dn;
	voiceIO[wrVoice].ioType = it;
	voiceIO[wrVoice].eventType = et;
}

void AddRecoverEvt(u8 tt,u8 bn,u8 dn,u8 it,u8 et)
{
u16 iptr;	
	
	if (totRecoverEvt>0) {
		for (iptr=0;iptr<totRecoverEvt;iptr++) {
			if ( recoverEvt[iptr].type == tt && recoverEvt[iptr].bNum == bn && recoverEvt[iptr].dNum == dn && recoverEvt[iptr].ioType == it && recoverEvt[iptr].eventType == et )
				return;
		}
	}
	if (totRecoverEvt<100) {
		recoverEvt[totRecoverEvt].type = tt;
		recoverEvt[totRecoverEvt].bNum = bn;
		recoverEvt[totRecoverEvt].dNum = dn;
		recoverEvt[totRecoverEvt].ioType = it;
		recoverEvt[totRecoverEvt].eventType = et;
		totRecoverEvt++;
	}
}

void RemoveVoiceIO(u8 tt,u8 bn, u8 dn, u8 it, u8 et)
{
u16 iptr;	
	
  if (totVoice>0) {
		
		for (iptr=0;iptr<totVoice && (voiceIO[iptr].type !=tt || voiceIO[iptr].bNum !=bn || voiceIO[iptr].dNum != dn || voiceIO[iptr].ioType!=it || voiceIO[iptr].eventType != et) ;iptr++);
		if (iptr<totVoice) {
			for (;iptr<totVoice;iptr++)
				voiceIO[iptr]=voiceIO[iptr+1];
			totVoice--;
			wrVoice=totVoice;
			if (totVoice==0) {
				RESETBIT(lSEnable);
				RESETBIT(mSEnable);
				RESETBIT(hSEnable);
				SOUND_OFF;
			}
		}
	}
}

void DealIOStat(u8 bNum, USART_DATA *usart_data) {
	u8 tmpVal,i,j;
	
	if ((usart_data->rdBuff[1] & 0x01) !=0) SeterrStat(myConf1.mNum, bNum, 0, 0, 210);
	else DelerrStat(myConf1.mNum,bNum, 0, 0, 210);
	
	if ((usart_data->rdBuff[1] & 0x02) !=0) {
		for (i=0;i<240;i++) {
			if (oldStatIOB[bNum-1][i] != usart_data->rdBuff[i+2]) {
				tmpVal= oldStatIOB[bNum-1][i] ^ usart_data->rdBuff[i+2];
				j=0;
				while (tmpVal!=0) {
					if ((tmpVal & 0x01) !=0) { 
						if ((ioInit[bNum-1][i] == 4 && j==5) || (ioInit[bNum-1][i] == 1 && j==5) || (ioInit[bNum-1][i] == 2 && j==5) || (ioInit[bNum-1][i] == 3 && (j==5||j==6))
								|| (ioInit[bNum-1][i] == 1 && j==5)) {
							if (((usart_data->rdBuff[i+2] >> j) & 0x01) == 1) { SetfireStat(myConf1.mNum, bNum, i+1, 0, j); }
//							else SeterrStat(myConf1.mNum, bNum, i+1, 0, j);
						}
						else if ((ioInit[bNum-1][i] == 5 && j==5) || (ioInit[bNum-1][i] == 6 && j==5) || (ioInit[bNum-1][i] == 7 && (j==5 || j==6)) || (ioInit[bNum-1][i] == 8 && j==5)) {
							if (((usart_data->rdBuff[i+2] >> j) & 0x01) == 1) { SetioStat(myConf1.mNum, bNum, i+1, 0, j); }
							else { DelioStat(myConf1.mNum, bNum, i+1, 0, j); }
						}
						else	{				
							if (((usart_data->rdBuff[i+2] >> j) & 0x01) == 1) { SeterrStat(myConf1.mNum, bNum, i+1, 0, j); }
							else { DelerrStat(myConf1.mNum,bNum, i+1, 0, j); }
						}
					}						
					j++;
					tmpVal = tmpVal >> 1;
				}
				oldStatIOB[bNum-1][i] = usart_data->rdBuff[i+2];
				nowLine[4] = 4;
				dispLine=4;
			}
		}
	}
}

u8 DelerrStat(u8 mn,u8 bn, u8 dn, u8 it, u8 et) {
u16 iptr;	
	
  if (totErr>0) {
		
		for (iptr=1;iptr<totErr+1 && (errStat[iptr].mNum !=mn || errStat[iptr].bNum !=bn || errStat[iptr].dNum != dn || errStat[iptr].ioType!=it || errStat[iptr].eventType != et) ;iptr++);
		if (iptr<totErr+1) {
			if ((errStat[iptr].stat & 0x01)==0x01) {
				AddRecoverEvt(1,bn,dn,it,et);
				myStat2 |= 0x01;
			}

			for (;iptr<totErr;iptr++)
				errStat[iptr]=errStat[iptr+1];
			totErr--;
			wrErr=totErr;
			if (dispErr>wrErr) dispErr=wrErr;
			if (nowLine[4] == 4)  dispLine=4;
			if (totErr==0) ERROROFF;
			if (oldScr<3) {
				if (totAct>0) 
					nowLine[4]=5;
				else if (totErr>0) 
					nowLine[4]=4;
				else 
					nowLine[4]=2;
				dispLine=4;
			}
		}
		RemoveVoiceIO(1,bn,dn,it,et);
	}
	return 1;
}


u8 SeterrStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et) {
u16 iptr;
u8 tmpVal;	
	
	for (iptr=1;iptr<=totErr;iptr++) {
		if ( errStat[iptr].mNum ==mn && errStat[iptr].bNum == bn && errStat[iptr].dNum == dn && errStat[iptr].ioType == it && errStat[iptr].eventType == et )
			return 0;
	}
	if (ISRESET(lSEnable)) { SETBIT(lSEnable); lSound=0; }
	ERRORON;
	AddVoiceIO(1,bn,dn,it,et);
	totErr++;
	wrErr++;
	myStat2 |= 0x02;
	if (wrErr >TOTALRECORD) { totErr = TOTALRECORD; wrErr=1; }
	if (totErr >TOTALRECORD) totErr = TOTALRECORD;
	errStat[wrErr].mNum = mn;
	errStat[wrErr].bNum = bn;
	errStat[wrErr].dNum = dn;
	errStat[wrErr].dType = ioInit[bn-1][dn-1];
	errStat[wrErr].ioType = it;
	errStat[wrErr].eventType = et;
	errStat[wrErr].year = machineDate.tm_year % 100;
	errStat[wrErr].mon = machineDate.tm_mon + 1;
	errStat[wrErr].day = machineDate.tm_mday;
	errStat[wrErr].hour = machineDate.tm_hour;
	errStat[wrErr].min = machineDate.tm_min;
	errStat[wrErr].sec= machineDate.tm_sec;
	errStat[wrErr].stat= 0;
	if (myConf1.enPrint == 1) printRec(1, &errStat[wrErr]);
	tmpVal=AddFlashRecoard(ERRBLOCK,(u8 *)&errStat[wrErr],sizeof(IOSTAT),MAXERRBLOCKS);
	if (myConf1.isFullErrB != tmpVal && myConf1.isFullErrB == 0) {
		myConf1.isFullErrB = tmpVal;
		WriteConf();
	}
	dispErr=wrErr;

	if (oldScr<3) {
		if (totAct>0) 
			nowLine[4]=5;
		else if (totErr>0) 
			nowLine[4]=4;
		else 
			nowLine[4]=2;
		dispLine=4;
	}
	
	return 1;
}


u8 DelioStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et) {
u16 iptr;
	
  if (totBJ>0) {
		for (iptr=1;iptr<totBJ+1 && (ioStat[iptr].mNum !=mn || ioStat[iptr].bNum !=bn || ioStat[iptr].dNum != dn || ioStat[iptr].ioType!=it || ioStat[iptr].eventType != et) ;iptr++);
		if (iptr<totBJ+1) {
			for (;iptr<totBJ;iptr++)
				ioStat[iptr]=ioStat[iptr+1];
			totBJ--;
			wrBJ=totBJ;
			if (dispBJ>wrBJ) dispBJ=wrBJ;
			if (nowLine[3] == 1)  { dispLine=3; oldTab =3; }
		}
		RemoveVoiceIO(2,bn,dn,it,et);
	}
	return 1;
}

u8 SetioStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et) {
u16 iptr;
u8 tmpVal;	
	
	for (iptr=1;iptr<=totBJ;iptr++) {
		if ( ioStat[iptr].mNum == mn && ioStat[iptr].bNum == bn && ioStat[iptr].dNum == dn && ioStat[iptr].ioType == it && ioStat[iptr].eventType == et )
			return 0;
	}
	if (ISRESET(mSEnable)) { mSound=0; SETBIT(mSEnable);}
	
	if (nowLine[3] == 1)  { dispLine=3; oldTab =3; }
	totBJ++;
	wrBJ++;
	AddVoiceIO(2,bn,dn,it,et);
	if (wrBJ >TOTALRECORD) { totBJ = TOTALRECORD; wrBJ=1; }
	if (totBJ >TOTALRECORD) totBJ = TOTALRECORD;
	ioStat[wrBJ].mNum = mn;
	ioStat[wrBJ].bNum = bn;
	ioStat[wrBJ].dNum = dn;
	ioStat[wrBJ].dType = ioInit[bn-1][dn-1];
	ioStat[wrBJ].ioType = it;
	ioStat[wrBJ].eventType = et;
	ioStat[wrBJ].year = machineDate.tm_year % 100;
	ioStat[wrBJ].mon = machineDate.tm_mon + 1;
	ioStat[wrBJ].day = machineDate.tm_mday;
	ioStat[wrBJ].hour = machineDate.tm_hour;
	ioStat[wrBJ].min = machineDate.tm_min;
	ioStat[wrBJ].sec= machineDate.tm_sec;
	ioStat[wrBJ].stat= 0;
	dispBJ=wrBJ;
	tmpVal=AddFlashRecoard(IOBLOCK,(u8 *)&ioStat[wrBJ],sizeof(IOSTAT),MAXIOBLOCKS);
	if (myConf1.isFullIOB != tmpVal && myConf1.isFullIOB == 0) {
		myConf1.isFullIOB = tmpVal;
		WriteConf();
	}
	
	return 1;
}

u8 DelfireStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et) {
u16 iptr;	
	
  if (totFire>0) {
		for (iptr=1;iptr<totFire;iptr++)
			fireStat[iptr]=fireStat[iptr+1];
		totFire--;
		wrFire=totFire;
		if (dispFire>wrFire) dispFire=wrFire;
		if (totFire==0) { newScr=1; newTab=4; FIREOFF;}
		else if (nowLine[2] == 3)  dispLine=2;
	}
	return 1;
}
u8 SetfireStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et) {
u16 iptr;
u8 tmpVal;	
	
	if (oldScr!=2) newScr=2;
	
	for (iptr=1;iptr<=totFire;iptr++) {
		if ( fireStat[iptr].mNum == mn && fireStat[iptr].bNum == bn && fireStat[iptr].dNum == dn && fireStat[iptr].ioType == it && fireStat[iptr].eventType == et )
			return 0;
	}
	if (ISRESET(hSEnable)) { hSound=0; SETBIT(hSEnable); }
	
	totFire++;
	wrFire++;
	updateLine=1;
	oldTab=2;
	FIREON;
	if (wrFire >TOTALRECORD) { totFire = TOTALRECORD; wrFire=1; }
	if (totFire >TOTALRECORD) totFire = TOTALRECORD;
	fireStat[wrFire].mNum = mn;
	fireStat[wrFire].bNum = bn;
	fireStat[wrFire].dNum = dn;
	fireStat[wrFire].dType = ioInit[bn-1][dn-1];
	fireStat[wrFire].ioType = it;
	fireStat[wrFire].eventType = et;
	fireStat[wrFire].year = machineDate.tm_year % 100;
	fireStat[wrFire].mon = machineDate.tm_mon + 1;
	fireStat[wrFire].day = machineDate.tm_mday;
	fireStat[wrFire].hour = machineDate.tm_hour;
	fireStat[wrFire].min = machineDate.tm_min;
	fireStat[wrFire].sec= machineDate.tm_sec;
	fireStat[wrFire].stat= 0;
	myStat1 |= 0x40;
	dispFire=wrFire;
	tmpVal = AddFlashRecoard(FIREBLOCK,(u8 *)&fireStat[wrFire],sizeof(IOSTAT),MAXFIREBLOCKS);
	if (myConf1.isFullFireB != tmpVal && myConf1.isFullFireB == 0) {
		myConf1.isFullFireB = tmpVal;
		WriteConf();
	}
	return 1;
}

u8 DelpinbiStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et) {
u16 iptr;	
	
  if (totPinbi>0) {
		for (iptr=1;iptr<totPinbi;iptr++)
			ioStat[iptr]=ioStat[iptr+1];
		totPinbi--;
		wrPinbi=totPinbi;
		if (dispPinbi>wrPinbi) dispPinbi=wrPinbi;
		if (oldScr<2 && nowLine[4]<7) {
				if (totAct>0) 
					nowLine[4]=5;
				else if (totErr>0) 
					nowLine[4]=4;
				else 
					nowLine[4]=2;
				dispLine=4;
		}
	}
	return 1;
}
u8 SetpinbiStat(u8 mn,u8 bn, u8 dn, u8 it, u8 et) {
u16 iptr;	
	
	for (iptr=1;iptr<=totPinbi;iptr++) {
		if ( pinbiStat[iptr].mNum == mn && pinbiStat[iptr].bNum == bn && pinbiStat[iptr].dNum == dn && pinbiStat[iptr].ioType == it && pinbiStat[iptr].eventType == et )
			return 0;
	}
	totPinbi++;
	wrPinbi++;
	if (wrPinbi >TOTALRECORD) { totPinbi = TOTALRECORD; wrPinbi=1; }
	if (totPinbi >TOTALRECORD) totPinbi = TOTALRECORD;
	pinbiStat[wrPinbi].mNum = mn;
	pinbiStat[wrPinbi].bNum = bn;
	pinbiStat[wrPinbi].dNum = dn;
	pinbiStat[wrPinbi].dType = ioInit[bn-1][dn-1];
	pinbiStat[wrPinbi].ioType = it;
	pinbiStat[wrPinbi].eventType = et;
	pinbiStat[wrPinbi].year = machineDate.tm_year % 100;
	pinbiStat[wrPinbi].mon = machineDate.tm_mon + 1;
	pinbiStat[wrPinbi].day = machineDate.tm_mday;
	pinbiStat[wrPinbi].hour = machineDate.tm_hour;
	pinbiStat[wrPinbi].min = machineDate.tm_min;
	pinbiStat[wrPinbi].sec= machineDate.tm_sec;
	pinbiStat[wrPinbi].stat= 0;
	dispPinbi=wrPinbi;
	
	if (oldScr<3  && nowLine[4]<7) {
		if (totAct>0) 
			nowLine[4]=5;
		else if (totErr>0) 
			nowLine[4]=4;
		else 
			nowLine[4]=2;
		dispLine=4;
	}
	return 1;
}

u8 DelactStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et) {
u16 iptr;	
	
  if (totAct>0) {
		for (iptr=1;iptr<totAct;iptr++)
			actStat[iptr]=actStat[iptr+1];
		totAct--;
		wrAct=totAct;
		if (dispAct>wrAct) dispAct=wrAct;
		if (nowLine[4] == 5)  dispLine=4;
		RemoveVoiceIO(3,bn,dn,it,et);
	}
	return 1;
}
u8 SetactStat(u8 mn, u8 bn, u8 dn, u8 it, u8 et) {
u16 iptr;	
	
	for (iptr=1;iptr<=totAct;iptr++) {
		if ( actStat[iptr].mNum == mn && actStat[iptr].bNum == bn && actStat[iptr].dNum == dn && actStat[iptr].ioType == it && actStat[iptr].eventType == et )
			return 0;
	}
	if (ISRESET(mSEnable)) { mSound=0; SETBIT(mSEnable); }
	totAct++;
	wrAct++;
	AddVoiceIO(3,bn,dn,it,et);
	if (wrAct >TOTALRECORD) { totAct = TOTALRECORD; wrAct=1; }
	if (totAct >TOTALRECORD) totAct = TOTALRECORD;
	actStat[wrAct].mNum = mn;
	actStat[wrAct].bNum = bn;
	actStat[wrAct].dNum = dn;
	actStat[wrAct].dType = ioInit[bn-1][dn-1];
	actStat[wrAct].ioType = it;
	actStat[wrAct].eventType = et;
	actStat[wrAct].year = machineDate.tm_year % 100;
	actStat[wrAct].mon = machineDate.tm_mon + 1;
	actStat[wrAct].day = machineDate.tm_mday;
	actStat[wrAct].hour = machineDate.tm_hour;
	actStat[wrAct].min = machineDate.tm_min;
	actStat[wrAct].sec= machineDate.tm_sec;
	actStat[wrAct].stat= 0;
	dispAct=wrAct;
	
	return 1;
}
