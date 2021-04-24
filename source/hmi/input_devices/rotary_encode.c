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
static const int8_t re_table[] = {0,0,-1,0,0,0,0,1,1,0,0,0,0,-1,0,0};

/*******************************************************************************
 * Code
 ******************************************************************************/
void re_init(re_handle_t *handle) {

	memset(handle, 0, sizeof(re_handle_t));
}

void re_task(re_handle_t *handle, uint8_t a, uint8_t b) {

	handle->last = (handle->last << 2U) & 0x0F;
	if (a) handle->last |= 2U;
	if (b) handle->last |= 1U;
	handle->state += re_table[handle->last];

	if (((handle->last & 0x0A) == 0x0A) && (handle->state != 0)) {
		handle->position += handle->state;
		handle->state = 0;
	}
}

int32_t re_get_pos(re_handle_t *handle) {

	int32_t position;

	position = handle->position;
	handle->position = 0;

	return position;
}

