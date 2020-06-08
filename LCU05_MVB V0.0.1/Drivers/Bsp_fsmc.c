

#include "Bsp_fsmc.h"


/**
* @brief  Configures the FSMC and GPIOs to interface with the SRAM memory.
*         This function must be called before any write/read operation
*         on the SRAM.
* @param  None
* @retval None
*/
void Bsp_fsmc_init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure = {0};
	FSMC_NORSRAMTimingInitTypeDef  p    = {0};
	FSMC_NORSRAMTimingInitTypeDef  pw   = {0};
	GPIO_InitTypeDef GPIO_InitStructure = {0}; 

	/* Enable GPIOs clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF |
		                   RCC_AHB1Periph_GPIOG, ENABLE);

	/* Enable FSMC clock */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); 

	/*--------------------------- GPIOs Configuration ----------------------------*/
	/*
	+-------------------+--------------------+------------------+------------------+
	+                           SRAM pins assignment                               +
	+-------------------+--------------------+------------------+------------------+
	| PD0  <-> FSMC_D2  | PE7  <-> FSMC_D4   | PF0  <-> FSMC_A0 | PG0 <-> FSMC_A10 | 
	| PD1  <-> FSMC_D3  | PE8  <-> FSMC_D5   | PF1  <-> FSMC_A1 | PG1 <-> FSMC_A11 | 
	| PD4  <-> FSMC_NOE | PE9  <-> FSMC_D6   | PF2  <-> FSMC_A2 | PG2 <-> FSMC_A12 | 
	| PD5  <-> FSMC_NWE | PE10 <-> FSMC_D7   | PF3  <-> FSMC_A3 | PG3 <-> FSMC_A13 | 
	| PD14 <-> FSMC_D0  |-------------------+| PF4  <-> FSMC_A4 | PG4 <-> FSMC_A14 | 
	| PD15 <-> FSMC_D1  |                    | PF5  <-> FSMC_A5 | PG5 <-> FSMC_A15 |
	|-------------------+                    | PF12 <-> FSMC_A6 | PG12 <-> FSMC_IOCS|
	                                         | PF13 <-> FSMC_A7 |------------------+
	                                         | PF14 <-> FSMC_A8 | 
	                                         | PF15 <-> FSMC_A9 | 
	                                         | PF11 <-> FSMC_AEN|
	                                         |------------------+
	
	*/
	/* GPIOD configuration */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC); 
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin   = (GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_4  | 
	                                GPIO_Pin_5  | GPIO_Pin_14 | GPIO_Pin_15);
									
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* GPIOE configuration */
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource3 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource4 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10);

	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* GPIOF configuration */
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource0 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource2 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource3 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource4 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource5 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource11 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource12 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource13 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource14 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource15 , GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3  | 
		                           GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_11 |  GPIO_Pin_12 | 
								   GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);      

	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* GPIOG configuration */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource2 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource3 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource4 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource5 , GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3 | 
		                           GPIO_Pin_4  | GPIO_Pin_5);      

	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/*------------------------------ NE GPIO Configuration ---------------------------*/


	/* NE4 configuration, SRAM */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 

	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_FSMC);

	/*------------------------------ FSMC Configuration ------------------------------*/


	/*--------------------------------  MVB NORSRAM4 ---------------------------------*/
	p.FSMC_AddressSetupTime      = 0x0f;
	p.FSMC_AddressHoldTime       = 0x01;
	p.FSMC_DataSetupTime         = 0x1f;
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision           = 0;
	p.FSMC_DataLatency           = 0;
	p.FSMC_AccessMode            = FSMC_AccessMode_D;

	pw.FSMC_AddressSetupTime      = 0x0f;
	pw.FSMC_AddressHoldTime       = 0x01;
	pw.FSMC_DataSetupTime         = 0x1f;   
	pw.FSMC_BusTurnAroundDuration = 0;
	pw.FSMC_CLKDivision           = 0;
	pw.FSMC_DataLatency           = 0;
	pw.FSMC_AccessMode            = FSMC_AccessMode_D;

	FSMC_NORSRAMInitStructure.FSMC_Bank               = FSMC_Bank1_NORSRAM4;//FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux     = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType         = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth    = FSMC_MemoryDataWidth_8b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode    = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait   = FSMC_AsynchronousWait_Disable;  
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_High; 
	FSMC_NORSRAMInitStructure.FSMC_WrapMode           = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive   = FSMC_WaitSignalActive_DuringWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation     = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal         = FSMC_WaitSignal_Disable;//FSMC_WaitSignal_Enable; grape 2013.11.24
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode       = FSMC_ExtendedMode_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst         = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &pw;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	//FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);

}

