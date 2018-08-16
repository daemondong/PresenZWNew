// http://blog.csdn.net/licaihuameng/article/details/7758029
/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : fsmc_sram.c
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file provides a set of functions needed to drive the 
*                      IS61WV51216BLL SRAM memory mounted on STM3210E-EVAL board.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "fsmc_nor.h"
#include "stm32f10x.h"
#include "delay.h"
#include "USART.h"
#include "GloablVar.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define Bank1_SRAM2_ADDR    ((u32)0x64000000)
#define Bank1_NOR1_ADDR       ((u32)0x60000000)

#define SRAM_WRITE(Address, Data)  (*(vu16 *)(Address) = (Data))

/* Delay definition */   
#define BlockErase_Timeout    ((u32)0x00A00000)
#define ChipErase_Timeout     ((u32)0x30000000) 
#define Program_Timeout       ((u32)0x00001400)

/* Private macro -------------------------------------------------------------*/
//#define ADDR_SHIFT(A) (Bank1_NOR1_ADDR + (2 * (A)))
#define ADDR_SHIFT(A) (Bank1_NOR1_ADDR +  (A))
#define NOR_WRITE(Address, Data)  (*(vu8 *)(Address) = (Data))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Disp_RAM_Success(u8 stat);

/*******************************************************************************
* Function Name  : FSMC_SRAM_Init
* Description    : Configures the FSMC and GPIOs to interface with the SRAM memory.
*                  This function must be called before any write/read operation
*                  on the SRAM.
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
  GPIO_InitTypeDef GPIO_InitStructure; 
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOF, ENABLE);
  
/*-- GPIO Configuration ------------------------------------------------------*/
  /* SRAM Data lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  /* SRAM Address lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 |
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
   
  /* NOE and NWE and NE1 configuration */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* NE2 and NE3 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  /* NBL0, NBL1 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
  
/*--  Quick FSMC Configuration ------------------------------------------------------*/
/*  p.FSMC_AddressSetupTime = 0;
  p.FSMC_AddressHoldTime = 0;
  p.FSMC_DataSetupTime = 1;
  p.FSMC_BusTurnAroundDuration = 0;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;		*/
/*-- FSMC Configuration ------------------------------------------------------*/
  p.FSMC_AddressSetupTime = 1;
  p.FSMC_AddressHoldTime = 2;
  p.FSMC_DataSetupTime = 2;
  p.FSMC_BusTurnAroundDuration = 0;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;

	p.FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM2;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);  

  /*-- NOR FLASH FSMC Configuration ----------------------------------------------------*/
  p.FSMC_AddressSetupTime = 0x05;     /*ADDSET  ??????*/
  p.FSMC_AddressHoldTime = 0x00;    /*ADDHOLD ??????*/
  p.FSMC_DataSetupTime = 0x07;     /*DATAST ??????*/
  p.FSMC_BusTurnAroundDuration = 0x00;   /*BUSTURN ??????*/
  p.FSMC_CLKDivision = 0x00;      /*CLKDIV ????*/
  p.FSMC_DataLatency = 0x00;     /*DATLAT ??????*/
  p.FSMC_AccessMode = FSMC_AccessMode_B;   /*????*/

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;    // NOR/SRAM????,?4???
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;  //  ??????????????
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;   // ????????????????
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;  //?????????
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;   // ???????NOR????????????
                       //????????????(???????????)
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;   //   ??WAIT???????
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;    // ??????????
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState; // ??WAIT??????
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;  //   ?????????
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   //    ??????WAIT??
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;  // ?????????,??????????????????????
                    // ????????????
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;  //  ????????????
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;  //  ???????????
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;    //  ??????
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;    //
                 
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);        //

  /* Enable FSMC Bank1_NOR Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);        //

/*-- FSMC Configuration ------------------------------------------------------*/
  p.FSMC_AddressSetupTime = 1;
  p.FSMC_AddressHoldTime = 0;
  p.FSMC_DataSetupTime = 5;
  p.FSMC_BusTurnAroundDuration = 0;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;

	p.FSMC_AccessMode = FSMC_AccessMode_B;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  

}

