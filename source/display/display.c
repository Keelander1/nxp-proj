/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		dislpay.c						################
 ** Created on: 	04-22-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	MS		|04-23-2021	| imported display.c														|
 ** 2	|			|			|																			|
 ** 3	|			|			|																			|
 ** 4	|			|			|																			|
 ** 5	|			|			|																			|
 ** 6	|			|			|																			|
 ** 7	|			|			|																			|
 ** 8	|			|			|																			|
 ** 9	|			|			|																			|
 ** 10	|			|			|																			|
 ************************************************************************************************************
 **
 **	Description
 ************************************************************************************************************
 ** Header file for display functions:
 **
 ** Contains display initialization functions
 **
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/

#include "display.h"

__attribute__((section("SSD1309ImageBuffer"))) static uint8_t s_disp_0_buffer[DISP_0_BYTE_CNT] __attribute__((aligned(4U)));
i2c_m_rtos_handle_t g_disp_0_i2c_handle;
ssd1309_rtos_t g_disp_0;

/*******************************************************************************
 * DISPLAY_Init
 * init display
 ******************************************************************************/
void DISPLAY_Init()
{


	NVIC_SetPriority(FLEXCOMM2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);							//set interrupt priority for I2C2
	#ifndef DISP_I2C_DMA
		i2c_m_rtos_init(&g_disp_0_i2c_handle, I2C0_MASTER, I2C0_BAUDRATE, I2C0_MASTER_CLK_FREQ);
	#else
		NVIC_SetPriority(DMA0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY); 							//set interrupt priority for DMA0
		i2c_m_rtos_init(&g_disp_0_i2c_handle, I2C0_MASTER, DMA0, 5, I2C0_BAUDRATE, I2C0_MASTER_CLK_FREQ); 	//init I2C with RTOS and DMA
	#endif
		ssd1309_rtos_init(&g_disp_0, DISP0_I2C_ADDRESS, DISP_0_HEIGHT, DISP_0_WIDTH, s_disp_0_buffer);		//init LCD
}
