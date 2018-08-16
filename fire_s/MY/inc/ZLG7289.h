#ifndef _ZLG7289_H_
#define _ZLG7289_H_

#include "stm32f10x.h"

#define CS7289_ON GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define CS7289_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define KEY7289_ON GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define KEY7289_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define CLK7289_ON GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define CLK7289_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define DIO7289_ON GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define DIO7289_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define DIO7289_RANK GPIOB
#define DIO7289_PIN GPIO_Pin_12

void ZLG7289_cmd(char cmd);
void ZLG7289_cmd_dat(char cmd, char dat);

#define ZLG7289_Reset() ZLG7289_cmd(0xA4)
#define ZLG7289_Test() ZLG7289_cmd(0xBF)
#define ZLG7289_SHL() ZLG7289_cmd(0xA0)
#define ZLG7289_SHR() ZLG7289_cmd(0xA1)
#define ZLG7289_ROL() ZLG7289_cmd(0xA2)
#define ZLG7289_ROR() ZLG7289_cmd(0xA3)
#define ZLG7289_Flash(x) ZLG7289_cmd_dat(0x88,(x))
#define ZLG7289_Hide(x) ZLG7289_cmd_dat(0x98,(x))
#define ZLG7289_SegOn(seg) ZLG7289_cmd_dat(0xE0,(seg))
#define ZLG7289_SegOff(seg) ZLG7289_cmd_dat(0xC0,(seg))

char ZLG7289_Key(void);
void ZLG7289_Init(unsigned char t);

#endif //_ZLG7289_H_
