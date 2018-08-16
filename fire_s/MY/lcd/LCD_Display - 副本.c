#include "USART.h"
#include "GloablVar.h"
#include "LCD_WO24064C.h"
#include "stdio.h"
#include "rtc_time.h"

#define MID(A) (8*(30-(A))/2)/3
#define CLR Disp(0x00,0x00)

extern IOSTAT  ioStat[1200] ;   //共5类 报警
extern IOSTAT  fireStat[1200];   //共5类 火警
extern IOSTAT  pinbiStat[1200];   //共5类 屏蔽
extern IOSTAT  errStat[1200];   //共5类 故障
extern IOSTAT  actStat[1200];   //共5类 联动
extern u8 ioInit[20][240];

u8 OneLineMenuItem[10][15];     //菜单选项
u8 OneLineMenuSize[10];         //菜单选项长度
u8 OLMstposi,OLMedposi;

u8 TwoChoiceMenuTitle[20];     //两选菜单标题
u8 TwoChoiceMenuTitleSize;     //两选菜单标题长度
u8 TwoChoiceMenuItem[2][15];     //两选菜单内容 最多两字
u8 TwoChoiceMenuItemSize;				//两选菜单内容长度

void cpChinese2Str(u8 *ptr, u8 *cptr, u8 ll)
{
	u8 i;
	for (i=0;i<ll;i++) *(ptr+i) = *(cptr+i);
}

void SetOneLineMenu(u8 tt) {
	OneLineMenuSelect=0;
	OLMstposi=0;
	OLMedposi=0;
	OneLineNowMenu = tt;
	
	switch (tt) {
		case 1:
			cpChinese2Str(OneLineMenuItem[0], (u8*)"1查询 ", 6);
		  OneLineMenuSize[0]=6;
			cpChinese2Str(OneLineMenuItem[1], (u8*)"2设置 ", 6);
		  OneLineMenuSize[1]=6;
			cpChinese2Str(OneLineMenuItem[2], (u8*)"3配置 ", 6);
		  OneLineMenuSize[2]=6;
			cpChinese2Str(OneLineMenuItem[3], (u8*)"4控制 ", 6);
		  OneLineMenuSize[3]=6;
			cpChinese2Str(OneLineMenuItem[4], (u8*)"5调试 ", 6);
		  OneLineMenuSize[4]=6;
			cpChinese2Str(OneLineMenuItem[5], (u8*)"6屏蔽", 5);
		  OneLineMenuSize[5]=5;
		  OneLineMenuMax=6;
			break;

		case 2:
			cpChinese2Str(OneLineMenuItem[0], (u8*)"1查询配置 ", 10);
		  OneLineMenuSize[0]=10;
			cpChinese2Str(OneLineMenuItem[1], (u8*)"2查询档案 ", 10);
		  OneLineMenuSize[1]=10;
			cpChinese2Str(OneLineMenuItem[2], (u8*)"3查询电压", 9);
		  OneLineMenuSize[2]=9;
		  OneLineMenuMax=3;
			break;

		case 3:
			cpChinese2Str(OneLineMenuItem[0], (u8*)"1设置校时 ", 10);
		  OneLineMenuSize[0]=10;
			cpChinese2Str(OneLineMenuItem[1], (u8*)"2设置手自动 ", 12);
		  OneLineMenuSize[1]=12;
			cpChinese2Str(OneLineMenuItem[2], (u8*)"3启动自检 ", 10); 
		  OneLineMenuSize[2]=10;
			cpChinese2Str(OneLineMenuItem[3], (u8*)"4设置打印 ", 10);
		  OneLineMenuSize[3]=10;
			cpChinese2Str(OneLineMenuItem[4], (u8*)"5设置密码", 9);
		  OneLineMenuSize[4]=9;
		  OneLineMenuMax=5;
			break;

		case 4:
			cpChinese2Str(OneLineMenuItem[0], (u8*)"1配置主从机 ", 12);
		  OneLineMenuSize[0]=12;
			cpChinese2Str(OneLineMenuItem[1], (u8*)"2回路学习 ", 10);
		  OneLineMenuSize[1]=10;
			cpChinese2Str(OneLineMenuItem[2], (u8*)"3配置点编辑 ", 12); 
		  OneLineMenuSize[2]=12;
			cpChinese2Str(OneLineMenuItem[3], (u8*)"4配置调试态 ", 12);
		  OneLineMenuSize[3]=12;
			cpChinese2Str(OneLineMenuItem[4], (u8*)"5声光分组", 9);
		  OneLineMenuSize[4]=9;
		  OneLineMenuMax=5;
			break;

		case 5:
			cpChinese2Str(OneLineMenuItem[0], (u8*)"1查询火警 ", 10);
		  OneLineMenuSize[0]=10;
			cpChinese2Str(OneLineMenuItem[1], (u8*)"2查询报警 ", 10);
		  OneLineMenuSize[1]=10;
			cpChinese2Str(OneLineMenuItem[2], (u8*)"3查询故障 ", 10); 
		  OneLineMenuSize[2]=10;
			cpChinese2Str(OneLineMenuItem[3], (u8*)"4操作记录 ", 10);
		  OneLineMenuSize[3]=10;
		  OneLineMenuMax=4;
			break;
	}
}

void SetTwoChoiceMenu(u8 tt,u8 nc) {
	TwoChoiceMenuSelect=nc;
	TwoChoiceNowMenu = tt;
	
	switch (tt) {
		case 1:
			cpChinese2Str(TwoChoiceMenuTitle, (u8*)"声光手自动: ", 12);
		  TwoChoiceMenuTitleSize=12;
			cpChinese2Str(TwoChoiceMenuItem[0], (u8*)"手动 ", 5);
			cpChinese2Str(TwoChoiceMenuItem[1], (u8*)"自动 ", 5);
		  TwoChoiceMenuItemSize=5;
			break;

		case 2:
			cpChinese2Str(TwoChoiceMenuTitle, (u8*)"设置: ", 6);
		  TwoChoiceMenuTitleSize=6;
			cpChinese2Str(TwoChoiceMenuItem[0], (u8*)"用户密码 ", 9);
			cpChinese2Str(TwoChoiceMenuItem[1], (u8*)"调试密码 ", 9);
		  TwoChoiceMenuItemSize=9;
			break;

		case 3:
			cpChinese2Str(TwoChoiceMenuTitle, (u8*)"声光手自动： ", 12);
		  TwoChoiceMenuTitleSize=12;
			cpChinese2Str(TwoChoiceMenuItem[0], (u8*)"关 ", 3);
			cpChinese2Str(TwoChoiceMenuItem[1], (u8*)"开 ", 3);
		  TwoChoiceMenuItemSize=3;
			break;

		case 4:
			cpChinese2Str(TwoChoiceMenuTitle, (u8*)"自动打印： ", 10);
		  TwoChoiceMenuTitleSize=10;
			cpChinese2Str(TwoChoiceMenuItem[0], (u8*)"关 ", 3);
			cpChinese2Str(TwoChoiceMenuItem[1], (u8*)"开 ", 3);
		  TwoChoiceMenuItemSize=3;
			break;
	}
}


void Disp_LED_Success(u8 stat)
{
	if (stat == 1) ShowString8 (10, 0, (u8 *)"LED&KEY Test OK",15,0);
	else ShowString8 (10, 0, (u8 *)"LED&KEY Test Fail",17,0);
}

