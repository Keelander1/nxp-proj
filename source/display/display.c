/*
 * display.c
 *
 *  Created on: 14.12.2020
 *      Author: chris
 */

#include "display.h"

__attribute__((section("SSD1309ImageBuffer"))) static uint8_t s_disp_0_buffer[DISP_0_BYTE_CNT] __attribute__((aligned(4U)));
i2c_m_rtos_handle_t g_disp_0_i2c_handle;
ssd1309_rtos_t g_disp_0;


void DISPLAY_Init()
{
	 //// Pins initialisieren


	NVIC_SetPriority(FLEXCOMM2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	#ifndef DISP_I2C_DMA
		i2c_m_rtos_init(&g_disp_0_i2c_handle, I2C0_MASTER, I2C0_BAUDRATE, I2C0_MASTER_CLK_FREQ);
	#else
		NVIC_SetPriority(DMA0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
		i2c_m_rtos_init(&g_disp_0_i2c_handle, I2C0_MASTER, DMA0, 5, I2C0_BAUDRATE, I2C0_MASTER_CLK_FREQ);
	#endif
		ssd1309_rtos_init(&g_disp_0, DISP0_I2C_ADDRESS, DISP_0_HEIGHT, DISP_0_WIDTH, s_disp_0_buffer);
}
