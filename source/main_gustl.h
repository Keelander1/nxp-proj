/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		main_gustl.h					################
 ** Created on: 	04-20-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	EC/SM	|04-20-2021	| Created main_gustl.h														|
 ** 2	|			|			| 																			|
 ** 3	|			|			| 																			|
 ** 4	|			|			| 																			|
 ** 5	|			|			| 																			|
 ** 6	|			|			| 																			|
 ** 7	|			|			| 																			|
 ** 8	|			|			| 																			|
 ** 9	|			|			| 																			|
 ** 10	|			|			| 																			|
 ************************************************************************************************************
 ***********************************************************************************************************/


#ifndef MAIN_GUSTL_H_
#define MAIN_GUSTL_H_


/*******************************************************************************
 * Includes
 ******************************************************************************/
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Own header files */
#include "servo/servo.h"
#include "drive/drive.h"
#include "camera/camera.h"
#include "rpmMeas/rpmMeas.h"
#include "board/board_func.h"
#include "hmi.h"
#include "param.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include <stdbool.h>


#endif /* MAIN_GUSTL_H_ */