void Disp_RAM_Success(u8 stat)
{
	if (stat == 1) ShowString8 (20, 0, (u8 *)"RAM Test OK",11,0);
	else ShowString8 (20, 0, (u8 *)"RAM Test Fail",13,0);
}

void Disp_Flash_Success(u8 stat)
{
	if (stat == 1) ShowString8 (30, 0, (u8 *)"Flash Test OK",13,0);
	else ShowString8 (30, 0, (u8 *)"Flash Test Fail",15,0);
}

void Disp_ShowLine(u8 ll){
	u8 i;
	LCDSetAddr(ll,1,0,80);
	write_com(0x2c);
	for (i=0;i<120;i++) {
			write_dat(0xff); 
  }
	LCDSetAddr(0,64,0,80);
}

void Disp_ClearLine(u8 ll,u8 nl,u8 rr, u8 nr){
	u8 i,j;
	for (i=0;i<nl;i++) {
	  LCDSetAddr(ll+i,1,rr,nr);
	  write_com(0x2c);
	  for (j=0;j<(nr+1)/2;j++) {
			write_dat(0x00); 
    }
	}
	LCDSetAddr(0,64,0,80);
}

char *typeString(u8 dt,u8 t)
{
	if (t<=2)
		switch (t) {
			case 0:
				return("丢失 ");
			case 1:
				return("重码 ");
			case 2:
				return("类型E");
		}
		
	switch(dt) {
		case 1 :       //光电
			switch (t) {
				case 3:
					return("内部E");
				case 5:
					return("报警 ");  //火警类
			}
			break;

		case 2 :       //温感
			switch (t) {
				case 3:
					return("内部E");
				case 5:
					return("报警 ");  //火警类
			}
			break;

		case 3 :       //复合
			switch (t) {
				case 3:
					return("内部E");
				case 4:
					return("污染 ");
				case 5:
					return("烟感A");   //火警类
				case 6:
					return("温感A");  //火警类
			}
			break;

		case 4 :       //手报
			switch (t) {
				case 5:
					return("报警 ");   //火警类
			}
			break;

		case 5 :       //消火栓
			switch (t) {
				case 5:
					return("报警 ");  //报警类   改启动文字
			}
			break;

		case 6 :       //输入
			switch (t) {
				case 3:
					return("终端E");
				case 5:
					return("报警 "); //报警类
			}
			break;

		case 7 :       //输入出
			switch (t) {
				case 3:
					return("输入E"); 
				case 4:
					return("输出E");  
				case 5:
					return("输入A"); //报警类
				case 6:
					return("输出O"); //报警类
			}
			break;

		case 8 :       //声光
			switch (t) {
				case 3:
					return("内部E");
				case 5:
					return("报警 ");  //报警类
			}
			break;
	}
	return("未知 ");
}

char *typeFullString(u8 dt,u8 t)
{
	if (t<=2)
		switch (t) {
			case 0:
				return("丢失    ");
			case 1:
				return("重码    ");
			case 2:
				return("类型错误");
		}
		
	switch(dt) {
		case 1 :       //光电
			switch (t) {
				case 3:
					return("内部错误");
				case 5:
					return("报警    ");  //火警类
			}
			break;

		case 2 :       //温感
			switch (t) {
				case 3:
					return("内部错误");
				case 5:
					return("报警    ");  //火警类
			}
			break;

		case 3 :       //复合
			switch (t) {
				case 3:
					return("内部错误");
				case 4:
					return("污染    ");
				case 5:
					return("复合烟感");   //火警类
				case 6:
					return("复合温感");  //火警类
			}
			break;

		case 4 :       //手报
			switch (t) {
				case 5:
					return("报警    ");   //火警类
			}
			break;

		case 5 :       //消火栓
			switch (t) {
				case 5:
					return("报警    ");  //报警类   改启动文字
			}
			break;

		case 6 :       //输入
			switch (t) {
				case 3:
					return("终端错误");
				case 5:
					return("报警    "); //报警类
			}
			break;

		case 7 :       //输入出
			switch (t) {
				case 3:
					return("输入错误"); 
				case 4:
					return("输出错误");  
				case 5:
					return("输入动作"); //报警类
				case 6:
					return("输出动作"); //报警类
			}
			break;

		case 8 :       //声光
			switch (t) {
				case 3:
					return("内部故障");
				case 5:
					return("报警    ");  //报警类
			}
			break;
	}
	return("未知错误");
}


char *dotType(u8 dt,u8 t,u8 type)
{
	switch (t) {
		case 0:
		 return("空空空");
		case 1:
		 return("光电  ");
		case 2:
		 return("温感  ");
		case 3:
		 if (type==1) {
			 if (dt==5) return("复合烟");
			 if (dt==6) return("复合温");
		 }
		 return("复合  ");
		case 4:
		 return("手报  ");
		case 5:
		 return("消火栓");
		case 6:
		 return("输入  ");
		case 7:
		 return("输入出");
		case 8:
		 return("声光  ");
	}
	return("未知  ");
}

char *dotFullType(u8 dt,u8 t,u8 type)
{
	switch (t) {
		case 0:
		 return("未知节点");
		case 1:
		 return("光电探头");
		case 2:
		 return("温感探头");
		case 3:
		 if (type==1) {
			 if (dt==5) return("复合烟感");
			 if (dt==6) return("复合温感");
		 }
		 return("复合探头");
		case 4:
		 return("手报装置");
		case 5:
		 return("消火栓  ");
		case 6:
		 return("输入节点");
		case 7:
		 return("输入输出");
		case 8:
		 return("声光节点");
	}
	return("未知节点");
}

void decString(u8 *str, u16 n, u8 b) {
//	if (b==2) { *str=(n/10)+'0'; *(str+1)=(n%10)+'0'; return;}
//	if (b==3) { *str=(n/100)+'0'; n= n%100; *(str+1)=(n/10)+'0'; *(str+2)=(n%10)+'0'; return;}
//	if (b==4) { *str=(n/1000)+'0'; n= n%1000; *(str+1)=(n/100)+'0'; n= n%100; *(str+2)=(n/10)+'0'; *(str+3)=(n%10)+'0'; return;}
  if (b==2) { sprintf((char *)str,"%2d",n); return; }
  if (b==3) { sprintf((char *)str,"%3d",n); return; }
  if (b==4) { sprintf((char *)str,"%4d",n); return; }
}

void Disp_ShowTime(u8 ll,u8 rr,u8 mod){
	
	u8 strShow[10];
  
	switch(mod) {
		case 1:
			decString(strShow, machineDate.tm_year,4);
			ShowString8 (ll+8, rr, strShow, 4 ,0);
			ShowString16 (ll, rr+11, (u8*)"年", 2 ,0);
			
		  decString(strShow, machineDate.tm_mon + 1,2);
			ShowString8 (ll+8, rr+16, strShow, 2 ,0);
			ShowString16 (ll, rr+21, (u8*)"月", 2 ,0);
		
		  decString(strShow, machineDate.tm_mday,2);
			ShowString8 (ll+8, rr+26, strShow, 2 ,0);
			ShowString16 (ll, rr+31, (u8*)"日", 2 ,0);
		
		  decString(strShow, machineDate.tm_hour,2);
			ShowString8 (ll+8, rr+36, strShow, 2 ,0);
			ShowString16 (ll, rr+41, (u8*)"时", 2 ,0);
		
		  decString(strShow, machineDate.tm_min,2);
			ShowString8 (ll+8, rr+46, strShow, 2 ,0);
			ShowString16 (ll, rr+51, (u8*)"分", 2 ,0);
		
		  decString(strShow, machineDate.tm_sec,2);
			ShowString8 (ll+8, rr+56, strShow, 2 ,0);
			ShowString16 (ll, rr+61, (u8*)"秒", 2 ,0);

			sprintf((char *)strShow,"%02x%02x",testByte0,testByte1);
			ShowString8 (ll+8, rr+66, strShow, 4 ,0);
			break;
	}

}

