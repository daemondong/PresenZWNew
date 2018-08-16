/*-------------------------------------------------------------------------------
文件名称：key.c
文件描述：配置按键初始化参数       
备    注：无
---------------------------------------------------------------------------------*/
#include "key.h"
#include "stdio.h"

u8 CheckArea (NODESTRUCT n, u16 *st,u8 len) {
	u8 i;
	
	for (i=0;i<len;i++) 
		if (st[i]==n.area) return 1;
	
	return 0;
}

u8 CheckNumSector(u8 *ns,u8 len, u16 *st,u16 *ed, u8 type){
	u8 i;
	u16 val1,val2;
	
	if (type>1) return 1;
	val1=0;
	val2=0;
	for (i=0;i<len && ns[i]!='-';i++) {
		if (ns[i]<'0' || (ns[i])>'9') return 0;
		val1=val1*10+(ns[i]-'0');
	}
	
	*st=val1;
	if (i == len) { 
		if (val1>0 && ((val1<241 && type==0) || (val1<9999 && type==1))) { *ed = val1; return 1; }
		else return 0;
	}
	i=i+1;
	if (i == len) { return 0;}       //以-结尾，非法
	
	for (;i<len && ns[i]!='-';i++) {
		if (ns[i]<'0' || (ns[i])>'9') return 0;
		val2=val2*10+(ns[i]-'0');
	}
	if (i==len && val2>=val1 && val1>0 && ((val2<241 && type==0) || (val2<9999 && type==1))) {*ed = val2; return 1;}
	return 0;   //其他情况 非法          
}

u8 CheckNumString(u8 *numStr,u8 len,u16 *reNum, u8 *reLen,u8 type) {    //检查数字行输入是否合规  1,4-10,17
	u8 tmpStr[20],i,j,k;
	u16 st,ed,m,l;
	
	if (type>1) return 1;
	for (i=0,j=0,k=0,m=0;i<len;i++) {
		if (numStr[i] != ',' && numStr[i] != ' ') {
			tmpStr[j++] = numStr[i];
			m=1;
		}
		else if (numStr[i] == ',') {
			if (CheckNumSector(tmpStr,j,&st,&ed,type) == 0) return 0;
			j=0;
			for (l=st;l<=ed;l++) reNum[k++]=l;
			m=0;
		}
	}
	if (m==1) {
		if (CheckNumSector(tmpStr,j,&st,&ed,type) == 0) return 0;
		for (l=st;l<=ed;l++) reNum[k++]=l;
	}
	*reLen=k;
	return 1;
}

u8 deCode(u8 k) {
	switch(k) {
		case KEY00:
			return '0';
		case KEY01:
			return '1';
		case KEY02:
			return '2';
		case KEY03:
			return '3';
		case KEY04:
			return '4';
		case KEY05:
			return '5';
		case KEY06:
			return '6';
		case KEY07:
			return '7';
		case KEY08:
			return '8';
		case KEY09:
			return '9';
	}
	return ' ';
}

u16 str2Dec(u8 *str,u8 len) {
	u8 i;
	u16 rtv;
	rtv=0;
	for (i=0;i<len;i++) rtv=rtv*10+str[i]-'0';
	return rtv;
}

u16 str2BCD(u8 *str,u8 len) {
	u8 i;
	u16 rtv;
	rtv=0;
	for (i=0;i<len;i+=2) rtv=rtv*256+((str[i]-'0')*16+str[i+1]-'0');
	
	return rtv;
}

u8 testKey=1;

void Deal_pass(void) {

	u8 i;
	
	inputMode=0;
	if (OneLineNowMenu ==1) {       
		nowLine[4]=7;
		dispLine = 4;
		switch (OneLineMenuSelect) {
			case 2:
				inputMode = 10;	inputMax=24;		//进入屏蔽
				for (i=0;i<30;i++) inputStr[i]=0;
				inputPosi=0;
				dispLine=4;
				nowLine[4] =24;
				inputMLLine=1;
				break;

			case 3:
				SetOneLineMenu(3);				//进入设置子菜单
				break;
			
			case 5:        //控制屏幕
				dispLine=4;
				nowLine[4] =104;
				inputMode = 21;  inputMax=14;
				for (i=0;i<15;i++) inputStr[i]=0;
				inputPosi=0;
			  inputu8=0;
				break;
			
			case 6:
				SetOneLineMenu(4);				//进入配置子菜单
				break;
		}
	}
}

