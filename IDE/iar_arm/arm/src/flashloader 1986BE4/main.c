/**************************************************
 * Copyright 2004-2010 IAR Systems. All rights reserved.
 *
 * $Revision: 29 $
 **************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MDR1986BE4.h>
#include "flash_loader.h"
//#include "boot_loader.h"

#define STANDBY 0x30L
#define READ    0x00L
#define PROGRAM 0x21L
#define PGERASE 0x22L
#define READCDI 0x0BL
#define READDID 0x0CL

#define flash_unit uint32_t
#define BOOT_SIZE	341

const unsigned int BOOT_LOADER[BOOT_SIZE] =
{
0x20000408,
0x00000499,
0x0000049D,
0x0000049F,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x000004A1,
0x00000000,
0x00000000,
0x000004A3,
0x000004A5,
0x000004A7,
0x000004A9,
0x000004AB,
0x000004AD,
0x000004AF,
0x000004B1,
0x000004B3,
0x000004B5,
0x000004B7,
0x000004B9,
0x000004BB,
0x000004BD,
0x000004BF,
0x000004C1,
0x000004C3,
0x000004C5,
0x000004C7,
0x46854803,
0xFA36F000,
0x47004800,
0x000001EB,
0x20000408,
0x06092120,
0x30016808,
0xBD019001,
0x00004770,
0x4603B510,
0x4CEB00D8,
0x4CEB1902,
0x61E06810,
0x62A001E0,
0x20006851,
0x20056048,
0x20036088,
0x200060C8,
0x61486108,
0x61882002,
0x60482001,
0xBD1048E2,
0x6982BF00,
0x421A2380,
0x2200D0FB,
0x098A6302,
0x068A6242,
0x62820E92,
0x62C22260,
0x63024ADB,
0xE0034770,
0x421A2340,
0x6803D000,
0x23206982,
0xD1F7421A,
0x47706001,
0xBF004601,
0x22106988,
0xD1FB4210,
0x05006808,
0x47700D00,
0x22004601,
0xBF00E00B,
0x23106988,
0xD1FB4218,
0x05006808,
0xD0010D00,
0x47702001,
0x2A101C52,
0x2000D3F1,
0xB510E7F9,
0x21004602,
0xBF00E011,
0x24106990,
0xD1FB4220,
0x05006810,
0x43010D00,
0x060C0A18,
0x46034320,
0x02090A09,
0x03002001,
0x20011809,
0x42810380,
0x200FD3E9,
0x42010200,
0x2000D002,
0xBD1043C0,
0xE7FC4618,
0x4602B530,
0x2B00460B,
0x2401D002,
0xE0000264,
0x25012400,
0x43294621,
0x4DB04CAF,
0x462C612C,
0x60626021,
0x430C2440,
0x358125FF,
0x4DAB432C,
0x462C602C,
0x240068E0,
0x612C602C,
0x2200BD30,
0x60826042,
0x60C2880A,
0x6102884A,
0x61422200,
0x6182684A,
0xB0864770,
0x499C48A1,
0x48A161C8,
0x90046B80,
0x48A149A0,
0xFFE9F7FF,
0x48A149A0,
0xFFE5F7FF,
0xE0022700,
0x68002000,
0x2F641C7F,
0x2000D3FA,
0x489B9005,
0x07C06800,
0x90050F80,
0x68004896,
0x0FC007C0,
0x43089905,
0x20009005,
0x61084992,
0x618860C8,
0x61084992,
0x618860C8,
0x28009805,
0x2801D006,
0x2802D00B,
0x2803D00A,
0xE008D106,
0x02C92101,
0x43089804,
0x63884985,
0xBF00E0F7,
0x4983BF00,
0x63889804,
0x1E489905,
0xFF18F7FF,
0xBF009001,
0x46044883,
0xE00D9003,
0x98014621,
0xFF28F7FF,
0xF7FF9801,
0x3412FF41,
0xF7FF9801,
0x2800FF47,
0x9403D000,
0x4284487B,
0x9803D203,
0x285A1A20,
0x9803D3EA,
0x285A1A20,
0xE00DD3E2,
0x98014621,
0xFF0EF7FF,
0xF7FF9801,
0x9801FF27,
0xFF2EF7FF,
0xD0002800,
0x3412E003,
0x4284486E,
0xBF00D3EE,
0x19009803,
0x46210844,
0xF7FF9801,
0x2700FEF9,
0x9801E003,
0xFF10F7FF,
0x2F101C7F,
0x260DD3F9,
0x2000BF00,
0x0A309002,
0x2069D001,
0x98029002,
0xD1782800,
0xD0422E4C,
0x2E00DC06,
0x2E0DD009,
0x2E42D008,
0xE012D170,
0xD0742E52,
0xD1F92E59,
0xE085E036,
0x9801210D,
0xFEE3F7FF,
0x9801210A,
0xFEDFF7FF,
0x9801213E,
0xFEDBF7FF,
0x9801E078,
0xFEFFF7FF,
0x1C684605,
0x2069D102,
0xE06F9002,
0xD1022D00,
0x90022062,
0x214BE06A,
0x436101C9,
0x46294608,
0xF8ACF000,
0x2D404605,
0x2001D303,
0x42850580,
0x2062D302,
0xE0599002,
0x98012142,
0xFEB7F7FF,
0x98014629,
0xFEA2F7FF,
0xBF00E050,
0xF7FF9801,
0x4605FED6,
0xF7FF9801,
0x9000FED2,
0xD0021C68,
0x1C409800,
0x2069D102,
0xE03F9002,
0x98014631,
0xFE9DF7FF,
0x19409800,
0xE0129000,
0xD1042E59,
0x98017829,
0xFE93F7FF,
0x9801E00A,
0xFE9AF7FF,
0x0A184603,
0x2069D002,
0xE0059002,
0xBF00702B,
0x98001C6D,
0xD3E94285,
0x9802BF00,
0xD0032800,
0xE01CE01C,
0xE004E017,
0x9801214B,
0xFE77F7FF,
0x9801E014,
0xFE9BF7FF,
0x1C684605,
0x2069D102,
0xE00B9002,
0x98012152,
0xFE69F7FF,
0x60054819,
0xFE32F7FF,
0x2063BF00,
0xBF009002,
0x9802BF00,
0xD0032800,
0x98012145,
0xFE59F7FF,
0xF7FF9801,
0x4606FE61,
0xBF00E753,
0xE7FEBF00,
0x0000051C,
0x40020000,
0x40008000,
0x00000301,
0x8AAA5551,
0x40018000,
0x00061010,
0x40060000,
0x0000053C,
0x40088000,
0x00000534,
0x40090000,
0x00000A04,
0x0000129A,
0x20000000,
0x4700480B,
0xE7FEE7FE,
0xE7FEE7FE,
0xE7FEE7FE,
0xE7FEE7FE,
0xE7FEE7FE,
0xE7FEE7FE,
0xE7FEE7FE,
0xE7FEE7FE,
0xE7FEE7FE,
0xE7FEE7FE,
0x0000E7FE,
0x00000085,
0x460BB530,
0x20004601,
0x24012220,
0x460DE009,
0x429D40D5,
0x461DD305,
0x1B494095,
0x40954625,
0x46151940,
0x2D001E52,
0xBD30DCF1,
0x25014C06,
0xE0054E06,
0x68E34620,
0x432BC807,
0x34104798,
0xD3F742B4,
0xFDBCF7FF,
0x00000544,
0x00000544,
0x0002001A,
0x40088000,
0x0002001A,
0x40088000,
0x0002001A,
0x40088000,
0x00010001,
0x00000000,
0x00010001,
0x00000000,
0x00000000
};


void earase_flash(uint32_t is_all_flash, uint32_t address);
void write_boot ();
uint32_t Is_Flash_Earase = 0;


/*************************************************************************
 * FMC module initialization
 *************************************************************************/

