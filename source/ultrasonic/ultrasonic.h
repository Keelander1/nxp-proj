/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		camera.h						################
 ** Created on: 	29-10-2023						#####| |########  University of applied sciences
 ** Authors: 		Fürstberger Martin,				#####| |########  Landshut, Germany
 ** 				Simonis Leon,					#####| |########
 ** 				Schütz Tobias,					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				Wagner Martina,                 #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	MF		|04-15-2021	| created ultrasonic.h																|
 ** 2	|			|			| 																			|
 ** 3	|			|			| 																			|
 ** 4	|			|			| 																			|
 ** 5	|			|			| 																			|
 **  	|			|			| 																			|
 ** 6	|			|			|																			|
 ** 7	|			|			|																			|
 ** 8	|			|			|																			|
 ** 9	|			|			|																			|
 ** 10	|			|			|																			|
 ************************************************************************************************************
 **
 **	Description
 ************************************************************************************************************/
#ifndef ULTRASONIC_ULTRASONIC_H_
#define ULTRASONIC_ULTRASONIC_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_common.h"
#include "fsl_ctimer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "board.h"
#include "fsl_sctimer.h"
#include "param.h"


/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Structures
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void Ultrasonic_Init(void);
void MRTIMER0_Init(void);
void PIN_INT_INIT(void);


#endif /* ULTRASONIC_ULTRASONIC_ */
