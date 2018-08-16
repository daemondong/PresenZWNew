#include "ZLG7289.h"


extern u8 Led05, LedRev05;
extern u8 Led06, LedRev06;

extern void ZLG7289_DIO(u8 tt);

unsigned char ZLG7289_Delay_t;

void Disp_LED_Success(u8 stat);

void ZLG7289_ShortDelay()
{
	unsigned char t = ZLG7289_Delay_t;
	while ( --t != 0 );
}

void ZLG7289_LongDelay(u16 ts)
{
	u16 t = ZLG7289_Delay_t * 6;
	u16 tt = ts;
	while ( --tt != 0)
	  while ( --t != 0 );
}

void ZLG7289_SPI_Write(char dat)
{
	unsigned char t = 8;
	do
	{
		GPIO_WriteBit(DIO7289_RANK, DIO7289_PIN,(BitAction)((dat >> (t-1)) & 0x01));
		CLK7289_ON;
		ZLG7289_ShortDelay();
		CLK7289_OFF;
		ZLG7289_ShortDelay();
 } while ( --t != 0 );
}

char ZLG7289_SPI_Read()
{
	char dat;
	unsigned char t = 8;
	DIO7289_ON;
	ZLG7289_DIO(1);
	do
	{
		CLK7289_ON;
		ZLG7289_ShortDelay();
		dat <<= 1;
		if ( GPIO_ReadInputDataBit(DIO7289_RANK,DIO7289_PIN) ) dat++;
		CLK7289_OFF;
		ZLG7289_ShortDelay();
	} while ( --t != 0 );
	return dat;
}

void ZLG7289_cmd(char cmd)
{
//	char sav = IE;
//	IE &= 0xFA; //disable int
	CS7289_OFF;
	ZLG7289_LongDelay(2);
	ZLG7289_SPI_Write(cmd);
	CS7289_ON;
	ZLG7289_LongDelay(2);
// IE = sav; //?? IE
}

void ZLG7289_cmd_dat(char cmd, char dat)
{
//char sav = IE;
// IE &= 0xFA; //disable int
	CS7289_OFF;
	ZLG7289_LongDelay(2);
	ZLG7289_SPI_Write(cmd);
	ZLG7289_LongDelay(1);
	ZLG7289_SPI_Write(dat);
	CS7289_ON;
	ZLG7289_LongDelay(2);
// IE = sav; //?? IE
}

char ZLG7289_Key()
{
	char key;
	CS7289_OFF;
//	ZLG7289_LongDelay(2);
	ZLG7289_SPI_Write(0x15);
//	ZLG7289_LongDelay(1);
	key = ZLG7289_SPI_Read();
	CS7289_ON;
//	ZLG7289_LongDelay(10);
//	ZLG7289_LongDelay(2);
	ZLG7289_DIO(0);
	return key;
}

void ZLG7289_Download(u8 mod, char x, u8 dp, char dat)
{  
	u8 ModDat[3] = {0x80,0xC8,0x90};
	char d1,d2;
  if ( mod > 2 ) mod = 2;
	d1 = ModDat[mod];
	x &= 0x07;
	d1 |= x;
	d2 = dat & 0x7F;
  if ( dp ) d2 |= 0x80;
  ZLG7289_cmd_dat(d1,d2); 
}

void ZLG7289_Init(unsigned char t)
{
	u8 i;
	
//	CS7289_ON;
//	CLK7289_OFF;
//	DIO7289_ON;
//	KEY7289_ON;
	ZLG7289_Delay_t = t;
  
//	while (1) {
//	ZLG7289_cmd(0xA4);
//	ZLG7289_LongDelay(100);
//	ZLG7289_cmd(0xBF);
//	}
//	while(1) {
//		ZLG7289_LongDelay(100);
//		i = ZLG7289_Key();
//	}
//	for ( i=0; i<7; i++ )  {   
//	  ZLG7289_LongDelay(100); 
//	  ZLG7289_Download(2,5,0,0x7f >> (6-i));  
//	}
//	  ZLG7289_LongDelay(5); 
//	for ( i=0; i<7; i++ )  {   
//	  ZLG7289_LongDelay(100); 
//	  ZLG7289_Download(2,6,0,0x7f >> (6-i));  
//	}
//	  ZLG7289_LongDelay(5); 
//	ZLG7289_LongDelay(100);
//	ZLG7289_cmd_dat(0x88,0xF0);
	Led05 = 0;
	Led06 = 0;
	ZLG7289_Download(2,5,0,Led05);
	ZLG7289_Download(2,6,0,Led06);
//	ZLG7289_LongDelay(5);
//	Disp_LED_Success(1);
	//while (1) ZLG7289_cmd_dat(0x88,0);
}

