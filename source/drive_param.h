/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		param.h							################
 ** Created on: 	05-03-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	MS		|05-xx-2021	| created drive_param.h														|
 ** 2	|	TS		|30-05-2023	| _drive_param_t: stop, limit: uint32_t-> int32_t							|
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

#ifndef DRIVE_PARAM_H_
#define DRIVE_PARAM_H_

typedef struct _drive_param_t {
	int32_t speed;
	int32_t stop;
	int32_t limit;
	int32_t object;
	int32_t l_l;
	int32_t l_r;
} drive_param_t;

typedef struct _motor_chnl_param_t {
	int32_t init;
	int32_t min;
	int32_t max;
	uint32_t fac;
} motor_chnl_param_t;

typedef struct _motor_param_t {
	motor_chnl_param_t servo;
	motor_chnl_param_t BLDCLeft;
	motor_chnl_param_t BLDCRight;
} motor_param_t;

typedef struct _state_control_t {
	float testParam;
	float testParam2;
} state_control_t;

#endif /* DRIVE_PARAM_H_ */
