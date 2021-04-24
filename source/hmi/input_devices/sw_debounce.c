/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		sw_debounce.c					################
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
 ** 1	|	MS		|04-24-2021	| imported sw_debounce.c												|
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
 ** contains switch debounce functions
 **
 ** ENC_A (Encoder signal A) at Pin P[3][20] (J9 Pin9)
 ** ENC_B (Encoder signal B) at Pin P[3][22] (J9 Pin11)
 ** ENC_SW (Encoder switch) at Pin P[3][21] (J9 Pin13)
 **
 ** SW (switch) at Pin P[3][30] (J9 Pin15)s
 ************************************************************************************************************
 ***********************************************************************************************************/

#include "sw_debounce.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
void sw_init(sw_handle_t *handle) {

	memset(handle, 0, sizeof(sw_handle_t));
	handle->ct0 = INT32_MAX;
	handle->ct1 = INT32_MAX;
}

void sw_task(sw_handle_t *handle, uint32_t inputs) {

	uint32_t i;

	i = handle->state ^ inputs;
	handle->ct0 = ~(handle->ct0 & i);
	handle->ct1 = handle->ct0 ^ (handle->ct1 & i);
	i &= handle->ct0 & handle->ct1;
	handle->up |= ~inputs & i;
	handle->state ^= i;
	handle->down |= handle->state & i;
}

uint32_t sw_get_down(sw_handle_t *handle) {

	uint32_t down;

	down = handle->down;
	handle->down = 0U;

	return down;
}

uint32_t sw_get_up(sw_handle_t *handle) {

	uint32_t up;

	up = handle->up;
	handle->up = 0U;

	return up;
}

