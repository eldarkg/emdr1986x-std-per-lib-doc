/**
  ******************************************************************************
  * @file    Examples/MDR1986BE4_EVAL/ADC/ADC1_DMA/MDR32F9Qx_it.c
  * @author  Milandr Application Team
  * @version V1.2.0
  * @date    12/09/2014
  * @brief   Main Interrupt Service Routines.
  *
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR1986BE4.h"
#include "MDR32F9Qx_it.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_dma.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_timer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : DMA_IRQHandler
* Description    : This function handles DMA global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA_IRQHandler(void)
{

}

void UART1_IRQHandler(void)
{
  uint32_t temp_1;
  
  //UARTx_HandlerWork(MDR_UART2);
  if (UART_GetITStatusMasked(MDR_UART1, UART_IT_RX) == SET)
  {
    temp_1 = MDR_UART1->DR;
    
    UART_ClearITPendingBit(MDR_UART1, UART_IT_RX);

    while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE)!= SET)
    {
    }
    UART_SendData (MDR_UART1,0x44);
    

  }
}

void TIMER1_IRQHandler(void)
{
  uint32_t temp_2 = 0;


    if (TIMER_GetITStatus(MDR_TIMER1, TIMER_STATUS_CNT_ARR) == SET)
    {   
    //TIMER_ClearITPendingBit(MDR_TIMER1, TIMER_STATUS_CNT_ARR);
        MDR_TIMER1->STATUS &= ~TIMER_STATUS_CNT_ARR;
      
          while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE)!= SET)
    {
    }
    UART_SendData (MDR_UART1,0x35);



    }
}
/******************* (C) COPYRIGHT 2014 Milandr *********/

/* END OF FILE MDR32F9Qx_it.c */
