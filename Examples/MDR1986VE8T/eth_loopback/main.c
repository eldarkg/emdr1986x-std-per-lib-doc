/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   This example shows how to configure PHY and MAC, transmit and receive some low level data
  ******************************************************************************
  * <br><br>
  *
  *
  * <h2><center>&copy; COPYRIGHT 2015 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR1986VE8T.h"
#include "MDR32F8_it.h"
#include "spec.h"

#include "mdr32f8_config.h"
#include "mdr32f8_port.h"
#include "mdr32f8_clkctrl.h"
#include "mdr32f8_eth.h"

/** @addtogroup __MDR32F8_StdPeriph_Examples
  * @{
  */

/** @addtogroup __MDR32F8_Board_For_MCU
  * @{
  */

/** @addtogroup SPI_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SIZE 65
#define COUNT 48

#define ETH_BUF 0x21008000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
ETH_InitTypeDef  ETH_InitStruct;

uint32_t tmp;

uint32_t DstBuf[23];
uint32_t SrcBuf[23] = {0x00000040, 0xFFFFFFFF, 0xFFFFFFFF, 0x04040405, 0x01010105, 0x01010106, 0x01010107, 0x01010108, 0x01010105, 0x01010106, 
0x01010107, 0x01010108, 0x01010105, 0x01010106, 0x01010107, 0x01010108, 0x01010105, 0x01010106, 0x01010107, 0x01010108, 
0x01010105, 0x01010106, 0x0101017};
	
uint32_t *DstBuf_p = DstBuf;
uint32_t *SrcBuf_p = SrcBuf;
			
/* Private function prototypes -----------------------------------------------*/
void POR_disable();
void KEY_reg_accs();
void BlinkLine(uint32_t Pin);
void Ini_BUF(uint32_t addr,uint32_t size,uint32_t delta);
uint32_t TestETH();
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
		
