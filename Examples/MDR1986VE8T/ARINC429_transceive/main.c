/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   This example shows how to configure ARINC channels, transmit and receive a simple array of data
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
#include "mdr32f8_arinc_rx.h"
#include "mdr32f8_arinc_tx.h"

/** @addtogroup __MDR32F8_StdPeriph_Examples
  * @{
  */

/** @addtogroup __MDR32F8_Board_For_MCU
  * @{
  */

/** @addtogroup ARINC429_transceive
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VD7 PORT_Pin_16
#define VD8 PORT_Pin_17
#define VD9 PORT_Pin_18
#define VD10 PORT_Pin_19
#define VD11 PORT_Pin_20
#define VD12 PORT_Pin_21
#define VD13 PORT_Pin_22
#define VD14 PORT_Pin_23
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
ARINC429R_InitChannelTypeDef ARINC429R_InitChannelStruct;
ARINC429T_InitChannelTypeDef ARINC429T_InitChannelStruct;

uint32_t i = 0;

uint32_t TestData[32] = { 0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
            						 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8 };
				
					 uint32_t ReadCh7[32];		
					 uint32_t ReadCh8[32];	
			
/* Private function prototypes -----------------------------------------------*/
void Blinky(uint32_t VD);
/* Private functions ---------------------------------------------------------*/
void Delay(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}												 

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
		
int main(void)
{	
	POR_disable();
	
	CLKCTRL_DeInit();
	/*HSE0 as the clk source*/
	CLKCTRL_HSEconfig(CLKCTRL_HSE0_CLK_ON);
	
	/* Check HSE success, enable PLL0, check PLL0 success, select MAX_CLK src */
	while(CLKCTRL_HSEstatus(CLKCTRL_HSEn_STAT_HSE0_RDY) != SUCCESS){}
		
	CLKCTRL_MAX_CLKSelection(CLKCTRL_MAX_CLK_HSE0div1);	

	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTB_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTC_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTE_EN, ENABLE);
	CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_ARCRX0_EN, ENABLE);
	CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_ARCTX0_EN, ENABLE);

	KEY_reg_accs();
	
	/* Configure PORTC LED pins [16:23] for output */
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_16|PORT_Pin_17|PORT_Pin_18|PORT_Pin_19|
																	 PORT_Pin_20|PORT_Pin_21|PORT_Pin_22|PORT_Pin_23);
	
  PORT_InitStructure.PORT_SOE    = PORT_SOE_OUT;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPD = PORT_SPD_OFF;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;

  PORT_Init(PORTC, &PORT_InitStructure);
	
	/* Configure PORTE pin 14 for output channel 4 (-)*/
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_14);//out
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_11;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTE, &PORT_InitStructure);	
	/* Configure PORTB pin 15 for output channel 4 (+)*/
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_15);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_11;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTB, &PORT_InitStructure);	
	/* Configure PORTE pins [12:13] for input channel 8 (-/+) */	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_12 | PORT_Pin_13);//in
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_11;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTE, &PORT_InitStructure);	
	/* Configure PORTE pins [10:11] for input channel 7 (-/+) */	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_10 | PORT_Pin_11);//in
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_11;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTE, &PORT_InitStructure);
		
	ARINC_CLK_en(ARC0_CLKdiv1);

	/* ARINC429R channel 7,8 configuration */
	ARINC429R_InitChannelStruct.ARINC429R_CLK = ARINC429R_CLK_100_KHz;
	ARINC429R_InitChannelStruct.ARINC429R_SD = DISABLE;
	ARINC429R_InitChannelStruct.ARINC429R_LB = DISABLE;
	ARINC429R_InitChannelStruct.ARINC429R_SDI1 = RESET;
	ARINC429R_InitChannelStruct.ARINC429R_SDI2 = RESET;
	ARINC429R_InitChannelStruct.ARINC429R_DIV = 5;
	
	ARINC429R_ChannelInit(ARINC429R_CHANNEL7, &ARINC429R_InitChannelStruct);
	ARINC429R_ChannelCmd(ARINC429R_CHANNEL7, ENABLE);
	
	ARINC429R_ChannelInit(ARINC429R_CHANNEL8, &ARINC429R_InitChannelStruct);
	ARINC429R_ChannelCmd(ARINC429R_CHANNEL8, ENABLE);
	
	ARINC429R_Parity_en(ARINC429R_CHANNEL7, ARINC429R_ODD_ADD_TO_PARITY, DISABLE);
	ARINC429R_Parity_en(ARINC429R_CHANNEL8, ARINC429R_ODD_ADD_TO_PARITY, DISABLE);
	
	/* ARINC429T channel 4 configuration */
	ARINC429T_InitChannelStruct.ARINC429T_CLK = ARINC429T_CLK_100_KHz;
	ARINC429T_InitChannelStruct.ARINC429T_DIV = 5;
	ARINC429T_InitChannelStruct.ARINC429T_EN_PAR = DISABLE;
	ARINC429T_InitChannelStruct.ARINC429T_ODD = ARINC429T_ODD_ADD_TO_PARITY;
	
	ARINC429T_ChannelInit(ARINC429T_CHANNEL4, &ARINC429T_InitChannelStruct);
	ARINC429T_ChannelCmd(ARINC429T_CHANNEL4, ENABLE);
		
	for(i = 0; i < 32; i++){

	ARINC429T_SendData(ARINC429T_CHANNEL4, TestData[i]);
		
	Blinky(VD8);
		
			ARINC429R_SetChannel(ARINC429R_CHANNEL7);
						ReadCh7[i]=ARINC429R_ReceiveData();
		
			ARINC429R_SetChannel(ARINC429R_CHANNEL8);
						ReadCh8[i]=ARINC429R_ReceiveData();
		
	}
	/*Main cycle*/									
	while (1);
}

void Blinky(uint32_t VD)
{	
		PORT_SetBits(PORTC, VD);
		Delay(10000);
		PORT_ResetBits(PORTC, VD);
		Delay(10000);
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

