/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		display.h						################
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
 ** 1	|	MS		|04--2021	| imported display.h														|
 ** 2	|			|			| 																			|
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
 ** Header file for camera initialization functions:
 **
 ** Contains includes and function prototypes
 **
** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/

#ifndef DISPLAY_DISPLAY_H_
#define DISPLAY_DISPLAY_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "interface/i2c_rtos_interface.h"
#include "display/driver/ssd1309_rtos.h"
#include "board.h"
#include "FreeRTOS.h"
#include "menu/menu_rtos.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
extern i2c_m_rtos_handle_t g_disp_0_i2c_handle; 	//I2C freertos handle
extern ssd1309_rtos_t g_disp_0;						//LCD handle

/*
//******************************
//Configure Pin P[3][23] (J9 Pin3) (I2C_SDA/FC2_CTS_SDA_SSEL0)
#define PIO323_DIGIMODE_DIGITAL 0x01u 	//pin is digital pin
#define PIO323_FUNC_ALT1 0x01u			//pin function is I2C SDA
#define PIO323_I2CSLEW_I2C_MODE 0x00u	//pin slew rate is in I2C mode
//******************************


//******************************
//Configure Pin P[3][24] (J9 Pin1) (I2C_SCL/FC2_RTS_SCL_SSEL1)
#define PIO324_DIGIMODE_DIGITAL 0x01u	//pin is digital pin
#define PIO324_FUNC_ALT1 0x01u			//pin function is I2C SDA
#define PIO324_I2CSLEW_I2C_MODE 0x00u	//pin slew rate is in I2C mode
//******************************
*/

//******************************
//Configure I2C interface
#define I2C0_MASTER_CLK_FREQ 	(12000000)	//Flexcom2 clock
#define I2C0_MASTER 			I2C2		//Flexcom2 peripheral
#define I2C0_BAUDRATE 			(400000)	//I2C clock frequency
//******************************

//configure I2C components of LCD
//******************************
#define DISP_0_HEIGHT			64U			//LCD hight in pixels
#define DISP_0_WIDTH			128U		//LCD width in pixels
#define DISP_0_BYTE_CNT			(DISP_0_HEIGHT * DISP_0_WIDTH) >> 3U //LCD pixel count in byte
#define DISP0_I2C_ADDRESS		0x3C //display I2C address
//******************************

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * DISPLAY_Init
 * init display
 ******************************************************************************/
void DISPLAY_Init();

#endif /* DISPLAY_DISPLAY_H_ */
