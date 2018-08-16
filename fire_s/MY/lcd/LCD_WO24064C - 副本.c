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
u8 Flashable;
			
Uchar  Get_NewDoulebit(Uchar hl4, Uchar ll, u8 *b) {
	
	 Uchar rVal = 0;
	 
	 if ((b[ll] & (1<< (7-hl4))) >0) rVal |= 0x0F;
	 if ((b[ll+1] & (1<< (7-hl4))) >0) rVal |= 0xF0;
	 return rVal;
	
}

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

void LCDSetAddr(u8 sl,u8 nl,u8 sr,u8 nr) 
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

void ShowChar16 (Uchar ll, Uchar rr, Uchar *Char)
{
    Uchar cnt,ii = 0;
	
	Flashable = 0;	
	for( cnt=0; cnt<16; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
		{
			LCDSetAddr(ll+cnt,1,rr,4);
			write_com(0x2c);
					write_dat(Get_Doulebit(7 ,*(Char + ii) * 256 + *(Char + ii+1))); 
				  write_dat(Get_Doulebit(6 ,*(Char + ii) * 256 + *(Char + ii+1)));
					write_dat(Get_Doulebit(5 ,*(Char + ii) * 256 + *(Char + ii+1))); 
				  write_dat(Get_Doulebit(4 ,*(Char + ii) * 256 + *(Char + ii+1)));
					write_dat(Get_Doulebit(3 ,*(Char + ii) * 256 + *(Char + ii+1))); 
				  write_dat(Get_Doulebit(2 ,*(Char + ii) * 256 + *(Char + ii+1)));
					write_dat(Get_Doulebit(1 ,*(Char + ii) * 256 + *(Char + ii+1))); 
				  write_dat(Get_Doulebit(0 ,*(Char + ii) * 256 + *(Char + ii+1)));
			    write_dat(0);
				ii += 2;
    }
		LCDSetAddr(0,64,0,80);
		Flashable = 1;
}

void ShowString8 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod)
{
    Uchar cnt,i;
	
		Flashable = 0;
	  for( cnt=0; cnt<4; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
		{
			LCDSetPosi(ll+cnt,rr);
//			write_com(0xA8);
			for (i=0;i<len*8;i++) {
				if (mod == 1){
					write_dat(~Get_NewDoulebit(i % 8, cnt*2, GUI_F8x8_acFont[Char[i/8]-32]));
				}
				else {
					write_dat(Get_NewDoulebit(i % 8, cnt*2, GUI_F8x8_acFont[Char[i/8]-32]));
				}
			}
//			write_com(0xAF);
    }
		Flashable = 1;
	
}

void ShowString8Old (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod)
{
    Uchar cnt,i;
	
		Flashable = 0;
	  for( cnt=0; cnt<8; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
		{
			LCDSetAddr(ll+cnt,1,rr,len*3);
			write_com(0x2c);
			for (i=0;i<len;i++) {
				if (mod == 1){
					write_dat(~Get_Doulebit(3 ,GUI_F8x8_acFont[Char[i]-32][cnt])); 
					write_dat(~Get_Doulebit(2 ,GUI_F8x8_acFont[Char[i]-32][cnt])); 
					write_dat(~Get_Doulebit(1 ,GUI_F8x8_acFont[Char[i]-32][cnt])); 
					write_dat(~Get_Doulebit(0 ,GUI_F8x8_acFont[Char[i]-32][cnt]));
				}
				else {
					write_dat(Get_Doulebit(3 ,GUI_F8x8_acFont[Char[i]-32][cnt])); 
					write_dat(Get_Doulebit(2 ,GUI_F8x8_acFont[Char[i]-32][cnt])); 
					write_dat(Get_Doulebit(1 ,GUI_F8x8_acFont[Char[i]-32][cnt])); 
					write_dat(Get_Doulebit(0 ,GUI_F8x8_acFont[Char[i]-32][cnt]));
				}
			}
      if (len * 8 % 3 != 0) write_dat(0);
    }
		LCDSetAddr(0,64,0,80);
		Flashable = 1;
}

void ShowStringPosi8 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod, u8 posi)
{
    Uchar cnt,i;
	
		Flashable = 0;
	  for( cnt=0; cnt<8; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
		{
			LCDSetAddr(ll+cnt,1,rr,len*3);
			write_com(0x2c);
			for (i=0;i<len;i++) {
				if (mod == 1 || posi==i){
					write_dat(~Get_Doulebit(3 ,GUI_F8x8_acFont[Char[i]-32][cnt])); 
					write_dat(~Get_Doulebit(2 ,GUI_F8x8_acFont[Char[i]-32][cnt])); 
					write_dat(~Get_Doulebit(1 ,GUI_F8x8_acFont[Char[i]-32][cnt])); 
					write_dat(~Get_Doulebit(0 ,GUI_F8x8_acFont[Char[i]-32][cnt]));
				}
				else {
					write_dat(Get_Doulebit(3 ,GUI_F8x8_acFont[Char[i]-32][cnt])); 
					write_dat(Get_Doulebit(2 ,GUI_F8x8_acFont[Char[i]-32][cnt])); 
					write_dat(Get_Doulebit(1 ,GUI_F8x8_acFont[Char[i]-32][cnt])); 
					write_dat(Get_Doulebit(0 ,GUI_F8x8_acFont[Char[i]-32][cnt]));
				}
			}
      if (len * 8 % 3 != 0) write_dat(0);
    }
		LCDSetAddr(0,64,0,80);
		Flashable = 1;
}

u8 FSMC_NOR_Read(u32 ReadAddr);

void ShowString16 (u8 ll, u8 rr, u8 *Char, u8 len, u8 mod)
{
    Uchar cnt,i;
	  u32 offset;
	
		Flashable = 0;
	  for( cnt=0; cnt<16; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
		{
			LCDSetAddr(ll+cnt,1,rr,len*3);
			write_com(0x2c);
			for (i=0;i<len;i++) {
				if (Char[i]<128) {
				  if (cnt<8) {
						write_dat(0x00); 
						write_dat(0x00); 
						write_dat(0x00); 
						write_dat(0x00);  
					}
					else {
						if ((mod == 1 && Char[i]!=' ') ||(mod==2 && i>0 && i!=len-1)) {
							write_dat(~Get_Doulebit(3 ,GUI_F8x8_acFont[Char[i]-32][cnt-8])); 
							write_dat(~Get_Doulebit(2 ,GUI_F8x8_acFont[Char[i]-32][cnt-8])); 
							write_dat(~Get_Doulebit(1 ,GUI_F8x8_acFont[Char[i]-32][cnt-8])); 
							write_dat(~Get_Doulebit(0 ,GUI_F8x8_acFont[Char[i]-32][cnt-8]));
						}
						else {
							write_dat(Get_Doulebit(3 ,GUI_F8x8_acFont[Char[i]-32][cnt-8])); 
							write_dat(Get_Doulebit(2 ,GUI_F8x8_acFont[Char[i]-32][cnt-8])); 
							write_dat(Get_Doulebit(1 ,GUI_F8x8_acFont[Char[i]-32][cnt-8])); 
							write_dat(Get_Doulebit(0 ,GUI_F8x8_acFont[Char[i]-32][cnt-8]));
						}
					}
				}
				else {
					offset = ((((u32)Char[i])-0xa1)*94+(Char[i+1]-0xa1))*32;
					i++;
					if (mod == 1 ||mod == 2){
						write_dat(~Get_Doulebit(3 ,FSMC_NOR_Read(offset+cnt*2))); 
						write_dat(~Get_Doulebit(2 ,FSMC_NOR_Read(offset+cnt*2))); 
						write_dat(~Get_Doulebit(1 ,FSMC_NOR_Read(offset+cnt*2))); 
						write_dat(~Get_Doulebit(0 ,FSMC_NOR_Read(offset+cnt*2)));
						write_dat(~Get_Doulebit(3 ,FSMC_NOR_Read(offset+cnt*2+1))); 
						write_dat(~Get_Doulebit(2 ,FSMC_NOR_Read(offset+cnt*2+1))); 
						write_dat(~Get_Doulebit(1 ,FSMC_NOR_Read(offset+cnt*2+1))); 
						write_dat(~Get_Doulebit(0 ,FSMC_NOR_Read(offset+cnt*2+1)));
					}
					else {
						write_dat(Get_Doulebit(3 ,FSMC_NOR_Read(offset+cnt*2))); 
						write_dat(Get_Doulebit(2 ,FSMC_NOR_Read(offset+cnt*2))); 
						write_dat(Get_Doulebit(1 ,FSMC_NOR_Read(offset+cnt*2))); 
						write_dat(Get_Doulebit(0 ,FSMC_NOR_Read(offset+cnt*2)));
						write_dat(Get_Doulebit(3 ,FSMC_NOR_Read(offset+cnt*2+1))); 
						write_dat(Get_Doulebit(2 ,FSMC_NOR_Read(offset+cnt*2+1))); 
						write_dat(Get_Doulebit(1 ,FSMC_NOR_Read(offset+cnt*2+1))); 
						write_dat(Get_Doulebit(0 ,FSMC_NOR_Read(offset+cnt*2+1)));
					}
				}
			}
      if (len * 8 % 3 != 0) write_dat(0);
    }
		LCDSetAddr(0,64,0,80);
		Flashable = 1;
}

void ShowStringPosi16 (u8 ll, u8 rr, u8 *Char, u8 len, u8 posi)
{
    Uchar cnt,i;
	  u32 offset;
	
		Flashable = 0;
	  for( cnt=0; cnt<16; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
		{
			LCDSetAddr(ll+cnt,1,rr,len*3);
			write_com(0x2c);
			for (i=0;i<len;i++) {
					offset = ((((u32)Char[i])-0xa1)*94+(Char[i+1]-0xa1))*32;
					i++;
					if (i/2==posi){
						write_dat(~Get_Doulebit(3 ,FSMC_NOR_Read(offset+cnt*2))); 
						write_dat(~Get_Doulebit(2 ,FSMC_NOR_Read(offset+cnt*2))); 
						write_dat(~Get_Doulebit(1 ,FSMC_NOR_Read(offset+cnt*2))); 
						write_dat(~Get_Doulebit(0 ,FSMC_NOR_Read(offset+cnt*2)));
						write_dat(~Get_Doulebit(3 ,FSMC_NOR_Read(offset+cnt*2+1))); 
						write_dat(~Get_Doulebit(2 ,FSMC_NOR_Read(offset+cnt*2+1))); 
						write_dat(~Get_Doulebit(1 ,FSMC_NOR_Read(offset+cnt*2+1))); 
						write_dat(~Get_Doulebit(0 ,FSMC_NOR_Read(offset+cnt*2+1)));
					}
					else {
						write_dat(Get_Doulebit(3 ,FSMC_NOR_Read(offset+cnt*2))); 
						write_dat(Get_Doulebit(2 ,FSMC_NOR_Read(offset+cnt*2))); 
						write_dat(Get_Doulebit(1 ,FSMC_NOR_Read(offset+cnt*2))); 
						write_dat(Get_Doulebit(0 ,FSMC_NOR_Read(offset+cnt*2)));
						write_dat(Get_Doulebit(3 ,FSMC_NOR_Read(offset+cnt*2+1))); 
						write_dat(Get_Doulebit(2 ,FSMC_NOR_Read(offset+cnt*2+1))); 
						write_dat(Get_Doulebit(1 ,FSMC_NOR_Read(offset+cnt*2+1))); 
						write_dat(Get_Doulebit(0 ,FSMC_NOR_Read(offset+cnt*2+1)));
					}
			}
      if (len * 8 % 3 != 0) write_dat(0);
    }
		LCDSetAddr(0,64,0,80);
		Flashable = 1;
}

void ShowChar8 (Uchar ll, Uchar rr, Uchar *Char)
{
    Uchar cnt;
	
		Flashable = 0;
	  for( cnt=0; cnt<8; cnt++)		//   128 com		 //	 //com 32 ( 0x20)---com  159
		{
			LCDSetAddr(ll+cnt,1,rr,2);
			write_com(0x2c);
					write_dat(Get_Doulebit(3 ,*(Char + cnt))); 
					write_dat(Get_Doulebit(2 ,*(Char + cnt))); 
					write_dat(Get_Doulebit(1 ,*(Char + cnt))); 
					write_dat(Get_Doulebit(0 ,*(Char + cnt)));
          write_dat(0);
    }
		LCDSetAddr(0,64,0,80);
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
   Disp(0x00,0x00);		 //FULL OFF
   Pause();  
//   Disp(0xF0,0x00);		 //FULL OFF
//   Pause();  
//   Disp(0x0F,0x00);		 //FULL OFF
//   Pause();  	 
//   Disp(0x00,0x00);		 //FULL OFF
//   Pause();  
//	ShowString16 (16, 0, "中国尚泰科技有限公司",20,0);
	
	ShowString8 (0, 10, "ABCDEFGHIJKL",12,0);
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
	
	write_com(0xC0);
	write_com(0x02);
	
	write_com(0xAF);
}


void LCDOld_initial(void)
{
   

//      RES_ON;
//    	delay(200);
//    	RES_OFF;
//    	delay(200);
//    	RES_ON;
//    	delay(200);

    write_com(0xD7);		// Disable Auto Read
	write_dat(0x9F);		
	write_com(0xE0);		// Enable OTP Read
	write_dat(0x00);		
		
	write_com(0xE3);		// OTP Up-Load
		
	write_com(0xE1);		// OTP Control Out
	write_com(0x11);		// Sleep Out
	write_com(0x28);		// Display OFF
		
		
	 write_com(0xC0);		//---------------------- Vop = 0XB9h //   0Xc0------------------------
	 write_dat(0xA1);		// 06  0x3a  vop  10.1v //          //0xeb
	 write_dat(0x00);		// 01

	write_com(0xC3);		// BIAS  --------------------------
	write_dat(0x05);		// 0x01= 1/13	BIAS //    0x04	1/10BIAS  // 0x00    1/14  bias  // 0x05  1/9bias//

	write_com(0xC4);		//-------------------------- Booster = x5
	write_dat(0x04);		
	
	write_com(0xD0);		// Enable Analog Circuit
	write_dat(0x1D);


			
	//write_com(0xB5);		// N-Line = 13
	//write_dat(0x00);		//8d //9f//1f --------------------------------------(     )



	write_com(0x38);		//	M=0: Gray mode	M=1: Monochrome mode

//	write_com(0x39);		// B/W Mode	 Display Mode  ------------	 Monochrome mode

	write_com(0x3A);		// Enable DDRAM Interface/
//	write_dat(0x02);
	write_dat(0x83);

    write_com(0x36);		// Scan Direction Setting/Display Control-------------------
		write_dat(0xC8);		//seg  0---283   com  0--160 Set scan direction of COM andSEG

                           
	write_com(0xB0);		// Duty Setting	-------------------------------------------
	write_dat(0x3F);		//0x7f==>1/128Duty	 0x9f==>1/160duty   0x3f==>1/64duty    -----------------------------------
	
	write_com(0xB3);		//FOSC Divider	   0 1    2 Divisions
	write_dat(0x01);


	//write_com(0xB4);		
	//write_dat(0xA0);


	//write_com(0x30); // Partial Display Area = COM0 ~ COM32
	//write_dat(0x00);
	//write_dat(0x00);
	//write_dat(0x00);
	//write_dat(0x20); //COM32 0X20?/

	write_com(0x2A);		// Column Address Setting
	write_dat(0x00);		// SEG144 -> SEG383
	write_dat(48);        //	42	
	write_dat(0x00);		
	write_dat(127);		// 240/3 -1 =79
			
	write_com(0x2B);		// Row Address Setting
	write_dat(0x00);		// 
	write_dat(96);		 // com 64 ( 0x3F)
	write_dat(0x00);			
	write_dat(159);		 //COM-1=ANS   64-1=63

	write_com(0xF1);		// 
	write_dat(0x14);		// 
	write_dat(0x14);		 // 
	write_dat(0x14);			
	write_dat(0x14);		 //


//	write_com(0x2A);

	write_com(0x29);		// Display ON
//	Pause();
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
	write_com(0xAF);
	   delay(7000);
}

void DispOld(Uchar dat1,Uchar dat2)
 {

  Uchar cnt;
  Uchar cnt1;

	
	 Flashable = 0;
	 LCDSetAddr(0,64,0,80);
	 write_com(0x2C);      //Write Display Data
		  
    for(cnt1=0;cnt1<64;cnt1++)	   // com?	  
    {										 
        for(cnt=0;cnt<80;cnt++)		//1/3  SEG
        {     
            write_dat(dat1);
            write_dat(dat2);
					  write_dat(0);
        }
		}
	   delay(7000);
	  Flashable = 1;
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
