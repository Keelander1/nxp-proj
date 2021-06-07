/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		disp_adapter.c					################
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
 ** 1	|	MS		|04-23-2021	| imported disp_adapter.c													|
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
 ** Contains function which configures the data packed which is send over I2C
 **
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/


#include "display.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
static i2c_master_transfer_t rtos_xfer; //I2C transfer object

/*******************************************************************************
 * Code
 ******************************************************************************/
/*******************************************************************************
 * ssd1309_send
 * init LCD with RTOS operation
 * param obj: 		LCD object,
 * param d_type:	is data a command (0) or data (1)
 * param data:		data
 * param size:		size of data
 ******************************************************************************/
void ssd1309_send(ssd1309_t *obj, ssd1309_data_t d_type, uint8_t *data, uint32_t size) {

	rtos_xfer.slaveAddress = obj->config.id; 			//set display I2C address (0x3C)
	rtos_xfer.direction = kI2C_Write; 					//set direction to master write
	rtos_xfer.subaddress = d_type == CMD ? 0x00 : 0x40;	//set subaddress to determine if the signal is data (0x00) or a command (0x40)
	rtos_xfer.subaddressSize = 1;						//length of subaddress in byte
	rtos_xfer.data = data; 								//data to transfer
	rtos_xfer.dataSize = size; 							//set size of data
	rtos_xfer.flags = kI2C_TransferDefaultFlag; 		//send start and stop bit

	i2c_m_rtos_transfer(&g_disp_0_i2c_handle, &rtos_xfer); //transfer data with I2C interface
}
