#include "LCD_WO24064C.h"
#include "string.h"

unsigned char CHARHZ[80]={
	    0x04,0x80,0x0e,0xa0,0x78,0x90,0x08,0x90,0x08,0x84,0xff,0xfe,0x08,0x80,0x08,0x90,    //我
	    0x0a,0x90,0x0c,0x60,0x18,0x40,0x68,0xa0,0x09,0x20,0x0a,0x14,0x28,0x14,0x10,0x0c,
//	    0x04,0x00,0x04,0x02,0x04,0x02,0x04,0x04,0x04,0x08,0x04,0x30,0x05,0xc0,0xfe,0x00,    //大
//	    0x05,0x80,0x04,0x60,0x04,0x10,0x04,0x08,0x04,0x04,0x0c,0x06,0x04,0x04,0x00,0x00};
	    0x00,0x00,0x00,0x04,0xff,0xfe,0x04,0x40,0x04,0x40,0x04,0x44,0x7f,0xfe,0x44,0x44,    //西
	    0x44,0x44,0x44,0x44,0x44,0x44,0x48,0x34,0x50,0x04,0x40,0x04,0x7f,0xfc,0x40,0x04};

extern unsigned char GUI_F8x8_acFont[][8];
extern unsigned char GUI_F8x16_acFont[][16];
u8 Flashable;
u8 FSMC_NOR_Read(u32 ReadAddr);

			

Uchar  Get_CDoulebit(Uchar hl4, Uchar ll, u32 offset) {
	
	 Uchar rVal = 0;
	
	 rVal=FSMC_NOR_Read(offset+ll*16+15-hl4);
	 
/*	 if (hl4<8) {
		 if ((FSMC_NOR_Read(offset+ll*2+1) & (1<< (hl4))) >0) rVal |= 0x0F;
		 if ((FSMC_NOR_Read(offset+ll*2+3) & (1<< (hl4))) >0) rVal |= 0xF0;
	 }
	 else {
		 if ((FSMC_NOR_Read(offset+ll*2) & (1<< (hl4-8))) >0) rVal |= 0x0F;
		 if ((FSMC_NOR_Read(offset+ll*2+2) & (1<< (hl4-8))) >0) rVal |= 0xF0;
	 }
*/
	 return rVal;
	
}

Uchar  Get_NDoulebit(Uchar hl4, u16 c1, u16 c2) {
	
	 Uchar rVal = 0;
	 
	 if ((c1 & (1<< (hl4))) >0) rVal |= 0x0F;
	 if ((c2 & (1<< (hl4))) >0) rVal |= 0xF0;
	 return rVal;
	
}

/*Uchar  Get_NewDoulebit(Uchar hl4, Uchar ll, u8 *b) {
	
	 Uchar i,rVal = 0;
	
	 for (i=0;i<8;i++)
		if (((*(b+i)) & (1<< hl4)) >0) rVal |= (1 << i);
//	   rVal |= ((*(b+i)) & (1<< hl4));
	 
//	 if ((b[ll] & (1<< (hl4))) >0) rVal |= 0x0F;
//	 if ((b[ll+1] & (1<< (hl4))) >0) rVal |= 0xF0;
	 return rVal;
	
}
*/
Uchar  Get_Doulebit(Uchar hl4, u16 b) {
	 u16 i;
	 Uchar rVal = 0;
	
   
	i = (b >> (hl4*2)) & 0x03;
   switch (i) {
		 case 0:
       rVal = 0;
       break;		 
		 case 1:
       rVal = 0xF;
       break;		 
		 case 2:
       rVal = 0xF0;
       break;		 
		 case 3:
       rVal = 0xFF;
       break;		 
	 }
//	if (hl4 == 1 ) {  // get high 4 bit double
//		 for (i=3; i<4 ;i++) 
//			if ( b & (0x01 << (i+4)) ) rVal = rVal | (0x1 << (i*2))  | (0x01 << (i*2 +1));
//	 }
//	 if (hl4 == 0 ){ // get low 4 bit double
//		 for (i=0; i< ;i++) 
//			if ( b & (0x01 << i) ) rVal = rVal | (0x01 << (i*3))  | (0x01 << (i*3 +1)) | (0x01 << (i*3 +2));
//	 }
	 return (rVal);
}

void LCDSetPosi(u8 sl,u8 sr)
{
	write_com(sr &0x0f);                //列
	write_com(0x10 | (sr>> 4));
	
	write_com(0x60 | (sl & 0x0f));      //行
	write_com(0x70 | (sl >> 4));
}	

