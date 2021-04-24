/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		input_devices.h					################
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
 ** 1	|	MS		|04-24-2021	| imported input_devices.h														|
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

#ifndef HMI_INPUT_DEVICES_INPUT_DEVICES_H_
#define HMI_INPUT_DEVICES_INPUT_DEVICES_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"

#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_rit.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ID_BUTTON_0		(0)
#define ID_BUTTON_1		(1)
#define ID_ENCODER_0	(2)

#define ID_BUTTON_MASK	((1 << ID_EVENT_BUTTON_0) | (1 << ID_EVENT_BUTTON_1))
#define ID_ENCODER_MASK	(1 << ID_EVENT_ENCODER_0)
#define ID_MASK			(ID_BUTTON_MASK | ID_ENCODER_MASK)

/*******************************************************************************
 * API
 ******************************************************************************/
void id_init();
uint32_t id_get_button_state();
int32_t id_get_encoder_state();


#endif /* HMI_INPUT_DEVICES_INPUT_DEVICES_H_ */
