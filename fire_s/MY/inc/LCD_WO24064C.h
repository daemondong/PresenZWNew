#ifndef _LCD_WO24064C_H_
#define _LCD_WO24064C_H_

#include "stm32f10x.h"

#define Uint unsigned int
#define Uchar unsigned char
	
#define RES_ON GPIO_SetBits(GPIOE,GPIO_Pin_5)
#define RES_OFF GPIO_ResetBits(GPIOE,GPIO_Pin_5)

#define LIGHT_REV GPIO_WriteBit(GPIOE, GPIO_Pin_3,(BitAction)(1-(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_3))))


#define AOUTOB GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define STEP GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define UP GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define DOWN GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)

#define Bank1_LCD_D    ((uint32_t)0x68000002)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x68000000)     //disp Reg ADDR

void write_com(Uchar com);
void write_dat(Uchar dat);
void LCD_initial(void);
void Disp(Uchar dat1,Uchar dat2);
void Disp1(Uchar dat3,Uchar dat4);
void Disp2(Uchar dat5,Uchar dat6);
void Disp3(Uchar dat1,Uchar dat2);
void DispBmp(Uchar *Pbmp);
void delay(unsigned int COUNT);
void Pause(void);
void LCD_Test(void);
void ShowChar16 (Uchar ll, Uchar rr, Uchar *Char);
void ShowString8 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod);
void ShowString16 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod);
void ShowChar8 (Uchar ll, Uchar rr, Uchar Char);
//void LCDSetAddr(u8 sl,u8 nl,u8 sr,u8 nr);
void LCDSetPosi(u8 sl,u8 sr);



#endif /* _LCD_WO24064C_H_ */
