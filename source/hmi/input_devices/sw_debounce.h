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
 * sw_handle_t
 * switch handle
 * param ct0:		ct0 forms with ct1 32 2Bit counters (ct0 is BIT0)
 * param ct1:		ct1 forms with ct0 32 2Bit counters (ct1 is BIT1)
 * param state:		contains all states of all inputs
 * param down:		bitmask of all pressed buttons
 * param up:		bitmask of all released buttons
 ******************************************************************************/
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

/*******************************************************************************
 * sw_init
 * init push button
 * Must be called before main task will be executed
 ******************************************************************************/
void sw_init(sw_handle_t *handle);

/*******************************************************************************
 * sw_task
 * task for debouncing switch BIT0 is SW and BIT1 is ENC_SW
 * param handle:		switch handle
 * param inputs:		switch inputs (BIT0: ENC_SW, BIT1: SW) if pressed BIT is set
 * Call this function in an ISR every 10ms.
 * https://www.mikrocontroller.net/articles/Entprellung; Sec Timer-Verfahren (nach Peter Dannegger)
 ******************************************************************************/
void sw_task(sw_handle_t *handle, uint32_t inputs);

/*******************************************************************************
 * sw_get_down
 * Returns a bitmask of all switches which were detected with falling signal edge.
 * param handle:		switch handle
 ******************************************************************************/
uint32_t sw_get_down(sw_handle_t *handle);

/*******************************************************************************
 * sw_get_up
 * Returns a bitmask of all switches which were detected with rising signal edge.
 * param handle:		switch handle
 ******************************************************************************/
uint32_t sw_get_up(sw_handle_t *handle);

#endif /* HMI_INPUT_DEVICES_SW_DEBOUNCE_H_ */
