
#include "USART.h"
#include "GloablVar.h"
#include "LCD_WO24064C.h"
#include "stdio.h"
#include "rtc_time.h"

//#define MID(A) (8*(30-(A))/2)/3
#define MID(A) 8*(30-(A))/2
#define CLR Disp(0x00,0x00)


extern IOSTAT  ioStat[1200] ;   //共5类 报警
extern IOSTAT  fireStat[1200];   //共5类 火警
extern IOSTAT  pinbiStat[1200];   //共5类 屏蔽
extern IOSTAT  errStat[1200];   //共5类 故障
extern IOSTAT  actStat[1200];   //共5类 联动
extern u8 ioInit[20][240];

u8 windowRam[1600];

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

u8 getNodeStat(u8 s) 
{
	u8 i;
	for (i=0;i<8 && ((s & (1 << i)) ==0) ;i++);
	
	return i;
}

void showMessage(u8 tt, u8 tl)
{
	clearLastTime=tl;
	Disp_ClearLine(14,2,0,240);
#ifndef ENGLISH
	switch(tt) {
		case 1:
			ShowString16 (14, 5, (u8*)"输入数据有误，请重新输入！",26,0);
			break;
		case 2:
			ShowString16 (14, 5, (u8*)"保存中... 请稍候！",18,0);
			break;
		case 3:
			ShowString16 (14, 5, (u8*)"数据已保存，请继续输入！",24,0);
			break;
		case 4:
			ShowString16 (14, 5, (u8*)"输入机器号错误，请重新输入！",28,0);
		  break;
		case 5:
			ShowString16 (14, 5, (u8*)"数据已保存！",12,0);
			break;
	}
#else
	switch(tt) {
		case 1:
			ShowString816 (14, 5, (u8*)"Invalid data, Please reinput!",29,0);
			break;
		case 2:
			ShowString816 (14, 5, (u8*)"Saving...",9,0);
			break;
		case 3:
			ShowString816 (14, 5, (u8*)"Data saved, Please go on!",25,0);
			break;
		case 4:
			ShowString816 (14, 5, (u8*)"Invalid box No., Please reinput!",32,0);
		  break;
		case 5:
			ShowString816 (14, 5, (u8*)"Data Saved!",11,0);
			break;
	}
#endif
}


