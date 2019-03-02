/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   This example shows how to configure DAC module and output data using DMA in ping pong mode
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
#include "mdr32f8_tim.h"
#include "mdr32f8_dma.h"
#include "mdr32f8_dac.h"

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
#define CCR2_Val 5
#define DMA_SELECT(CHANNEL)	(0x00000001<<(CHANNEL))
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
TIMER_CntInitTypeDef sTIM_CntInit;
TIMER_ChnInitTypeDef sTIM_ChnInit;
TIMER_ChnOutInitTypeDef sTIM_ChnOutInit;
DMA_ChannelInitTypeDef DMA_InitStr;
DMA_CtrlDataInitTypeDef DMA_PriCtrlStr;
DMA_CtrlDataInitTypeDef DMA_AltCtrlStr;

volatile uint32_t ch = 0;
volatile uint32_t c = 0;

uint32_t dat;
uint32_t tmpDac0 = 0;

uint16_t SinDAC[32] = { 2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056,
												4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
												2047, 1647, 1263,  909,  599,  344,  155,   38,
												   0,   38,  155,  344,  599,  909, 1263, 1647}; 

uint16_t SinDAC2[32] = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
												4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
												4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
												4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095}; 													 
			
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void POR_disable();
void KEY_reg_accs();

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
	

	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTC_EN, ENABLE);
	CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_DAC0_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_DMA0_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_TMR1_EN, ENABLE);

	KEY_reg_accs();
	
	/*PORT configuration*/
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_25);
	PORT_InitStructure.PORT_CPULLUP  = PORT_CPULLUP_ON;
  PORT_InitStructure.PORT_CANALOG  = PORT_CANALOG_DIGITAL;
  PORT_Init(PORTC, &PORT_InitStructure);
		
	/* DMA Configuration */
  /* Reset all DMA settings */
  DMA_DeInit();
  DMA_StructInit(&DMA_InitStr);
  /* Set Primary Control Data */
  DMA_PriCtrlStr.DMA_SourceBaseAddr = (uint32_t)SinDAC;
  DMA_PriCtrlStr.DMA_DestBaseAddr = (uint32_t)(&(DAC0->DATA));
  DMA_PriCtrlStr.DMA_SourceIncSize = DMA_SourceIncHalfword;
  DMA_PriCtrlStr.DMA_DestIncSize = DMA_DestIncNo;
  DMA_PriCtrlStr.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_PriCtrlStr.DMA_Mode = DMA_Mode_PingPong;
  DMA_PriCtrlStr.DMA_CycleSize = 32;
  DMA_PriCtrlStr.DMA_NumContinuous = DMA_Transfers_1;
  DMA_PriCtrlStr.DMA_SourceProtCtrl = DMA_SourcePrivileged;
  DMA_PriCtrlStr.DMA_DestProtCtrl = DMA_DestPrivileged;
  /* Set Alternate Control Data */
  DMA_AltCtrlStr.DMA_SourceBaseAddr = (uint32_t)SinDAC2;
  DMA_AltCtrlStr.DMA_DestBaseAddr   = (uint32_t)(&(DAC0->DATA));
  DMA_AltCtrlStr.DMA_SourceIncSize = DMA_SourceIncHalfword;
  DMA_AltCtrlStr.DMA_DestIncSize = DMA_DestIncNo;
  DMA_AltCtrlStr.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_AltCtrlStr.DMA_Mode = DMA_Mode_PingPong;
  DMA_AltCtrlStr.DMA_CycleSize = 32;
  DMA_AltCtrlStr.DMA_NumContinuous = DMA_Transfers_1;
  DMA_AltCtrlStr.DMA_SourceProtCtrl = DMA_SourcePrivileged;
  DMA_AltCtrlStr.DMA_DestProtCtrl = DMA_DestPrivileged;
  /* Set Channel Structure */
  DMA_InitStr.DMA_PriCtrlData = &DMA_PriCtrlStr;
  DMA_InitStr.DMA_AltCtrlData = &DMA_AltCtrlStr;
  DMA_InitStr.DMA_Priority = DMA_Priority_Default;
  DMA_InitStr.DMA_UseBurst = DMA_BurstClear;
  DMA_InitStr.DMA_SelectDataStructure = DMA_CTRL_DATA_PRIMARY;

  /* Init DMA channel TIM1*/
  DMA_Init(DMA_Channel_TIM1, &DMA_InitStr);

  /* Enable dma_req or dma_sreq to generate DMA request */
  MDR_DMA->CHNL_REQ_MASK_CLR = DMA_SELECT(DMA_Channel_TIM1);
  MDR_DMA->CHNL_USEBURST_CLR = DMA_SELECT(DMA_Channel_TIM1);

  /* Enable DMA_Channel_TIM1 */
  DMA_Cmd(DMA_Channel_TIM1, ENABLE);	
		
	DAC_CLK_en(DAC0, DAC_CLKSRC_HSE0, 499);
	
	DAC0->KEY = _KEY_;
	
	DAC_Init(DAC0, 0x1, 0x3);
	DAC_Cmd(DAC0, ENABLE);	
			
        TIM_CLK_en(TIM1, TIM_CLKdiv1);		
        /* TIMER1 Configuration */
        /* Time base configuration */
        TIMER_DeInit(MDR_TMR1);
        TIM_CLK_en(TIM1, TIM_CLKdiv1);
  sTIM_CntInit.TIMER_Prescaler                = 0;
  sTIM_CntInit.TIMER_Period                   = 0xFF;
  sTIM_CntInit.TIMER_CounterMode              = TIMER_CntMode_ClkFixedDir;
  sTIM_CntInit.TIMER_CounterDirection         = TIMER_CntDir_Up;
  sTIM_CntInit.TIMER_EventSource              = TIMER_EvSrc_None;
  sTIM_CntInit.TIMER_FilterSampling           = TIMER_FDTS_TIMER_CLK_div_1;
  sTIM_CntInit.TIMER_ARR_UpdateMode           = TIMER_ARR_Update_Immediately;
  sTIM_CntInit.TIMER_ETR_FilterConf           = TIMER_Filter_1FF_at_TIMER_CLK;
  sTIM_CntInit.TIMER_ETR_Prescaler            = TIMER_ETR_Prescaler_None;
  sTIM_CntInit.TIMER_ETR_Polarity             = TIMER_ETRPolarity_NonInverted;
  sTIM_CntInit.TIMER_BRK_Polarity             = TIMER_BRKPolarity_NonInverted;
  TIMER_CntInit (MDR_TMR1,&sTIM_CntInit);

  /* Enable DMA for TIMER1 */
  TIMER_DMACmd(MDR_TMR1,(TIMER_STATUS_CNT_ARR), ENABLE);

  /* TIMER1 enable counter */
  TIMER_Cmd(MDR_TMR1,ENABLE);

	MDR_DMA->CHMUX0 = 9;//Set TMR1_REQ to DMA ch0
  /* Enable DMA IRQ */
  NVIC_EnableIRQ(DMA_IRQn);			
			
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