void Disp_Line(u8 LineType,u8 LineNum) {
	
	u8 strShow[20],tmpVal,i,j;
	u8 rr = (LineNum-1)*16;
	u8 OLMdispll,OLMNextItemll;
	
	nowLine[LineNum] = LineType;
	switch (LineType) {
	//	case 0:
	//	  Disp_ClearLine(rr,16,0,240);
	//		break;
		
		case 1:
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"报警",4,lineMod[LineNum] & 0x01);
			decString(strShow, dispBJ,4);
			ShowString8 (rr, 11, strShow,4,0);
			decString(strShow, totBJ,4);
			ShowString8 (rr+8, 11, strShow,4,0);
		  lineMod[LineNum] &= 0xFD;
		  if (totBJ>0) {
				strShow[0]=' ';
				strShow[1]=' ';
				decString(strShow+2, ioStat[dispBJ].bNum ,2);
				ShowString8 (rr, 22, strShow,4,0);
				decString(strShow, ioStat[dispBJ].dNum ,4);
				ShowString8 (rr+8, 22, strShow,4,0);
		
				lineMod[LineNum] |= 0x02; //多行轮显
			}
		  lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 51:
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"报警",4,lineMod[LineNum] & 0x01);
			if (isFindRec == 0) { ShowString16 (rr, 11, (u8*)"无记录",6,0); lineMod[LineNum] &= 0xFD; }
			else {
				decString(strShow, disRec,4);
				ShowString8 (rr, 11, strShow,4,0);
				decString(strShow, totRec,4);
				ShowString8 (rr+8, 11, strShow,4,0);
				lineMod[LineNum] &= 0xFD;
				if (totRec>0) {
					strShow[0]=' ';
					strShow[1]=' ';
					decString(strShow+2, nowRec.bNum ,2);
					ShowString8 (rr, 22, strShow,4,0);
					decString(strShow, nowRec.dNum ,4);
					ShowString8 (rr+8, 22, strShow,4,0);
		
					lineMod[LineNum] |= 0x02; //多行轮显
				}
			}
		  lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 2:
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"屏蔽",4,lineMod[LineNum] & 0x01);
			decString(strShow, dispPinbi,4);
			ShowString8 (rr, 11, strShow,4,0);
			decString(strShow, totPinbi,4);
			ShowString8 (rr+8, 11, strShow,4,0);
		  lineMod[LineNum] &= 0xFD;
		  if (totPinbi > 0) {
				strShow[0]=' ';
				strShow[1]=' ';
				decString(strShow+2, pinbiStat[dispPinbi].bNum ,2);
				ShowString8 (rr, 22, strShow,4,0);
				decString(strShow, pinbiStat[dispPinbi].dNum ,4);
				ShowString8 (rr+8, 22, strShow,4,0);

				lineMod[LineNum] |= 0x02; //多行轮显
			}
  		lineMod[LineNum] |= 0x04; //可获焦点
		  break;

		case 3:
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"火警",4,lineMod[LineNum] & 0x01);
			decString(strShow, dispFire,4);
			ShowString8 (rr, 11, strShow,4,0);
			decString(strShow, totFire,4);
			ShowString8 (rr+8, 11, strShow,4,0);
		  lineMod[LineNum] &= 0xFD;
		  if (totFire > 0) {
				strShow[0]=' ';
				strShow[1]=' ';
				decString(strShow+2, fireStat[dispFire].bNum ,2);
				ShowString8 (rr, 22, strShow,4,0);
				decString(strShow, fireStat[dispFire].dNum ,4);
				ShowString8 (rr+8, 22, strShow,4,0);

				lineMod[LineNum] |= 0x02; //多行轮显
			}
  		lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 53:				//查询火警
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"火警",4,lineMod[LineNum] & 0x01);
			if (isFindRec == 0) { ShowString16 (rr, 11, (u8*)"无记录",6,0); lineMod[LineNum] &= 0xFD; }
			else {
				
				decString(strShow, disRec,4);
				ShowString8 (rr, 11, strShow,4,0);
				decString(strShow, totRec,4);
				ShowString8 (rr+8, 11, strShow,4,0);
				lineMod[LineNum] &= 0xFD;
				strShow[0]=' ';
				strShow[1]=' ';
				decString(strShow+2, nowRec.bNum ,2);
				ShowString8 (rr, 22, strShow,4,0);
				decString(strShow, nowRec.dNum ,4);
				ShowString8 (rr+8, 22, strShow,4,0);

				lineMod[LineNum] |= 0x02; //多行轮显
			}
			break;

		case 4:
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"故障",4,lineMod[LineNum] & 0x01);
			decString(strShow, dispErr,4);
			ShowString8 (rr, 11, strShow,4,0);
			decString(strShow, totErr,4);
			ShowString8 (rr+8, 11, strShow,4,0);
		  lineMod[LineNum] &= 0xFD;
		  if (totErr>0) {
				strShow[0]=' ';
				strShow[1]=' ';
				if (errStat[dispErr].mNum>0) {
					decString(strShow+2, errStat[dispErr].mNum ,2);
					ShowString8 (rr, 22, strShow,4,0);
				}
		
				if (errStat[dispErr].ioType == 250) {
					sprintf((char *)strShow,"%4d",errStat[dispErr].bNum);
					ShowString8 (rr+8, 22, strShow,4,0);
					lineMod[LineNum] |= 0x02; //多行轮显;
        }
				else if (errStat[dispErr].ioType == 251) {
					sprintf((char *)strShow,"%1d%03d",errStat[dispErr].bNum,errStat[dispErr].dNum);
					ShowString8 (rr+8, 22, strShow,4,0);
					lineMod[LineNum] |= 0x02; //多行轮显;
				}
				else if (errStat[dispErr].ioType == 252) {  //2号串口通讯故障
					sprintf((char *)strShow,"%4d",errStat[dispErr].bNum);
					ShowString8 (rr+8, 22, strShow,4,0);
					lineMod[LineNum] |= 0x02; //多行轮显;
				}
				else lineMod[LineNum] |= 0x02; //多行轮显
				flashLine=0x08;
			}
		  lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 54:
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"故障",4,lineMod[LineNum] & 0x01);
		  if (isFindRec == 0) { ShowString16 (rr, 11, (u8*)"无记录",6,0); lineMod[LineNum] &= 0xFD; }
			else {
				decString(strShow, disRec,4);
				ShowString8 (rr, 11, strShow,4,0);
				decString(strShow, totRec,4);
				ShowString8 (rr+8, 11, strShow,4,0);
				if (totRec>0) {
					strShow[0]=' ';
					strShow[1]=' ';
					if (nowRec.mNum>0) {
						decString(strShow+2, nowRec.mNum ,2);
						ShowString8 (rr, 22, strShow,4,0);
					}
		
					if (nowRec.ioType == 250) {
						sprintf((char *)strShow,"%4d",nowRec.bNum);
						ShowString8 (rr+8, 22, strShow,4,0);
						lineMod[LineNum] |= 0x02; //多行轮显;
					}
					else if (nowRec.ioType == 251) {
						sprintf((char *)strShow,"%1d%03d",nowRec.bNum,nowRec.dNum);
						ShowString8 (rr+8, 22, strShow,4,0);
						lineMod[LineNum] |= 0x02; //多行轮显;
					}
					else if (nowRec.ioType == 252) {  //2号串口通讯故障
						sprintf((char *)strShow,"%4d",nowRec.bNum);
						ShowString8 (rr+8, 22, strShow,4,0);
						lineMod[LineNum] |= 0x02; //多行轮显;
					}
					else lineMod[LineNum] |= 0x02; //多行轮显
				}
			}
		  lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 5:
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"联动",4,lineMod[LineNum] & 0x01);
			decString(strShow, dispAct,4);
			ShowString8 (rr, 11, strShow,4,0);
			decString(strShow, totAct,4);
			ShowString8 (rr+8, 11, strShow,4,0);
		  lineMod[LineNum] &= 0xFD;
		  if (totAct>0) {
				strShow[0]=' ';
				strShow[1]=' ';
				if (actStat[dispAct].mNum>0) {
					decString(strShow+2, actStat[dispAct].mNum ,2);
					ShowString8 (rr, 22, strShow,4,0);
				}
				sprintf((char *)strShow,"%4d",errStat[dispErr].bNum);
				ShowString8 (rr+8, 22, strShow,4,0);
      }		
		  lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 6:
		  Disp_ClearLine(rr,16,0,240);
			tmpVal=ioInit[fireStat[1].bNum-1][fireStat[1].dNum-1];
			ShowString16 (rr, 0, (u8*)"首警",4,lineMod[LineNum] & 0x01);
			if (myConf1.mNum>0) {
				decString(strShow, myConf1.mNum,4);
				ShowString8 (rr, 11, strShow,4,0);
			}
			sprintf((char *)strShow,"%1d%03d",fireStat[1].bNum,fireStat[1].dNum);
			ShowString8 (rr+8, 11, strShow,4,0);
			lineMod[LineNum] &= 0xFD;															//无轮显
		  ShowString16 (rr, 22, (u8*)dotType(fireStat[1].eventType,tmpVal,1),6,0);
			sprintf((char *)strShow,"%02d-%02d-%02d",fireStat[1].year,fireStat[1].mon,fireStat[1].day);
			ShowString8 (rr, 38, strShow,8,0);
			sprintf((char *)strShow,"%02d:%02d:%02d",fireStat[1].hour,fireStat[1].min,fireStat[1].sec);
			ShowString8 (rr+8, 38, strShow,8,0);
		  ShowString16 (rr, 59, (u8*)"一二三四",8,0);
		  
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 7: //单行菜单
		  Disp_ClearLine(rr,16,0,240);
		  if (OneLineMenuSelect<=OLMstposi || OneLineMenuSelect <= OLMedposi) {
				if (OneLineMenuSelect<OLMstposi) OLMstposi=OneLineMenuSelect;
				if (OLMstposi >0 ) {
					ShowString16 (rr, 0, (u8 *)"<<",2,0);
				}
				OLMedposi=OLMstposi;
				OLMdispll=6;
				OLMNextItemll = 0;
				while (OLMdispll<75-OLMNextItemll && OLMedposi< OneLineMenuMax) {
					ShowString16 (rr, OLMdispll, OneLineMenuItem[OLMedposi],OneLineMenuSize[OLMedposi],2*(OneLineMenuSelect==OLMedposi?1:0));
					OLMdispll += OneLineMenuSize[OLMedposi]*8/3+1;
					OLMedposi++;
					if (OLMedposi< OneLineMenuMax) OLMNextItemll=OneLineMenuSize[OLMedposi]*8/3+1;
				}
				if (OLMedposi < OneLineMenuMax) {
					ShowString16 (rr, 75, (u8 *)">>",2,0);
				}
				OLMedposi--;
			}
		  
		  else if (OneLineMenuSelect>=OLMedposi) {
				if (OneLineMenuSelect>OLMedposi) OLMedposi=OneLineMenuSelect;

				OLMstposi=OLMedposi;
				OLMdispll=75;
				OLMNextItemll = 1;
				while (OLMdispll>6+OLMNextItemll && OLMNextItemll>0) {
					ShowString16 (rr, OLMdispll-OneLineMenuSize[OLMstposi]*8/3-1, OneLineMenuItem[OLMstposi],OneLineMenuSize[OLMstposi],2*(OneLineMenuSelect==OLMstposi?1:0));
					OLMdispll -= OneLineMenuSize[OLMstposi]*8/3+1;
					if (OLMstposi>0) { OLMNextItemll=OneLineMenuSize[OLMstposi-1]*8/3+1; OLMstposi--;}
					else OLMNextItemll=0;
				}
				if (OLMNextItemll >0) {
					ShowString16 (rr, 0, (u8 *)"<<",2,0);
				}
				if (OLMedposi < OneLineMenuMax - 1) {
					ShowString16 (rr, 75, (u8 *)">>",2,0);
				}
			}