void SetOneLineMenu(u8 tt) {
	OneLineMenuSelect=0;
	OLMstposi=0;
	OLMedposi=0;
	OneLineNowMenu = tt;
	if (tt==1) MenuDeep=0;
	
	switch (tt) {
		case 1:
			cpChinese2Str(OneLineMenuItem[0], (u8*)"查询", 4);
		  OneLineMenuSize[0]=4;
			cpChinese2Str(OneLineMenuItem[1], (u8*)"查档", 4);
		  OneLineMenuSize[1]=4;
			cpChinese2Str(OneLineMenuItem[2], (u8*)"屏蔽", 4);   //3行屏蔽 点 区 类型
		  OneLineMenuSize[2]=4;
			cpChinese2Str(OneLineMenuItem[3], (u8*)"设置", 4);
		  OneLineMenuSize[3]=4;
			cpChinese2Str(OneLineMenuItem[4], (u8*)"调试", 4);  //1. 点位测试(点位测试通讯100次) 2.点位分析(跟踪回码数据) 
		  OneLineMenuSize[4]=4;
			cpChinese2Str(OneLineMenuItem[5], (u8*)"控制", 4);  //控制声光输出，关闭，及2路继电器输出，关闭
		  OneLineMenuSize[5]=4;
			cpChinese2Str(OneLineMenuItem[6], (u8*)"配置", 4);
		  OneLineMenuSize[6]=4;
		  OneLineMenuMax=7;
			break;

		case 2:
			cpChinese2Str(OneLineMenuItem[0], (u8*)"配置", 4); //探测器数量，（屏蔽数量）烟感数量 ，温感数量，手报，声光
		  OneLineMenuSize[0]=4;
			cpChinese2Str(OneLineMenuItem[1], (u8*)"点位", 4);  //点位数据
		  OneLineMenuSize[1]=4;
			cpChinese2Str(OneLineMenuItem[2], (u8*)"通讯", 4);  //机器编号，主机地址（汉字地址），波特率，通讯状态
		  OneLineMenuSize[2]=4;
			cpChinese2Str(OneLineMenuItem[3], (u8*)"电源", 4);  //主备电状态及电压
		  OneLineMenuSize[3]=4;
		  OneLineMenuMax=4;
			break;

		case 3:
			cpChinese2Str(OneLineMenuItem[0], (u8*)"校时", 4);
		  OneLineMenuSize[0]=4;
			cpChinese2Str(OneLineMenuItem[1], (u8*)"声光手自动", 10);
		  OneLineMenuSize[1]=10;
			cpChinese2Str(OneLineMenuItem[2], (u8*)"启动自检", 8); 
		  OneLineMenuSize[2]=8;
			cpChinese2Str(OneLineMenuItem[3], (u8*)"打印", 4);
		  OneLineMenuSize[3]=4;
			cpChinese2Str(OneLineMenuItem[4], (u8*)"密码", 4);
		  OneLineMenuSize[4]=4;
		  OneLineMenuMax=5;
			break;

		case 4:
			cpChinese2Str(OneLineMenuItem[0], (u8*)"主从机", 6);
		  OneLineMenuSize[0]=6;
			cpChinese2Str(OneLineMenuItem[1], (u8*)"回路学习", 8);
		  OneLineMenuSize[1]=8;
			cpChinese2Str(OneLineMenuItem[2], (u8*)"点编辑", 6); 
		  OneLineMenuSize[2]=6;
			cpChinese2Str(OneLineMenuItem[3], (u8*)"调试态", 6);   //开，关 给回路板发信息初始化
		  OneLineMenuSize[3]=6;
			cpChinese2Str(OneLineMenuItem[4], (u8*)"声光分组", 8);
		  OneLineMenuSize[4]=8;
			cpChinese2Str(OneLineMenuItem[5], (u8*)"联动", 2);
		  OneLineMenuSize[5]=2;
		  OneLineMenuMax=5;
			break;

		case 5:
			cpChinese2Str(OneLineMenuItem[0], (u8*)"火警档案", 8);
		  OneLineMenuSize[0]=8;
			cpChinese2Str(OneLineMenuItem[1], (u8*)"启动档案", 8);
		  OneLineMenuSize[1]=8;
			cpChinese2Str(OneLineMenuItem[2], (u8*)"故障档案", 8); 
		  OneLineMenuSize[2]=8;
			cpChinese2Str(OneLineMenuItem[3], (u8*)"其他档案", 8);
		  OneLineMenuSize[3]=8;
//			cpChinese2Str(OneLineMenuItem[3], (u8*)"4操作记录 ", 10);
//		  OneLineMenuSize[3]=10;
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
			cpChinese2Str(TwoChoiceMenuItem[0], (u8*)"手动", 4);
			cpChinese2Str(TwoChoiceMenuItem[1], (u8*)"自动", 4);
		  TwoChoiceMenuItemSize=4;
			break;

		case 2:
			cpChinese2Str(TwoChoiceMenuTitle, (u8*)"设置: ", 6);
		  TwoChoiceMenuTitleSize=6;
			cpChinese2Str(TwoChoiceMenuItem[0], (u8*)"用户密码", 8);
			cpChinese2Str(TwoChoiceMenuItem[1], (u8*)"调试密码", 8);
		  TwoChoiceMenuItemSize=8;
			break;

		case 3:
			cpChinese2Str(TwoChoiceMenuTitle, (u8*)"调试态： ", 8);
		  TwoChoiceMenuTitleSize=8;
			cpChinese2Str(TwoChoiceMenuItem[0], (u8*)"调试", 4);
			cpChinese2Str(TwoChoiceMenuItem[1], (u8*)"正常", 4);
		  TwoChoiceMenuItemSize=4;
			break;

		case 4:
			cpChinese2Str(TwoChoiceMenuTitle, (u8*)"自动打印： ", 10);
		  TwoChoiceMenuTitleSize=10;
			cpChinese2Str(TwoChoiceMenuItem[0], (u8*)"关", 2);
			cpChinese2Str(TwoChoiceMenuItem[1], (u8*)"开", 2);
		  TwoChoiceMenuItemSize=2;
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


void Disp_ShowLine(u8 ll){
	u8 i,tmp[240];
	LCDSetPosi(ll/8,0);
	get_dat();
	for (i=0;i<240;i++) {
		  tmp[i]=get_dat() | (1<<(ll%8));
  }
	LCDSetPosi(ll/8,0);
	for (i=0;i<240;i++) {
			write_dat(tmp[i]); 
  }
}

u8 set1Data(u8 *n, u8 s, u8 e,u8 m)    //将数字n的第s到e位置1 或 0
{
	u8 i;
	
	for (i=s;i<=e;i++) {
		if (m==1) *n |= (1<<i);
		else *n &= (~(1<<i));
	}
	return *n;
}

void Disp_DrawLine(u8 ll, u8 rs, u8 re, u8 m){
	
	u8 i,tmp[240];
	if (m==0) {
		if (re>239) re=239;
		LCDSetPosi(ll/8,239-re);
		get_dat();
		for (i=rs;i<re;i++) {
		  tmp[i-rs]=get_dat() | (1<<(ll%8));
		}
		LCDSetPosi(ll/8,239-re);
		for (i=rs;i<re;i++) {
			write_dat(tmp[i-rs]); 
		}
	}
	else {
		if (rs/8==re/8) {
			LCDSetPosi(rs/8,239-ll);
			get_dat();
			tmp[0]=get_dat();
			LCDSetPosi(rs/8,239-ll);
			write_dat(set1Data(tmp, rs%8, re%8,1)); 
		}
		else {
			for (i=0;i<(re/8-rs/8);i++) {
				LCDSetPosi(i+rs/8,239-ll);
				get_dat();
				tmp[0]=get_dat();
				LCDSetPosi(i+rs/8,239-ll);
				if (i==0) write_dat(set1Data(tmp, rs%8, 7,1));
				else write_dat(0xff);
			}
			LCDSetPosi(re/8,239-ll);
			get_dat();
			tmp[0]=get_dat();
			LCDSetPosi(re/8,239-ll);
			write_dat(set1Data(tmp, 0, re%8,1)); 
		}
	}
}

void Disp_DrawRect(u8 x, u8 y, u8 x1, u8 y1, u8 m){  //画长方型 0 擦除 1 边框 2 擦除后边框
	u8 i,j,t;
	
	if (m==0 || m==2) {
		if (y/8==y1/8) {
			for (i=x;i<=x1;i++) {
				LCDSetPosi(y/8,239-i);
				get_dat();
				t=get_dat();
				LCDSetPosi(y/8,239-i);
				write_dat(set1Data(&t,  y%8, y1%8,0));
			}
		}
		else {
			for (j=0;j<(y1/8-y/8);j++) {
				for (i=x;i<=x1;i++) {
					LCDSetPosi(j+y/8,239-i);
					get_dat();
					t=get_dat();
					LCDSetPosi(j+y/8,239-i);
					if (j==0) write_dat(set1Data(&t,  y%8, 7,0));
					else write_dat(0x00);
				}
			}
			for (i=x;i<=x1;i++) {
				LCDSetPosi(y1/8,239-i);
				get_dat();
				t=get_dat();
				LCDSetPosi(y1/8,239-i);
				write_dat(set1Data(&t,  0, y1%8,0));
			}
		}
	}
	
	if (m>0) {
		Disp_DrawLine(y, x, x1, 0);
		Disp_DrawLine(y1, x, x1, 0);
		Disp_DrawLine(x, y, y1, 1);
		Disp_DrawLine(x1, y, y1, 1);
	}
}

void ShowWindow(u8 wn,u8 m) {
u8 x,y,x1,y1,l,i,j;
u16 k;
u8 wt[30],wtl,wc[5][15],wcl[5],tc,tcl;
u8 strShow[15];
	
	switch (wn/16) {
		case 1:
			x=75; x1=165; y=43;y1=93; l=6;   //是否保存
			break;
		case 2:
			x=40; x1=200; y=43;y1=93; l=6;   //保存哪些栏目数据
			break;
		case 0x0F:
			x=40; x1=200; y=43;y1=93; l=6;   //输入密码
			break;
		default:
			RESETBIT(IsWindow);
			return;
	}
	switch (wn%16) {
		case 1:
			sprintf(wt,"是否保存？"); wtl=10;
			sprintf(wc[0],"是"); wcl[0]=2;
			sprintf(wc[1],"否"); wcl[1]=2;
			tc=2;
			tcl=6;
			break;
		case 2:
			sprintf(wt,"保存哪些栏目数据？"); wtl=18;
			sprintf(wc[0],"当前"); wcl[0]=4;
			sprintf(wc[1],"所有"); wcl[1]=4;
			sprintf(wc[2],"放弃"); wcl[2]=4;
			tc=3;
			tcl=16;
			break;
		case 0x0F:
			sprintf(wt,"请输入密码"); wtl=10;
			sprintf(wc[0],"密码错误！"); wcl[0]=10;
			break;
		default:
			RESETBIT(IsWindow);
			return;
	}
	
	if (m==1) {
		for (i=0,k=0;i<(y1/8-y/8)+1;i++) {
			LCDSetPosi(i+y/8,239-x1);
			for (j=x;j<=x1;j++) {
				write_dat(windowRam[k++]);
			}
		}
		RESETBIT(IsWindowShowed);
		return;
	}
	if (ISRESET(IsWindowShowed)) {
		for (i=0,k=0;i<(y1/8-y/8)+1;i++) {
			LCDSetPosi(i+y/8,239-x1);
			get_dat();
			for (j=x;j<=x1;j++) {
				windowRam[k++]=get_dat();
			}
		}
		Disp_DrawRect(x,y,x1,y1,2);
		ShowString16 (l, x+5, wt,wtl,0);
		SETBIT(IsWindowShowed);
	}
	
	switch (wn/16) {
		case 1:
		case 2:
			tcl=x+(x1-x-tcl*8)/2;
			for (i=0;i<tc;i++) {
				ShowString16 (l+3, tcl, wc[i],wcl[i],i==(windowSelect & 0x0F)?1:0);
				tcl += (wcl[i]*8+16);
			}
	
			windowSelect = (windowSelect << 4) | (windowSelect & 0x0F);
			windowTc=tc;
			break;
		case 0x0F:
			if (windowTime>0) {
				ShowString16(l+3, x+5, wc[0], wcl[0],0);
			}
			else {
				for (i=0;i<inputPosi;i++) strShow[i]='*';
				strShow[i]='_';
				ShowString8(l+3, x+5, strShow ,i+1,0);
			}
			break;
	}
}
void Disp_ClearLine(u8 ll,u8 nl,u8 rr, u8 nr){
	u8 i,j;
	for (i=0;i<nl;i++) {
	  LCDSetPosi(ll+i,rr);
	  for (j=0;j<nr;j++) {
			write_dat(0x00); 
    }
	}
}

char *typeString(u8 dt,u8 t)
{
	if (t<=2)
		switch (t) {
			case 0:
				return("  丢失  ");
			case 1:
				return("  重码  ");
			case 2:
				return("类型错误");
		}
		
	switch(dt) {
		case 1 :       //光电
			switch (t) {
				case 3:
					return("内部故障");
				case 5:
					return("  报警  ");  //火警类
			}
			break;

		case 2 :       //温感
			switch (t) {
				case 3:
					return("内部故障");
				case 5:
					return("  报警  ");  //火警类
			}
			break;

		case 3 :       //复合
			switch (t) {
				case 3:
					return("内部故障");
				case 4:
					return("  污染  ");
				case 5:
					return("烟感报警");   //火警类
				case 6:
					return("温感报警");  //火警类
			}
			break;

		case 4 :       //手报
			switch (t) {
				case 5:
					return("  报警  ");   //火警类
			}
			break;

		case 5 :       //消火栓
			switch (t) {
				case 5:
					return("  报警  ");  //报警类   改启动文字
			}
			break;

		case 6 :       //输入
			switch (t) {
				case 3:
					return("终端故障");
				case 5:
					return("  报警  "); //报警类
			}
			break;

		case 7 :       //输入出
			switch (t) {
				case 3:
					return("输入故障"); 
				case 4:
					return("输出故障");  
				case 5:
					return("输入反馈"); //报警类
				case 6:
					return("输出动作"); //报警类
			}
			break;

		case 8 :       //声光
			switch (t) {
				case 3:
					return("内部故障");
				case 5:
					return("  报警  ");  //报警类
			}
			break;
	}
	return("  未知  ");
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
		 return("  空    ");
		case 1:
		 return("光电感烟");
		case 2:
		 return("  温感  ");
		case 3:
		 return("烟温复合");
		case 4:
		 return("  手报  ");
		case 5:
		 return("消火栓  ");
		case 6:
		 return("  输入  ");
		case 7:
		 return("输入输出");
		case 8:
		 return("  声光  ");
		case 9:
		 return("接口模块");
	}
	return("  未知  ");
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
  
#ifndef ENGLISH
	switch(mod) {
		case 1:
			decString(strShow, machineDate.tm_year,4);
			ShowString816 (ll, rr, strShow, 4 ,0);
			ShowString16 (ll, rr+32, (u8*)"年", 2 ,0);
			
		  decString(strShow, machineDate.tm_mon + 1,2);
			ShowString816 (ll, rr+48, strShow, 2 ,0);
			ShowString16 (ll, rr+64, (u8*)"月", 2 ,0);
		
		  decString(strShow, machineDate.tm_mday,2);
			ShowString816 (ll, rr+80, strShow, 2 ,0);
			ShowString16 (ll, rr+96, (u8*)"日", 2 ,0);
		
		  decString(strShow, machineDate.tm_hour,2);
			ShowString816 (ll, rr+112, strShow, 2 ,0);
			ShowString16 (ll, rr+128, (u8*)"时", 2 ,0);
		
		  decString(strShow, machineDate.tm_min,2);
			ShowString816 (ll, rr+144, strShow, 2 ,0);
			ShowString16 (ll, rr+160, (u8*)"分", 2 ,0);
		
		  decString(strShow, machineDate.tm_sec,2);
			ShowString816 (ll, rr+176, strShow, 2 ,0);
			ShowString16 (ll, rr+192, (u8*)"秒", 2 ,0);

//			sprintf((char *)strShow,"%02x%02x",testByte0,testByte1);
//			ShowString8 (ll+1, rr+208, strShow, 4 ,0);
			break;
	}
#else
	rr=rr+32;
	switch(mod) {
		case 1:
			decString(strShow, machineDate.tm_year,4);
			ShowString816 (ll, rr, strShow, 4 ,0);
			ShowString816 (ll, rr+32, (u8*)"/", 1 ,0);
			
		  sprintf(strShow, "%02d",machineDate.tm_mon + 1);
			ShowString816 (ll, rr+40, strShow, 2 ,0);
			ShowString816 (ll, rr+56, (u8*)"/", 1 ,0);
		
		  sprintf(strShow, "%02d",machineDate.tm_mday);
			ShowString816 (ll, rr+64, strShow, 2 ,0);
//			ShowString816 (ll, rr+80, (u8*)"-", 1 ,0);
		
		  sprintf(strShow, "%02d",machineDate.tm_hour);
			ShowString816 (ll, rr+88, strShow, 2 ,0);
			ShowString816 (ll, rr+104, (u8*)":", 1 ,0);
		
		  sprintf(strShow, "%02d",machineDate.tm_min);
			ShowString816 (ll, rr+112, strShow, 2 ,0);
			ShowString816 (ll, rr+128, (u8*)":", 1 ,0);
		
		  sprintf(strShow, "%02d",machineDate.tm_sec);
			ShowString816 (ll, rr+136, strShow, 2 ,0);
//			ShowString816 (ll, rr+152, (u8*)":", 1 ,0);

//			sprintf((char *)strShow,"%02x%02x",testByte0,testByte1);
//			ShowString8 (ll+1, rr+208, strShow, 4 ,0);
			break;
	}
#endif
}

void Disp_Line(u8 LineType,u8 LineNum) {
	
	u8 strShow[20],tmpVal,i,j;
	u8 rr = (LineNum-1)*4;

	nowLine[LineNum] = LineType;
	switch (LineType) {
	//	case 0:
	//	  Disp_ClearLine(rr,16,0,240);
	//		break;
		
		case 1:
		  Disp_ClearLine(rr,4,0,240);
			ShowString16 (rr+1, 0, (u8*)"报警",4,lineMod[LineNum] & 0x01);
		  lineMod[LineNum] &= 0xFD;
		  if (totBJ>0) {
				sprintf((char *)strShow,"%04d",dispBJ);
//			decString(strShow, dispBJ,4);
				ShowString8 (rr, 32, strShow,4,0);
				sprintf((char *)strShow,"%04d",totBJ);
//			decString(strShow, totBJ,4);
				ShowString8 (rr+1, 32, strShow,4,0);
				if (errStat[dispErr].mNum>0) {
					decString(strShow, ioStat[dispBJ].mNum ,3);  //单回路 3位
					ShowString8 (rr, 68, strShow,3,0);
				}
				sprintf((char *)strShow,"%1d%03d",ioStat[dispBJ].bNum,ioStat[dispBJ].dNum);
//					sprintf((char *)strShow,"%03d",errStat[dispErr].dNum);   // 单回路 3位 无板号
				ShowString8 (rr+1, 68, strShow,4,0);
				tmpVal=ioStat[dispBJ].dType;
				
					ShowString16 (rr, 96, (u8*)dotType(ioStat[dispBJ].eventType,tmpVal,0),8,0);
				sprintf((char *)strShow,"%02d-%02d-%02d",ioStat[dispBJ].year,ioStat[dispBJ].mon,ioStat[dispBJ].day);
				ShowString8 (rr, 164, strShow,8,0);
				sprintf((char *)strShow,"%02d:%02d:%02d",ioStat[dispBJ].hour,ioStat[dispBJ].min,ioStat[dispBJ].sec);
				ShowString8 (rr+1, 164, strShow,8,0);
					
				ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),ioStat[dispBJ].dNum-1);
				ShowString16 (rr+2, 32, oneNode.addr,16,0);
				ShowString16 (rr+2, 164, (u8*)typeString(tmpVal,ioStat[dispBJ].eventType),8,0);
		
			}
			else ShowString16 (rr+1, 64, "无报警信息",10,0);
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
		  Disp_ClearLine(rr,4,0,240);
			ShowString16 (rr+1, 0, (u8*)"屏蔽",4,lineMod[LineNum] & 0x01);
		  lineMod[LineNum] &= 0xFD;
		  if (totPinbi>0) {
				sprintf((char *)strShow,"%04d",dispPinbi);
				ShowString8 (rr, 32, strShow,4,0);
				sprintf((char *)strShow,"%04d",totPinbi);
				ShowString8 (rr+1, 32, strShow,4,0);
				if (pinbiStat[dispPinbi].mNum>0) {
					decString(strShow, pinbiStat[dispPinbi].mNum ,3);  //单回路 3位
					ShowString8 (rr, 68, strShow,3,0);
				}
				sprintf((char *)strShow,"%1d%03d",pinbiStat[dispPinbi].bNum,pinbiStat[dispPinbi].dNum);
//					sprintf((char *)strShow,"%03d",errStat[dispErr].dNum);   // 单回路 3位 无板号
				ShowString8 (rr+1, 68, strShow,4,0);
				tmpVal=pinbiStat[dispPinbi].dType;
				
				ShowString16 (rr, 96, (u8*)dotType(pinbiStat[dispPinbi].eventType,tmpVal,0),8,0);
				sprintf((char *)strShow,"%02d-%02d-%02d",pinbiStat[dispPinbi].year,pinbiStat[dispPinbi].mon,pinbiStat[dispPinbi].day);
				ShowString8 (rr, 164, strShow,8,0);
				sprintf((char *)strShow,"%02d:%02d:%02d",pinbiStat[dispPinbi].hour,pinbiStat[dispPinbi].min,pinbiStat[dispPinbi].sec);
				ShowString8 (rr+1, 164, strShow,8,0);
					
				ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),pinbiStat[dispPinbi].dNum-1);
				ShowString16 (rr+2, 32, oneNode.addr,16,0);
				ShowString16 (rr+2, 164, (u8*)typeString(tmpVal,pinbiStat[dispPinbi].eventType),8,0);
		
			}
			else ShowString16 (rr+1, 64, "无屏蔽信息",10,0);
		  lineMod[LineNum] |= 0x04; //可获焦点
		  break;

		case 3:
		  Disp_ClearLine(rr,4,0,240);
			ShowString16 (rr+1, 0, (u8*)"火警",4,lineMod[LineNum] & 0x01);
		  lineMod[LineNum] &= 0xFD;
		  if (totFire>0) {
				sprintf((char *)strShow,"%04d",dispFire);
				ShowString8 (rr, 32, strShow,4,0);
				sprintf((char *)strShow,"%04d",totFire);
				ShowString8 (rr+1, 32, strShow,4,0);
				if (fireStat[dispFire].mNum>0) {
					decString(strShow, fireStat[dispFire].mNum ,3);  //单回路 3位
					ShowString8 (rr, 68, strShow,3,0);
				}
				sprintf((char *)strShow,"%1d%03d",fireStat[dispFire].bNum,fireStat[dispFire].dNum);
//					sprintf((char *)strShow,"%03d",errStat[dispErr].dNum);   // 单回路 3位 无板号
				ShowString8 (rr+1, 68, strShow,4,0);
				tmpVal=fireStat[dispFire].dType;
				
				ShowString16 (rr, 96, (u8*)dotType(fireStat[dispFire].eventType,tmpVal,0),8,0);
				sprintf((char *)strShow,"%02d-%02d-%02d",fireStat[dispFire].year,fireStat[dispFire].mon,fireStat[dispFire].day);
				ShowString8 (rr, 164, strShow,8,0);
				sprintf((char *)strShow,"%02d:%02d:%02d",fireStat[dispFire].hour,fireStat[dispFire].min,fireStat[dispFire].sec);
				ShowString8 (rr+1, 164, strShow,8,0);
					
				ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),fireStat[dispFire].dNum-1);
				ShowString16 (rr+2, 32, oneNode.addr,16,0);
				ShowString16 (rr+2, 164, (u8*)typeString(tmpVal,fireStat[dispFire].eventType),8,0);
		
			}
			else ShowString16 (rr+1, 64, "无火警信息",10,0);
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
		  Disp_ClearLine(rr,4,0,240);
			ShowString16 (rr+1, 0, (u8*)"故障",4,lineMod[LineNum] & 0x01);
			sprintf((char *)strShow,"%04d",dispErr);