/*void LCDSetAddr(u8 sl,u8 nl,u8 sr,u8 nr) 
{
	write_com(0x2a);
	write_dat(0x00);
	write_dat(48+sr);
	write_dat(0x00);
	write_dat(48+sr+nr);

	write_com(0x2b);
	write_dat(0x00);
	write_dat(96+sl);
	write_dat(0x00);
	write_dat(96+sl+nl);

}
*/
void ShowChar16 (Uchar ll, Uchar rr, Uchar *Char)
{
    Uchar cnt,ii;
//	  u16 c1,c2;
	
	Flashable = 0;	
//	for (ii=0;ii<8;ii++) {
	for (ii=0;ii<2;ii++) {
		LCDSetPosi(ll+ii,rr);
//		c1=*(Char + ii*4) * 256 + *(Char + ii*4+1);
//		c2=*(Char + ii*4+2) * 256 + *(Char + ii*4+3);
		
//		for( cnt=0; cnt<16; cnt++) write_dat(Get_NDoulebit(cnt ,c1,c2));
		for( cnt=0; cnt<16; cnt++) write_dat(*(Char + (15-cnt)+ii*16));
		
	}
	Flashable = 1;
}

void ShowString8 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod)
{
    Uchar cnt,i;
	
		Flashable = 0;
		cnt=0;
//	  for( cnt=0; cnt<4; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
		{
			LCDSetPosi(ll+cnt,240-rr-len*8);
//			write_com(0xA8);
			for (i=0;i<len*8;i++) {
				if (mod == 1){
//					write_dat(~Get_NewDoulebit(i % 8, cnt, GUI_F8x8_acFont[Char[len-i/8-1]-32]));
					write_dat(~GUI_F8x8_acFont[Char[len-i/8-1]-32][i%8]);
				}
				else {
					write_dat(GUI_F8x8_acFont[Char[len-i/8-1]-32][i%8]);
//					write_dat(Get_NewDoulebit(i % 8, cnt, GUI_F8x8_acFont[Char[len-i/8-1]-32]));
				}
			}
//			write_com(0xAF);
    }
		Flashable = 1;
	
}

void ShowString816 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod)
{
    Uchar cnt,i;
	
		Flashable = 0;
		cnt=0;
	  for( cnt=0; cnt<2; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
		{
			LCDSetPosi(ll+cnt,240-rr-len*8);
//			write_com(0xA8);
			for (i=0;i<len*8;i++) {
				if (mod == 1){
//					write_dat(~Get_NewDoulebit(i % 8, cnt, GUI_F8x8_acFont[Char[len-i/8-1]-32]));
					write_dat(~GUI_F8x16_acFont[Char[len-i/8-1]-32][i%8+cnt*8]);
				}
				else {
					write_dat(GUI_F8x16_acFont[Char[len-i/8-1]-32][i%8+cnt*8]);
//					write_dat(Get_NewDoulebit(i % 8, cnt, GUI_F8x8_acFont[Char[len-i/8-1]-32]));
				}
			}
//			write_com(0xAF);
    }
		Flashable = 1;
	
}

void ShowStringPosi8 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod, u8 posi)
{
    Uchar cnt,i;
	
		Flashable = 0;
	  cnt=0;
//	  for( cnt=0; cnt<4; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
		{
			LCDSetPosi(ll+cnt,240-rr-len*8);
//			write_com(0xA8);
			for (i=0;i<len*8;i++) {
				if (mod == 1|| posi==len-i/8-1){
					write_dat(~GUI_F8x8_acFont[Char[len-i/8-1]-32][i%8]);
				}
				else {
					write_dat(GUI_F8x8_acFont[Char[len-i/8-1]-32][i%8]);
				}
			}
//			write_com(0xAF);
    }
		Flashable = 1;
	
}


void ShowString16 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod)
{
    Uchar cnt,i,ci;
	  u32 offset;
	
		Flashable = 0;
//	  for( cnt=0; cnt<8; cnt++)		
	  for( cnt=0; cnt<2; cnt++)		
		{
//			LCDSetPosi(ll+cnt,rr);
			LCDSetPosi(ll+cnt,240-rr-len*8);
			ci=0;
			for (i=0;i<len*8;i++) {
				if (Char[len-i/8-1]<128) {
				  if (cnt==0) {
						write_dat(0x00); 
					}
					else {
						if ((mod == 1 && Char[len-i/8-1]!=' ') ||(mod==2 && i>0 && (i/8)!=len-1)) {
							write_dat(~GUI_F8x8_acFont[Char[len-i/8-1]-32][i%8]);
						}
						else {
							write_dat(GUI_F8x8_acFont[Char[len-i/8-1]-32][i%8]);
						}
					}
				}
				else {
					if (ci==0) offset = ((((u32)Char[len-i/8-2])-0xa1)*94+(Char[len-i/8-1]-0xa1))*32;
					if (mod == 1 ||mod == 2){
						write_dat(~Get_CDoulebit(ci, cnt, offset));
					}
					else {
						write_dat(Get_CDoulebit(ci, cnt, offset));
					}
					ci++;
					if (ci>=16) ci=0;
				}
			}
    }
		Flashable = 1;
}

