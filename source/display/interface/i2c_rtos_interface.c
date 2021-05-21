/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		i2c_rtos_interface.c			################
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
 ** 1	|	MS		|04-23-2021	| imported i2c_rtos_interface.c														|
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
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/

#include "i2c_rtos_interface.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * callback
 * callback for I2C DMA
 * param base: 			I2C peripheral
 * param drv_handle:	I2C dma handle
 * param status:		I2C status and errors
 * param userData:		user data
 ******************************************************************************/
#ifndef DISP_I2C_DMA
static void callback(I2C_Type *base, i2c_master_handle_t *drv_handle, status_t status, void *userData);
#else
static void callback(I2C_Type *base, i2c_master_dma_handle_t *drv_handle, status_t status, void *userData);
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

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
void i2c_m_rtos_init(i2c_m_rtos_handle_t *handle, I2C_Type *base, DMA_Type *dma_base, uint32_t dma_chnl, uint32_t baudrate, uint32_t src_clock) {

	handle->base = base;						//set I2C peripheral address in I2C master RTOS handle
	handle->dma_base = dma_base;				//set DMA peripheral address in I2C master RTOS handle

	handle->dma_chnl = dma_chnl;				//set DMA channel in I2C master RTOS handle

	handle->sem = xSemaphoreCreateBinary();		//create semaphore
	handle->mutex = xSemaphoreCreateMutex();	//create mutex

	DMA_Init(handle->dma_base);					//init DMA


	/*******************************************************************************
	 init I2C peripheral
	 ******************************************************************************/
	I2C_MasterGetDefaultConfig(&handle->config);				//get I2C master default configuration
	handle->config.baudRate_Bps = baudrate;						//set baudrate in I2C master RTOS handle

	I2C_MasterInit(handle->base, &handle->config, src_clock);	//init I2C master
	SYSCON -> FCLKSEL[2] = 0; 									//Flexcom2 clock: FRO 12 MHz


	/*******************************************************************************
	 set DMA
	 ******************************************************************************/
	DMA_EnableChannel(handle->dma_base, handle->dma_chnl); 						//enable channel 5 (Flexcom2 I2C Master
	DMA_CreateHandle(&handle->dma_handle, handle->dma_base, handle->dma_chnl);	//create DMA handle

	//create I2C transfer with DMA
	I2C_MasterTransferCreateHandleDMA(handle->base, &handle->drv_handle, callback, (void*)handle, &handle->dma_handle);
}

/*******************************************************************************
 * callback
 * callback for I2C DMA
 * param base: 			I2C peripheral
 * param drv_handle:	I2C dma handle
 * param status:		I2C status and errors
 * param userData:		user data
 ******************************************************************************/
static void callback(I2C_Type *base, i2c_master_dma_handle_t *drv_handle, status_t status, void *userData) {

    i2c_m_rtos_handle_t *handle = (i2c_m_rtos_handle_t *)userData;	//update I2C master RTOS handle
    BaseType_t reschedule;
    handle->async_status = status; 									//update status
    xSemaphoreGiveFromISR(handle->sem, &reschedule);				//give semaphore
    portYIELD_FROM_ISR(reschedule);
}

/*******************************************************************************
 * i2c_m_rtos_transfer
 * transfer data via I2C
 * param handle:		I2C master RTOS handle
 * param transfer:		transfer handle which contains data
 ******************************************************************************/
status_t i2c_m_rtos_transfer(i2c_m_rtos_handle_t *handle, i2c_master_transfer_t *transfer) {

	//try to take semaphor. If failed return busy status
	if (xSemaphoreTake(handle->mutex, portMAX_DELAY) != pdTRUE)
		return kStatus_I2C_Busy;

#ifndef DISP_I2C_DMA
	if (I2C_MasterTransferNonBlocking(handle->base, &handle->drv_handle, transfer) != kStatus_Success) {
#else
	//try to transfer data. If failed return busy status
	if (I2C_MasterTransferDMA(handle->base, &handle->drv_handle, transfer) != kStatus_Success) {
#endif
		xSemaphoreGive(handle->mutex);
		return kStatus_I2C_Busy;
	}

	// Wait for transfer to finish.
	xSemaphoreTake(handle->sem, portMAX_DELAY);

	// Unlock handle mutex.
	xSemaphoreGive(handle->mutex);

	// Return status.
	return handle->async_status;
}

