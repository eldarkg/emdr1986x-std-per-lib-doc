/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   This example shows how to configure External Bus and test it writing some data to external RAM, reading and verifying it
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR1986VE8T.h"
#include "spec.h"

#include "mdr32f8_config.h"
#include "mdr32f8_port.h"
#include "mdr32f8_clkctrl.h"
#include "mdr32f8_ebc.h"

/** @addtogroup __MDR32F8_StdPeriph_Examples
  * @{
  */

/** @addtogroup __MDR32F8_Board_For_MCU
  * @{
  */

/** @addtogroup led_button_exti
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
EBC_RGN_InitTypeDef EBC_RGNx_IS;
/* Private define ------------------------------------------------------------*/
//1645RU5U has 512 Kb
#define	NUM_BYTES	0x80000

#define NUM_WORDS	(NUM_BYTES >> 2)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t res;

uint32_t* addr;
uint32_t tmpRam, tmp1Ram, hasErrorRam;
uint32_t hr;

/* Private function prototypes -----------------------------------------------*/
void ExtBus_PortsConfig(void);
void TestRam_EbcConfig(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	uint32_t tmp;
	
	/* Disable Power-on-Reset control. Hold the RESET button down until operation complete */
	POR_disable();
	
	/* Set CLKCTRL to default */
	CLKCTRL_DeInit();
	
	/* Enable HSE0 clock */
	CLKCTRL_HSEconfig(CLKCTRL_HSE0_CLK_ON);
	
	/* Check HSE success, enable PLL0, check PLL0 success, select MAX_CLK src */
	while(CLKCTRL_HSEstatus(CLKCTRL_HSEn_STAT_HSE0_RDY) != SUCCESS){}
	
	CLKCTRL_MAX_CLKSelection(CLKCTRL_MAX_CLK_HSE0div1);	

	ExtBus_PortsConfig();
	TestRam_EbcConfig();

	addr = (uint32_t*)0x10000000;
	tmpRam = 0;		
			
	while(tmpRam < NUM_WORDS)
	{
		*addr++ = tmpRam++;
	}		
	
	addr = (uint32_t*)0x10000000;
	tmpRam = 0;
	tmp1Ram = 0;
	hasErrorRam = 0;
	
	while(tmpRam < NUM_WORDS)
	{
		tmp1Ram = *addr++;
		if(tmp1Ram != tmpRam) hasErrorRam++;
		tmpRam++;
	}	
	
	while(1);
	
}

void ExtBus_PortsConfig(void)
{
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTC_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTD_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTE_EN, ENABLE);

	/* Allow write to PORT regs */
	KEY_reg_accs();
	
	/*---------- Config DATA Bus of EBC ----------*/
	/*---------- PortD[31:30] - DATA[1:0]; PortE[5:0] - DATA[7:2] ----------*/	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_30|PORT_Pin_31);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_2;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTD, &PORT_InitStructure);
		
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_0|PORT_Pin_1|PORT_Pin_2|PORT_Pin_3|PORT_Pin_4|PORT_Pin_5);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_2;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTE, &PORT_InitStructure);

	/*---------- Config ADDRESS Bus of EBC ----------*/
	/*---------- PortC[31:30] - Address[1:0], PortD[16:0] - Address[18:2] ----------*/
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_30|PORT_Pin_31);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_2;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTC, &PORT_InitStructure);

	PORT_InitStructure.PORT_Pin   = ( PORT_Pin_0|PORT_Pin_1|PORT_Pin_2|PORT_Pin_3|
																		PORT_Pin_4|PORT_Pin_5|PORT_Pin_6|PORT_Pin_7|
																		PORT_Pin_8|PORT_Pin_9|PORT_Pin_10|PORT_Pin_11|
																		PORT_Pin_12|PORT_Pin_13|PORT_Pin_14|PORT_Pin_15|PORT_Pin_16 );
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_2;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTD, &PORT_InitStructure);
	
	/*---------- Config nOE, nWE, nCS signals of EBC ----------*/
	/*---------- PortD[23] - nOE, PortD[24] - nWE, PortD[19] - nCS0 ----------*/	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_19|PORT_Pin_23|PORT_Pin_24);
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_2;
  PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
  PORT_Init(PORTD, &PORT_InitStructure);
}

void TestRam_EbcConfig(void)
{
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_EBC_EN, ENABLE);	
	EXT_BUS_CNTR->KEY = _KEY_;
	
	EBC_RGNx_IS.RGN_DIVOCLK = 3;
	EBC_RGNx_IS.RGN_WS_HOLD = 3;
	EBC_RGNx_IS.RGN_WS_SETUP  = 3;
	EBC_RGNx_IS.RGN_WS_ACTIVE = 3;
	EBC_RGNx_IS.RGN_MODE = EBC_MODE_8X;
	EBC_RGNx_IS.RGN_ECCEN = DISABLE;
	
	EBC_RGNx_Init(RGN0, &EBC_RGNx_IS);
	EBC_RGNx_Cmd(RGN0, ENABLE);
	
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