/*******************************************************************************
* Function Name  : FSMC_SRAM_WriteBuffer
* Description    : Writes a Half-word buffer to the FSMC SRAM memory. 
* Input          : - pBuffer : pointer to buffer. 
*                  - WriteAddr : SRAM memory internal address from which the data
*                    will be written.
*                  - NumHalfwordToWrite : number of half-words to write. 
*                    
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_WriteBuffer(u16* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite)
{
  for(; NumHalfwordToWrite != 0; NumHalfwordToWrite--) /* while there is data to write */
  {
    /* Transfer data to the memory */
    *(u16 *) (Bank1_SRAM2_ADDR + WriteAddr) = *pBuffer++;
    
    /* Increment the address*/  
    WriteAddr += 2;
  }   
}

/*******************************************************************************
* Function Name  : FSMC_SRAM_ReadBuffer
* Description    : Reads a block of data from the FSMC SRAM memory.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the SRAM memory.
*                  - ReadAddr : SRAM memory internal address to read from.
*                  - NumHalfwordToRead : number of half-words to read.
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_ReadBuffer(u16* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead)
{
  for(; NumHalfwordToRead != 0; NumHalfwordToRead--) /* while there is data to read */
  {
    /* Read a half-word from the memory */
    *pBuffer++ = *(vu16*) (Bank1_SRAM2_ADDR + ReadAddr);

    /* Increment the address*/  
    ReadAddr += 2;
  }  
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

 

/***************************************************************
* Function Name  : FSMC_SRAM_WriteHalfWord
* Description    : Writes a half-word to the SRAM memory. 
* Input          : - WriteAddr : SRAM memory internal address to write to.
*                  - Data : Data to write. 
* Output         : None
* Return         : 
*******************************************************************************/
void FSMC_SRAM_WriteHalfWord(u32 WriteAddr, u16 Data)
{
  //NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
 // NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
 // NOR_WRITE(ADDR_SHIFT(0x05555), 0x00A0);
  SRAM_WRITE((Bank1_SRAM2_ADDR + WriteAddr), Data);

  //return (FSMC_NOR_GetStatus(Program_Timeout));
}

/******************************************************************************
* Function Name  : FSMC_SRAM_ReadHalfWord
* Description    : Reads a half-word from the SRAM memory. 
* Input          : - ReadAddr : NOR memory internal address to read from.
* Output         : None
* Return         : Half-word read from the SRAM memory
*******************************************************************************/
u16 FSMC_SRAM_ReadHalfWord(u32 ReadAddr)
{
  //NOR_WRITE(ADDR_SHIFT(0x005555), 0x00AA); 
  //NOR_WRITE(ADDR_SHIFT(0x002AAA), 0x0055);  
  //NOR_WRITE((Bank1_NOR1_ADDR + ReadAddr), 0x00F0 );

  return (*(vu16 *)((Bank1_SRAM2_ADDR + ReadAddr)));
}
 
#define BUFFER_SIZE        0x80
#define WRITE_READ_ADDR    0x7F80

void Fill_Buffer(u16 *pBuffer, u16 BufferLenght, u32 Offset)
{
  u16 IndexTmp = 0;

  /* Put in global buffer same values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp * (1+16+256+256*16)   + Offset;
  }
}

void Fill_8Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset)
{
  u16 IndexTmp = 0;

  /* Put in global buffer same values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp * (1+16)   + Offset;
  }
}

void Test_RAM(void) {

	u16 TxBuffer[BUFFER_SIZE];
	u16 RxBuffer[BUFFER_SIZE];
	u32 WriteReadStatus = 0, Index = 0;
	
  Fill_Buffer(TxBuffer, BUFFER_SIZE, 0x3212);
  FSMC_SRAM_WriteBuffer(TxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);


  /* Read data from FSMC SRAM memory */
  FSMC_SRAM_ReadBuffer(RxBuffer, WRITE_READ_ADDR, BUFFER_SIZE); 

  /* Read back SRAM memory and check content correctness */   
  for (Index = 0x00; (Index < BUFFER_SIZE) && (WriteReadStatus == 0); Index++)
  {
    if (RxBuffer[Index] != TxBuffer[Index])
    {
      WriteReadStatus = Index + 1;
    }
  } 

  if (WriteReadStatus == 0) Disp_RAM_Success(1);
	else Disp_RAM_Success(0);
}

