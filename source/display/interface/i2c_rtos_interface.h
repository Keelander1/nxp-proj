/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		i2C_rtos_interface.h			################
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
 ** 1	|	MS		|04-23-2021	| imported i2c_rtos_interface.h													|
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
 ** contains function for I2C and RTOS operation
 **
 ** Use this data in ssd1309_write_char to draw the char
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/

#ifndef DISPLAY_INTERFACE_I2C_RTOS_INTERFACE_H_
#define DISPLAY_INTERFACE_I2C_RTOS_INTERFACE_H_

#include "FreeRTOS.h"
#include "fsl_i2c.h"
#include "semphr.h"

//I2C with DMA opertaion
#define DISP_I2C_DMA

#ifdef DISP_I2C_DMA
#include "fsl_i2c_dma.h"
#endif


/*******************************************************************************
 * i2c_m_rtos_handle_t
 * contains I2C RTOS configuration
 * param base:			I2C memory address
 * param drv_handle:	I2C master DMA handle
 * param dma_handle:	DMA handle
 * param dma_base:		DMA memory address
 * param dma_chnl:		DMA cannel
 * param config:		I2C master configuration
 * param async_status:	I2C status and errors
 * param sem:			semaphore
 * param mutex:			mutex
 ******************************************************************************/
typedef struct {
	I2C_Type *base;
#ifndef DISP_I2C_DMA
	i2c_master_handle_t drv_handle;
#else
	i2c_master_dma_handle_t drv_handle;
	dma_handle_t dma_handle;
	DMA_Type *dma_base;
	uint32_t dma_chnl;
#endif
	i2c_master_config_t config;
	status_t async_status;
	SemaphoreHandle_t sem;
	SemaphoreHandle_t mutex;
} i2c_m_rtos_handle_t;


#ifndef DISP_I2C_DMA
void i2c_m_rtos_init(i2c_m_rtos_handle_t *handle, I2C_Type *base, uint32_t baudrate, uint32_t src_clock);
#else

/*******************************************************************************
 * i2c_m_rtos_init
 * init I2C master with RTOS operaton
 * param handle: 		I2C master RTOS handle
 * param base:			I2C peripheral address
 * param dma_base:		DMA peripheral address
 * param dma_chnl:		DMA chanel
 * param baudrate:		I2C baudrate
 * param src_clock:		I2C source clock
 ******************************************************************************/
void i2c_m_rtos_init(i2c_m_rtos_handle_t *handle, I2C_Type *base, DMA_Type *dma_base, uint32_t dma_chnl, uint32_t baudrate, uint32_t src_clock);
#endif

#endif /* DISPLAY_INTERFACE_I2C_RTOS_INTERFACE_H_ */
