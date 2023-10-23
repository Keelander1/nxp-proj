/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		param.h							################
 ** Created on: 	04-28-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	MS		|04-xx-2021	| imported param.h														|
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
 ** Header file for display functions:
 **
 ** Contains EEPROM functions
 **
 ************************************************************************************************************
 ***********************************************************************************************************/


#ifndef PARAM_H_
#define PARAM_H_

#include <stdint.h>

#include "fsl_eeprom.h"		//include eeprom registers

#include "drive_param.h"	//include drive parameters

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EEPROM_SOURCE_CLOCK kCLOCK_BusClk				//define EEPROM source clock
#define EEPROM_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)	//get EEPROM frequency

#define PARAM_EEPROM_PAGE_NUM	0						//write to EEPROM page 0

#define PARAM_DATA_SIZE sizeof(all_param_t)

#define FSL_FEATURE_EEPROM_PAGE_SIZE (FSL_FEATURE_EEPROM_SIZE / FSL_FEATURE_EEPROM_PAGE_COUNT)	//calculate EEPROM page size

/*******************************************************************************
 * system_param_t
 * contains system parameters
 * param buzzer:	activate or deactivate buzzer
 ******************************************************************************/

typedef struct _system_param_t {
	uint32_t buzzer;
} system_param_t;

/*******************************************************************************
 * camera_param_t
 * contains camera parameters
 * param throttle:	throttle for flag detection
 ******************************************************************************/
typedef struct _camera_param_t {
	int32_t edge_distance[2];
	uint32_t edge_show;
	uint32_t exposure_show;
//	int8_t calibrationCamera[2][128];
} camera_param_t;

/*******************************************************************************
 * all_param_t
 * contains all parameters
 * param drive:		contains drive parameters
 * param motors:	contains motor parameters
 * param system:	contains system parameters
 ******************************************************************************/
typedef struct _all_param_t {
	drive_param_t drive;
	motor_param_t motors;
	system_param_t system;
	camera_param_t camera;
} all_param_t;


extern uint8_t const_all_param[FSL_FEATURE_EEPROM_PAGE_SIZE];	//array with all parameters in integers
extern all_param_t *all_param;									//pointer to all_param_t

extern int8_t calibrationCamera_storage[2][128];
/*******************************************************************************
 * API
 ******************************************************************************/
/*******************************************************************************
 * PARAM_init
 * init EEPROM functionalit
 ******************************************************************************/
void PARAM_Init();

/*******************************************************************************
 * paramm_save
 * save parameters to EEPROM
 ******************************************************************************/
void param_save();

#endif /* PARAM_H_ */
