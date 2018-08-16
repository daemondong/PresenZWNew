/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "USART.h"
#include "GloablVar.h"

extern void TimingDelay_Decrement(void);
extern void TIM2_Int(void);

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


void TIM2_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM2_Int();
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
*/
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}
//串口1中断
void USART1_IRQHandler(void)
{  
//     if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)  
//      {       
//              USART_SendData(USART1, USART_ReceiveData(USART1));             
//      }
     if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)  
      {       
              DealUSART(1,&usart1_data, USART_ReceiveData(USART1),0);             
      }
     if(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET)  
      {       
//						USART_ClearITPendingBit(USART1,USART_IT_TXE);
//						USART_ClearFlag(USART1,USART_FLAG_TC);
//						USART_ClearFlag(USART1,USART_FLAG_TXE);
            SendUSART(1,USART1,&usart1_data);             
      }      
}
//串口2中断
void USART2_IRQHandler(void)
{  
		if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)  
      {       
              DealUSART(2,&usart2_data, USART_ReceiveData(USART2),0);             
      }
//     if(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == SET)  
//      {       
//              SendUSART(2,USART2,&usart2_data);             
//      }      
      
}
//串口3中断
void USART3_IRQHandler(void)
{  
u16 mySR;
	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)  
      {       
        mySR=USART3->SR;      
				DealUSART(3,&usart3_data, USART_ReceiveData(USART3),mySR);             
      }
//     if(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == SET)  
//      {       
//              SendUSART(3,USART3,&usart3_data);             
//      }      
      
}
//串口4中断
void UART4_IRQHandler(void)
{  
     if(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == SET)  
      {       
              DealUSART(4,&usart4_data, USART_ReceiveData(UART4),0);             
      }
 //     if(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == SET)  
 //     {       
 //             SendUSART(4,UART4,&usart4_data);             
 //     }      
     
}
//串口5中断
void UART5_IRQHandler(void)  
{  
     if(USART_GetFlagStatus(UART5, USART_FLAG_RXNE) == SET)  
      {       
              DealUSART(5,&usart5_data, USART_ReceiveData(UART5),0);             
      }
//     if(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == SET)  
//      {       
//              SendUSART(5,UART5,&usart5_data);             
//      }      
      
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