int main(void)
{	
	uint32_t reg;
	
	POR_disable();
	
	CLKCTRL_DeInit();
	/* Enable HSE0 clock */
	CLKCTRL_HSEconfig(CLKCTRL_HSE0_CLK_ON);
	
	/* Check HSE success, enable PLL0, check PLL0 success, select MAX_CLK src */
	while(CLKCTRL_HSEstatus(CLKCTRL_HSEn_STAT_HSE0_RDY) != SUCCESS){}
		
	/* Select PLL0 clk src, PLL0_N, PLL0_Q to get FINT = FIN*(PLLn_N)/(PLLn_Q+1) */
	CLKCTRL_CPU_PLLconfig(PLL0, CLKCTRL_PLLn_CLK_SELECT_HSE0div2, 0, 8);//PLLn, SRC, Q, N
		while(CLKCTRL_CPU_PLLstatus(0) != SUCCESS){}
			CLKCTRL_MAX_CLKSelection(CLKCTRL_MAX_CLK_PLL0);		

	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_ENET0_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTC_EN, ENABLE);

	KEY_reg_accs();

	ETH_ClockDeInit();
	ETH_PHY_ClockConfig(ETH_PHY_CLOCK_SOURCE_PLL0, ETH_PHY_HCLKdiv1);
	ETH_ClockCMD(ENABLE);
	
	/* Init ETH_InitStruct members with its default value */
	ETH_StructInit((ETH_InitTypeDef * ) &ETH_InitStruct);
			
	ETH_InitStruct.ETH_PHY_LB = DISABLE;
	ETH_InitStruct.ETH_PHY_DLB = DISABLE;
	ETH_InitStruct.ETH_PHY_HALFD = DISABLE;
	/*ETH_InitStruct.ETH_PHY_EARLY_DV = 0;*///RESERVED
	ETH_InitStruct.ETH_PHY_DIR = ETH_PHY_CONTROL_DIR_DIRECT;
	ETH_InitStruct.ETH_PHY_BASE_2 = ETH_PHY_CONTROL_BASE_2_TWISTED;
	ETH_InitStruct.ETH_PHY_LINK_PERIOD = 0x2;
	ETH_InitStruct.ETH_PHY_EXT_EN = ETH_PHY_CONTROL_EXT_EN_INTER;
	ETH_InitStruct.ETH_PHY_A_SELR = ETH_PHY_CONTROL_A_SELR_INTER;
	ETH_InitStruct.ETH_PHY_A_TRIM = 0x4;
	ETH_InitStruct.ETH_PHY_A_TRIMR = 0x80;
		
	ETH_InitStruct.ETH_Extension_Mode  = ENABLE;
	ETH_InitStruct.ETH_ColWnd 				 = 0x5;
	ETH_InitStruct.ETH_HalfDuplex_Mode = DISABLE;
	ETH_InitStruct.ETH_Buffer_Mode 		 = ETH_BUFFER_MODE_FIFO;
	ETH_InitStruct.ETH_Register_CLR 	 = DISABLE;
	ETH_InitStruct.ETH_Loopback_Mode   = DISABLE;
	ETH_InitStruct.ETH_DBG_XF          = ENABLE;
	ETH_InitStruct.ETH_DBG_RF          = ENABLE;
	
	ETH_InitStruct.ETH_Receive_All_Packets = ENABLE;
	ETH_InitStruct.ETH_Error_CRC_Frames_Reception = ENABLE;
	ETH_InitStruct.ETH_Receiver_Event_Mode = ETH_RECEIVER_EVENT_MODE_PACET_RECEIVED;
	ETH_InitStruct.ETH_Receiver_BE = ETH_RECEIVER_BE_BIG_ENDIAN;
		
	ETH_InitStruct.ETH_Transmitter_BE = ETH_TRANSMITTER_BE_BIG_ENDIAN;
	ETH_InitStruct.ETH_Transmitter_Event_Mode = ETH_TRANSMITTER_EVENT_MODE_XFIFO_ALMOST_EMPTY;
	ETH_InitStruct.ETH_Automatic_Pad_Strip = ENABLE;
	ETH_InitStruct.ETH_Automatic_Preamble = ENABLE;
	ETH_InitStruct.ETH_Automatic_CRC_Strip = ENABLE;
	ETH_InitStruct.ETH_Automatic_IPG = ENABLE;
	ETH_InitStruct.ETH_Retry_Counter = 0xF;	
		
	ETH_Init(MDR_ETH0, (ETH_InitTypeDef *) &ETH_InitStruct);
	
	ETH_PHY_Reset(MDR_ETH0);
	ETH_PHYCmd(MDR_ETH0, ENABLE);

	ETH_Start(MDR_ETH0);

	tmp = TestETH();
	
	/*Main cycle*/									
	while (1);

}

uint32_t TestETH()
{
	int i;
	uint32_t var;

	  //fill tx buffer
    for(i=0;i<23;i++)
    {
      var = ETH_BUF+4;
      *((volatile unsigned int *)(var)) = SrcBuf[i];
    }
		
		while(ETH_GetFlagStatus(MDR_ETH0, ETH_MAC_IT_RF_OK) != SET){}
			
		//read rx data
    for(i=0;i<23;i++)
    {
      var = ETH_BUF;
      DstBuf[i] = *((volatile unsigned int *)(var));
    }		
}

//-----------------------------assert_param--------------------------------
#if (USE_ASSERT_INFO == 1)
void assert_failed(uint32_t file_id, uint32_t line)
{
  /* User can add his own implementation to report the source file ID and line number.
     Ex: printf("Wrong parameters value: file Id %d on line %d\r\n", file_id, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint32_t file_id, uint32_t line, const uint8_t* expr);
{
  /* User can add his own implementation to report the source file ID, line number and
     expression text.
     Ex: printf("Wrong parameters value (%s): file Id %d on line %d\r\n", expr, file_id, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_ASSERT_INFO */

/**
  * @}
  */ 	

/**
  * @}
  */ 	

/**
  * @}
  */ 	

/******************* (C) COPYRIGHT 2015 Milandr *********/

/* END OF FILE main.c */