void ShowStringPosi16 (u8 ll, u8 rr, u8 *Char, u8 len, u8 posi)
{
    Uchar cnt,i,ci;
	  u32 offset;
	
		Flashable = 0;
	  for( cnt=0; cnt<2; cnt++)		
		{
//			LCDSetPosi(ll+cnt,rr);
			LCDSetPosi(ll+cnt,240-rr-len*8);
			ci=0;
			for (i=0;i<len*8;i++) {
				if (Char[len-i/8-1]<128) {
				  if (cnt==0) {
						write_dat(0x00); 
					}
					else {
						if (len-i/8-1==posi) {
							write_dat(GUI_F8x8_acFont[Char[len-i/8-1]-32][i%8]);
						}
						else {
							write_dat(GUI_F8x8_acFont[Char[len-i/8-1]-32][i%8]);
						}
					}
				}
				else {
					if (ci==0) offset = ((((u32)Char[len-i/8-2])-0xa1)*94+(Char[len-i/8-1]-0xa1))*32;
					if (len/2-i/16-1==posi){
						write_dat(~Get_CDoulebit(ci, cnt, offset));
					}
					else {
						write_dat(Get_CDoulebit(ci, cnt, offset));
					}
					ci++;
					if (ci>=16) ci=0;
				}
			}
    }
		Flashable = 1;
}