//			decString(strShow, dispErr,4);
			ShowString8 (rr, 32, strShow,4,0);
			sprintf((char *)strShow,"%04d",totErr);
//			decString(strShow, totErr,4);
			ShowString8 (rr+1, 32, strShow,4,0);
		  lineMod[LineNum] &= 0xFD;
		  if (totErr>0) {
				tmpVal=errStat[dispErr].dType;
				if (errStat[dispErr].mNum>0) {
					decString(strShow, errStat[dispErr].mNum ,3);  //单回路 3位
					ShowString8 (rr, 68, strShow,3,0);
				}
		
				if (errStat[dispErr].eventType >= 200 && errStat[dispErr].eventType<208) {
					sprintf((char *)strShow,"%4d",errStat[dispErr].bNum);
					ShowString8 (rr+1, 68, strShow,4,0);
					switch (errStat[dispErr].eventType-200) {
						case 2:
							ShowString16 (rr, 96, (u8*)"主电故障",8,0);
							break;

						case 3:
							ShowString16 (rr, 96, (u8*)"备电故障",8,0);
							break;

						case 4:
							ShowString16 (rr, 96, (u8*)"充电故障",8,0);
							break;

						case 5:
							ShowString16 (rr, 96, (u8*)"辅助输出短路",12,0);
							break;
						}
						sprintf((char *)strShow,"%02d-%02d-%02d",errStat[dispErr].year,errStat[dispErr].mon,errStat[dispErr].day);
						ShowString8 (rr+2, 96, strShow,8,0);
						sprintf((char *)strShow,"%02d:%02d:%02d",errStat[dispErr].hour,errStat[dispErr].min,errStat[dispErr].sec);
						ShowString8 (rr+2, 96, strShow,8,0);
        }
				else if (errStat[dispErr].eventType <100) {
//					sprintf((char *)strShow,"%1d%03d",errStat[dispErr].bNum,errStat[dispErr].dNum);
					sprintf((char *)strShow,"%03d",errStat[dispErr].dNum);   // 单回路 3位 无板号
					ShowString8 (rr+1, 68, strShow,3,0);
					ShowString16 (rr, 96, (u8*)dotType(errStat[dispErr].eventType,tmpVal,0),8,0);
					sprintf((char *)strShow,"%02d-%02d-%02d",errStat[dispErr].year,errStat[dispErr].mon,errStat[dispErr].day);
					ShowString8 (rr, 164, strShow,8,0);
					sprintf((char *)strShow,"%02d:%02d:%02d",errStat[dispErr].hour,errStat[dispErr].min,errStat[dispErr].sec);
					ShowString8 (rr+1, 164, strShow,8,0);
					
					ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),errStat[dispErr].dNum-1);
					ShowString16 (rr+2, 32, oneNode.addr,16,0);
					ShowString16 (rr+2, 164, (u8*)typeString(tmpVal,errStat[dispErr].eventType),8,0);
				}
				else if (errStat[dispErr].eventType == 210 || errStat[dispErr].eventType == 211) {  //2号串口通讯故障
					sprintf((char *)strShow,"%4d",errStat[dispErr].bNum);
					ShowString8 (rr+1, 64, strShow,4,0);
					switch (InterBoard[errStat[dispErr].bNum].Type) {
							case IOBOARD :
									if (errStat[dispErr].eventType==211) ShowString16 (rr, 96, (u8*)"回路板通讯故障",14,0);
									if (errStat[dispErr].eventType==210) ShowString16 (rr, 96, (u8*)"回路板短路故障",14,0);
									break;

							case POWERBOARD :
									ShowString16 (rr, 96, (u8*)"电源板通讯故障",14,0);
									break;

							case FIREBOARD :
									ShowString16 (rr, 96, (u8*)"灭火板通讯故障",14,0);
									break;

							case MULTILINEBOARD :
									ShowString16 (rr, 96, (u8*)"多线盘通讯故障",14,0);
									break;

							default :
									ShowString16 (rr, 96, (u8*)"未知板通讯故障",14,0);
									break;
					}
					sprintf((char *)strShow,"%02d-%02d-%02d",errStat[dispErr].year,errStat[dispErr].mon,errStat[dispErr].day);
					ShowString8 (rr+2, 96, strShow,8,0);
					sprintf((char *)strShow,"%02d:%02d:%02d",errStat[dispErr].hour,errStat[dispErr].min,errStat[dispErr].sec);
					ShowString8 (rr+3, 96, strShow,8,0);
				}
			}
		  lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 54:
		  Disp_ClearLine(8,8,0,240);
				for (i=0,j=0;i<MenuDeep;i++) {
					ShowString16 (8,5+j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, 5+j, (u8*)"->",2,0);
						j+=16;
					}
				}
		  if (isFindRec == 0) { ShowString16 (12, 11, (u8*)"无历史记录",10,0); lineMod[LineNum] &= 0xFD; }
			else {
				rr=10;
				sprintf((char *)strShow,"%04d",disRec);
				ShowString8 (rr, 32, strShow,4,0);
				sprintf((char *)strShow,"%04d",totRec);
				ShowString8 (rr+1, 32, strShow,4,0);
				if (nowRec.mNum>0) {
					decString(strShow, nowRec.mNum ,3);  //单回路 3位
					ShowString8 (rr, 68, strShow,3,0);
				}
				switch (nowRecType) {
					case 3:    //故障档案
						if (nowRec.eventType >= 200 && nowRec.eventType<208) {
							sprintf((char *)strShow,"%4d",nowRec.bNum);
							ShowString8 (rr+1, 68, strShow,4,0);
							switch (nowRec.eventType-200) {
								case 2:
									ShowString16 (rr, 96, (u8*)"主电故障",8,0);
									break;

								case 3:
									ShowString16 (rr, 96, (u8*)"备电故障",8,0);
									break;

								case 4:
									ShowString16 (rr, 96, (u8*)"充电故障",8,0);
									break;

								case 5:
									ShowString16 (rr, 96, (u8*)"辅助输出短路",12,0);
									break;
							}
							sprintf((char *)strShow,"%02d-%02d-%02d",nowRec.year,nowRec.mon,nowRec.day);
							ShowString8 (rr+2, 96, strShow,8,0);
							sprintf((char *)strShow,"%02d:%02d:%02d",nowRec.hour,nowRec.min,nowRec.sec);
							ShowString8 (rr+2, 96, strShow,8,0);
						}
						else if (nowRec.eventType <100) {
							sprintf((char *)strShow,"%03d",nowRec.dNum);   // 单回路 3位 无板号
							ShowString8 (rr+1, 68, strShow,3,0);
							ShowString16 (rr, 96, (u8*)dotType(nowRec.eventType,nowRec.dType,0),8,0);
							sprintf((char *)strShow,"%02d-%02d-%02d",nowRec.year,nowRec.mon,nowRec.day);
							ShowString8 (rr, 164, strShow,8,0);
							sprintf((char *)strShow,"%02d:%02d:%02d",nowRec.hour,nowRec.min,nowRec.sec);
							ShowString8 (rr+1, 164, strShow,8,0);
					
							ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),nowRec.dNum-1);
							ShowString16 (rr+2, 32, oneNode.addr,16,0);
							ShowString16 (rr+2, 164, (u8*)typeString(nowRec.dType,nowRec.eventType),8,0);
						}
						else if (nowRec.eventType == 210 || nowRec.eventType == 211) {  //2号串口通讯故障
							sprintf((char *)strShow,"%4d",nowRec.bNum);
							ShowString8 (rr+1, 64, strShow,4,0);
							switch (InterBoard[nowRec.bNum].Type) {
									case IOBOARD :
										if (nowRec.eventType==211) ShowString16 (rr, 96, (u8*)"回路板通讯故障",14,0);
										if (nowRec.eventType==210) ShowString16 (rr, 96, (u8*)"回路板短路故障",14,0);
										break;

									case POWERBOARD :
										ShowString16 (rr, 96, (u8*)"电源板通讯故障",14,0);
										break;

									case FIREBOARD :
										ShowString16 (rr, 96, (u8*)"灭火板通讯故障",14,0);
										break;

									case MULTILINEBOARD :
										ShowString16 (rr, 96, (u8*)"多线盘通讯故障",14,0);
										break;

									default :
										ShowString16 (rr, 96, (u8*)"未知板通讯故障",14,0);
										break;
							}
							sprintf((char *)strShow,"%02d-%02d-%02d",nowRec.year,nowRec.mon,nowRec.day);
							ShowString8 (rr+2, 96, strShow,8,0);
							sprintf((char *)strShow,"%02d:%02d:%02d",nowRec.hour,nowRec.min,nowRec.sec);
							ShowString8 (rr+3, 96, strShow,8,0);
					}
					break;
				}
			}
			break;

		case 5:
		  Disp_ClearLine(rr,4,0,240);
			ShowString16 (rr+1, 0, (u8*)"联动",4,lineMod[LineNum] & 0x01);
		  lineMod[LineNum] &= 0xFD;
		  if (totAct>0) {
				sprintf((char *)strShow,"%04d",dispAct);
				ShowString8 (rr, 32, strShow,4,0);
				sprintf((char *)strShow,"%04d",totAct);
				ShowString8 (rr+1, 32, strShow,4,0);
				if (actStat[dispAct].mNum>0) {
					decString(strShow, actStat[dispAct].mNum ,3);  //单回路 3位
					ShowString8 (rr, 68, strShow,3,0);
				}
				sprintf((char *)strShow,"%1d%03d",actStat[dispAct].bNum,actStat[dispAct].dNum);
//					sprintf((char *)strShow,"%03d",errStat[dispErr].dNum);   // 单回路 3位 无板号
				ShowString8 (rr+1, 68, strShow,4,0);
				tmpVal=actStat[dispAct].dType;
				
				ShowString16 (rr, 96, (u8*)dotType(actStat[dispAct].eventType,tmpVal,0),8,0);
				sprintf((char *)strShow,"%02d-%02d-%02d",actStat[dispAct].year,actStat[dispAct].mon,actStat[dispAct].day);
				ShowString8 (rr, 164, strShow,8,0);
				sprintf((char *)strShow,"%02d:%02d:%02d",actStat[dispAct].hour,actStat[dispAct].min,actStat[dispAct].sec);
				ShowString8 (rr+1, 164, strShow,8,0);
					
				ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),actStat[dispAct].dNum-1);
				ShowString16 (rr+2, 32, oneNode.addr,16,0);
				ShowString16 (rr+2, 164, (u8*)typeString(tmpVal,actStat[dispAct].eventType),8,0);
		
			}
			else ShowString16 (rr+1, 64, "无联动信息",10,0);
		  lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 6:
		  Disp_ClearLine(rr,4,0,240);
			tmpVal=ioInit[fireStat[1].bNum-1][fireStat[1].dNum-1];
			ShowString16 (rr, 0, (u8*)"首警",4,lineMod[LineNum] & 0x01);
			if (myConf1.mNum>0) {
				decString(strShow, myConf1.mNum,4);
				ShowString8 (rr, 11, strShow,4,0);
			}
			sprintf((char *)strShow,"%1d%03d",fireStat[1].bNum,fireStat[1].dNum);
			ShowString8 (rr+8, 11, strShow,4,0);
			lineMod[LineNum] &= 0xFD;															//无轮显
		  ShowString16 (rr, 22, (u8*)dotType(fireStat[1].eventType,tmpVal,1),8,0);
			sprintf((char *)strShow,"%02d-%02d-%02d",fireStat[1].year,fireStat[1].mon,fireStat[1].day);
			ShowString8 (rr, 38, strShow,8,0);
			sprintf((char *)strShow,"%02d:%02d:%02d",fireStat[1].hour,fireStat[1].min,fireStat[1].sec);
			ShowString8 (rr+8, 38, strShow,8,0);
		  ShowString16 (rr, 59, (u8*)"一二三四",8,0);
		  
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 7: //单行菜单
		  Disp_ClearLine(8,8,0,240);
		  if (OneLineNowMenu==1) rr=8;
		  else {
				rr=10;
				for (i=0,j=0;i<MenuDeep;i++) {
					ShowString16 (8,5+j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, 5+j, (u8*)"->",2,0);
						j+=16;
					}
				}
			}
		  for (i=0; i<OneLineMenuMax;i++) {
				sprintf((char *)strShow,"%1d.",i+1);
				ShowString8 (rr+1+(i/2)*2, 5+(i%2)*120, strShow,2,0);
				ShowString16 (rr+(i/2)*2, 29+(i%2)*120, OneLineMenuItem[i],OneLineMenuSize[i],(OneLineMenuSelect==i?1:0));
			}
			break;

		case 8: //密码
		  Disp_ClearLine(8,8,0,240);
				for (i=0,j=0;i<MenuDeep;i++) {
					ShowString16 (8,5+j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, 5+j, (u8*)"->",2,0);
						j+=16;
					}
				}
			ShowString16 (11, 2, (u8*)"请输入密码:",11,0);
			for (tmpVal=0;tmpVal<inputPosi;tmpVal++) strShow[tmpVal]='*';
		  strShow[tmpVal]='_';
		  ShowString8(12, 93, strShow ,tmpVal+1,0);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 9: //密码错误
			ShowString16 (11, 2, (u8*)"密码错误，请按ESC键返回菜单！",29,0);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