/******************************************************************************
* Function Name  : FSMC_NOR_ReadID
* Description    : Reads NOR memory's Manufacturer and Device Code.
* Input          : - NOR_ID: pointer to a NOR_IDTypeDef structure which will hold
*                    the Manufacturer and Device Code.
* Output         : None
* Return         : None
*******************************************************************************/
/*void FSMC_NOR_ReadID(NOR_IDTypeDef* NOR_ID)
{
  NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x05555), 0x0090);

  NOR_ID->Manufacturer_Code = *(vu16 *) ADDR_SHIFT(0x0000);
  NOR_ID->Device_Code1 = *(vu16 *) ADDR_SHIFT(0x0001);
  NOR_ID->Device_Code2 = *(vu16 *) ADDR_SHIFT(0x000E);
  NOR_ID->Device_Code3 = *(vu16 *) ADDR_SHIFT(0x000F);
}
*/

/*******************************************************************************
* Function Name  : FSMC_NOR_EraseBlock
* Description    : Erases the specified Nor memory block.
* Input          : - BlockAddr: address of the block to erase.
* Output         : None
* Return         : NOR_Status:The returned value can be: NOR_SUCCESS, NOR_ERROR
*                  or NOR_TIMEOUT
*******************************************************************************/
void FSMC_NOR_EraseBlock(u32 BlockAddr)
{
/*  NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x05555), 0x0080);
  NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
  NOR_WRITE((Bank1_NOR1_ADDR + BlockAddr), 0x30);
*/
  NOR_WRITE(ADDR_SHIFT(0x00555), 0xAA);
  NOR_WRITE(ADDR_SHIFT(0x002AA), 0x55);
  NOR_WRITE(ADDR_SHIFT(0x00555), 0x80);
  NOR_WRITE(ADDR_SHIFT(0x00555), 0xAA);
  NOR_WRITE(ADDR_SHIFT(0x002AA), 0x55);
//  NOR_WRITE(ADDR_SHIFT(BlockAddr), 0x20);
  NOR_WRITE((Bank1_NOR1_ADDR + BlockAddr), 0x20);
	
	Delay_us(3000);
}

/*******************************************************************************
* Function Name  : FSMC_NOR_EraseChip
* Description    : Erases the entire chip.
* Input          : None                      
* Output         : None
* Return         : NOR_Status:The returned value can be: NOR_SUCCESS, NOR_ERROR
*                  or NOR_TIMEOUT
*******************************************************************************/
void FSMC_NOR_EraseChip(void)
{
/*  NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x05555), 0x0080);
  NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x05555), 0x0010);
*/
  NOR_WRITE(ADDR_SHIFT(0x00555), 0xAA);
  NOR_WRITE(ADDR_SHIFT(0x002AA), 0x55);
  NOR_WRITE(ADDR_SHIFT(0x00555), 0x80);
  NOR_WRITE(ADDR_SHIFT(0x00555), 0xAA);
  NOR_WRITE(ADDR_SHIFT(0x002AA), 0x55);
  NOR_WRITE(ADDR_SHIFT(0x00555), 0x10);
	
  Delay_us(8000);
}

/******************************************************************************
* Function Name  : FSMC_NOR_WriteHalfWord
* Description    : Writes a half-word to the NOR memory. 
* Input          : - WriteAddr : NOR memory internal address to write to.
*                  - Data : Data to write. 
* Output         : None
* Return         : NOR_Status:The returned value can be: NOR_SUCCESS, NOR_ERROR
*                  or NOR_TIMEOUT
*******************************************************************************/
void FSMC_NOR_Write(u32 WriteAddr, u8 Data)
{
/*  NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x05555), 0x00A0);
  NOR_WRITE((Bank1_NOR1_ADDR + WriteAddr), Data);
*/
  NOR_WRITE(ADDR_SHIFT(0x00555), 0xAA);
  NOR_WRITE(ADDR_SHIFT(0x002AA), 0x55);
  NOR_WRITE(ADDR_SHIFT(0x00555), 0xA0);
  NOR_WRITE((Bank1_NOR1_ADDR + WriteAddr), Data);

  Delay_us(3);
}