void ProgramDelay(uint32_t Loops)
{
  volatile uint32_t i = Loops;
  for (; i > 0; i--);
}
//#define DEBUG 1
#if DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#else
#define DEBUG_PRINTF(...)
#endif

#if DEBUG
PUTCHAR_PROTOTYPE{
        MDR_UART1->DR = ch;
        // Loop until the end of transmission
		while((MDR_UART1->FR & (1 << 5)) == (1 << 5));
  return (ch);
}
#endif
#if USE_ARGC_ARGV
uint32_t FlashInit(void *base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags,
                   int argc, char const *argv[])
#else
uint32_t FlashInit(void *base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags)
#endif
{
	MDR_RST_CLK->PER2_CLOCK = 0xFFFFFFFF;
  	MDR_RST_CLK->CPU_CLOCK&=~(3<<8);

#if DEBUG	
	// Отладка
	MDR_RST_CLK->PER1_CLOCK = 0x30;

	/* enable HSE */
	MDR_RST_CLK->HS_CONTROL	= 1;
	/* wait HSE */
	while((MDR_RST_CLK->CLOCK_STATUS & (1 << 2)) != (1 << 2));

	/* use pll frequency */
	MDR_RST_CLK->CPU_CLOCK = (2 << 0) | (0 << 2) | (1 << 8);

	/* set per 1 clock = HSE/2 = 4 MHz */
	/* set per 2 clock = HSE*4 = 32 MHz */
	MDR_RST_CLK->PER1_CLOCK =  (0 << 0) | (0 << 2)  | (1 << 4);

	/* enable clock for uart, port and adciu */
	MDR_RST_CLK->PER2_CLOCK |= (1 << 1 ) | (1 << 13) | (1 << 17);
	MDR_RST_CLK->UART_CLOCK =  1 << 24  ;
	
	/*  set portb 0 and portb 1 for uart */
	MDR_PORTB->ANALOG = (3 << 0);
	MDR_PORTB->FUNC	= (1 << 0) | (1 << 2);
	MDR_PORTB->OE = (0 << 0) | (1 << 0);
	MDR_PORTB->PWR = (3 << 0) | (3 << 2);
	MDR_PORTB->PULL = 0;
	MDR_PORTB->GFEN = 0;
	MDR_PORTB->PD = 0;

	// Настройка UART
	/* uart settings */
	MDR_UART1->IBRD = 4;
	MDR_UART1->FBRD = 29;
	MDR_UART1->LCR_H = (1 << 4) | (3 << 5);
	MDR_UART1->CR = (1<<8) | (1<<9) | (1<<0);

	//DEBUG_PRINTF("test");
#endif	
	//FlashErase(0,0);
	return RESULT_OK;
}