void	Deal_Key(u8 k){
	u8 i,j,kval,l,tmpLen;
	u16 a;
	u16 tmpPoint[240];

	
	switch (k){
	  case KEYRESET:
			NVIC_SystemReset();
//			NVIC_SETFAULTMASK();
//			GenerateSystemReset();
			
//			LedNew06 = testKey;
//		  testKey=(testKey << 1);
//		  if (testKey==0) testKey=1;
			return;

/*		case KEYSILENT:
			RESETBIT(hSEnable);
			RESETBIT(lSEnable);
			RESETBIT(mSEnable);
			totVoice=0;
		  wrVoice=0;
		  SILENTON;
			return; */
	}
	
	kval=deCode(k);

	if (ISSET(IsWindow)) {
		if (inputPosi<10 && inputMode==1 && kval>='0' &&  kval<='9' ) {
			windowSelect = 0x10;
			inputStr[inputPosi++]=k;  //密码输入方式
			if (inputPosi>=10) inputPosi=9;
		}
		switch (k) {
			case KEYESC:
				RESETBIT(IsWindow);
				ShowWindow(windowNum,1);
				if (windowNum/16<=10) windowSelect=windowTc-1;
				if (inputMode==1) inputMode=0;
//				oldScr=0;
				break;
			
			case KEYOK:
				if (inputMode==1) {
					inputMode=0;
					for (i=0;i<10 && inputStr[i]==myConf1.password[i];i++);
					for (j=0;j<10 && inputStr[j]==myConf1.suPassword[j];j++);
					if  (i<10 && j<10) {
						windowTime=3;						//密码错误
						windowSelect = 0x10;
						MenuDeep=0;
					}
					else {
						if (i==10) passwordIn=0xF0;
						if (j==10) passwordIn=0xF1;
						RESETBIT(IsWindow);
						ShowWindow(windowNum,1);
						Deal_pass();
					}
				}
//				oldScr=0;
				break;
			
			case KEYLEFT:
				if (windowNum/16<=10) {
					if ((windowSelect % 16) >0)  windowSelect = (windowSelect & 0xF0) | (windowSelect % 16 -1);
					else windowSelect = (windowSelect & 0xF0) | (windowTc -1);
				}
				break;

			case KEYRIGHT:
				if (windowNum/16<=10) {
					if ((windowSelect % 16) < windowTc - 1)  windowSelect = (windowSelect & 0xF0) | (windowSelect % 16 + 1);
					else windowSelect = (windowSelect & 0xF0);
				}
				break;
				
			
		}
		return;
	}
	
	
	if (inputMode>0) {            //输入模式
		if (kval>='0' && kval<='9') {
			if (inputPosi<10 && (inputMode==1 || inputMode==5)) {
				inputStr[inputPosi++]=k;  //密码输入方式
				if (inputPosi>=10) inputPosi=9;
			}
			if (inputMode==7 && inputPosi < 42) {  //多位点编辑明文输入方式
				if (inputPosi<6) subLine1=1;
				else if (inputPosi<10) subLine1=2;
				else subLine1=3;
				inputStr[inputPosi++]=kval;
				if (inputPosi<=3 & inputPosi>0) 
					inputStr[2+inputPosi]=inputStr[inputPosi-1];
				if (inputPosi==43) inputPosi=42;
				else if (inputPosi==42) inputPosi=38;
				if (inputPosi<6) subLine1=1;
				else if (inputPosi<10) subLine1=2;
				else subLine1=3;
				if (inputPosi>10 && (inputPosi-10)%4==0) {
					i=((inputPosi-10)/4-1)*2;
					inputChinese[i] = (inputStr[inputPosi-4]-'0')*10 + (inputStr[inputPosi-3]-'0') + 0xa0; 
					inputChinese[i+1] = (inputStr[inputPosi-2]-'0')*10 + (inputStr[inputPosi-1]-'0') + 0xa0;
					if (inputChinese[i]<0xa1 || inputChinese[i+1]<0xa1) 
						cpChinese2Str(inputChinese+i,(u8*)"　",2);
				}
			}
			if ((inputMode==4 || inputMode==8 || inputMode==10 || inputMode==20 || inputMode==21 ) && inputPosi<inputMax ) {    //多位明文输入方式
				inputStr[inputPosi++]=kval;					
				if (inputPosi>=inputMax) inputPosi=inputMax-1;
			}
			if (inputMode==9 && inputMLPosi[inputMLLine]<25 ) {   //多位多行明文输入方式
				inputMLStr[inputMLLine][inputMLPosi[inputMLLine]++]=kval;					
			}
		}
		if (inputMode==20 ) {
			switch (k) {
				case KEYLEFT:
					if (inputPosi>0) inputPosi--;
					break;

				case KEYRIGHT:
					if (inputPosi<3) inputPosi++;
					break;

				case KEYTAB:
					break;
				
				case KEYDOWN:
					i=str2Dec(inputStr,3);
				  if (i>1) sprintf((char *)inputStr,"%03d",i-1);
				  break;
				
				case KEYUP:
					i=str2Dec(inputStr,3);
				  if (i<240) sprintf((char *)inputStr,"%03d",i+1);
					break;
			}
		}
		
		else if (inputMode==21 ) {
			switch (k) {
				case KEYLEFT:
					if (inputPosi>0) inputPosi--;
					break;

				case KEYRIGHT:
					if (inputPosi<inputMax) inputPosi++;
					break;

				case KEYTAB:
					switch (inputu8/16) {
						case 0:
							inputu8 ^=0x01;
							break;
						case 1:
							inputu8 ^=0x02;
							break;
						case 2:
							inputu8 ^=0x04;
							break;
					}
					break;
				
				case KEYDOWN:
					if (inputu8/16==2) inputu8 = inputu8 & 0x0f;
				  else inputu8 = (inputu8/16 +1)*16 + (inputu8 & 0x0f);
				  break;
				
				case KEYUP:
					if (inputu8/16==0) inputu8 = 0x20 | (inputu8 & 0x0f);
				  else inputu8 = (inputu8/16 - 1)*16 + (inputu8 & 0x0f);
					break;
			}
		}
		
		else if (inputMode==8 ) {
			switch (k) {
				case KEYLEFT:
					if (inputPosi>0) inputPosi--;
					break;

				case KEYRIGHT:
					if (inputPosi<4) inputPosi++;
					break;

				case KEYTAB:
					break;
				
				case KEYDOWN:
				case KEYUP:
					if (inputPosi>1) inputPosi=0;
					else inputPosi = 2;
					break;
			}
		}

		else if (inputMode==7) {
			switch (k) {
				case KEYLEFT:
					if (inputPosi<6) subLine1=1;
					else if (inputPosi<10) subLine1=2;
					else subLine1=3;
					if (inputPosi>0) {
						if (inputPosi<=9) inputPosi--;
						else {
							if (inputPosi<42) {
								i = (inputPosi-10)/4;
								subLine1=2;
								if (i==0) inputPosi=9;
								else inputPosi = 10+(i-1)*4;
							}
							else if (inputPosi==42) inputPosi=43;
							else inputPosi=5;
						}
					}
					if (inputPosi<6) subLine=1;
					else if (inputPosi<10) subLine=2;
					else subLine=3;
					break;
				case KEYRIGHT:
					if (inputPosi<6) subLine1=1;
					else if (inputPosi<10) subLine1=2;
					else subLine1=3;
					if (inputPosi<42) {
						if (inputPosi<=9) inputPosi++;
						else {
							i = (inputPosi-10)/4;
							subLine1=2;
						  if (i==5) inputPosi=34;
							else inputPosi = 10+(i+1)*4;
							if (inputPosi==42) inputPosi=38;
						}
					}
					else if (inputPosi==42) inputPosi=6;
					else if (inputu8==1 || inputu8==3) inputPosi=42;
					else inputPosi=6;
					if (inputPosi<6) subLine=1;
					else if (inputPosi<10) subLine=2;
					else subLine=3;
					
					break;
				case KEYTAB:
					inputu8++;
					if (inputu8>8) inputu8 = 0;
					subLine=1;
				  inputPosi=43;
					break;
			}
    }
		
		else if (inputMode==9) {
			switch (k) {
				case KEYLEFT:
					if (inputMLPosi[inputMLLine]>0) inputMLPosi[inputMLLine]--;
					break;

				case KEYRIGHT:
					if (inputMLPosi[inputMLLine]<25) inputMLPosi[inputMLLine]++;
					break;

				case KEYTAB:
					inputMLStr[inputMLLine][28]=1-inputMLStr[inputMLLine][28];
					break;
				
//					inputMLLine++;
//					if (inputMLLine>=4) inputMLLine = 0;
//					break;
				
				case KEYDOWN:
				case KEYUP:
					if (inputMLLine==1) inputMLLine=2;
					else inputMLLine = 1;
					break;
				
				case KEYSILENT:
					if (inputMLPosi[inputMLLine]<25) inputMLStr[inputMLLine][inputMLPosi[inputMLLine]++]=',';
					break;

				case KEYRESET:
					if (inputMLPosi[inputMLLine]<25) inputMLStr[inputMLLine][inputMLPosi[inputMLLine]++]=' ';
					break;

				case KEYVOICE:
					if (inputMLPosi[inputMLLine]<25) inputMLStr[inputMLLine][inputMLPosi[inputMLLine]++]='-';
					break;

			}
    }

		else if (inputMode==10) {
			switch (k) {
				case KEYLEFT:
					if (inputMLLine==1) {
						if (inputStr[28]<2 && inputPosi>0) inputPosi--;
						else {
							if (inputu8==0) inputu8 = 8;
							else inputu8--; 
						}
					}
					else 
						inputStr[29]=1-inputStr[29];
					break;

				case KEYRIGHT:
					if (inputMLLine==1) {
						if (inputStr[28]<2 && inputPosi<25) inputPosi++;
						else {
							if (inputu8==8) inputu8 = 0;
							else inputu8++; 
						}
					}
					else 
						inputStr[29]=1-inputStr[29];
					break;

				case KEYTAB:
					inputStr[28]++;
				  if (inputStr[28]>2) inputStr[28]=0;
					break;
				
				case KEYDOWN:
				case KEYUP:
					if (inputMLLine==1) inputMLLine=2;
					else inputMLLine = 1;
					break;
				
				case KEYSILENT:
					if (inputPosi<24) inputStr[inputPosi++]=',';
					break;

				case KEYRESET:
					if (inputPosi<24) inputStr[inputPosi++]=' ';
					break;

				case KEYVOICE:
					if (inputPosi<24) inputStr[inputPosi++]='-';
					break;

			}
    }

		else if (inputMode==5 && (passwordIn & 0x0F)==1) {         //密码设置
			switch (k) {
				case KEYLEFT:
				case KEYRIGHT:
					TwoChoiceMenuSelect = 1-TwoChoiceMenuSelect;
					break;
			}
		}
		
		if (k==KEYOK) {
			
			switch(inputMode) {
					
				case 4:   //时间输入完毕
					machineDate.tm_year = str2Dec(inputStr,2) + 2000;
					machineDate.tm_mon = str2Dec(inputStr+2,2) - 1;
					machineDate.tm_mday = str2Dec(inputStr+4,2);
					machineDate.tm_hour = str2Dec(inputStr+6,2);
					machineDate.tm_min = str2Dec(inputStr+8,2);
					machineDate.tm_sec = str2Dec(inputStr+10,2);
					Time_SetCalendarTime(machineDate);
					inputMode=0;
					nowLine[4] = 7;
					if (MenuDeep>0) MenuDeep--;
				break;
				
				case 5:  //密码设定
					if (nowLine[4] == 12){
						for (i=0;i<10;i++) tmpPassword[i]=inputStr[i];
						nowLine[4] = 13;
						for (i=0;i<12;i++) inputStr[i]=0;
						inputPosi=0;
					} 
					else if (nowLine[4] == 13) {
						for (i=0;i<10 && tmpPassword[i]==inputStr[i]; i++);
						if (i<10) {
							inputMode=2;                 //两次密码不一致
							nowLine[4] =14;
						}
						else {
							if (TwoChoiceMenuSelect==0) for (i=0;i<10; i++) myConf1.password[i] = tmpPassword[i];
							else if ((passwordIn & 0x0F)==1) for (i=0;i<10; i++) myConf1.suPassword[i] = tmpPassword[i];
							WriteConf();
							nowLine[4] = 7;
							inputMode=0;
						}
						if (MenuDeep>0) MenuDeep--;
					}
				break;
					
				case 6:   //主从机设置完毕
					i=str2Dec(inputStr,2);
					if (i>0 && i<=MAXMACHINENUM) {
						if (nowLine[4]==16) printNow[i-1]=twoChoice;
					}
					inputMode=0;
					nowLine[4] = 10;
					if (MenuDeep>0) MenuDeep--;
				break;
					
				case 7:   //点编辑设置完毕
					i=str2Dec(inputStr,3);
					j=str2Dec(inputStr+3,3);
					a=str2Dec(inputStr+6,4);
//					a=str2BCD(inputStr+6,4);
					if (i==0 || j==0 || i>j || a==0 || j>240) {
						inputPosi=0;
						showMessage(1,3);
					}
					else {
						showMessage(2,10);
						oneNode.type =inputu8;
						if (oneNode.type>0 && oneNode.type<10) oneNode.area = a;
						else oneNode.area = 0;
						cpChinese2Str(oneNode.addr, inputChinese, 16);
						oneNode.stat=0;
						oneNode.year=machineDate.tm_year-2000;
						oneNode.mon=machineDate.tm_mon+1;
						oneNode.day=machineDate.tm_mday;
						oneNode.hour=machineDate.tm_hour;
						oneNode.min=machineDate.tm_min;
						oneNode.sec=machineDate.tm_sec;
	
	
						for (l=i-1;l<j;l++)
							WriteFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),l);
						inputPosi=0;
						showMessage(3,3);
//						if (MenuDeep>0) MenuDeep--;
					}
				break;
					
				case 9:   //声光点位设置完毕
					if (inputMLPosi[1]>0) {
						i=CheckNumString(inputMLStr[1],inputMLPosi[1],tmpPoint,&tmpLen,inputMLStr[1][28]);
						if (i==0) {
							inputMLPosi[1]=0;
							inputMLLine = 1;
							showMessage(1, 3);
						}
					}
					if (inputMLPosi[2]>0) {
						i=CheckNumString(inputMLStr[2],inputMLPosi[2],tmpPoint,&tmpLen,inputMLStr[2][28]);
						if (i==0) {
							inputMLPosi[2]=0;
							inputMLLine = 2;
							showMessage(1, 3);
						}
					}
					if (i>0) {
						showMessage(5, 3);
						for (i=0;i<30;i++) myConf1.voiceConf1[i]=inputMLStr[1][i];
						for (i=0;i<30;i++) myConf1.voiceConf2[i]=inputMLStr[2][i];
						WriteConf();
						nowLine[4]=7;
						inputMode=0;
						if (MenuDeep>0) MenuDeep--;
					}
					break;
					
				case 10:   //屏蔽设置完毕
					i=CheckNumString(inputStr,inputPosi,tmpPoint,&tmpLen,inputStr[28]);
					if (i==0) {
						inputPosi=0;
						inputMLLine=1;
						showMessage(1, 3);
					}
					else {
						showMessage(2, 15);
						if (inputStr[28]==0) {
							for (l=0;l<tmpLen;l++) {
								ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),tmpPoint[l]-1);

								oneNode.stat = (oneNode.stat & 0x7F) | (0x80 * (1-inputStr[29]));
								if ((oneNode.stat & 0x80) == 0) DelpinbiStat(myConf1.mNum,1, tmpPoint[l], 1, 1);
								else SetpinbiStat(myConf1.mNum,1, tmpPoint[l], 1, 1);
								WriteFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),tmpPoint[l]-1);
							}
						}
						else if (inputStr[28]==1){
							for (l=0;l<240;l++) {
								ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),l);
								if (CheckArea(oneNode, tmpPoint,tmpLen) == 1) {
								
									oneNode.stat = (oneNode.stat & 0x7F) | (0x80 * (1-inputStr[29]));
									if ((oneNode.stat & 0x80) == 0) DelpinbiStat(myConf1.mNum,1, l+1, 1, 1);
									else SetpinbiStat(myConf1.mNum,1, l+1, 1, 1);
									WriteFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),l);
								}
							}
						}
						else {
							for (l=0;l<240;l++) {
								ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),l);
								if (oneNode.type==inputu8) {
									oneNode.stat = (oneNode.stat & 0x7F) | (0x80 * (1-inputStr[29]));
									if ((oneNode.stat & 0x80) == 0) DelpinbiStat(myConf1.mNum,1, l+1, 1, 1);
									else SetpinbiStat(myConf1.mNum,1, l+1, 1, 1);
									WriteFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),l);
								}
							}
						}
						
						inputPosi=0;
						showMessage(3, 3);
					}
					//if (MenuDeep>0) MenuDeep--;
				break;
					
				case 8:   //主从机设置完毕
					i=str2Dec(inputStr,2);
					if (i>99) {
						inputPosi=0;
						showMessage(4, 3);
					}
					else {
						myConf1.mNum = i;
						i=str2Dec(inputStr+2,2);
						myConf1.maxMac = i;
						WriteConf();
						dispLine=4;
						nowLine[4] =7;
						inputMode = 0;
						if (MenuDeep>0) MenuDeep--;
					}
				break;
					
					
			}
		}
		if (k==KEYESC && inputMode > 0) {  //退出输入，返回顶级菜单
			inputMode=0;
			newScr=1;
			nowLine[4] =7;
			dispLine=4;
			if (MenuDeep>0) MenuDeep--;
			SetOneLineMenu(OneLineNowMenu);
		}
		updateLine = 1;
		if (inputMode==8 || inputMode==9 || inputMode==10) {
//			oldTab=1;
			dispLine=1;
		}
		return;
	}
	
	if (nowLine[4]==100) {
		if (k==KEYOK || k== KEYESC) {
			oldScr=0;
			newScr=1;
			nowLine[4]=7;
			dispLine=4;
			if (MenuDeep>0) MenuDeep--;
		}
 		return;
	}
	switch (k) {
		case KEYUP:
			if (nowLine[4]<7) {
				newTab--;
				if (newTab == 0) newTab = MAXDISPLINE;
				while ((lineMod[newTab] & 0x04) == 0) {
					newTab --;
					if (newTab<=0) newTab = MAXDISPLINE;
				}
			}
			if (nowLine[4]==7) {
				updateLine = 1;
				i=OneLineMenuSelect;
				if (OneLineMenuSelect>1) OneLineMenuSelect -=2;
				else {
					OneLineMenuSelect=OneLineMenuMax-1;
					if (i != (OneLineMenuSelect % 2)) OneLineMenuSelect--;
				}
			}
			break;
		
		case KEYDOWN:
			if (nowLine[4]<7) {
				newTab++;
				if (newTab > MAXDISPLINE) newTab = 1;
				while ((lineMod[newTab] & 0x04) == 0) {
					newTab ++;
					if (newTab > MAXDISPLINE) newTab = 1;
				}
			}
			if (nowLine[4]==7) {
				updateLine = 1;
				i=OneLineMenuSelect;
				if (OneLineMenuSelect<OneLineMenuMax-2) OneLineMenuSelect +=2;
				else {
					OneLineMenuSelect=0;
					if ((i%2) != OneLineMenuSelect) OneLineMenuSelect++;
				}
			}
			break;

		case KEYLEFT:
			updateLine = 1;
			switch (nowLine[oldTab]) {
				case 1:
					if (totBJ > 0) {
						dispBJ--;
						if (dispBJ <= 0) dispBJ = totBJ;
					}
				  break;

				case 2:
					if (totPinbi > 0) {
						dispPinbi--;
						if (dispPinbi <= 0) dispPinbi = totPinbi;
					}
				  break;

				case 3:
					if (totFire > 0) {
						dispFire--;
						if (dispFire <= 0) dispFire = totFire;
					}
				  break;

				case 4:
					if (totErr > 0) {
						dispErr--;
						if (dispErr <= 0) dispErr = totErr;
					}
				  break;

				case 5:
					if (totAct > 0) {
						dispAct--;
						if (dispAct <= 0) dispAct = totAct;
					}
				  break;


				case 53:
					if (totRec>0 && disRec>1) {
						disRec--;
						GetPreFlashRecoard(FIREBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXFIREBLOCKS);
					}
					break;
				case 54:
					if (totRec>0 && disRec>1) {
						disRec--;
						GetPreFlashRecoard(ERRBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXERRBLOCKS);
					}
					break;
				case 51:
					if (totRec>0 && disRec>1) {
						disRec--;
						GetPreFlashRecoard(IOBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXIOBLOCKS);
					}
					break;
					
				case 7:
					 oneLineMenu = (oneLineMenu >> 1);  //菜单选择
					 if (oneLineMenu == 0) oneLineMenu=16;
				   if (OneLineMenuSelect==0) OneLineMenuSelect=OneLineMenuMax-1;
				   else OneLineMenuSelect--;
				break;

				case 10:
					 setupSubMenu = (setupSubMenu >> 1);  //设置子菜单选择
					 if (setupSubMenu == 0) setupSubMenu=16;
				break;

				case 15:
					twoChoice=1-twoChoice;
				break;

				case 17:
					TwoChoiceMenuSelect=1-TwoChoiceMenuSelect;
				break;
			}	

			if (oldScr==4) {
				subLine1=MultiLineMenuSelect/2+2;
				if (MultiLineMenuSelect>0) MultiLineMenuSelect--;
				else MultiLineMenuSelect=4;
				subLine=MultiLineMenuSelect/2+2;
				dispLine=1;
			}
			if (oldScr==8) {
					if (totRec>0 && disRec>1) {
						disRec--;
						switch(nowRecType) {
							case 1:
								GetPreFlashRecoard(FIREBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXFIREBLOCKS);
								break;
							case 2:
								GetPreFlashRecoard(IOBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXIOBLOCKS);
								break;
							case 3:
								GetPreFlashRecoard(ERRBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXERRBLOCKS);
								break;
						}
						dispLine =1;
					}
			}
			break;

		case KEYRIGHT:
			updateLine = 1;
			switch (nowLine[oldTab]) {
				case 1:
					if (totBJ > 0) {
						dispBJ++;
						if (dispBJ > totBJ) dispBJ = 1;
					}
				break;

				case 2:
					if (totPinbi > 0) {
						dispPinbi++;
						if (dispPinbi > totPinbi) dispPinbi = 1;
					}
				break;

				case 3:
					if (totFire > 0) {
						dispFire++;
						if (dispFire > totFire) dispFire = 1;
					}
				break;

				case 4:
					if (totErr > 0) {
						dispErr++;
						if (dispErr > totErr) dispErr = 1;
					}
				break;

				case 5:
					if (totAct > 0) {
						dispAct++;
						if (dispAct > totAct) dispAct = 1;
					}
				break;
					
				case 53:
					if (totRec>0 && totRec>disRec) {
						disRec++;
						GetNextFlashRecoard(FIREBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXFIREBLOCKS);
					}
					break;
				case 54:
					if (totRec>0 && totRec>disRec) {
						disRec++;
						GetNextFlashRecoard(ERRBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXERRBLOCKS);
					}
					break;
				case 51:
					if (totRec>0 && totRec>disRec) {
						disRec++;
						GetNextFlashRecoard(IOBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXIOBLOCKS);
					}
					break;
					
				case 7:
					 oneLineMenu = (oneLineMenu << 1);  //菜单选择
					 if (oneLineMenu == 32) oneLineMenu=1;
				   OneLineMenuSelect++;
				   if (OneLineMenuSelect>=OneLineMenuMax) OneLineMenuSelect=0;
				break;

				case 10:
					 setupSubMenu = (setupSubMenu << 1);  //设置子菜单选择
					 if (setupSubMenu == 32) setupSubMenu=1;
				break;
				
				case 15:
					twoChoice=1-twoChoice;
				break;

				case 17:
					TwoChoiceMenuSelect=1-TwoChoiceMenuSelect;
				break;
			}
			
			if (oldScr==4) {
				subLine1=MultiLineMenuSelect/2+2;
				MultiLineMenuSelect++;
				if (MultiLineMenuSelect>4) MultiLineMenuSelect=0;
				subLine=MultiLineMenuSelect/2+2;
				dispLine=1;
			}
			if (oldScr==8) {
					if (totRec>0 && totRec>disRec) {
						disRec++;
						switch(nowRecType) {
							case 1:
								GetNextFlashRecoard(FIREBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXFIREBLOCKS);
								break;
							case 2:
								GetNextFlashRecoard(IOBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXIOBLOCKS);
								break;
							case 3:
								GetNextFlashRecoard(ERRBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXERRBLOCKS);
								break;
						}
						dispLine =1;
					}
				}
			break;
		
		case KEYTAB:
			if (nowLine[4]<7) {
				if (oldTab==4) {
					if (nowLine[4]==2)  nowLine[4] =4;
					else if (nowLine[4]==4)  nowLine[4] =5;
					else nowLine[4] =2;
					dispLine=4;
				}
			}
			break;
			
	  case KEY01:
		case KEY02:
	  case KEY03:
		case KEY04:
	  case KEY05:
		case KEY06:
	  case KEY07:
		case KEY08:
	  case KEY09:
		case KEY00:
			if (oldTab==4 && nowLine[4] == 7 ) { 
				oneLineMenu = 1; updateLine = 1; 
				if (k!=KEY00 && OneLineMenuMax >=kval-'0') {
					OneLineMenuSelect=kval-'1';
					Deal_Key(KEYOK);
				}
			}  //菜单选择
			else if (oldTab==4 && nowLine[4] == 10 ) { setupSubMenu = 1; updateLine = 1;}  //设置子菜单选择
			
			if (oldScr==4 && kval>'0' && kval < '6') { MultiLineMenuSelect=kval-'1'; oldScr=100;}
				
			if(k == KEY08 && oldScr<3) {
				SETBIT(needAction);
//				LedNew06 = LedNew06 >> 1;
//				if (LedNew06==0) LedNew06=0x80;
				
			}
			if(k == KEY09 && oldScr<3) {
				for (i=0;i<45;i++) 
					FSMC_NOR_EraseBlock(NODEBLOCK+i*128);
				for (i=0;i<MAXERRBLOCKS;i++) 
					FSMC_NOR_EraseBlock(ERRBLOCK+i*128);
				myConf1.isFullErrB=0;
				for (i=0;i<MAXIOBLOCKS;i++) 
					FSMC_NOR_EraseBlock(IOBLOCK+i*128);
				myConf1.isFullIOB=0;
				for (i=0;i<MAXFIREBLOCKS;i++) 
					FSMC_NOR_EraseBlock(FIREBLOCK+i*128);
				myConf1.isFullFireB=0;
				WriteConf();
			}

			if(k == KEY00 && oldScr<3) {
				newTab=4;
				dispLine=4;
				nowLine[4] =7;
				oneLineMenu = 1;
				SetOneLineMenu(1);
				MenuDeep=0;
			}
			break;



		case KEYVOICE:
			if (CheckNumString(myConf1.voiceConf1,25,tmpPoint,&tmpLen,myConf1.voiceConf1[28])>0) {
				REVBIT(voiceStat);
				if (ISRESET(voiceStat)) VOICELIGHTOFF;
				else VOICELIGHTON;
			}
			break;

		case KEYSILENT:
			RESETBIT(hSEnable);
			RESETBIT(lSEnable);
			RESETBIT(mSEnable);
			totVoice=0;
		  wrVoice=0;
		  SILENTON;
			break;

		case KEYOK:
			switch (nowLine[oldTab]) {
				case 7:					//单行菜单
					MenuTLen[MenuDeep]=	OneLineMenuSize[OneLineMenuSelect];
				  cpChinese2Str(MenuTitle[MenuDeep++], OneLineMenuItem[OneLineMenuSelect],OneLineMenuSize[OneLineMenuSelect]);

					switch (OneLineNowMenu) {
						case 1:						//主菜单
							if (OneLineMenuSelect <2) {    //查询功能无需密码 进查询子菜单
								dispLine=4;
								if (OneLineMenuSelect==0) SetOneLineMenu(2);
								else SetOneLineMenu(5);
							}
							else {
/*								inputMode = 1;
								for (i=0;i<12;i++) inputStr[i]=0;
								inputPosi=0;
								dispLine=4;
								nowLine[4] =8;
*/							if ((passwordIn & 0xF0) == 0xF0)
									Deal_pass();
								else if (ISRESET(IsWindow)) {
									SETBIT(IsWindow);
									windowNum=0xFF;
									windowSelect = 0x10;
									inputMode = 1;
									for (i=0;i<12;i++) inputStr[i]=0;
									inputPosi=0;
									RESETBIT(IsWindowShowed);
								}
							}
							break;
						
						case 4:																			//配置子菜单
							switch (OneLineMenuSelect) {
								case 0:																	//配置主从机
									inputMode = 8; inputMax=4;
									sprintf((char *)inputStr,"%02d%02d",myConf1.mNum,myConf1.maxMac);
									inputPosi=0;
									dispLine=4;
									nowLine[4] =22;
									break;

						
								case 4:                                 //声光分组
									inputMode = 9;
									for (i=0;i<4;i++) {
										sprintf((char *)inputMLStr[i],"%25c",' ');
										inputMLPosi[i]=0;
										inputMLStr[i][28]=0;
									}
									inputMLLine=1;
									dispLine=4;
									nowLine[4] =23;
									break;
									
								case 2:																	//配置点类型
									dispLine=4;
									nowLine[4] =19;
									inputMode = 7;
									sprintf((char *)inputStr,"%03d%03d%04d%032d",1,1,1,0);
									cpChinese2Str(inputChinese,(u8*)"　　　　　　　　",16);
									inputu8=0;
									inputPosi=0;
									break;
								
								case 3:
									SetTwoChoiceMenu(3,myConf1.mRunningStat);
									dispLine=4;
									nowLine[4] =17;
									break;
							}
							break;

						case 2:						//查询子菜单
							switch (OneLineMenuSelect) {
								case 0:		//查询配置
									dispLine=4;
								  nowLine[4]=100;
									break;
								case 1:		//查询点位
									dispLine=4;
								  nowLine[4]=101;
								  for (i=0;i<4;i++) inputStr[i]=0;
								  inputPosi=0;
								  inputMode=20; inputMax=3;
//									isFindRec=GetLastFlashRecoard(FIREBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXFIREBLOCKS);
									break;
								case 2:		//查询通讯
									dispLine=4;
								  nowLine[4]=103;
									break;
								case 3:		//查询电压
									Disp_Line(26,4);
									SETBIT(queryPowerVlot);
									break;
							}
							break;
							
						case 5:																			//查询档案子菜单
							switch (OneLineMenuSelect) {
								case 0:																	//查询火警
									isFindRec=GetLastFlashRecoard(FIREBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXFIREBLOCKS,myConf1.isFullFireB);
									nowRecType=1;
//									newScr=8;
									break;
								case 1:																	//查询报警
									isFindRec=GetLastFlashRecoard(IOBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXIOBLOCKS,myConf1.isFullIOB);
									nowRecType=2;
//									newScr=8;
									break;
								case 2:																	//查询故障
									isFindRec=GetLastFlashRecoard(ERRBLOCK,(u8 *)&nowRec,sizeof(IOSTAT),MAXERRBLOCKS,myConf1.isFullErrB);
									nowRecType=3;
									dispLine=4;
									nowLine[4] = 54;
//									newScr=8;
//									dispLine=4;
//									nowLine[4] = 54;
									break;
							}
							break;

						case 3:																			//设置子菜单
							switch (OneLineMenuSelect) {
								case 0:																	//设置时间
									inputMode = 4;	inputMax=12;			//时间输入
								  if (machineDate.tm_year<2000) 
										sprintf((char *)inputStr,"%02d%02d%02d%02d%02d%02d",0,machineDate.tm_mon+1,machineDate.tm_mday,
																										machineDate.tm_hour,machineDate.tm_min,machineDate.tm_sec);
									else

										sprintf((char *)inputStr,"%02d%02d%02d%02d%02d%02d",machineDate.tm_year-2000,machineDate.tm_mon+1,machineDate.tm_mday,
																										machineDate.tm_hour,machineDate.tm_min,machineDate.tm_sec);
									inputPosi=0;
									dispLine=4; 
									nowLine[4] =11;
									break;

								case 1:																	//设置手自动
									SetTwoChoiceMenu(1,myConf1.mAutomation);
									dispLine=4;
									nowLine[4] =17;
									break;

								case 2:																	//自检
									dispLine=4;
									nowLine[4] =18;
								  storeLed05=LedNew05;
								  storeLed06=LedNew06;
									LedNew05=0xff;
									LedNew06=0xff;
									selfTesting=SELFTESTTIME;
									break;

								case 3:																	//设置自动打印
									SetTwoChoiceMenu(4,myConf1.enPrint);
									dispLine=4;
									nowLine[4] =17;
									break;

								case 4:																	//设置密码
										SetTwoChoiceMenu(2,0);
									
										inputMode = 5;				//设置密码输入
										for (i=0;i<12;i++) inputStr[i]=0;
										inputPosi=0;
										dispLine=4;
										nowLine[4] =12;
									break;
							}
							break;
							
					}
					break;
			
				case 15: 
					inputMode = 5;				//设置密码输入
					for (i=0;i<12;i++) inputStr[i]=0;
					inputPosi=0;
					dispLine=4;
					nowLine[4] =12;
					break;
			
				case 17:        //两选菜单
					switch(TwoChoiceNowMenu) {
						case 1 :																			//手动 0 自动 1  是否触发声光
							myConf1.mAutomation = TwoChoiceMenuSelect;
							WriteConf();
							nowLine[4] = 7;
							dispLine=4;
							if (MenuDeep>0) MenuDeep--;
							break;

						case 2 :																			//用户密码 0 调试密码 1
							passwordIn=TwoChoiceMenuSelect;
							inputMode = 5;				//设置密码输入
							for (i=0;i<12;i++) inputStr[i]=0;
							inputPosi=0;
							nowLine[4] = 12;
							dispLine=4;
							break;

						case 3 :																			//调试态 0 正常 1 
							myConf1.mRunningStat = TwoChoiceMenuSelect;
							WriteConf();
							nowLine[4] = 7;
							dispLine=4;
							if (MenuDeep>0) MenuDeep--;
							break;

						case 4 :																			//手动 0 自动 1  自动打印
							myConf1.enPrint = TwoChoiceMenuSelect;
							WriteConf();
							nowLine[4] = 7;
							dispLine=4;
							if (MenuDeep>0) MenuDeep--;
							break;

					}
					break;

				case 10:
					switch(setupSubMenu) {
						case 1 :
							inputMode = 4;	inputMax=12;			//时间输入
							sprintf((char *)inputStr,"%02d%02d%02d%02d%02d%02d",machineDate.tm_year-2000,machineDate.tm_mon+1,machineDate.tm_mday,
																										machineDate.tm_hour,machineDate.tm_min,machineDate.tm_sec);
							inputPosi=0;
							dispLine=4;
							nowLine[4] =11;
							break;
					
						case 2 :
							break;
					
						case 16 :
							twoChoice=0;
							if ((passwordIn & 0x0F)!=0) {
								dispLine=4;
								nowLine[4] =15;
							}
							else {
								inputMode = 5;				//设置密码输入
								for (i=0;i<12;i++) inputStr[i]=0;
								inputPosi=0;
								dispLine=4;
								nowLine[4] =12;
							}
						break;
					}
				break;
			}
			
			if (oldScr==4) {				//多行配置菜单
				switch (MultiLineMenuSelect) {
					case 0:								//配置主从机
					break;
					
					case 2:								//输入点编辑
						newScr=3;
						inputMode = 7;
						sprintf((char *)inputStr,"%03d%03d%04d%024d",1,1,1,0);
						cpChinese2Str(inputChinese,(u8*)"　　　　　　",12);
						inputu8=0;
						inputPosi=0;
					break;

					case 4:								//配置声光分组
					break;
					
				}
			}
			break;

		case KEYESC:
			if (OneLineNowMenu>1 ) {
				dispLine=4;
				nowLine[4] =7;
				if (MenuDeep>0) MenuDeep--;
				if (MenuDeep==0) SetOneLineMenu(1);
				else 	SetOneLineMenu(OneLineNowMenu);
			}
			else Back_to_Screen();
			break;


	}
}