//			ShowString16 (rr, 0, OneLineMenuItem[0],5,2*(oneLineMenu &lineMod[LineNum] & 0x01));
//			ShowString16 (rr, 16, OneLineMenuItem[1],5,2*((oneLineMenu >> 1)&lineMod[LineNum] & 0x01));
//			ShowString16 (rr, 32, OneLineMenuItem[2],5,2*((oneLineMenu >> 2)&lineMod[LineNum] & 0x01));
//			ShowString16 (rr, 48, OneLineMenuItem[3],5,2*((oneLineMenu >> 3)&lineMod[LineNum] & 0x01));
//			ShowString16 (rr, 64, OneLineMenuItem[4],5,2*((oneLineMenu >> 4)&lineMod[LineNum] & 0x01));
//		  lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 8: //密码
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"请输入密码:",11,0);
			for (tmpVal=0;tmpVal<inputPosi;tmpVal++) strShow[tmpVal]='*';
		  strShow[tmpVal]='_';
		  ShowString8(rr+8, 30, strShow ,tmpVal+1,0);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 9: //密码错误
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"密码错误，请按ESC键返回菜单！",29,0);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 10: //设置子菜单
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"1",1,0);
		  ShowString16 (rr, 3,(u8*)"时间",4,setupSubMenu &lineMod[LineNum] & 0x01);
			ShowString16 (rr, 16, (u8*)"2",1,0);
		  ShowString16 (rr, 19,(u8*)"打印",4,(setupSubMenu >> 1) &lineMod[LineNum] & 0x01);
			ShowString16 (rr, 32, (u8*)"3",1,0);
		  ShowString16 (rr, 35,(u8*)"自动",4,(setupSubMenu >> 2) &lineMod[LineNum] & 0x01);
			ShowString16 (rr, 48, (u8*)"4",1,0);
		  ShowString16 (rr, 51,(u8*)"输出",4,(setupSubMenu >> 3) &lineMod[LineNum] & 0x01);
			ShowString16 (rr, 64, (u8*)"5",1,0);
		  ShowString16 (rr, 67,(u8*)"密码",4,(setupSubMenu >> 4) &lineMod[LineNum] & 0x01);
		  lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 11: //设置时间
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"20",2,0);
		  if (inputPosi/2==0) ShowStringPosi8(rr+8, 5, inputStr, 2 ,0,inputPosi);
		  else ShowString8(rr+8, 5, inputStr, 2 ,0);
		  ShowString16 (rr, 10,(u8*)"年",2,0);
		  if (inputPosi/2==1) ShowStringPosi8(rr+8, 15, inputStr+2, 2 ,0,inputPosi-2);
		  else ShowString8(rr+8, 15, inputStr+2, 2 ,0); 
		  ShowString16 (rr, 20,(u8*)"月",2,0);
		  if (inputPosi/2==2) ShowStringPosi8(rr+8, 25, inputStr+4, 2 ,0,inputPosi-4);
		  else ShowString8(rr+8, 25, inputStr+4, 2 ,0); 
		  ShowString16 (rr, 30,(u8*)"日",2,0);
		  if (inputPosi/2==3) ShowStringPosi8(rr+8, 35, inputStr+6, 2 ,0,inputPosi-6);
		  else ShowString8(rr+8, 35, inputStr+6, 2 ,0); 
		  ShowString16 (rr, 40,(u8*)"时",2,0);
		  if (inputPosi/2==4) ShowStringPosi8(rr+8, 45, inputStr+8, 2 ,0,inputPosi-8);
		  else ShowString8(rr+8, 45, inputStr+8, 2 ,0); 
		  ShowString16 (rr, 50,(u8*)"分",2,0);
		  if (inputPosi/2==5) ShowStringPosi8(rr+8, 55, inputStr+10, 2 ,0,inputPosi-10);
		  else ShowString8(rr+8, 55, inputStr+10, 2 ,0); 
		  ShowString16 (rr, 60,(u8*)"秒",2,0);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 12: //新密码设置
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"请输入新密码:",13,0);
			for (tmpVal=0;tmpVal<inputPosi;tmpVal++) strShow[tmpVal]='*';
		  strShow[tmpVal]='_';
		  ShowString8(rr+8, 35, strShow ,tmpVal+1,0);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 13: //再次输入新密码
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"请再次输入新密码:",17,0);
			for (tmpVal=0;tmpVal<inputPosi;tmpVal++) strShow[tmpVal]='*';
		  strShow[tmpVal]='_';
		  ShowString8(rr+8, 46, strShow ,tmpVal+1,0);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 14: 
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"密码不一致，请按ESC键返回！",27,0);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 15:   
		  Disp_ClearLine(rr,16,0,240);
