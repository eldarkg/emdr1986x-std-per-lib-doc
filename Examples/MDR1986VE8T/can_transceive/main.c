/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   This example shows how to configure CAN bus, transmit and receive bytes of data
  ******************************************************************************
  * <br><br>
  *
  *
  * <h2><center>&copy; COPYRIGHT 2015 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR1986VE8T.h"
#include "spec.h"

#include "mdr32f8_config.h"
#include "mdr32f8_port.h"
#include "mdr32f8_clkctrl.h"
#include "mdr32f8_can.h"

/** @addtogroup __MDR32F8_StdPeriph_Examples
  * @{
  */

/** @addtogroup __MDR32F8_Board_For_MCU
  * @{
  */

/** @addtogroup can_transceive
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
CAN_InitTypeDef  CAN_InitStructure;
CAN_TxMsgTypeDef TxMsg;
CAN_RxMsgTypeDef RxMsg;
CAN_DataTypeDef RecBuf;

uint32_t dat;
uint32_t mes_l;
uint32_t mes_h;
			
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
		
int main(void)
{	
	POR_disable();
	
	CLKCTRL_DeInit();
	/* Enable HSE0 clock */
	CLKCTRL_HSEconfig(CLKCTRL_HSE0_CLK_ON);
	
	/* Check HSE success, enable PLL0, check PLL0 success, select MAX_CLK src */
	while(CLKCTRL_HSEstatus(CLKCTRL_HSEn_STAT_HSE0_RDY) != SUCCESS){}
		
	CLKCTRL_MAX_CLKSelection(CLKCTRL_MAX_CLK_HSE0div1);	

	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTB_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTC_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTD_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTE_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_CAN0_EN, ENABLE);

	KEY_reg_accs();
	
  /* Configure PORTC LED pins [16:23] for output */
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_16|PORT_Pin_17|PORT_Pin_18|PORT_Pin_19|
																	 PORT_Pin_20|PORT_Pin_21|PORT_Pin_22|PORT_Pin_23);
	
  PORT_InitStructure.PORT_SOE    = PORT_SOE_OUT;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPD = PORT_SPD_OFF;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;

  PORT_Init(PORTC, &PORT_InitStructure);
	
	/*PORT configuration*/
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_22 | PORT_Pin_21);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_4;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTD, &PORT_InitStructure);
	
	CAN_CLK_en(CAN_CLKdiv1);

	/* CAN register init */
  CAN_DeInit(MDR_CAN0);

  /* CAN cell init */
  CAN_StructInit (&CAN_InitStructure);

  CAN_InitStructure.CAN_ROP  = ENABLE;
  CAN_InitStructure.CAN_SAP  = ENABLE;
  CAN_InitStructure.CAN_STM  = DISABLE;
  CAN_InitStructure.CAN_ROM  = DISABLE;
  CAN_InitStructure.CAN_PSEG = CAN_PSEG_Mul_2TQ;
  CAN_InitStructure.CAN_SEG1 = CAN_SEG1_Mul_5TQ;
  CAN_InitStructure.CAN_SEG2 = CAN_SEG2_Mul_5TQ;
  CAN_InitStructure.CAN_SJW  = CAN_SJW_Mul_4TQ;
  CAN_InitStructure.CAN_SB   = CAN_SB_3_SAMPLE;
  CAN_InitStructure.CAN_BRP  = 0;//4
	
	CAN_Init(MDR_CAN0,&CAN_InitStructure);

  /* transmit */
  TxMsg.IDE     = CAN_ID_EXT;
  TxMsg.DLC     = 0x08;
  TxMsg.PRIOR_0 = DISABLE;
  TxMsg.ID      = 0x12345678;
  TxMsg.Data[1] = 0x01234567;
  TxMsg.Data[0] = 0x89ABCDEF;
	
  CAN_Cmd(MDR_CAN0, ENABLE);
	
  CAN_Receive(MDR_CAN0, 0, DISABLE);
	CAN_Transmit(MDR_CAN0, 1, &TxMsg);
	
	while((CAN_GetBufferStatus(MDR_CAN0, 0)&CAN_BUF_CON_RX_FULL) == RESET);
	CAN_GetReceivedData(MDR_CAN0, 0, RecBuf);
	mes_l = RecBuf[0];		
	mes_h = RecBuf[1];
	
	/*Main cycle*/									
	while (1);
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

