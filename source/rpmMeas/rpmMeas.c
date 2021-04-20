/*
 * rpmMeas.c
 *
 *  Created on: 19.04.2021
 *      Author: chris
 */


#include "rpmMeas.h"

const ctimer_config_t RPMMEAS_config = {
		.mode = kCTIMER_TimerMode,   /* TC is incremented every rising APB bus clock edge */
		.input = kCTIMER_Capture_0,  /*!< Timer capture channel 0 */
		.prescale = 0                /*!< Prescale value 0 --> */
};

ctimer_callback_t ctimer_callback_table[] = {NULL, NULL, NULL, NULL, rpmMeas_Left_ISR, rpmMeas_Right_ISR, NULL, NULL};

volatile uint32_t oldValue = 0;
volatile uint32_t difference = 0;

void RPMMEAS_Init()
{
	//Set Port0 Pin 24 to CLKOUT Function
	IOCON->PIO[0][24]       &= 0xFFFFFFF0;      //clear FUNC bits of P0.24
	IOCON->PIO[0][24]     	|= 0x3;           	//set FUNC bits to CTIMER2_CAP0 function FUNC3 P0.24
	GPIO->DIR[0]           	&= ~1<<12;         	//set CTIMER2_CAP0 pin to input

	//Set Port0 Pin 25 to CLKOUT Function
	IOCON->PIO[0][25]       &= 0xFFFFFFF0;      //clear FUNC bits of P0.25
	IOCON->PIO[0][25]     	|= 0x3;           	//set FUNC bits to CTIMER2_CAP1 function FUNC3 P0.25
	GPIO->DIR[0]           	&= ~1<<12;         	//set CTIMER2_CAP1 pin to input

	/* CTIMER1 peripheral initialization */
	CTIMER_Init(CTIMER2, &RPMMEAS_config); //(CTIMER_Type, ctimer_config_t)

	CTIMER_RegisterCallBack(CTIMER2, &ctimer_callback_table[0], kCTIMER_MultipleCallback);

	//Setup Capture Left Drive Capture Channel 0
	CTIMER_SetupCapture(CTIMER2, CTIMER2_RPMMEAS_LEFT_CHANNEL, kCTIMER_Capture_RiseEdge, 1);
	//Setup Capture Right Drive Capture Channel 1
	CTIMER_SetupCapture(CTIMER2, CTIMER2_RPMMEAS_RIGHT_CHANNEL, kCTIMER_Capture_RiseEdge, 1);

	/* Start the timer */
	CTIMER_StartTimer(CTIMER2); //Timer starten

}

void rpmMeas_Left_ISR(uint32_t flags)
{

}

void rpmMeas_Right_ISR(uint32_t flags)
{
	difference = CTIMER_GetTimerCountValue(CTIMER2)-oldValue;
	oldValue = CTIMER_GetTimerCountValue(CTIMER2);
}






