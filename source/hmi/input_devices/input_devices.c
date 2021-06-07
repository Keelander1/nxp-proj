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
 ** ENC_A (Encoder signal A) at Pin P[3][20] (J9 Pin 9)
 ** ENC_B (Encoder signal B) at Pin P[3][22] (J9 Pin 11)
 ** ENC_SW (Encoder switch) at Pin P[3][21] (J9 Pin 13)
 **
 ** SW (switch) at Pin P[3][30] (J9 Pin 15)
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

static sw_handle_t sw_handle; //sw handle
static re_handle_t re_handle; //rotary encoder handle

/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * id_init
 * init input devices
 ******************************************************************************/
void id_init() {

	sw_init(&sw_handle);	//init switch handle
	re_init(&re_handle);	//init rotary encoder handle

	//**********************************************************************
	//set Repetitive Interrupt Timer to generate an interrupt for debounce purpose
	//**********************************************************************
	RIT_Init(RIT, &rit_config);										//init RIT
	RIT_SetCountAutoClear(RIT, true);								//clear RIT when compare value is reached
	RIT_SetTimerCompare(RIT, (uint64_t)(SystemCoreClock / 1000UL));	//set compare value to 12000

	NVIC_SetPriority(RIT_IRQn, 0);	//set high priority for RIT
	EnableIRQ(RIT_IRQn);			//enable RIT interrupt

	RIT_StartTimer(RIT);			//start RIT
}

/*******************************************************************************
 * id_get_button_state
 * return the state of all buttons
 ******************************************************************************/
uint32_t id_get_button_state() {

	uint32_t res = 0;

	//disable RIT IRQ
	if (DisableIRQ(RIT_IRQn) == kStatus_Success) {

		res = sw_get_up(&sw_handle) & 0x03;	//get BIT0 and BIT1 for encoder button and push button

		//enable RIT IRQ
		EnableIRQ(RIT_IRQn);
	}

	return res;
}

/*******************************************************************************
 * id_get_encoder_state
 * return how much steps the rotary encoder turned
 ******************************************************************************/
int32_t id_get_encoder_state() {

	int32_t res = 0;

	//disable RIT IRQ
	if (DisableIRQ(RIT_IRQn) == kStatus_Success) {

		res = re_get_pos(&re_handle);	//get amount of steps and rotation

		//enable RIT IRQ
		EnableIRQ(RIT_IRQn);
	}

	return res;
}

/*******************************************************************************
 * id_get_encoder_state
 * return how much steps the rotary encoder turned
 ******************************************************************************/
void RIT_IRQHandler() {

	static uint8_t counter = 0;

	// Period 1ms
	//calculate direction and amount of steps of rotary encoder
	re_task(&re_handle,
			GPIO_PinRead(BOARD_INITPINS_RE_A_GPIO, BOARD_INITPINS_RE_A_PORT, BOARD_INITPINS_RE_A_PIN),
			GPIO_PinRead(BOARD_INITPINS_RE_B_GPIO, BOARD_INITPINS_RE_B_PORT, BOARD_INITPINS_RE_B_PIN));

	if (counter >= 10) {
		// Period 10ms

		//debounce ENC_SW and SW
		sw_task(&sw_handle,
				(GPIO_PinRead(BOARD_INITPINS_SW_GPIO, BOARD_INITPINS_SW_PORT, BOARD_INITPINS_SW_PIN) & 0x01) |
				((GPIO_PinRead(BOARD_INITPINS_USER_SW_GPIO, BOARD_INITPINS_USER_SW_PORT, BOARD_INITPINS_USER_SW_PIN) & 0x01) << 1));

		counter = 0;	//reset counter
	}

	counter++;	//increment counter

	//clear RIT status flags
	RIT_ClearStatusFlags(RIT, kRIT_TimerFlag);

	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
	  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
	__DSB();
#endif
}