/*******************************************************************************
* Function Name  : FSMC_NOR_WriteBuffer
* Description    : Writes a half-word buffer to the FSMC NOR memory. 
* Input          : - pBuffer : pointer to buffer. 
*                  - WriteAddr : NOR memory internal address from which the data 
*                    will be written.
*                  - NumHalfwordToWrite : number of Half words to write. 
* Output         : None
* Return         : NOR_Status:The returned value can be: NOR_SUCCESS, NOR_ERROR
*                  or NOR_TIMEOUT
*******************************************************************************/
void FSMC_NOR_WriteBuffer(u8* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite)
{

  do
  {
    /* Transfer data to the memory */
    FSMC_NOR_Write(WriteAddr, *pBuffer++);
    WriteAddr += 1 ;
    NumHalfwordToWrite--;
  }
  while(NumHalfwordToWrite != 0);
  
}


/******************************************************************************
* Function Name  : FSMC_NOR_ReadHalfWord
* Description    : Reads a half-word from the NOR memory. 
* Input          : - ReadAddr : NOR memory internal address to read from.
* Output         : None
* Return         : Half-word read from the NOR memory
*******************************************************************************/
u8 FSMC_NOR_Read(u32 ReadAddr)
{
/*  NOR_WRITE(ADDR_SHIFT(0x005555), 0x00AA); 
  NOR_WRITE(ADDR_SHIFT(0x002AAA), 0x0055);  
  NOR_WRITE((Bank1_NOR1_ADDR + ReadAddr), 0x00F0 );
*/
  return (*(vu8 *)((Bank1_NOR1_ADDR + ReadAddr)));
}

/*******************************************************************************
* Function Name  : FSMC_NOR_ReadBuffer
* Description    : Reads a block of data from the FSMC NOR memory.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the NOR memory.
*                  - ReadAddr : NOR memory internal address to read from.
*                  - NumHalfwordToRead : number of Half word to read.
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_NOR_ReadBuffer(u8* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead)
{
/*  NOR_WRITE(ADDR_SHIFT(0x05555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x02AAA), 0x0055);
  NOR_WRITE((Bank1_NOR1_ADDR + ReadAddr), 0x00F0);
*/
  u8 rdB;
	for(; NumHalfwordToRead != 0x00; NumHalfwordToRead--) /* while there is data to read */
  {
    /* Read a Halfword from the NOR */
    rdB = *(vu8 *)((Bank1_NOR1_ADDR + ReadAddr));
    *pBuffer++ = rdB & 0xFF;
//		*pBuffer++ = rdB >> 16;
    ReadAddr +=1; 
  }  
}

void Test_NORFlash(void) {
	u8 TxBuffer[BUFFER_SIZE];
	u8 RxBuffer[BUFFER_SIZE];
	u32 WriteReadStatus = 0, Index = 0;

  /* Erase the NOR memory block to write on */
//  FSMC_NOR_EraseBlock(WRITE_READ_ADDR);

  /* Write data to FSMC NOR memory */
  /* Fill the buffer to send */
  Fill_8Buffer(TxBuffer, BUFFER_SIZE, 0x10);
//  FSMC_NOR_WriteBuffer(TxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);

  /* Read data from FSMC NOR memory */
//  FSMC_NOR_ReadBuffer(RxBuffer, WRITE_READ_ADDR, BUFFER_SIZE); 

  /* Read back NOR memory and check content correctness */   
  for (Index = 0x00; (Index < BUFFER_SIZE) && (WriteReadStatus == 0); Index++)
  {
    if ((RxBuffer[Index] & 0xff) != (TxBuffer[Index] & 0xff) )
    {
      WriteReadStatus = Index + 1;
    }
  }

//  if (WriteReadStatus == 0) Disp_Flash_Success(1);
//	else Disp_Flash_Success(0);
 	
}

void EraseFlashBlocks(u32 StartAddr,u16 num){
	u16 i;
	for (i=0;i<num;i++) FSMC_NOR_EraseBlock(StartAddr + i*128);
}

