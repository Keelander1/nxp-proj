/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "servo/servo.h"
#include "display/hmi/hmi.h"
#include "drive/drive.h"
#include "camera/camera.h"
#include "rpmMeas/rpmMeas.h"

#include "main_gustl.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"


#include "pin_mux.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void LED_Blink(void *pvParameters);
void CLKOUT_Init(void);
void STD_Init(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitLEDsPins();
    CLKOUT_Init();
    STD_Init();
    CAM_Init();
    RPMMEAS_Init();

    //init servo
    SERVO_Init();

    //initialice BLDC motors
    BLDC_Init();

    if (xTaskCreate(LED_Blink, "LED_Blink", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL) !=
        pdPASS)
    {
    	LED3_ON();
    }
    if (xTaskCreate(hmi_task, "hmi_task", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL) !=
           pdPASS)
       {
       	LED3_ON();
       }

    vTaskStartScheduler();
    while(1){

    }
    return 0;
}

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void LED_Blink(void *pvParameters) //f = 1Hz
{
    while(1){
    	LED1_TOGGLE();
        vTaskDelay(500);
    }
}

void STD_Init(void)
{
	LED1_OFF();
	LED2_OFF();
	LED3_OFF();
}

void CLKOUT_Init(void) //Initialize Port 3 Pin 12 for CLKOUT Function (J12 Pin 9)
{
	//***********************************
	//Set Port3 Pin 12 to CLKOUT Function
	IOCON->PIO[3][12]       &= 0xFFFFFFF0;      //clear FUNC bits of P3.12
	IOCON->PIO[3][12]     	|= 0x5;           	//set FUNC bits to CLKOUT function FUNC5 P3.12
	GPIO->DIR[3]           	|= 1<<12;         	//set CLKOUT pin to output (sec. 12.3.2)
	SYSCON->CLKOUTSELA 		= 0x04;            	//sec. 3.5.21
	SYSCON->CLKOUTDIV    	= 219;              //set divider to 220 --> sees 1MHz (sec. 3.5.23)
	//***********************************
}



