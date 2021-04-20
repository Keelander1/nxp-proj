/*
 * i2c_rtos_interface.c
 *
 *  Created on: 14.12.2020
 *      Author: chris
 */
#include "i2c_rtos_interface.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#ifndef DISP_I2C_DMA
static void callback(I2C_Type *base, i2c_master_handle_t *drv_handle, status_t status, void *userData);
#else
static void callback(I2C_Type *base, i2c_master_dma_handle_t *drv_handle, status_t status, void *userData);
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
void i2c_m_rtos_init(i2c_m_rtos_handle_t *handle, I2C_Type *base, DMA_Type *dma_base, uint32_t dma_chnl, uint32_t baudrate, uint32_t src_clock) {

	handle->base = base;
	handle->dma_base = dma_base;

	handle->dma_chnl = dma_chnl;

	handle->sem = xSemaphoreCreateBinary();
	handle->mutex = xSemaphoreCreateMutex();

	DMA_Init(handle->dma_base);

	I2C_MasterGetDefaultConfig(&handle->config);
	handle->config.baudRate_Bps = baudrate;

	I2C_MasterInit(handle->base, &handle->config, src_clock);

	DMA_EnableChannel(handle->dma_base, handle->dma_chnl);
	DMA_CreateHandle(&handle->dma_handle, handle->dma_base, handle->dma_chnl);

	I2C_MasterTransferCreateHandleDMA(handle->base, &handle->drv_handle, callback, (void*)handle, &handle->dma_handle);
}

static void callback(I2C_Type *base, i2c_master_dma_handle_t *drv_handle, status_t status, void *userData) {

    i2c_m_rtos_handle_t *handle = (i2c_m_rtos_handle_t *)userData;
    BaseType_t reschedule;
    handle->async_status = status;
    xSemaphoreGiveFromISR(handle->sem, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}
