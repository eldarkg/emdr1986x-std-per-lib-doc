/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   This example shows how to configure SPI in master mode and transmit some data
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
#include "mdr32f8_uart.h"
#include "mdr32f8_ssp.h"

/** @addtogroup __MDR32F8_StdPeriph_Examples
  * @{
  */

/** @addtogroup __MDR32F8_Board_For_MCU
  * @{
  */

/** @addtogroup spi_mastermode
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
SSP_InitTypeDef SSP_InitStructure;

volatile uint32_t ch = 0;
volatile uint32_t c = 0;

uint32_t dat;
			
/* Private function prototypes -----------------------------------------------*/
void POR_disable();
void KEY_reg_accs();
void BlinkLine(uint32_t Pin);
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
	while(CLKCTRL_HSEstatus(CLKCTRL_HSEn_STAT_HSE0_RDY) != SUCCESS){}
	
	/* Select PLL0 clk src, PLL0_N, PLL0_Q to get FINT = FIN*(PLLn_N)/(PLLn_Q+1) */
	CLKCTRL_CPU_PLLconfig(PLL0, CLKCTRL_PLLn_CLK_SELECT_HSE0div1, 3, 7);//PLLn, SRC, Q, N
	while(CLKCTRL_CPU_PLLstatus(0) != SUCCESS){}
			
	CLKCTRL_MAX_CLKSelection(CLKCTRL_MAX_CLK_PLL0);	
	
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTB_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTC_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTD_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTE_EN, ENABLE);
	CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_SSP0_EN, ENABLE);

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
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_26 | PORT_Pin_25);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_6;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTD, &PORT_InitStructure);
	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_23);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_6;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTE, &PORT_InitStructure);
	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_24);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_6;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTB, &PORT_InitStructure);
	
	SSP_CLK_en(SSP_CLKSRC_MAX_CLK, SSP_CLKdiv1);
	
	SSP_StructInit (&SSP_InitStructure);

  SSP_InitStructure.SSP_SCR  = 0x10;
  SSP_InitStructure.SSP_CPSDVSR = 2;
  SSP_InitStructure.SSP_Mode = SSP_ModeMaster;
  SSP_InitStructure.SSP_WordLength = SSP_WordLength16b;
  SSP_InitStructure.SSP_SPH = SSP_SPH_1Edge;
  SSP_InitStructure.SSP_SPO = SSP_SPO_Low;
  SSP_InitStructure.SSP_FRF = SSP_FRF_SPI_Motorola;
  SSP_InitStructure.SSP_HardwareFlowControl = SSP_HardwareFlowControl_SSE;
  SSP_Init (MDR_SSP0,&SSP_InitStructure);

	/* Enable SSP0 */
  SSP_Cmd(MDR_SSP0, ENABLE);
	
	/*Main cycle*/									
	while (1)
	{
		while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
		SSP_SendData(MDR_SSP0, 0x35);
		
		dat = 0;
		Blinky(VD7);
		Blinky(VD8);

		while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
		dat = SSP_ReceiveData(MDR_SSP0);
		
		Blinky(VD7);
		Blinky(VD8);
	}
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