//		case 10: //设置子菜单
//			break;

		case 11: //设置时间
		  Disp_ClearLine(8,8,0,240);
				for (i=0,j=0;i<MenuDeep;i++) {
					ShowString16 (8,5+j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, 5+j, (u8*)"->",2,0);
						j+=16;
					}
				}
			ShowString8 (12, 0, (u8*)"20",2,0);
		  if (inputPosi/2==0) ShowStringPosi8(12, 16, inputStr, 2 ,0,inputPosi);
		  else ShowString8(12, 16, inputStr, 2 ,0);
		  ShowString16 (11, 32,(u8*)"年",2,0);
		  if (inputPosi/2==1) ShowStringPosi8(12, 48, inputStr+2, 2 ,0,inputPosi-2);
		  else ShowString8(12, 48, inputStr+2, 2 ,0); 
		  ShowString16 (11, 64,(u8*)"月",2,0);
		  if (inputPosi/2==2) ShowStringPosi8(12, 80, inputStr+4, 2 ,0,inputPosi-4);
		  else ShowString8(12, 80, inputStr+4, 2 ,0); 
		  ShowString16 (11, 96,(u8*)"日",2,0);
		  if (inputPosi/2==3) ShowStringPosi8(12, 112, inputStr+6, 2 ,0,inputPosi-6);
		  else ShowString8(12, 112, inputStr+6, 2 ,0); 
		  ShowString16 (11,128 ,(u8*)"时",2,0);
		  if (inputPosi/2==4) ShowStringPosi8(12, 144, inputStr+8, 2 ,0,inputPosi-8);
		  else ShowString8(12, 144, inputStr+8, 2 ,0); 
		  ShowString16 (11, 160,(u8*)"分",2,0);
		  if (inputPosi/2==5) ShowStringPosi8(12, 176, inputStr+10, 2 ,0,inputPosi-10);
		  else ShowString8(12, 176, inputStr+10, 2 ,0); 
		  ShowString16 (11, 192,(u8*)"秒",2,0);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 12: //新密码设置
		  Disp_ClearLine(8,8,0,240);
				for (i=0,j=5;i<MenuDeep;i++) {
					ShowString16 (8,j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, j, (u8*)"->",2,0);
						j+=16;
					}
				}
			if	((passwordIn & 0x0F)==1) {
				ShowString16 (10, 5, TwoChoiceMenuItem[0],TwoChoiceMenuItemSize,(TwoChoiceMenuSelect==0?1:0));
				ShowString16 (10, 5+TwoChoiceMenuItemSize*8+3, TwoChoiceMenuItem[1],TwoChoiceMenuItemSize,(TwoChoiceMenuSelect==1?1:0));
			}
			else
				ShowString16 (10, 5, TwoChoiceMenuItem[0],TwoChoiceMenuItemSize,1);
				
			ShowString16 (13, 0, (u8*)"请输入新密码:",13,0);
			for (tmpVal=0;tmpVal<inputPosi;tmpVal++) strShow[tmpVal]='*';
		  strShow[tmpVal]='_';
		  ShowString8(14, 110, strShow ,tmpVal+1,0);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 13: //再次输入新密码
			Disp_ClearLine(13,2,0,240);
			ShowString16 (13, 0, (u8*)"请再次输入新密码:",17,0);
			for (tmpVal=0;tmpVal<inputPosi;tmpVal++) strShow[tmpVal]='*';
		  strShow[tmpVal]='_';
		  ShowString8(14, 140, strShow ,tmpVal+1,0);
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
		  Disp_ClearLine(8,8,0,240);
				for (i=0,j=5;i<MenuDeep;i++) {
					ShowString16 (8,j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, j, (u8*)"->",2,0);
						j+=16;
					}
				}
			ShowString16 (12, 5, TwoChoiceMenuItem[0],TwoChoiceMenuItemSize,(TwoChoiceMenuSelect==0?1:0));
			ShowString16 (12, 125, TwoChoiceMenuItem[1],TwoChoiceMenuItemSize,(TwoChoiceMenuSelect==1?1:0));
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 18:           //自检
		  Disp_ClearLine(8,8,0,240);
				for (i=0,j=5;i<MenuDeep;i++) {
					ShowString16 (8,j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, j, (u8*)"->",2,0);
						j+=16;
					}
				}
			ShowString16 (12, MID(11), (u8*)"正在自检...",11,0);
		  lineMod[LineNum] &= 0xFB; //不可获焦点
			break;

		case 19:           //配置点类型
		  if (clearLastTime>0) Disp_ClearLine(8,6,0,240);
			else Disp_ClearLine(8,8,0,240);
				for (i=0,j=5;i<MenuDeep;i++) {
					ShowString16 (8,j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, j, (u8*)"->",2,0);
						j+=16;
					}
				}
			ShowString16 (10, 0, (u8*)"从 ",3,0);
			if (inputPosi<3) ShowStringPosi8(11, 20, inputStr, 3 ,0,inputPosi);
			else ShowString8(11, 20, inputStr, 3 ,0);
			ShowString16 (10, 48, (u8*)"到 ",3,0);
			if (inputPosi>=3 && inputPosi<6) ShowStringPosi8(11, 68, inputStr+3, 3 ,0,inputPosi-3);
			else ShowString8(11, 68, inputStr+3, 3 ,0);
			
			ShowString16(10, 100, (u8*)"类型:", 5 ,inputPosi==43? 1:0);
			ShowString16(10, 140, (u8*)dotType(0,inputu8,0), 8 ,0);
				
			if (inputu8==1 || inputu8==3)  {
				ShowString16 (12, 0, (u8*)"灵敏度:",7,inputPosi==42? 1:0);
				ShowString8(13, 56, inputStr+42, 1 ,inputPosi==42? 1:0);
			}	
			ShowString16 (12, 70, (u8*)"位置:",5,(inputPosi>=10 && inputPosi<42)?1:0);
			if (inputPosi>=10 && inputPosi<42) ShowStringPosi16 (12, 110, inputChinese, 16, (inputPosi-10)/4);
			else ShowString16 (12, 110, inputChinese,16, 0);
				
			if (clearLastTime==0) {
				ShowString16 (14, 0, (u8*)"分区:",5,(inputPosi>=6 && inputPosi<10)? 1:0);
				if (inputPosi>=6 && inputPosi<10) ShowStringPosi8(15, 40, inputStr+6, 4 ,0,inputPosi-6);
				else ShowString8(15, 40, inputStr+6, 4 ,0);
				ShowString16 (14, 80, (u8*)"区位码: ",7,0);
				i = (inputPosi-10)/4;
				j = (inputPosi-10)%4;
				if (inputPosi>=10 && inputPosi<42) ShowStringPosi8(15, 136, inputStr+10+i*4, 4 ,0,j);
				else ShowString8(15, 136, inputStr+30, 4 ,0);
			}
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
		  Disp_ClearLine(8,8,0,240);
				for (i=0,j=5;i<MenuDeep;i++) {
					ShowString16 (8,j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, j, (u8*)"->",2,0);
						j+=16;
					}
				}
			ShowString16 (11, 5, (u8*)"本机编号:",9,0);
		  ShowStringPosi8(12, 77, inputStr, 2 ,0,inputPosi);
		  i=str2Dec(inputStr,2);
			if (i==0) ShowString16 (11, 100, (u8*)"单机",4,0);
		  else if (i==1) {
				ShowString16 (11, 100, (u8*)"主机",4,0);
				ShowString16 (13, 5, (u8*)"从机数量:",9,0);
				if (inputPosi>1) ShowStringPosi8(14, 77, inputStr+2, 2 ,0,inputPosi-2);
				else ShowString8(14, 77, inputStr+2, 2 ,0);
			}
			else ShowString16 (11, 100, (u8*)"从机",4,0);
			lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 23:           //配置声光分组
		  if (clearLastTime>0) Disp_ClearLine(8,6,0,240);
			else Disp_ClearLine(8,8,0,240);
				for (i=0,j=5;i<MenuDeep;i++) {
					ShowString16 (8,j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, j, (u8*)"->",2,0);
						j+=16;
					}
				}

			for (i=1;i<3;i++) {
				if (inputMLStr[i][28]==0) ShowString16 (10+(i-1)*2, 0, (u8*)"点",2,0);
				else ShowString16 (10+(i-1)*2, 0, (u8*)"区",2,0);
				if (inputMLLine==i) ShowStringPosi8 (11+(i-1)*2, 20, inputMLStr[i],25,0,inputMLPosi[i]);
				else ShowString8(11+(i-1)*2, 20, inputMLStr[i],25,0);
			}
			lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 24:           //配置屏蔽
		  if (clearLastTime>0) Disp_ClearLine(8,6,0,240);
			else Disp_ClearLine(8,8,0,240);
				for (i=0,j=5;i<MenuDeep;i++) {
					ShowString16 (8,j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, j, (u8*)"->",2,0);
						j+=16;
					}
				}
			if (inputStr[28]==0) ShowString16 (10, 0, (u8*)"点位:",5,inputMLLine==1?1:0);
			else if (inputStr[28]==1) ShowString16 (10, 0, (u8*)"区号:",5,inputMLLine==1?1:0);
			else ShowString16 (10, 0, (u8*)"类型:",5,inputMLLine==1?1:0);
			if (inputStr[28]<2 ) {
				if (inputMLLine==1)	ShowStringPosi8 (11, 40, inputStr,25,0,inputPosi);
				else ShowString8 (11, 40, inputStr,25,0);
			}
		  else {
				if (inputMLLine==1)	ShowString16(10, 40, (u8*)dotType(0,inputu8,0), 8 ,1);
				else ShowString16(10, 40, (u8*)dotType(0,inputu8,0), 8 ,0); 
			}
		  
			ShowString16 (12, 0, (u8*)"设置:",5,inputMLLine==2?1:0);
		  ShowString16(12, 44, (u8*)"屏蔽", 4 ,1 - inputStr[29]);
		  ShowString16(12, 84, (u8*)"解除", 4 ,inputStr[29]);
			lineMod[LineNum] |= 0x04; //可获焦点
			break;

		case 100:           //查询配置
			Disp_ClearLine(0,16,0,240);
				for (i=0,j=5;i<MenuDeep;i++) {
					ShowString16 (0,j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (1, j, (u8*)"->",2,0);
						j+=16;
					}
				}
				ShowString16 (2, 32, (u8*)"类型",4,0); 
				ShowString16 (2, 88, (u8*)"屏蔽",4,0);
				ShowString16 (2, 152, (u8*)"类型",4,0); 
				ShowString16 (2, 208, (u8*)"屏蔽",4,0);
				for (i=1;i<10;i++) {
					ShowString16 (4+((i-1)/2)*2, ((i-1)%2)*120, (u8*)dotType(0,i,0),8,0);
					sprintf((char *)strShow,"%03d",typeTot[i]);
					ShowString8 (5+((i-1)/2)*2, ((i-1)%2)*120+64, strShow,3,0);
					sprintf((char *)strShow,"%03d",typePingbi[i]);
					ShowString8 (5+((i-1)/2)*2, ((i-1)%2)*120+92, strShow,3,0);
				}
			break;

		case 101:           //查询点位
			Disp_ClearLine(8,8,0,240);
				for (i=0,j=5;i<MenuDeep;i++) {
					ShowString16 (8,j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, j, (u8*)"->",2,0);
						j+=16;
					}
				}
				ShowString16 (10, 0, (u8*)"点位:",5,0); 
				ShowStringPosi8 (11, 40, inputStr,3,0,inputPosi); 
				i=str2Dec(inputStr,3);
				if (i>0 && i<241) {
					ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),i-1);
					if (oneNode.type>0 && oneNode.type <=8) {
						ShowString16 (10, 70, (u8*)"类型:",5,0);
						ShowString16 (10, 110, (u8*)dotType(0,oneNode.type,0),8,0);
						ShowString16 (10, 176, (u8*)typeString(oneNode.type,getNodeStat(oldStatIOB[0][i])),8,0);
						if (oneNode.type==1 ||oneNode.type==3) {
							ShowString16 (12, 0, (u8*)"灵敏度:",7,0);
							sprintf((char *)strShow,"%1d",(oneNode.stat & 0x03)+1);
							ShowString8 (13, 56, strShow,1,0);
						}
						ShowString16 (12, 90, (u8*)"屏蔽:",5,0);
						if ((oneNode.stat & 0x80)==0) ShowString16 (12, 130, (u8*)"否",2,0);
						else ShowString16 (12, 130, (u8*)"是",2,0);
						ShowString16 (12, 165, (u8*)"区域:",5,0);
						sprintf((char *)strShow,"%04d",oneNode.area);
						ShowString16 (12, 205, strShow,4,0);
						ShowString16 (14, 0, (u8*)"地址:",5,0);
						ShowString16 (14, 40, oneNode.addr,16,0);
					}
					else {
						ShowString16 (13, MID(12), (u8*)"点位未配置！",12,0);
					}
				}
				else ShowString16 (13, MID(10), (u8*)"非法点位！",10,0);
			break;

		case 103:           //查询通讯
			Disp_ClearLine(8,8,0,240);
				for (i=0,j=5;i<MenuDeep;i++) {
					ShowString16 (8,j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, j, (u8*)"->",2,0);
						j+=16;
					}
				}
			if (myConf1.mNum ==0) 
				ShowString16 (11, 0, (u8*)"单机",4,0);
		  else if (myConf1.mNum ==1)
				ShowString16 (11, 0, (u8*)"主机",4,0);
			else { 
				sprintf((char *)strShow,"从机：%3d",myConf1.mNum);
				ShowString8 (11, 0, strShow,5,0);
			}
			break;

		case 104:           //控制屏幕
			Disp_ClearLine(8,8,0,240);
				for (i=0,j=5;i<MenuDeep;i++) {
					ShowString16 (8,j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, j, (u8*)"->",2,0);
						j+=16;
					}
				}
			ShowString16 (10, 0, (u8*)"声光点位：",10,0);
			if ((inputu8/16)==0) {
				ShowStringPosi8 (11, 80, inputStr,inputMax,0,inputPosi);
				ShowString16 (10, 200, (u8*)"开",2,((inputu8 & 0x01)==0)?0:1);
				ShowString16 (10, 224, (u8*)"关",2,((inputu8 & 0x01)==0)?1:0);
			}
			else { 
				ShowString8 (11, 80, inputStr,inputMax,0);
				ShowString16 (10, 200, (u8*)"开",2,0);
				ShowString16 (10, 224, (u8*)"关",2,0);
			}
			
			ShowString16 (12, 0, (u8*)"1号继电器：",11,0);
			if ((inputu8/16)==1) {
				ShowString16 (12, 120, (u8*)"开",2,((inputu8 & 0x02)==0)?0:1);
				ShowString16 (12, 160, (u8*)"关",2,((inputu8 & 0x02)==0)?1:0);
			}
			else {
				ShowString16 (12, 120, (u8*)"开",2,0);
				ShowString16 (12, 160, (u8*)"关",2,0);
			}
			ShowString16 (14, 0, (u8*)"2号继电器：",11,0);
			if ((inputu8/16)==2) {
				ShowString16 (14, 120, (u8*)"开",2,((inputu8 & 0x04)==0)?0:1);
				ShowString16 (14, 160, (u8*)"关",2,((inputu8 & 0x04)==0)?1:0);
			}
			else {
				ShowString16 (14, 120, (u8*)"开",2,0);
				ShowString16 (14, 160, (u8*)"关",2,0);
			}
			break;

		case 26:           //电压显示
			Disp_ClearLine(8,8,0,240);
				for (i=0,j=5;i<MenuDeep;i++) {
					ShowString16 (8,j, MenuTitle[i],MenuTLen[i],0);
					j += MenuTLen[i]*8;
					if (i!=MenuDeep-1) { 
						ShowString8 (9, j, (u8*)"->",2,0);
						j+=16;
					}
				}
			if (newStatPB & 0x01)
				ShowString16 (11, 0, (u8*)"主电正常:",9,0);
			else
				ShowString16 (11, 0, (u8*)"主电关闭:",9,0);
			sprintf((char *)strShow,"%4.1fV",mainVlot*0.2);
			ShowString8 (12, 72, strShow,5,0);
			if (newStatPB & 0x02)
				ShowString16 (13, 0, (u8*)"备电正常:",9,0);
			else
				ShowString16 (13, 0, (u8*)"备电关闭:",9,0);
			sprintf((char *)strShow,"%4.1fV",backVlot*0.2);
			ShowString8 (14, 72, strShow,5,0);
			
			if (newStatPB & 0x04) ShowString16 (11, 0, (u8*)"主电故障        ",16,0);
			if (newStatPB & 0x08) ShowString16 (13, 0, (u8*)"备电故障        ",16,0);
			if (newStatPB & 0x10) ShowString16 (11, 120, (u8*)"充电故障",8,0);
			if (newStatPB & 0x20) ShowString16 (13, 120, (u8*)"辅助输出短路",12,0);
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
					if (nowRec.eventType<100) {
						sprintf((char *)strShow,"%03d",nowRec.dNum);
						ShowString8 (rr+24, 29, strShow,3,0);
						
						ShowString16 (rr+16, 37, (u8*)"点",2,0);
					  ShowString16 (rr+16, 43, (u8*)dotType(nowRec.eventType,tmpVal,0),8,0);
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
					
					if (nowRec.eventType<100) {
								ShowString16 (rr, 6, (u8*)typeFullString(tmpVal,nowRec.eventType),8,0);
								ReadFlashFixRecoard(NODEBLOCK,(u8 *)&oneNode,sizeof(NODESTRUCT),nowRec.dNum-1);
								ShowString16 (rr, 30, oneNode.addr,12,0);
					}
					else if (nowRec.eventType>=200 && nowRec.eventType<208) {
								switch (nowRec.eventType-200) {
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
					}
					else if (nowRec.eventType==210 || nowRec.eventType==211) {
								switch (InterBoard[nowRec.bNum].Type) {
									case IOBOARD :
										if (nowRec.eventType==211) ShowString16 (rr+32, 6, (u8*)"回路板通讯故障",14,0);
										if (nowRec.eventType==210) ShowString16 (rr+32, 6, (u8*)"回路板短路故障",14,0);
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


	}
}

void Disp_Screen(void)
{
	
	u8 strShow[20];
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
#ifndef ENGLISH				
				if (myConf1.mRunningStat == 0) ShowString8 (1, MID(13), (u8*)"FS1000 V1.0 T",13,0);
			  else ShowString8 (1, MID(11), (u8*)"FS1000 V1.0",11,0);
#else
				if (myConf1.mRunningStat == 0) ShowString816 (0, MID(13), (u8*)"WESTERN 8600 T",14,0);
			  else ShowString816 (0, MID(11), (u8*)"WSTERN 8600",11,0);
			  ShowString816 (6, 0, (u8*)"abcdefghijklmnopqrstuvwxyz",26,0);
#endif
				
				if (myConf1.mNum >1) {
					sprintf((char *)strShow,"S%02d",myConf1.mNum);
					ShowString8 (1, 210, (u8*)strShow,3,0);
				}
				if (myConf1.mNum ==1) {
					sprintf((char *)strShow,"M%02d",myConf1.mNum);
					ShowString8 (1, 210, (u8*)strShow,3,0);
				}
			
				Disp_ShowTime(3,5,1);
				Disp_Line(1,3);
			  if (dispLine==0) {
					if (totAct>0) 
						Disp_Line(5,4);
					else if (totErr>0) 
						Disp_Line(4,4);
					else 
						Disp_Line(2,4);
					flashLine=0x03;
					flashstat=0;
				}
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
	
		if (oldLine != newLine) {
			oldLine=newLine;
		}

		if (newTime == 1) { newTime = 0; Disp_ShowTime(3,5,1);}
//		Disp_ShowLine(31);
		if (nowLine[4]!=100) Disp_ShowLine(63);
		if (newScr==1 && nowLine[4]<7) Disp_ShowLine(95);
	}
}

void Back_to_Screen(void) {
	if (totFire>0) newScr=2;
	else newScr=1;
	oldScr=0;
	inputMode=0;
	newTab=4;
	dispLine=4;
	OneLineNowMenu=0;
	if (totAct>0) nowLine[4] = 5;
	else if (totErr>0) nowLine[4] = 4;
	else nowLine[4] = 2;
}
