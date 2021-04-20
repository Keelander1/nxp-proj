/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		rpmMeas.h						################
 ** Created on: 	04-19-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	| CE/SM/AM  |04-19-2021	| Created rpmMeas.h															|
 ** 2	|	CE		|04-20-2021	| Commentated the code																			|
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
 ** Header file for speed-measurement initialization functions:
 **
 ** Contains includes, defines and function prototypes
 **
 ** CTIMER2 with Register Callback (Capture Channels 0 and 1)
 **
 ** Left  BLDC Phase 1 Input at Pin P[0][24] (J13 Pin6) Capture Channel 0
 ** Right BLDC Phase 1 Input at Pin P[0][25] (J13 Pin4) Capture Channel 1
 ************************************************************************************************************
 ***********************************************************************************************************/

#ifndef RPMMEAS_RPMMEAS_H_
#define RPMMEAS_RPMMEAS_H_


/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_ctimer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "board.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define CTIMER2_RPMMEAS_LEFT_CHANNEL kCTIMER_Capture_0 	//Left BLDC Phase 1 Channel 0
#define CTIMER2_RPMMEAS_RIGHT_CHANNEL kCTIMER_Capture_1	//Left BLDC Phase 1 Channel 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void RPMMEAS_Init(void);
void CTIMER2_Init(void);
void rpmMeas_Left_ISR(uint32_t);
void rpmMeas_Right_ISR(uint32_t);


#endif /* RPMMEAS_RPMMEAS_H_ */