/*void AddFlashRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem,u16 MaxBlocks){
	u16 i=0;
	u8 j,buff[128],prebuff1;
	u8 maxRecord = 126/SizeMem;
	
	FSMC_NOR_ReadBuffer(buff, StartAddr+i*128, 128);
	prebuff1=buff[1];
	while (buff[0]>=maxRecord && buff[0]!= 0xFF && i<MaxBlocks) {
		i++;
		FSMC_NOR_ReadBuffer(buff, StartAddr+i*128, 128);
		if (buff[0]==maxRecord && buff[1] + 1 == prebuff1) break;
	}
	
	if (buff[0] >= maxRecord && buff[0]!= 0xFF) {
		FSMC_NOR_ReadBuffer(buff, StartAddr, 128);
		i=1;
		buff[0] = 1;
		buff[1] += 1;
  }
	else if (buff[0]== 0xFF) { buff[0] =1; buff[1] = 0;}
	else buff[0]++;
	
	for (j=0;j<SizeMem;j++) buff[(buff[0]-1)*SizeMem+j+2] = *(ptr+j);   //存储于第二字节开始
	
	FSMC_NOR_EraseBlock(StartAddr + (i-1)*128);
	FSMC_NOR_WriteBuffer(buff, StartAddr + (i-1)*128, 128);
}
*/

u8 isFoundMark(u8 *buff,u8 SizeMem) {
	u8 i;
	
	if (buff[0]==0xff && buff[1] == 0xff) return 1; //空flash
	
	for (i=0;i<128-SizeMem;i++) if (buff[i]==0xA5 && buff[i+1]==0xEA) return i+3;
	
	return 0;
}

u8 AddFlashRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem,u16 MaxBlocks){
	u16 i=0;
	u8 j,buff[128],posi,isFull,wrNextB=0;
	
	isFull = 0;
	FSMC_NOR_ReadBuffer(buff, StartAddr+i*128, 128);
	posi = isFoundMark(buff,SizeMem);
	while (posi==0 && i<MaxBlocks-1) {
		i++;
		FSMC_NOR_ReadBuffer(buff, StartAddr+i*128, 128);
		posi = isFoundMark(buff,SizeMem);
	}
	
	if (posi==0) {
		FSMC_NOR_ReadBuffer(buff, StartAddr, 128);
		isFull = 1;
		i=0;
		posi=1;
  }
	else if (posi>2) posi = posi - 2;
	
	testByte0=isFull;
	testByte1=i;
	for (j=0;j<SizeMem;j++) buff[posi-1+j] = *(ptr+j);
	if (128-(posi-1+j)<SizeMem) {
		wrNextB=1;
	}
	else {
		buff[posi-1+j++] = 0xA5;
		buff[posi-1+j] = 0xEA;																		//两位标志字节 A5 EA
	}
	
	FSMC_NOR_EraseBlock(StartAddr + i*128);
	FSMC_NOR_WriteBuffer(buff, StartAddr + i*128, 128);
	
	if (wrNextB==1 && i<MaxBlocks-1) {
		FSMC_NOR_ReadBuffer(buff, StartAddr + (i+1)*128, 128);
		buff[0] = 0xA5;
		buff[1] = 0xEA;																					//两位标志字节 A5 EA
		FSMC_NOR_EraseBlock(StartAddr + (i+1)*128);
		FSMC_NOR_WriteBuffer(buff, StartAddr + (i+1)*128, 128);
	}
	
	return isFull;
}

u8 GetLastFlashRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem, u16 MaxBlocks,u8 isFull){
	u16 i=0;
	u8 j,buff[128],posi;
	
	nowRecPosi=0;
	nowRecBlk=0;
	posi=0;
	totRec=0;
	disRec=0;
	while (i<MaxBlocks && posi==0) {
		FSMC_NOR_ReadBuffer(buff, StartAddr+i*128, 128);
		if (buff[0]==0xff && buff[1] == 0xff) return 0; //空flash
		posi = isFoundMark(buff,SizeMem);
		i++;
	}
	
	if (posi==0) return 0;
	
	if (posi<=3) nowRecPosi=0;
	else nowRecPosi = posi-3-SizeMem;
	if (isFull==0)
		totRec=(i-1)*128/SizeMem + nowRecPosi/SizeMem + 1;
	else
		totRec= (128/SizeMem) * MaxBlocks;
	disRec=totRec;
	for (j=0;j<SizeMem;j++) *(ptr+j) = buff[nowRecPosi+j];
	nowRecBlk=i-1;
	return 1;
}