//			ShowString16 (rr, 0, (u8*)"设置",4,0);
//			ShowString16 (rr, 13, (u8*)"用户密码",8,1-twoChoice);
//			ShowString16 (rr, 36, (u8*)"调试密码",8,twoChoice);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 16:   
		  Disp_ClearLine(rr,16,0,240);
			if (myConf1.mNum==1) ShowStringPosi8(rr+8, 1, inputStr, 2 ,0,inputPosi);
		  else ShowString8(rr+8, 1, inputStr, 2 ,0); 
			ShowString16 (rr, 6, (u8*)"号机",4,0);
			ShowString16 (rr, 22, (u8*)"非实时打印",10,1-twoChoice);
			ShowString16 (rr, 52, (u8*)"实时打印",8,twoChoice);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 17:           //两选菜单
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, TwoChoiceMenuTitle,TwoChoiceMenuTitleSize,0);
			ShowString16 (rr, TwoChoiceMenuTitleSize*8/3+1, TwoChoiceMenuItem[0],TwoChoiceMenuItemSize,2*(TwoChoiceMenuSelect==0?1:0));
			ShowString16 (rr, TwoChoiceMenuTitleSize*8/3+1+TwoChoiceMenuItemSize*8/3+1, TwoChoiceMenuItem[1],TwoChoiceMenuItemSize,2*(TwoChoiceMenuSelect==1?1:0));
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 18:           //自检
		  Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, MID(11), (u8*)"正在自检...",11,0);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 19:           //配置点类型
		  if (subLine==1 || subLine1==1 ||subLine==0) {
				Disp_ClearLine(rr,16,0,240);
				ShowString16 (rr, 0, (u8*)"从 ",3,0);
				if (inputPosi<3) ShowStringPosi8(rr+8, 8, inputStr, 3 ,0,inputPosi);
				else ShowString8(rr+8, 8, inputStr, 3 ,0);
				ShowString16 (rr, 17, (u8*)"到 ",3,0);
				if (inputPosi>=3 && inputPosi<6) ShowStringPosi8(rr+8, 25, inputStr+3, 3 ,0,inputPosi-3);
				else ShowString8(rr+8, 25, inputStr+3, 3 ,0);
				ShowString16(rr, 40, (u8*)"类型:", 5 ,0);
				ShowString16(rr, 54, (u8*)dotType(0,inputu8,0), 6 ,1);
			}
		  if (subLine==2 || subLine1==2 ||subLine==0) {
				Disp_ClearLine(rr+16,16,0,240);
				ShowString16 (rr+16, 0, (u8*)"分区 ",5,0);
				if (inputPosi>=6 && inputPosi<10) ShowStringPosi8(rr+16+8, 14, inputStr+6, 4 ,0,inputPosi-6);
				else ShowString8(rr+16+8, 14, inputStr+6, 4 ,0);
				ShowString16 (rr+16, 30, (u8*)"位置:",5,0);
				if (inputPosi>=10 && inputPosi<34) ShowStringPosi16 (rr+16, 44, inputChinese, 12, (inputPosi-10)/4);
				else ShowString16 (rr+16, 44, inputChinese,12, 0);
//		  ShowStringPosi8(rr+8, 17, inputStr+3, 3 ,0,inputPosi);
			}
		  if (subLine==3 || subLine1==3 ||subLine==0) {
				
				Disp_ClearLine(rr+32,16,0,240);
				ShowString16 (rr+32, 30, (u8*)"区位码: ",7,0);
				i = (inputPosi-10)/4;
				j = (inputPosi-10)%4;
				if (inputPosi>=10 && inputPosi<34) ShowStringPosi8(rr+32+8, 52, inputStr+10+i*4, 4 ,0,j);
				else ShowString8(rr+32+8, 52, inputStr+30, 4 ,0);
			}
			subLine=0;
			subLine1=0;
			lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 21:           //配置整屏菜单
			ShowString16 (rr, MID(11), (u8*)"配 置 菜 单 ",11,0);
			if (subLine==2 || subLine1==2 || subLine==0) {
				ShowString16 (rr+16, 0, (u8*)"1配置主从机 ",12,2*(MultiLineMenuSelect==0?1:0));
				ShowString16 (rr+16, 40, (u8*)"2回路学习 ",10,2*(MultiLineMenuSelect==1?1:0));
			}
			if (subLine==3 || subLine1==3 || subLine==0) {
				ShowString16 (rr+32, 0, (u8*)"3配置点编辑 ",12,2*(MultiLineMenuSelect==2?1:0));
				ShowString16 (rr+32, 40, (u8*)"4配置调试态 ",12,2*(MultiLineMenuSelect==3?1:0));
			}
			if (subLine==4 || subLine1==4 || subLine==0) {
				ShowString16 (rr+48, 0, (u8*)"5声光分组",9,2*(MultiLineMenuSelect==4?1:0));
		  }
			subLine=0;
			subLine1=0;
			lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 22:           //配置主从机
			ShowString16 (rr, MID(10), (u8*)"配置主从机",10,0);
			ShowString16 (rr+16, 1, (u8*)"本机编号:",9,0);
		  ShowStringPosi8(rr+24, 27, inputStr, 2 ,0,inputPosi);
			Disp_ClearLine(rr+32,16,0,240);
		  i=str2Dec(inputStr,2);
			if (i==0) ShowString16 (rr+16, 34, (u8*)"单机",4,0);
		  else if (i==1) {
				ShowString16 (rr+16, 34, (u8*)"主机",4,0);
				ShowString16 (rr+32, 1, (u8*)"从机数量:",9,0);
				if (inputPosi>1) ShowStringPosi8(rr+40, 27, inputStr+2, 2 ,0,inputPosi-2);
				else ShowString8(rr+40, 27, inputStr+2, 2 ,0);
			}
			else ShowString16 (rr+16, 34, (u8*)"从机",4,0);
			lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 23:           //配置声光分组
			ShowString16 (rr, MID(12), (u8*)"配置声光分组",12,0);

			for (i=1;i<3;i++) {
				if (inputMLStr[i][28]==0) ShowString16 (rr+i*16, 0, (u8*)"点",2,0);
				else ShowString16 (rr+i*16, 0, (u8*)"区",2,0);
				if (inputMLLine==i) ShowStringPosi8 (rr+i*16+8, 6, inputMLStr[i],25,0,inputMLPosi[i]);
				else ShowString8(rr+i*16+8, 6, inputMLStr[i],25,0);
			}
			lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 24:           //配置屏蔽
			ShowString16 (rr, MID(10), (u8*)"配 置 屏 蔽",11,0);
			if (inputStr[28]==0) ShowString16 (rr+16, 0, (u8*)"点位:",5,inputMLLine==1?1:0);
			else if (inputStr[28]==1) ShowString16 (rr+16, 0, (u8*)"区号:",5,inputMLLine==1?1:0);
			else ShowString16 (rr+16, 0, (u8*)"类型:",5,inputMLLine==1?1:0);
		  Disp_ClearLine(rr+16,16,16,224);
			if (inputStr[28]<2 ) {
				if (inputMLLine==1)	ShowStringPosi8 (rr+24, 16, inputStr,25,0,inputPosi);
				else ShowString8 (rr+24, 16, inputStr,25,0);
			}
		  else {
				if (inputMLLine==1)	ShowString16(rr+16, 16, (u8*)dotType(0,inputu8,0), 6 ,1);
				else ShowString16(rr+16, 16, (u8*)dotType(0,inputu8,0), 6 ,0); 
			}
		  
			ShowString16 (rr+32, 0, (u8*)"设置:",5,inputMLLine==2?1:0);
		  ShowString16(rr+32, 20, (u8*)"屏蔽", 4 ,1 - inputStr[29]);
		  ShowString16(rr+32, 43, (u8*)"解除", 4 ,inputStr[29]);
			lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 25:           //查询显示
			switch (nowRecType) {
				case 1:			
					ShowString16 (rr, MID(12), (u8*)"查询火警记录",12,0);
					break;
				
				case 2:			
					ShowString16 (rr, MID(12), (u8*)"查询报警记录",12,0);
					break;
				
				case 3:			
					ShowString16 (rr, MID(12), (u8*)"查询故障记录",12,0);
					break;
				
				case 4:			
					ShowString16 (rr, MID(12), (u8*)"查询操作记录",12,0);
					break;
				
			}
		  lineMod[LineNum] &= 0xFD; 
			if (isFindRec == 0) { ShowString16 (rr+16, 12, (u8*)"无历史记录！",12,0);}
			else {
					if (nowRec.mNum>0) {
						decString(strShow, nowRec.mNum ,2);
						ShowString8 (rr+24, 0, strShow,4,0);
						ShowString16 (rr+16, 6, (u8*)"号机",4,0);
					}
					else ShowString16 (rr+16, 6, (u8*)"本机",4,0);
					sprintf((char *)strShow,"%2d",nowRec.bNum);
					ShowString8 (rr+24, 17, strShow,2,0);
					ShowString16 (rr+16, 23, (u8*)"板",2,0);
					if (nowRec.ioType==251) {
						sprintf((char *)strShow,"%03d",nowRec.dNum);
						ShowString8 (rr+24, 29, strShow,3,0);
						
						ShowString16 (rr+16, 37, (u8*)"点",2,0);
					  ShowString16 (rr+16, 43, (u8*)dotType(nowRec.eventType,tmpVal,0),6,0);
						sprintf((char *)strShow,"%02d-%02d-%02d",nowRec.year,nowRec.mon,nowRec.day);
						ShowString8 (rr+16, 59, strShow,8,0);
						sprintf((char *)strShow,"%02d:%02d:%02d",nowRec.hour,nowRec.min,nowRec.sec);
						ShowString8 (rr+24, 59, strShow,8,0);
					}
					else
					{
						Disp_ClearLine(rr+16,16,120,120);
						sprintf((char *)strShow,"%02d-%02d-%02d",nowRec.year,nowRec.mon,nowRec.day);
						ShowString8 (rr+16, 40, strShow,8,0);
						sprintf((char *)strShow,"%02d:%02d:%02d",nowRec.hour,nowRec.min,nowRec.sec);
						ShowString8 (rr+24, 40, strShow,8,0);
					}
					
					Disp_ClearLine(rr+32,16,0,240);
					
						switch (nowRec.ioType) {

							case 250:
								switch (nowRec.eventType) {
									case 2:
										ShowString16 (rr+32, 6, (u8*)"主电故障",8,0);
										break;

									case 3:
										ShowString16 (rr+32, 6, (u8*)"备电故障",8,0);
										break;

									case 4:
										ShowString16 (rr+32, 6, (u8*)"充电故障",8,0);
										break;

									case 5:
										ShowString16 (rr+32, 6, (u8*)"辅助输出短路",12,0);
										break;
								}
								break;
								
							case 252:
								switch (InterBoard[nowRec.bNum].Type) {
									case IOBOARD :
										if (nowRec.eventType==9) ShowString16 (rr+32, 6, (u8*)"回路板通讯故障",14,0);
										if (nowRec.eventType==10) ShowString16 (rr+32, 6, (u8*)"回路板短路故障",14,0);
										break;

									case POWERBOARD :
										ShowString16 (rr+32, 6, (u8*)"电源板通讯故障",14,0);
										break;

									case FIREBOARD :
										ShowString16 (rr+32, 6, (u8*)"灭火板通讯故障",14,0);
										break;

									case MULTILINEBOARD :
										ShowString16 (rr+32, 6, (u8*)"多线盘通讯故障",14,0);
										break;

									default :
										ShowString16 (rr+32, 6, (u8*)"未知板通讯故障",14,0);
										break;
								}
								break;
								
							case 251:
								ShowString16 (rr, 6, (u8*)typeFullString(tmpVal,nowRec.eventType),8,0);
								ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),nowRec.dNum-1);
								ShowString16 (rr, 30, oneNode.addr,12,0);
								break;
						}
						
				if (disRec>1) ShowString8 (rr+56, 0, (u8 *)"<<",2,0);
				else ShowString8 (rr+56, 0, (u8 *)"  ",2,0);
				ShowString16 (rr+48, 6, (u8*)"当前记录:",9,0);
				sprintf((char *)strShow,"%3d",disRec);
				ShowString8 (rr+56, 30, strShow,3,0);
						
				ShowString16 (rr+48, 40, (u8*)"总记录数:",9,0);
				sprintf((char *)strShow,"%3d",totRec);
				ShowString8 (rr+56, 64, strShow,3,0);
				if (disRec<totRec) ShowString8 (rr+56, 74, (u8 *)">>",2,0);
				else ShowString8 (rr+56, 74, (u8 *)"  ",2,0);
			}
			break;

		case 26:           //电压显示
			Disp_ClearLine(rr,16,0,240);
			ShowString16 (rr, 0, (u8*)"主电:",5,0);
			sprintf((char *)strShow,"%4.1fV",mainVlot*0.2);
			ShowString8 (rr+8, 14, strShow,5,0);
			ShowString16 (rr, 40, (u8*)"备电:",5,0);
			sprintf((char *)strShow,"%4.1fV",backVlot*0.2);
			ShowString8 (rr+8, 54, strShow,5,0);
			break;


	}
}