uint32_t FlashWrite(void *block_start,
                    uint32_t offset_into_block,
                    uint32_t count,
                    char const *buffer)
{
    DEBUG_PRINTF("\t\tFlashWrite block_start 0x%x, offset_into_block 0x%x, count 0x%X, buffer 0x%x\n\r",(uint32_t)block_start,offset_into_block,count,(uint32_t)buffer);	
	
	uint32_t * ptr_buffer = (uint32_t *)buffer;  
	uint32_t size = 0;
	volatile uint32_t y = 0;
	
	if((MDR_BKP->REG_0E&0x00000800) == 0x00000000) return(0);
    MDR_RST_CLK->PER2_CLOCK = 0xFFFFFFFF;
	
	while( size < count/4 ){
	  
		MDR_EEPROM->KEY = 0x8AAA5551;
		
		MDR_EEPROM->CMD = 0x0001 ;
		MDR_EEPROM->ADR = (uint32_t) ((uint32_t)block_start + offset_into_block + size*4);
		MDR_EEPROM->DI  = ptr_buffer[size];
		
		MDR_EEPROM->CMD = 0x1041;	// Set PROG XE EN
		for (y=0;y<40;y++); // 5 мкс
		
		MDR_EEPROM->CMD = 0x3041; // SET NVSTR
		for (y=0;y<80;y++);	//10 мкс
		
		MDR_EEPROM->CMD = 0x30C1; // SET YE
		for (y=0;y<400;y++); 	//40 мкс
		
		MDR_EEPROM->CMD = 0x3041; // CLEAR YE
		MDR_EEPROM->CMD = 0x2041;	// CLER PROG 
		for (y=0;y<40;y++);		// 5 мкс
		
		MDR_EEPROM->CMD = 0x0001; // CLEAR NVSTR XE
		for (y=0;y<10;y++);
		
		MDR_EEPROM->CMD = 0x0000; // CLEAR NVSTR XE
		MDR_EEPROM->KEY = 0;
		    
		size++;
	}                

	//for(int i = 0; i < 16; i++)
	//  printf("\tflash_data = 0x%x\r\n", ptr[i]);

    return(RESULT_OK);
}

