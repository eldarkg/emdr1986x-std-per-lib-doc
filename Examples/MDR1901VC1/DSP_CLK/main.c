/**
  ******************************************************************************
  * @file    Examples/MDR32F9Q3_EVAL/RST_CLK/CPU_Clock/main.c
  * @author  Milandr Application Team
  * @version V1.2.0
  * @date    04/07/2011
  * @brief   Main program body.
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
  * <h2><center>&copy; COPYRIGHT 2011 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_dsp.h"

/** @addtogroup __MDR32F9Qx_StdPeriph_Examples MDR32F9Qx StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR32F9Q3_EVAL MDR32F9Q3 Evaluation Board
  * @{
  */

/** @addtogroup RST_CLK_CPU_Clock_93 RST_CLK_CPU_Clock
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BLINK_NUM 5
#define BLINK_DELAY 20000

#define LED1            PORT_Pin_11
#define LED2            PORT_Pin_12
#define LED_PORT		MDR_PORTB
#define LED_PORT_PCLK	RST_CLK_PCLK_PORTB

/* Private macro -------------------------------------------------------------*/
#define DSP_MEMORY 		0x30020000
/* Private variables ---------------------------------------------------------*/
RST_CLK_FreqTypeDef CLK_FrequencyStructure;
PORT_InitTypeDef PORT_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);
void ConfigureDSPClock(void);

/* Private functions ---------------------------------------------------------*/

void LEDOn(uint32_t LED_Num)
{
  PORT_SetBits(LED_PORT, LED_Num);
}

void LEDOff(uint32_t LED_Num)
{
  PORT_ResetBits(LED_PORT, LED_Num);
}

/**
  * @brief  Blink with LED1
  * @param  num - blinks number
  * @param  del - delay
  * @retval None
  */
void BlinkLED1(uint32_t num, uint32_t del)
{
uint32_t cnt;

  for ( cnt = 0; cnt < num; cnt++)
  {
    LEDOn(LED1);
    Delay(del);
    LEDOff(LED1);
    Delay(del);
  }
}

/**
  * @brief  Indicate error condition with LED3
  * @param  None
  * @retval None
  */
void IndicateError(void)
{
  /* Switch LED3 on and off in case of error */
  LEDOn(LED2);
  Delay(BLINK_DELAY);
  LEDOff(LED2);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
#ifdef __CC_ARM
int main(void)
#else
void main(void)
#endif
{
  RST_CLK_PCLKcmd(LED_PORT_PCLK, ENABLE);

  /* Configure all unused PORT pins to low power consumption */

  PORT_StructInit(&PORT_InitStructure);
  PORT_InitStructure.PORT_Pin = (PORT_Pin_All & ~(LED1 | LED2));
  PORT_Init(MDR_PORTB, &PORT_InitStructure);


  /* Configure PORTF pins 0,1 for output to switch LEDs on/off */

  PORT_InitStructure.PORT_Pin   = (LED1 | LED2);
  PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;

  PORT_Init(LED_PORT, &PORT_InitStructure);

  /* Consequently turn all three used LEDs on and off */

  LEDOn(LED1);
  Delay(4*BLINK_DELAY);
  LEDOff(LED1);
  Delay(4*BLINK_DELAY);
  LEDOn(LED2);
  Delay(4*BLINK_DELAY);
  LEDOff(LED2);
  Delay(4*BLINK_DELAY);

  /* Configure the DSP CPU clock */
  ConfigureDSPClock();

  /* Reset the DSP settings to its default values */
  DSP_DeInit();

  /* Disable reset at the DSP block */
  DSP_RST_Cmd(ENABLE);

  /* Disable reset at  the DSP memory  */
  DSP_MEM_Cmd(ENABLE);

  /* Disable reset at the DSP  peripheral */
  DSP_PER_Cmd(ENABLE);

  /* Enable clock for the DSP CPU, data memory and program data */
  DSP_CLKPCLKCmd((DSP_CLK_PCLK_CDM | DSP_CLK_PCLK_CPM | DSP_CLK_PCLK_CPU), ENABLE);

  /* Inifinity loop */
  while(1);

}


/**
 * @brief Configure the DSP CPU clock
 */
void ConfigureDSPClock(void)
{
	  /* Enable the HSE */
	  RST_CLK_HSEconfig(RST_CLK_HSE_ON);

	  /* Get HSE status */
	  if(RST_CLK_HSEstatus() == ERROR){
		  IndicateError();
		  while(1);
	  }

	  /* Set the CPU Prescaler */
	  RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);

	  /* Select the CPU clock source */
	  RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

	  /* Select the DSP CPU C1 clock source */
	  RST_CLK_DSP_Selection(DSP_C1_CLOCK_HSE);

	  /* Config the DSP PLL */
	  RST_CLK_DSPPLLConfig(DSP_PLL_MUL10);

	  /* Enable the DSP PLL */
	  RST_CLK_DSPPLL_CMD(ENABLE);

	  /* Get status the DSP PLL */
	  if(RST_CLK_DSP_PLLStatus() == ERROR){
		  IndicateError();
		  while(1);
	  }
	  /* Select the clock source for the DSP CPU from PLL */
	  RST_CLK_DSP_PLLUse(ENABLE);

	  /* Set the DSP CPU prescaler at one */
	  RST_CLK_DSPPrescaler(DSP_PRESCALER1);

	  /* Enalbe the DSP CPU clock */

	  RST_CLK_DSPCmd(ENABLE);
}

/**
  * @brief  Executes delay loop.
  * @param  nCount: specifies the delay.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}

/**
  * @brief  Reports the source file ID, the source line number
  *         and expression text (if USE_ASSERT_INFO == 2) where
  *         the assert_param error has occurred.
  * @param  file_id: pointer to the source file name
  * @param  line: assert_param error line source number
  * @param  expr:
  * @retval None
  */
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

/** @} */ /* End of group RST_CLK_CPU_Clock_93 */

/** @} */ /* End of group __MDR32F9Q3_EVAL */

/** @} */ /* End of group __MDR32F9Qx_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2011 Milandr *********/

/* END OF FILE main.c */