u8 GetPreFlashRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem, u16 MaxBlocks){
	u8 j,buff[128];
	
//	if (nowRecPosi==0 && nowRecBlk==0) return 0;
	if (nowRecPosi==0) {
		if (nowRecBlk==0) nowRecBlk=MaxBlocks-1;
		else nowRecBlk--;
		FSMC_NOR_ReadBuffer(buff, StartAddr+nowRecBlk*128, 128);
		nowRecPosi = (128/SizeMem - 1) * SizeMem;
		for (j=0;j<SizeMem;j++) *(ptr+j) = buff[nowRecPosi+j];
	}
	else {
		FSMC_NOR_ReadBuffer(buff, StartAddr+nowRecBlk*128, 128);
		nowRecPosi -= SizeMem;
		for (j=0;j<SizeMem;j++) *(ptr+j) = buff[nowRecPosi+j];
	}
	return 1;
}


u8 GetNextFlashRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem, u16 MaxBlocks){
	u8 j,buff[128];
	
  if (nowRecPosi>128-2*SizeMem) { 
		nowRecBlk++; 
		if (nowRecBlk==MaxBlocks) nowRecBlk=0;
		nowRecPosi=0; 
	}
	else nowRecPosi += SizeMem;
	
	FSMC_NOR_ReadBuffer(buff, StartAddr+nowRecBlk*128, 128);
	if (buff[nowRecPosi]==0xA5 && buff[nowRecPosi+1] == 0xEA) return 0;
		
	for (j=0;j<SizeMem;j++) *(ptr+j) = buff[nowRecPosi+j];
	return 1;
}

void WriteFlashFixRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem,u16 RecordNum){
	u16 stblock=(RecordNum*SizeMem)/128;
	u8 stposi=(RecordNum*SizeMem)%128;
	u8 i,j,buff[128];
	
	FSMC_NOR_ReadBuffer(buff, StartAddr+stblock*128, 128);
	if (128-stposi>=SizeMem) {
		for (i=0;i<SizeMem;i++) *(buff+stposi+i) = *(ptr+i);
		FSMC_NOR_EraseBlock(StartAddr + stblock*128);
		for (j=0;j<0x80;j++)
			FSMC_NOR_Write(StartAddr + stblock*128 + j, buff[j]);
	}
	else {
		for (i=0;i<128-stposi;i++) *(buff+stposi+i) = *(ptr+i);
		FSMC_NOR_EraseBlock(StartAddr + stblock*128);
		for (j=0;j<0x80;j++)
			FSMC_NOR_Write(StartAddr + stblock*128 + j, buff[j]);
		FSMC_NOR_ReadBuffer(buff, StartAddr+(stblock+1)*128, 128);
		for (j=0;i<SizeMem;i++,j++) *(buff+j) = *(ptr+i);
		FSMC_NOR_EraseBlock(StartAddr + (stblock+1)*128);
		for (j=0;j<0x80;j++)
			FSMC_NOR_Write(StartAddr + (stblock+1)*128 + j, buff[j]);
	}
}

void ReadFlashFixRecoard(u32 StartAddr,u8 *ptr,u8 SizeMem,u16 RecordNum){
	u16 stblock=(RecordNum*SizeMem)/128;
	u8 stposi=(RecordNum*SizeMem)%128;
	u8 i,j,buff[128];
	
	for (j=0;j<0x80;j++) buff[j]=FSMC_NOR_Read(StartAddr+stblock*128+j);
//	FSMC_NOR_ReadBuffer(buff, StartAddr+stblock*128, 128);
	if (128-stposi>=SizeMem) {
		for (i=0;i<SizeMem;i++)  *(ptr+i) = *(buff+stposi+i);
	}
	else {
		for (i=0;i<128-stposi;i++) *(ptr+i) = *(buff+stposi+i);
		for (j=0;j<0x80;j++) buff[j]=FSMC_NOR_Read(StartAddr+(stblock+1)*128+j);
//		FSMC_NOR_ReadBuffer(buff, StartAddr+(stblock+1)*128, 128);
		for (j=0;i<SizeMem;i++,j++)  *(ptr+i) = *(buff+j);
	}
	
	if (ptr[0]==0xff) for (i=0;i<SizeMem;i++) ptr[i]=0;                  //空记录填0
}