void Disp_Flash(u8 fl) {
	u8 rr,i;
	u8 strShow[10],tmpVal;

	i=1;
	while (fl!=0) {
		if (((lineMod[i] & 0x02) != 0) && ((fl & 0x01) !=0) ) {
			rr = (i-1)*16;
			switch(nowLine[i]) {
				case 1 :
					tmpVal=ioStat[dispBJ].dType;
					if (flashstat == 0) {
						Disp_ClearLine(rr,16,33,207);
						ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),ioStat[dispBJ].dNum-1);
						ShowString16 (rr, 33, oneNode.addr,12,0);
						ShowString16 (rr, 66, (u8*)typeString(tmpVal,ioStat[dispBJ].eventType),5,0);
					}
					else {
						Disp_ClearLine(rr,16,33,207);
						ShowString16 (rr, 33, (u8*)dotType(ioStat[dispBJ].eventType,tmpVal,0),6,0);
						sprintf((char *)strShow,"%02d-%02d-%02d",ioStat[dispBJ].year,ioStat[dispBJ].mon,ioStat[dispBJ].day);
						ShowString8 (rr, 50, strShow,8,0);
						sprintf((char *)strShow,"%02d:%02d:%02d",ioStat[dispBJ].hour,ioStat[dispBJ].min,ioStat[dispBJ].sec);
						ShowString8 (rr+8, 50, strShow,8,0);
					}
					break;

				case 51 :
					tmpVal=nowRec.dType;
					if (flashstat == 0) {
						Disp_ClearLine(rr,16,33,207);
						ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),nowRec.dNum-1);
						ShowString16 (rr, 33, oneNode.addr,12,0);
						ShowString16 (rr, 66, (u8*)typeString(tmpVal,nowRec.eventType),5,0);
					}
					else {
						Disp_ClearLine(rr,16,33,207);
						ShowString16 (rr, 33, (u8*)dotType(nowRec.eventType,tmpVal,0),6,0);
						sprintf((char *)strShow,"%02d-%02d-%02d",nowRec.year,nowRec.mon,nowRec.day);
						ShowString8 (rr, 50, strShow,8,0);
						sprintf((char *)strShow,"%02d:%02d:%02d",nowRec.hour,nowRec.min,nowRec.sec);
						ShowString8 (rr+8, 50, strShow,8,0);
					}
					break;

				case 2 :
					ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),pinbiStat[dispPinbi].dNum-1);
					tmpVal=oneNode.type;
					if (flashstat == 0) {
						Disp_ClearLine(rr,16,33,207);
//						ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),pinbiStat[dispPinbi].dNum-1);
						ShowString16 (rr, 33, oneNode.addr,12,0);
					}
					else {
						Disp_ClearLine(rr,16,33,207);
						ShowString16 (rr, 33, (u8*)dotType(pinbiStat[dispPinbi].eventType,tmpVal,1),6,0);
						sprintf((char *)strShow,"%02d-%02d-%02d",pinbiStat[dispPinbi].year,pinbiStat[dispPinbi].mon,pinbiStat[dispPinbi].day);
						ShowString8 (rr, 50, strShow,8,0);
						sprintf((char *)strShow,"%02d:%02d:%02d",pinbiStat[dispPinbi].hour,pinbiStat[dispPinbi].min,pinbiStat[dispPinbi].sec);
						ShowString8 (rr+8, 50, strShow,8,0);
					}
					break;

				case 3 :
					tmpVal=fireStat[dispFire].dType;
					if (flashstat == 0) {
						Disp_ClearLine(rr,16,33,207);
						ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),fireStat[dispFire].dNum-1);
						ShowString16 (rr, 33, oneNode.addr,12,0);