void ShowChar8 (Uchar ll, Uchar rr, Uchar Char)
{
    Uchar cnt,i;
	
		Flashable = 0;
	  for (i=0;i<2;i++) {
			LCDSetPosi(ll+i,rr);
			for( cnt=0; cnt<8; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
				write_dat(GUI_F8x16_acFont[Char-32][cnt+i*8]);
		}
		Flashable = 1;
}

u8 FSMC_NOR_Read(u32 ReadAddr);
void FSMC_NOR_ReadBuffer(u8* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead);

void LCD_Test(void)
{
	u8 i,j;
	u8 RxBuffer[0x80];
	u32 readBlock;

// while(1)
 {
   
//   FSMC_NOR_ReadBuffer(RxBuffer, 0x7F80, 0x80);
//	 LIGHT_REV;
//	 Disp(0xff,0xff);		 //FULL ON
//   Pause();
//   Disp(0x00,0x00);		 //FULL OFF
   Pause();  
//   Disp(0xF0,0x00);		 //FULL OFF
//   Pause();  
//   Disp(0x0F,0x00);		 //FULL OFF
//   Pause();  	 
//   Disp(0x00,0x00);		 //FULL OFF
//   Pause();  
	ShowString16 (3, 0, "C中国A尚泰科技有限公司B",23,2);
	
//	ShowString8 (1, 10, "ABCDEFGHIJKL",12,1);
	ShowString816 (10, 10, "ABCDEFGHIJKL",12,0);
//	 ShowChar8(1,10,'A');
//	 ShowChar8(1,18,'B');
//	 ShowChar8(1,26,'C');
//	 ShowChar8(1,34,'D');
//	 ShowChar8(1,42,'E');
//	write_com(0xA8);
	 
//	 for (i=0;i<4;i++) {
//		 for (j=0;j<8;j++) { ShowChar16(i*8,j*32,CHARHZ); ShowChar16(i*8,j*32+16,CHARHZ+32); }
//	 }
//	write_com(0xAF);
//	 ShowChar16(48,30,CHARHZ);
//	 ShowChar16(56,46,CHARHZ+32);
//  Pause();
//  Disp(0x00,0x00);		 //FULL OFF
//  Pause();
//	ShowString8 (0, (8*(30-12)/2)/3, "Self Test...",12,0);

   
	

//   DispBmp(ABCD);
//   Pause();

//   DispBmp(Cross2);
//   Pause();
   //Disp(0x00,0x00);		 //FULL OFF
   //Pause();
//   DispBmp(WS);	
//   Pause();
  	}
//   delay(400);
   
}


void LCDFlash_Off (void) 
{
//  if (Flashable == 1) ShowChar8 (32, 13, GUI_F8x8_acFont[0]);
}


void LCDFlash_On (void)
{
//   if (Flashable == 1) ShowChar8 (32, 13, GUI_F8x8_acFont['B'-32]);
}


//========================================================
void LCD_initial(void) {
	
	write_com(0xE2);
	delay(2000);
	write_com(0x2f);//set pump control	   internal vlcd(11x charge pump)
	delay(200);
	write_com(0xEA);//set bias=1/11 这个需要根据你们具体的bias来更改
  write_com(0x81);//set vop
//	write_com(0xB6);//set PM,这个参数需要根据你们的VOP来更改   df
	write_com(0x96);//set PM,这个参数需要根据你们的VOP来更改   df
	write_com(0xc8);//set n-line inversion
	write_com(0x1f);//set n-line inversion	  value 34  16  1f

//  write_com(0xC1);

	//writei(0xAf);//set display enable  16-shade
	write_com(0xA9);//set display enable  b/w
  write_com(0xA3);

	write_com(0xd1);//set display pattern
	
	write_com(0xA9);
	
	Disp(0x00,0x00);
	
}



//==============================================================================
void Disp(Uchar dat1,Uchar dat2)
{
  u16 cnt;
  Uchar cnt1;
	
	LCDSetPosi(0,0);
	write_com(0xA8);

    for(cnt1=0;cnt1<80;cnt1++)	   // com?	  
    {										 
        for(cnt=0;cnt<256;cnt++)		//1/3  SEG
        {     
            write_dat(dat1);
//            write_dat(dat2);
        }
		}
	write_com(0xA9);
	   delay(7000);
}


void Disp1(Uchar dat3,Uchar dat4)
 {

  Uchar cnt;
  Uchar cnt1;

   write_com(0x2c);      //Write Display Data
		  
    for(cnt1=0;cnt1<64;cnt1++)
    {
        for(cnt=0;cnt<40;cnt++)
        {
        if(cnt1%2==0)
            {write_dat(dat3);write_dat(dat4);}
            else 
            {write_dat(dat4);write_dat(dat3);}
        }
		}
	   delay(7000);
		}


void Disp2(Uchar dat5,Uchar dat6)
 {

  Uchar cnt;
  Uchar cnt1;

    write_com(0x2c);      //Write Display Data
		  
    for(cnt1=0;cnt1<32;cnt1++)
    {
        for(cnt=0;cnt<80;cnt++)
        {
        if(cnt1%2==0)
            {write_dat(dat5);
			write_dat(dat5);}
            else 
            {write_dat(dat6);
			write_dat(dat6);}
        }
		}
	   delay(7000);
		}

void Disp3(Uchar dat1,Uchar dat2)
 {

  Uchar cnt;
  Uchar cnt1;




    write_com(0x2C);      //Write Display Data
		  
    for(cnt1=0;cnt1<64;cnt1++)	   // com?	  
    {										 
        for(cnt=0;cnt<80;cnt++)		//1/3  SEG
        {     
            write_dat(dat1);
            write_dat(dat2);
        }
		}
	   delay(7000);


		}
 
/*
  void DispBmp (Uchar *Pbmp)

 { Uchar cnt;
  Uchar cnt1;
   
   	write_com(0x2A);		// Column Address Setting
	write_dat(0x00);		// SEG48 -> SEG335
	write_dat(0x30);		
	write_dat(0x00);		
	write_dat(0x7F);
			
	write_com(0x2B);		// Row Address Setting
	write_dat(0x00);		//  //com 32 ( 0x20)---com  159
	write_dat(0x20);		
	write_dat(0x00);		
	write_dat(0x9F);
	
	write_com(0x2c);

	 for( cnt=0; cnt<128; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
    {
        for(cnt1=0;cnt1<80;cnt1++)	//   1/3 seg //     
        {
         
		   write_dat(*Pbmp++);
		    
        }
    }
 delay(6000);
}


 */

  void DispBmp (Uchar *Pbmp)

 {
    Uchar cnt;
    Uchar cnt1;

write_com(0x2c);     //Write Display Data

	 for( cnt=0; cnt<64; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
    {
        for(cnt1=0;cnt1<80;cnt1++)	//   1/3 seg //     
        {
         
		  write_dat(*Pbmp++);
		    
        }
    }
 delay(7000);
}


 
 



 void write_com(Uchar com)
{
/*	CS1_OFF;
	A0_OFF;
	RW_OFF;

  Write_Data(com);
	E_ON;
	delay(1);
	;
	RW_ON;
	CS1_ON; */
	*(__IO uint16_t *) (Bank1_LCD_C)= com;
	delay(1);
}
//========================================================
void write_dat(Uchar dat)
{
/*	CS1_OFF;
	A0_ON;
	RW_OFF;

  Write_Data(dat);
	E_ON;
	delay(1);
	;
	RW_ON;
	CS1_ON;*/
	*(__IO uint16_t *) (Bank1_LCD_D)= dat;
	delay(1);
}

u8 get_dat()
{
	return (u8)(*(__IO uint16_t *) (Bank1_LCD_D));
}

///////////////////////////////////////////////////////////////

void delay(unsigned int COUNT)     // ?????
  {
    unsigned char TT;
    while(COUNT--)
    for(TT=160;TT>0;TT--);          //???? 0.1ms
  }

//////////////////////////////////////////////////////////////////
void Pause()
{
	delay(100000);
}
