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
/*******************************************************************************
 * sw_init
 * init push button
 ******************************************************************************/
void sw_init(sw_handle_t *handle) {

	memset(handle, 0, sizeof(sw_handle_t));		//set whole memory of handle to zero
	handle->ct0 = INT32_MAX;					//set all bits in ct0 to 1
	handle->ct1 = INT32_MAX;					//set all bits in ct1 to 1
}

/*******************************************************************************
 * sw_task
 * task for debouncing switch BIT0 is SW and BIT1 is ENC_SW
 * param handle:		switch handle
 * param inputs:		switch inputs (BIT0: ENC_SW, BIT1: SW) if pressed BIT is set
 ******************************************************************************/
void sw_task(sw_handle_t *handle, uint32_t inputs) {

	uint32_t i;

	i = handle->state ^ inputs; 					//save changed switch states
	handle->ct0 = ~(handle->ct0 & i);				//reset or count ct0
	handle->ct1 = handle->ct0 ^ (handle->ct1 & i);	//reset or count ct1
	i &= handle->ct0 & handle->ct1;					//count until roll over
	handle->up |= ~inputs & i;						//bit changed from 0->1: key press detected
	handle->state ^= i;								//then toggle debounced state
	handle->down |= handle->state & i;				//down is a bitmask of all pressed inputs
}
/*******************************************************************************
 * sw_get_down
 * Returns a bitmask of all switches which were detected with falling signal edge.
 * param handle:		switch handle
 ******************************************************************************/
uint32_t sw_get_down(sw_handle_t *handle) {

	uint32_t down;

	down = handle->down;	//get down value
	handle->down = 0U;		//reset down value

	return down;			//return down
}
/*******************************************************************************
 * sw_get_up
 * Returns a bitmask of all switches which were detected with rising signal edge.
 * param handle:		switch handle
 ******************************************************************************/
uint32_t sw_get_up(sw_handle_t *handle) {

	uint32_t up;

	up = handle->up;		//get up value
	handle->up = 0U;		//reset up value

	return up;				//return up
}