//						ShowString16 (rr, 66, typeString(tmpVal,fireStat[dispFire].eventType),5,0);
					}
					else {
						Disp_ClearLine(rr,16,33,207);
						ShowString16 (rr, 33, (u8*)dotType(fireStat[dispFire].eventType,tmpVal,1),6,0);
						sprintf((char *)strShow,"%02d-%02d-%02d",fireStat[dispFire].year,fireStat[dispFire].mon,fireStat[dispFire].day);
						ShowString8 (rr, 50, strShow,8,0);
						sprintf((char *)strShow,"%02d:%02d:%02d",fireStat[dispFire].hour,fireStat[dispFire].min,fireStat[dispFire].sec);
						ShowString8 (rr+8, 50, strShow,8,0);
					}
					break;

				case 53 :
					tmpVal=nowRec.dType;
					if (flashstat == 0) {
						Disp_ClearLine(rr,16,33,207);
						ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),nowRec.dNum-1);
						ShowString16 (rr, 33, oneNode.addr,12,0);
//						ShowString16 (rr, 66, typeString(tmpVal,fireStat[dispFire].eventType),5,0);
					}
					else {
						Disp_ClearLine(rr,16,33,207);
						ShowString16 (rr, 33, (u8*)dotType(nowRec.eventType,tmpVal,1),6,0);
						sprintf((char *)strShow,"%02d-%02d-%02d",nowRec.year,nowRec.mon,nowRec.day);
						ShowString8 (rr, 50, strShow,8,0);
						sprintf((char *)strShow,"%02d:%02d:%02d",nowRec.hour,nowRec.min,nowRec.sec);
						ShowString8 (rr+8, 50, strShow,8,0);
					}
					break;

				case 4 :
					tmpVal=errStat[dispErr].dType;
					if (flashstat != 0) {
						Disp_ClearLine(rr,16,33,207);
						switch (errStat[dispErr].ioType) {

							case 250:
								switch (errStat[dispErr].eventType) {
									case 2:
										ShowString16 (rr, 33, (u8*)"主电故障",8,0);
										break;

									case 3:
										ShowString16 (rr, 33, (u8*)"备电故障",8,0);
										break;

									case 4:
										ShowString16 (rr, 33, (u8*)"充电故障",8,0);
										break;

									case 5:
										ShowString16 (rr, 33, (u8*)"辅助输出短路",12,0);
										break;
								}
								sprintf((char *)strShow,"%02d-%02d-%02d",errStat[dispErr].year,errStat[dispErr].mon,errStat[dispErr].day);
								ShowString8 (rr, 55, strShow,8,0);
								sprintf((char *)strShow,"%02d:%02d:%02d",errStat[dispErr].hour,errStat[dispErr].min,errStat[dispErr].sec);
								ShowString8 (rr+8, 55, strShow,8,0);
								break;
								
							case 252:
								sprintf((char *)strShow,"%4d",errStat[dispErr].bNum);
								ShowString8 (rr+8, 22, strShow,4,0);
								switch (InterBoard[errStat[dispErr].bNum].Type) {
									case IOBOARD :
										if (errStat[dispErr].eventType==9) ShowString16 (rr, 33, (u8*)"回路板通讯故障",14,0);
										if (errStat[dispErr].eventType==10) ShowString16 (rr, 33, (u8*)"回路板短路故障",14,0);
										break;

									case POWERBOARD :
										ShowString16 (rr, 33, (u8*)"电源板通讯故障",14,0);
										break;

									case FIREBOARD :
										ShowString16 (rr, 33, (u8*)"灭火板通讯故障",14,0);
										break;

									case MULTILINEBOARD :
										ShowString16 (rr, 33, (u8*)"多线盘通讯故障",14,0);
										break;

									default :
										ShowString16 (rr, 33, (u8*)"未知板通讯故障",14,0);
										break;
								}
								break;
								
							case 251:
								ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),errStat[dispErr].dNum-1);
								ShowString16 (rr, 33, oneNode.addr,12,0);
								ShowString16 (rr, 66, (u8*)typeString(tmpVal,errStat[dispErr].eventType),5,0);
								break;
						}
					}
					else if (errStat[dispErr].ioType!=250){
						Disp_ClearLine(rr,16,33,207);
						if (errStat[dispErr].ioType==251) ShowString16 (rr, 33, (u8*)dotType(errStat[dispErr].eventType,tmpVal,0),6,0);
						sprintf((char *)strShow,"%02d-%02d-%02d",errStat[dispErr].year,errStat[dispErr].mon,errStat[dispErr].day);
						ShowString8 (rr, 50, strShow,8,0);
						sprintf((char *)strShow,"%02d:%02d:%02d",errStat[dispErr].hour,errStat[dispErr].min,errStat[dispErr].sec);
						ShowString8 (rr+8, 50, strShow,8,0);
					}
					break;

				case 54 :
					tmpVal=nowRec.dType;
					if (flashstat != 0) {
						Disp_ClearLine(rr,16,33,207);
						switch (nowRec.ioType) {

							case 250:
								switch (nowRec.eventType) {
									case 2:
										ShowString16 (rr, 33, (u8*)"主电故障",8,0);
										break;

									case 3:
										ShowString16 (rr, 33, (u8*)"备电故障",8,0);
										break;

									case 4:
										ShowString16 (rr, 33, (u8*)"充电故障",8,0);
										break;

									case 5:
										ShowString16 (rr, 33, (u8*)"辅助输出短路",12,0);
										break;
								}
								break;
								
							case 252:
								sprintf((char *)strShow,"%4d",nowRec.bNum);
								ShowString8 (rr+8, 22, strShow,4,0);
								switch (InterBoard[nowRec.bNum].Type) {
									case IOBOARD :
										if (nowRec.eventType==9) ShowString16 (rr, 33, (u8*)"回路板通讯故障",14,0);
										if (nowRec.eventType==10) ShowString16 (rr, 33, (u8*)"回路板短路故障",14,0);
										break;

									case POWERBOARD :
										ShowString16 (rr, 33, (u8*)"电源板通讯故障",14,0);
										break;

									case FIREBOARD :
										ShowString16 (rr, 33, (u8*)"灭火板通讯故障",14,0);
										break;

									case MULTILINEBOARD :
										ShowString16 (rr, 33, (u8*)"多线盘通讯故障",14,0);
										break;

									default :
										ShowString16 (rr, 33, (u8*)"未知板通讯故障",14,0);
										break;
								}
								break;
								
							case 251:
								ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),nowRec.dNum-1);
								ShowString16 (rr, 33, oneNode.addr,12,0);
								ShowString16 (rr, 66, (u8*)typeString(tmpVal,nowRec.eventType),5,0);
								break;
						}
					}
					else {
						Disp_ClearLine(rr,16,33,207);
						if (nowRec.ioType==251) ShowString16 (rr, 33, (u8*)dotType(nowRec.eventType,tmpVal,0),6,0);
						sprintf((char *)strShow,"%02d-%02d-%02d",nowRec.year,nowRec.mon,nowRec.day);
						ShowString8 (rr, 50, strShow,8,0);
						sprintf((char *)strShow,"%02d:%02d:%02d",nowRec.hour,nowRec.min,nowRec.sec);
						ShowString8 (rr+8, 50, strShow,8,0);
					}
					break;
			}
		}
		fl = (fl >> 1);
		i++;
	}

}