/*************************************************************************
 * FMC Block erase
 *************************************************************************/
uint32_t FlashErase(void *block_start,
                    uint32_t block_size)
{
	DEBUG_PRINTF("FlasErase block_start 0x%x, block_size 0x%x\n\r",(uint32_t)block_start,block_size);
	
	//if(MDR_BKP->REG_0E & BKP_REG_0E_FPOR)
	//	DEBUG_PRINTF("earse all\r\n");
	
	for (uint32_t y = 0; y<=0x18000; y=y+0x8000){
		DEBUG_PRINTF("Erasing...\r\n");
		DEBUG_PRINTF("%d\r\n",((MDR_BKP->REG_0E & BKP_REG_0E_FPOR) >> BKP_REG_0E_FPOR_Pos));
		earase_flash(((MDR_BKP->REG_0E & BKP_REG_0E_FPOR) >> BKP_REG_0E_FPOR_Pos), (uint32_t)y);	
	}
	MDR_EEPROM->CMD = 0;
	// востанавливаем 
	if((MDR_BKP->REG_0E & BKP_REG_0E_FPOR) == 0){
		DEBUG_PRINTF("write boot\r\n");
		write_boot ();
	}
	return(RESULT_OK);
}



/*************************************************************************
 * FMC deinit
 *************************************************************************/
uint32_t FlashSignoff()
{
  /*Enable ISP function*/
  /*Enable ISP function*/
 
  return RESULT_OK;
}

void earase_flash(uint32_t is_all_flash, uint32_t address)
{
	uint32_t i;
	uint32_t j = 1;;
	if(is_all_flash)
		j = 0;
	MDR_EEPROM->CMD = 0x1 | (j << EEPROM_CMD_IFREN_Pos);
	MDR_EEPROM->KEY = 0x8AAA5551;
	 
	MDR_EEPROM->ADR = address;
  	MDR_EEPROM->DI  = 0;
	MDR_EEPROM->CMD = 0x0001 | (j << EEPROM_CMD_IFREN_Pos);
	MDR_EEPROM->CMD = 0x0003 | (j << EEPROM_CMD_IFREN_Pos);
	MDR_EEPROM->CMD = 0x0001 | (j << EEPROM_CMD_IFREN_Pos);
	MDR_EEPROM->CMD = 0x0c41 | (j << EEPROM_CMD_IFREN_Pos);
	
  	for (i=0;i<40;i++);
	
	MDR_EEPROM->CMD = 0x2c41 | (j << EEPROM_CMD_IFREN_Pos);
  	for (i=0;i<320000;i++);
	MDR_EEPROM->CMD = 0x2841 | (j << EEPROM_CMD_IFREN_Pos);
  	for (i=0;i<800;i++)
	MDR_EEPROM->CMD = 0x0001;
	
	MDR_EEPROM->CMD = 0x00;
}


void write_boot ()
{
	int i,y;
	MDR_EEPROM->KEY = 0x8AAA5551;
	MDR_EEPROM->CMD = 0x0201;
	
	for(i=0;i<BOOT_SIZE;i++){	
	  	MDR_EEPROM->ADR = i*4;
	  	for (y=0;y<80;y++);
		MDR_EEPROM->CMD = 0x1241;	// Set PROG XE EN
	  	for (y=0;y<40;y++);
		MDR_EEPROM->CMD = 0x3241; // SET NVSTR
	  	MDR_EEPROM->DI  = BOOT_LOADER[i];
		MDR_EEPROM->CMD = 0x3243; // SET NVSTR
		MDR_EEPROM->CMD = 0x3241; // SET NVSTR
	  	for (y=0;y<100;y++);
		MDR_EEPROM->CMD = 0x32C1; // SET YE

	  	for (y=0;y<400;y++);
		MDR_EEPROM->CMD = 0x3241; // CLEAR YE
		MDR_EEPROM->CMD = 0x2241;	// CLER PROG 
  		for (y=0;y<40;y++);
		MDR_EEPROM->CMD = 0x0201; // CLEAR NVSTR XE
  		for (y=0;y<40;y++);
  	}
	
	MDR_EEPROM->CMD = 0x0000;
}
