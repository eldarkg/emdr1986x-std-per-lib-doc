/**
  ******************************************************************************
  * @file    Examples/MDR1986VE1T/DMA/TIMER_RAM/main.c
  * @author  Milandr Application Team
  * @version V1.2.0
  * @date    04/07/2013
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
  * <h2><center>&copy; COPYRIGHT 2013 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_dma.h"
#include "MDR32F9Qx_port.h"

/** @addtogroup __MDR32F9Qx_StdPeriph_Examples MDR32F9Qx StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR1986VE1T_EVAL MDR1986VE1T Opora Evaluation Board
  * @{
  */

/** @addtogroup DMA_TIMER_RAM_1T DMA_TIMER_RAM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize         32

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA_ChannelInitTypeDef DMA_InitStr;
DMA_CtrlDataInitTypeDef DMA_PriCtrlStr;
TIMER_CntInitTypeDef sTIM_CntInit;
TIMER_ChnInitTypeDef sTIM_ChnInit;
TIMER_ChnOutInitTypeDef sTIM_ChnOutInit;
PORT_InitTypeDef PORT_InitStructure;

uint16_t SrcBuf[BufferSize] IAR_SECTION ("EXECUTABLE_MEMORY_SECTION") __attribute__((section("EXECUTABLE_MEMORY_SECTION")));
uint16_t DstBuf[BufferSize] IAR_SECTION ("EXECUTABLE_MEMORY_SECTION") __attribute__((section("EXECUTABLE_MEMORY_SECTION")));

volatile TestStatus TransferStatus = FAILED;

extern DMA_CtrlDataTypeDef DMA_ControlTable[DMA_Channels_Number * (1 + DMA_AlternateData)];

/* Private function prototypes -----------------------------------------------*/
void Init_RAM (uint16_t *addr, uint32_t size);
uint32_t ps_rand(uint32_t addr);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
#ifdef __CC_ARM
int main(void)
#else
void main(void)
#endif
{
	uint32_t i;
	volatile uint32_t dma_status;

	RST_CLK_DeInit();
	/* Enable peripheral clocks --------------------------------------------------*/
	RST_CLK_PCLKcmd((RST_CLK_PCLK_RST_CLK | RST_CLK_PCLK_TIMER1 | RST_CLK_PCLK_DMA),ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTD, ENABLE);

	/* Disable all DMA request */
	MDR_DMA ->CHNL_REQ_MASK_CLR = 0xFFFFFFFF;
	MDR_DMA ->CHNL_USEBURST_CLR = 0xFFFFFFFF;

	RST_CLK_HSEconfig(RST_CLK_HSE_ON);
	while((RST_CLK_GetFlagStatus(RST_CLK_FLAG_HSERDY)) != SET );

	RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul16);
	RST_CLK_CPU_PLLcmd(ENABLE);
	while(RST_CLK_GetFlagStatus(RST_CLK_FLAG_PLLCPURDY) != SET);

	RST_CLK_CPU_PLLuse(ENABLE);

	RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

	/* Reset PORTE settings */
	PORT_DeInit(MDR_PORTD );

	/* Configure PORTD pins 7 */
	PORT_InitStructure.PORT_Pin = PORT_Pin_7;
	PORT_InitStructure.PORT_OE = PORT_OE_OUT;
	PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;
	PORT_Init(MDR_PORTD, &PORT_InitStructure);

	/* Init RAM */
	Init_RAM(SrcBuf, BufferSize);


	/* Reset all TIMER1 settings */
	TIMER_DeInit(MDR_TIMER1 );

	TIMER_BRGInit(MDR_TIMER1, TIMER_HCLKdiv1 );

	/* TIM1 configuration ------------------------------------------------*/
	/* Initializes the TIMERx Counter ------------------------------------*/
	sTIM_CntInit.TIMER_Prescaler = 0x0;
	sTIM_CntInit.TIMER_Period = 0x1F;
	sTIM_CntInit.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir;
	sTIM_CntInit.TIMER_CounterDirection = TIMER_CntDir_Up;
	sTIM_CntInit.TIMER_EventSource = TIMER_EvSrc_None;
	sTIM_CntInit.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1;
	sTIM_CntInit.TIMER_ARR_UpdateMode = TIMER_ARR_Update_Immediately;
	sTIM_CntInit.TIMER_ETR_FilterConf = TIMER_Filter_1FF_at_TIMER_CLK;
	sTIM_CntInit.TIMER_ETR_Prescaler = TIMER_ETR_Prescaler_None;
	sTIM_CntInit.TIMER_ETR_Polarity = TIMER_ETRPolarity_NonInverted;
	sTIM_CntInit.TIMER_BRK_Polarity = TIMER_BRKPolarity_NonInverted;
	TIMER_CntInit(MDR_TIMER1, &sTIM_CntInit);

	/* Initializes the TIMER1 Channel1 -------------------------------------*/
	TIMER_ChnStructInit(&sTIM_ChnInit);


	/* Enable TIMER1 DMA request */
	TIMER_DMACmd(MDR_TIMER1, TIMER_STATUS_CNT_ARR, TIMER_DMA_Channel0, ENABLE);

	/* Reset all DMA settings */
	DMA_DeInit();
	DMA_StructInit(&DMA_InitStr);

	/* DMA_Channel_TIM1 configuration ---------------------------------*/
	/* Set Primary Control Data */
	DMA_PriCtrlStr.DMA_SourceBaseAddr = (uint32_t) SrcBuf;
	DMA_PriCtrlStr.DMA_DestBaseAddr = (uint32_t) &MDR_PORTD->RXTX;
	DMA_PriCtrlStr.DMA_SourceIncSize = DMA_SourceIncHalfword;
	DMA_PriCtrlStr.DMA_DestIncSize = DMA_DestIncNo;
	DMA_PriCtrlStr.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_PriCtrlStr.DMA_Mode = DMA_Mode_Basic;
	DMA_PriCtrlStr.DMA_CycleSize = BufferSize;
	DMA_PriCtrlStr.DMA_NumContinuous = DMA_Transfers_1;
	DMA_PriCtrlStr.DMA_SourceProtCtrl = DMA_SourcePrivileged;
	DMA_PriCtrlStr.DMA_DestProtCtrl = DMA_DestPrivileged;
	/* Set Channel Structure */
	DMA_InitStr.DMA_PriCtrlData = &DMA_PriCtrlStr;
	DMA_InitStr.DMA_Priority = DMA_Priority_High;
	DMA_InitStr.DMA_UseBurst = DMA_BurstClear;
	DMA_InitStr.DMA_SelectDataStructure = DMA_CTRL_DATA_PRIMARY;
	/* Init DMA channel */
	DMA_Init(DMA_Channel_REQ_TIM1, &DMA_InitStr);

	/* Enable TIMER1 */
	TIMER_Cmd(MDR_TIMER1, ENABLE);

	/* Transfer complete */
	while (1) {
		dma_status = (DMA_ControlTable[DMA_Channel_REQ_TIM1 ].DMA_Control & 0x07);
		if (dma_status == 0)
			break;
	}

	while (1);
}

/**
  * @brief  Initialisation memory pseudo random value
  * @param  addr - memory address
  * @param  size - memory size
  * @retval None
  */
void Init_RAM (uint16_t *addr, uint32_t size)
{
uint32_t i;

  for (i = 0; i < size; i++)
  {
    *addr++ = (i&1) << 7;
  }
}

/**
  * @brief  Generation random values
  * @param  addr:
  * @retval random value
  */
uint32_t ps_rand(uint32_t addr)
{
uint32_t hash = 0;
uint32_t i;
char *key = (char *)&addr;

  for (i = 0; i < 4; i++)
  {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  for (i = 0; i < 256; i++)
  {
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
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

/** @} */ /* End of group DMA_TIMER_RAM_1T */

/** @} */ /* End of group __MDR1986VE1T */

/** @} */ /* End of group __MDR32F9Qx_StdPeriph_Examples */


/******************* (C) COPYRIGHT 2013 Milandr *********/

/* END OF FILE main.c */