void Disp_Screen(void)
{
	
	if (flashLine == 0 && newTime == 0) {
		LCD_BON;
		bonTime=BACKONTIME;
	}
	
	if (oldScr != newScr) {
		CLR;
		LCD_BON;
		clearLastTime=0;
		switch (newScr) {
			case 1: 
				if (myConf1.mRunningStat == 0) ShowString8 (4, MID(13), (u8*)"FS1000 V1.0 T",13,0);
			  else ShowString8 (4, MID(11), (u8*)"FS1000 V1.0",11,0);

				Disp_ShowTime(16,5,1);
			  Disp_Line(1,3);
			  if (totAct>0) 
					Disp_Line(5,4);
				else if (totErr>0) 
					Disp_Line(4,4);
			  else 
					Disp_Line(2,4);
			  flashLine=0x03;
				flashstat=0;
			  break;

			case 2: 
				Disp_Line(6,1);
				Disp_Line(3,2);
			  Disp_Line(1,3);
			  if (totAct>0) Disp_Line(5,4);
				else if (totErr>0) Disp_Line(4,4);
			  else Disp_Line(2,4);
				break;

			case 3:		//配置点编辑 
				Disp_Line(19,1);
				nowLine[2] = 0;
				nowLine[3] = 0;
				nowLine[4] = 0;
				break;

			case 4:     //配置整屏菜单
				Disp_Line(21,1);
				nowLine[2] = 0;
				nowLine[3] = 0;
				nowLine[4] = 0;
				break;

			case 5:		//配置主从机 
				Disp_Line(22,1);
				nowLine[2] = 0;
				nowLine[3] = 0;
				nowLine[4] = 0;
				break;

			case 6:		//配置声光分组 
				Disp_Line(23,1);
				nowLine[2] = 0;
				nowLine[3] = 0;
				nowLine[4] = 0;
				break;

			case 7:		//配置屏蔽 
				Disp_Line(24,1);
				nowLine[2] = 0;
				nowLine[3] = 0;
				nowLine[4] = 0;
				break;

			case 8:		//查询显示屏幕 
				Disp_Line(25,1);
				nowLine[2] = 0;
				nowLine[3] = 0;
				nowLine[4] = 0;
				break;

		}
		oldScr=newScr;
	}

		if (updateLine !=0) { updateLine=0; Disp_Line(nowLine[oldTab],oldTab); flashLine=(1 << (oldTab-1)); flashstat=1;}

		if (dispLine !=0) { 
			Disp_Line(nowLine[dispLine],dispLine); 
			flashLine=(1 << (dispLine-1)); 
			flashstat=1;
			dispLine=0; 
		}

	if (oldTab != newTab) {
		lineMod[oldTab] &= 0xFE;
		lineMod[newTab] |= 0x01;
		Disp_Line(nowLine[oldTab],oldTab);
		Disp_Line(nowLine[newTab],newTab);
		oldTab=newTab;
	}

  if (oldScr<3 || nowLine[4]==53) {
		if (flashLine != 0) {  
			Disp_Flash(flashLine); 
			flashLine = 0; 
		}
	
		if (oldLine != newLine) {
			oldLine=newLine;
		}

		if (newTime == 1) { newTime = 0; Disp_ShowTime(16,5,1);}
		Disp_ShowLine(31);
		Disp_ShowLine(47);
	}
}

void Back_to_Screen(void) {
	if (totFire>0) newScr=2;
	else newScr=1;
	inputMode=0;
	newTab=4;
	dispLine=4;
	if (totAct>0) nowLine[4] = 5;
	else if (totErr>0) nowLine[4] = 4;
	else nowLine[4] = 2;
}
