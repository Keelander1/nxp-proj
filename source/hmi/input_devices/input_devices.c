/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		input_devices.c					################
 ** Created on: 	04-23-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	MS		|04-24-2021	| imported input_devices.c														|
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
 ** Header file for HMI functions:
 **
 ** contains switch and encoder functions
 **
 ** ENC_A (Encoder signal A) at Pin P[3][20] (J9 Pin9)
 ** ENC_B (Encoder signal B) at Pin P[3][22] (J9 Pin11)
 ** ENC_SW (Encoder switch) at Pin P[3][21] (J9 Pin13)
 **
 ** SW (switch) at Pin P[3][30] (J9 Pin15)s
 ************************************************************************************************************
 ***********************************************************************************************************/
#include "sw_debounce.h"
#include "rotary_encode.h"

#include "input_devices.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const rit_config_t rit_config = {
		.enableRunInDebug = false
};

static sw_handle_t sw_handle;
static re_handle_t re_handle; //rotary encoder handle

/*******************************************************************************
 * Code
 ******************************************************************************/
void id_init() {

	sw_init(&sw_handle);
	re_init(&re_handle);

	RIT_Init(RIT, &rit_config);
	RIT_SetCountAutoClear(RIT, true);
	RIT_SetTimerCompare(RIT, (uint64_t)(SystemCoreClock / 1000UL));

	NVIC_SetPriority(RIT_IRQn, 0);
	EnableIRQ(RIT_IRQn);

	RIT_StartTimer(RIT);
}

uint32_t id_get_button_state() {

	uint32_t res = 0;

	if (DisableIRQ(RIT_IRQn) == kStatus_Success) {

		res = sw_get_up(&sw_handle) & 0x03;


		EnableIRQ(RIT_IRQn);
	}

	return res;
}

int32_t id_get_encoder_state() {

	int32_t res = 0;

	if (DisableIRQ(RIT_IRQn) == kStatus_Success) {

		res = re_get_pos(&re_handle);

		EnableIRQ(RIT_IRQn);
	}

	return res;
}

void RIT_IRQHandler() {

	static uint8_t counter = 0;

	// Period 1ms

	re_task(&re_handle,
			GPIO_PinRead(BOARD_INITPINS_RE_A_GPIO, BOARD_INITPINS_RE_A_PORT, BOARD_INITPINS_RE_A_PIN),
			GPIO_PinRead(BOARD_INITPINS_RE_B_GPIO, BOARD_INITPINS_RE_B_PORT, BOARD_INITPINS_RE_B_PIN));

	if (counter >= 10) {
		// Period 10ms

		sw_task(&sw_handle,
				(GPIO_PinRead(BOARD_INITPINS_SW_GPIO, BOARD_INITPINS_SW_PORT, BOARD_INITPINS_SW_PIN) & 0x01) |
				((GPIO_PinRead(BOARD_INITPINS_USER_SW_GPIO, BOARD_INITPINS_USER_SW_PORT, BOARD_INITPINS_USER_SW_PIN) & 0x01) << 1));

		counter = 0;
	}

	counter++;

	RIT_ClearStatusFlags(RIT, kRIT_TimerFlag);

	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
	  exception return operation might vector to incorrect interrupt */
	#if defined __CORTEX_M && (__CORTEX_M == 4U)
	    __DSB();
	#endif
}


