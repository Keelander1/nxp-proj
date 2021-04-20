/*
 * i2c_rtos_interface.h
 *
 *  Created on: 14.12.2020
 *      Author: chris
 */

#ifndef DISPLAY_INTERFACE_I2C_RTOS_INTERFACE_H_
#define DISPLAY_INTERFACE_I2C_RTOS_INTERFACE_H_

#include "FreeRTOS.h"
#include "fsl_i2c.h"
#include "semphr.h"

#define DISP_I2C_DMA

#ifdef DISP_I2C_DMA
#include "fsl_i2c_dma.h"
#endif

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
void i2c_m_rtos_init(i2c_m_rtos_handle_t *handle, I2C_Type *base, DMA_Type *dma_base, uint32_t dma_chnl, uint32_t baudrate, uint32_t src_clock);
#endif

#endif /* DISPLAY_INTERFACE_I2C_RTOS_INTERFACE_H_ */
