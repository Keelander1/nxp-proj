/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		rotary_encode.h					################
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
 ** 1	|	MS		|04-24-2021	| imported rotary_encode.h												|
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

#ifndef HMI_INPUT_DEVICES_ROTARY_ENCODE_H_
#define HMI_INPUT_DEVICES_ROTARY_ENCODE_H_

#include <stdint.h>
#include <string.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * re_handle_t
 * rotary encoder functionality
 * param last:		last state of signal a and signal b
 * param state:		actuall amount of rotations
 * param position:	if position > 0: clockwise rotation; if < 0: coutnerclockwise rotation; if = 0: no rotation
 ******************************************************************************/
typedef struct {
	volatile uint8_t last;
	volatile int8_t state;
	volatile int32_t position;
} re_handle_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*******************************************************************************
 * re_init
 * init rotary encoder
 * Must be called if memory is not initialized to 0.
 ******************************************************************************/
void re_init(re_handle_t *handle);

/*******************************************************************************
 * re_task
 * rotary encoder task; get direction of rotation and amount of steps
 * param handke:		rotary encoder handle
 * param a:				signal A
 * param b:				signal B
 * Call this function in an ISR every 1ms
 ******************************************************************************/
void re_task(re_handle_t *handle, uint8_t a, uint8_t b);

/*******************************************************************************
 * re_get_pos
 * return and reset position
 * param handke:		rotary encoder handle
 ******************************************************************************/
int32_t re_get_pos(re_handle_t *handle);


#endif /* HMI_INPUT_DEVICES_ROTARY_ENCODE_H_ */
