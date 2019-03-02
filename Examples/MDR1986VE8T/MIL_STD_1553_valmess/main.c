/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   This example shows how to configure two MIL_STD controllers in Bus Controller mode and Device mode, transmit some data and set VALMESS or ERROR status led
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
#include "mdr32f8_milstd.h"

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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
MIL_STD_1553_InitTypeDef MIL_STD_15531_InitStructure;
MIL_STD_1553_InitTypeDef MIL_STD_15532_InitStructure;

MIL_STD_1553_CommandWordTypeDef MIL_STD_1553_CommandWord1 = {0};
		
/* Private function prototypes -----------------------------------------------*/
void POR_disable();
void KEY_reg_accs();
void StartTRM();	
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
	/* Enable HSE0 clock */
	CLKCTRL_HSEconfig(CLKCTRL_HSE0_CLK_ON);
	
	/* Check HSE success, enable PLL0, check PLL0 success, select MAX_CLK src */
	while(CLKCTRL_HSEstatus(CLKCTRL_HSEn_STAT_HSE0_RDY) != SUCCESS){}
		
	/* Select PLL0 clk src, PLL0_N, PLL0_Q to get FINT = FIN*(PLLn_N)/(PLLn_Q+1) */
	CLKCTRL_CPU_PLLconfig(PLL0, CLKCTRL_PLLn_CLK_SELECT_HSE0div1, 0, 8);//PLLn, SRC, Q, N
		while(CLKCTRL_CPU_PLLstatus(0) != SUCCESS){}
			CLKCTRL_MAX_CLKSelection(CLKCTRL_MAX_CLK_PLL0);	

	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTA_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTB_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTC_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTD_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTE_EN, ENABLE);
	CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_MIL0_EN, ENABLE);
	CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_MIL1_EN, ENABLE);

	KEY_reg_accs();
	

  /* Configure PORTC LED pins [16:23] for output */
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_16|PORT_Pin_17|PORT_Pin_18|PORT_Pin_19|
																	 PORT_Pin_20|PORT_Pin_21|PORT_Pin_22|PORT_Pin_23);
	
  PORT_InitStructure.PORT_SOE    = PORT_SOE_OUT;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPD = PORT_SPD_OFF;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;

  PORT_Init(PORTC, &PORT_InitStructure);
	
	/* PORT configuration */
	/*****************************XP23*****************************/
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_28 | PORT_Pin_29 | PORT_Pin_30);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_13;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTB, &PORT_InitStructure);	
		
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3 | PORT_Pin_4 | PORT_Pin_5);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_13;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTC, &PORT_InitStructure);	
		
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_27 | PORT_Pin_28);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_13;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTD, &PORT_InitStructure);	
	/*****************************XP22*****************************/
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_1 | PORT_Pin_2);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_13;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTB, &PORT_InitStructure);
	
	PORT_InitStructure.PORT_Pin = (PORT_Pin_25 | PORT_Pin_26 | PORT_Pin_27 | PORT_Pin_28 | PORT_Pin_29);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_12;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTA, &PORT_InitStructure);
	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_22);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_12;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTA, &PORT_InitStructure);
	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_30 | PORT_Pin_31);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_13;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTA, &PORT_InitStructure);
		
  MIL_STD_1553_CLK_en(MIL_STD_15531, MIL_STD_1553_CLKdiv1);
	MIL_STD_1553_CLK_en(MIL_STD_15532, MIL_STD_1553_CLKdiv1);
	
	/*-- Bus Controller configuration -----------------------------------------*/
	MIL_STD_1553xStructInit(&MIL_STD_15531_InitStructure);
	
	/* Initialize MIL_STD_1553_InitStructure */
	MIL_STD_15531_InitStructure.MIL_STD_1553_Mode = MIL_STD_1553_ModeBusController;
	MIL_STD_15531_InitStructure.MIL_STD_1553_RERR = ENABLE;
	MIL_STD_15531_InitStructure.MIL_STD_1553_DIV = 80;
	MIL_STD_15531_InitStructure.MIL_STD_1553_RTA = 0;
	MIL_STD_15531_InitStructure.MIL_STD_1553_TRA = ENABLE;
	MIL_STD_15531_InitStructure.MIL_STD_1553_TRB = DISABLE;
	/* Configure MIL_STD_15531 parameters */
	MIL_STD_1553_Init(MIL_STD_15532, &MIL_STD_15531_InitStructure);

	/*-- Terminal device configuration -----------------------------------------*/
	MIL_STD_1553xStructInit(&MIL_STD_15532_InitStructure);
	
	/* Initialize MIL_STD_1553_InitStructure */
	MIL_STD_15532_InitStructure.MIL_STD_1553_Mode = MIL_STD_1553_ModeTerminal;
	MIL_STD_15532_InitStructure.MIL_STD_1553_RERR = DISABLE;
	MIL_STD_15532_InitStructure.MIL_STD_1553_DIV = 80;
	MIL_STD_15532_InitStructure.MIL_STD_1553_RTA = 0x01;
	MIL_STD_15532_InitStructure.MIL_STD_1553_TRA = ENABLE;
	MIL_STD_15532_InitStructure.MIL_STD_1553_TRB = ENABLE;
	/* Configure MIL_STD_15532 parameters */
	MIL_STD_1553_Init(MIL_STD_15531, &MIL_STD_15532_InitStructure);
	
	/* Enables MIL_STD_15532 peripheral */
	MIL_STD_1553_Cmd(MIL_STD_15532, ENABLE);
	/* Enables MIL_STD_15531 peripheral */
	MIL_STD_1553_Cmd(MIL_STD_15531, ENABLE);
	
	/*Main cycle*/									
	while(1)
	{
		
		StartTRM();
			
		while((MIL_STD_1553_BCSTART_bit_stat(MIL_STD_15532)) != RESET){}
			
		if(MIL_STD_1553_GetFlagStatus(MIL_STD_15532, MIL_STD_1553_FLAG_VALMESS) != RESET)	//Valmess flag set
		{
			PORT_SetBits(PORTC, PORT_Pin_16);
			PORT_ResetBits(PORTC, PORT_Pin_23);
		}
		if(MIL_STD_1553_GetFlagStatus(MIL_STD_15532, MIL_STD_1553_FLAG_ERR) != RESET)	//Error flag set
		{
			PORT_SetBits(PORTC, PORT_Pin_23);
			PORT_ResetBits(PORTC, PORT_Pin_16);
		}
				
		Delay(100000);
		
	}
}

void StartTRM()
{
	MIL_STD_1553_CommandWord1.Fields.Data = 5;
	MIL_STD_1553_CommandWord1.Fields.Subaddress = 1;
	MIL_STD_1553_CommandWord1.Fields.ReadWriteBit = 0;
	MIL_STD_1553_CommandWord1.Fields.TerminalDeviceAddress = 1;
	
	MIL_STD_1553_SetCommandWord(MIL_STD_15532, MIL_STD_1553_COMMAND_WORD1, &MIL_STD_1553_CommandWord1);

	MIL_STD_1553_Transmitter_CMD(MIL_STD_15532, MIL_STD_1553_TRANSMITTER_MAIN, ENABLE);
	MIL_STD_1553_StartTransmision(MIL_STD_15532);
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

