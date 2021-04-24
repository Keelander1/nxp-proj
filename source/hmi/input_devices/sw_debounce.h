/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		sw_debounce.h					################
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
 ** 1	|	MS		|04-24-2021	| imported sw_debounce.h												|
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

#ifndef HMI_INPUT_DEVICES_SW_DEBOUNCE_H_
#define HMI_INPUT_DEVICES_SW_DEBOUNCE_H_

#include <stdint.h>
#include <string.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Switch handle. */
typedef struct _sw_handle_t {
	volatile uint32_t ct0;
	volatile uint32_t ct1;
	volatile uint32_t state;
	volatile uint32_t down;
	volatile uint32_t up;
} sw_handle_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/*
 * @brief Init rotary encoder handle.
 *
 * Must be called before main task will be executed.
 */
void sw_init(sw_handle_t *handle);

/*!
 * @brief Main task for switch debouncing.
 *
 * Call this function in an ISR every 10ms.
 *
 * @param Up to 32 input states.
 */
void sw_task(sw_handle_t *handle, uint32_t inputs);

/*!
 * @brief Returns all switches which were detected with falling signal edge.
 *
 * @param The switch handle.
 * @retval Signal mask.
 */
uint32_t sw_get_down(sw_handle_t *handle);

/*!
 * @brief Returns all switches which were detected with rising signal edge.
 *
 * @param The switch handle.
 * @retval Signal mask.
 */
uint32_t sw_get_up(sw_handle_t *handle);

#endif /* HMI_INPUT_DEVICES_SW_DEBOUNCE_H_ */
