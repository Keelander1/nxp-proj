/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		rotary_encode.c					################
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
 ** 1	|	MS		|04-24-2021	| imported rotary_encode.c												|
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
 ** contains encoder functions
 **
 ** ENC_A (Encoder signal A) at Pin P[3][20] (J9 Pin9)
 ** ENC_B (Encoder signal B) at Pin P[3][22] (J9 Pin11)
 ** ENC_SW (Encoder switch) at Pin P[3][21] (J9 Pin13)
 **
 ** SW (switch) at Pin P[3][30] (J9 Pin15)s
 ************************************************************************************************************
 ***********************************************************************************************************/

#include "rotary_encode.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * re_init
 * determines if rotary encoder rotates clockwise (1) or counterclockwise (-1)
 ******************************************************************************/
static const int8_t re_table[] = {0,0,-1,0,0,0,0,1,1,0,0,0,0,-1,0,0};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*******************************************************************************
 * re_init
 * init rotary encoder
 ******************************************************************************/
void re_init(re_handle_t *handle) {

	memset(handle, 0, sizeof(re_handle_t));		////set whole memory of handle to zero
}

/*******************************************************************************
 * re_task
 * rotary encoder task; get direction of rotation and amount of steps
 * param handke:		rotary encoder handle
 * param a:				signal A
 * param b:				signal B
 ******************************************************************************/
void re_task(re_handle_t *handle, uint8_t a, uint8_t b) {

	handle->last = (handle->last << 2U) & 0x0F;						//save old value(a in BIT3 and b BIT2)
	if (a) handle->last |= 2U;										//save current a value in BIT1
	if (b) handle->last |= 1U;										//save current b value in BIT0
	handle->state += re_table[handle->last];						//get encoder state from re_table

	/*******************************************************************************
	if a_old and a = 1 (every dent)
	an if state != 0 (update only one time)
	update position
	 ******************************************************************************/
	if (((handle->last & 0x0A) == 0x0A) && (handle->state != 0)) {	//if last >= 10 and state !=0
		handle->position += handle->state;		//add state to position
		handle->state = 0;						//reset state
	}
}

/*******************************************************************************
 * re_get_pos
 * return and reset position
 * param handke:		rotary encoder handle
 ******************************************************************************/
int32_t re_get_pos(re_handle_t *handle) {

	int32_t position;

	position = handle->position;	//get position
	handle->position = 0;			//reset position

	return position;				//return position
}

