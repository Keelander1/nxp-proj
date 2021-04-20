/*
 * display.h
 *
 *  Created on: 14.12.2020
 *      Author: chris
 */

#ifndef DISPLAY_DISPLAY_H_
#define DISPLAY_DISPLAY_H_

#include "interface/i2c_rtos_interface.h"
#include "display/driver/ssd1309_rtos.h"
#include "board.h"
#include "FreeRTOS.h"

extern i2c_m_rtos_handle_t g_disp_0_i2c_handle;
extern ssd1309_rtos_t g_disp_0;

#define PIO323_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 1. */
#define PIO323_FUNC_ALT1 0x01u
/*!
 * @brief Controls slew rate of I2C pad.: I2C mode. */
#define PIO323_I2CSLEW_I2C_MODE 0x00u
/*!
 * @brief Select Analog/Digital mode.: Digital mode. */
#define PIO324_DIGIMODE_DIGITAL 0x01u
/*!
 * @brief Selects pin function.: Alternative connection 1. */
#define PIO324_FUNC_ALT1 0x01u
/*!
 * @brief Controls slew rate of I2C pad.: I2C mode. */
#define PIO324_I2CSLEW_I2C_MODE 0x00u


#define I2C0_MASTER_CLK_FREQ 	(12000000)
#define I2C0_MASTER 			I2C2

#define I2C0_BAUDRATE 			(400000)

#define DISP_0_HEIGHT			64U
#define DISP_0_WIDTH			128U
#define DISP_0_BYTE_CNT			(DISP_0_HEIGHT * DISP_0_WIDTH) >> 3U

#define DISP0_I2C_ADDRESS		0x3C

void DISPLAY_Init();

#endif /* DISPLAY_DISPLAY_H_ */
