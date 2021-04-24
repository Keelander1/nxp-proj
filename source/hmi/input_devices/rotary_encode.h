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
/*! @brief Rotary encoder handle. */
typedef struct {
	volatile uint8_t last;
	volatile int8_t state;
	volatile int32_t position;
} re_handle_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/*
 * @brief Init rotary encoder handle.
 *
 * Must be called if memory is not initialized to 0.
 */
void re_init(re_handle_t *handle);

/*!
 * @brief Main task for rotary encoder decoding.
 *
 * Call this function in an ISR every 1ms.
 */
void re_task(re_handle_t *handle, uint8_t a, uint8_t b);

/*!
 * @brief Returns the current position.
 *
 * @param The rotary encoders handle.
 */
int32_t re_get_pos(re_handle_t *handle);


#endif /* HMI_INPUT_DEVICES_ROTARY_ENCODE_H_ */
