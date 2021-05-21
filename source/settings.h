/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		settings.h						################
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
 ** 1	|	MS		|04-xx-2021	| imported settings.h															|
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
 ** Contains setting transfer functions for peripheral
 **
 ************************************************************************************************************
 ***********************************************************************************************************/
#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdint.h>

#include "menu.h"
#include "param.h"
#include "servo/servo.h"
//#include "drive_output.h"
#include "screen.h"

/*******************************************************************************
 * menu_func_servo_motor_init
 * init servo motor to middle positoion
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_servo_motor_init(uint8_t state);

/*******************************************************************************
 * menu_func_servo_motor_left
 * set max left steeringangle
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_servo_motor_left(uint8_t state);

/*******************************************************************************
 * menu_func_servo_motor_right
 * set max right steering angle
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_servo_motor_right(uint8_t state);

/*******************************************************************************
 * menu_func_bldc_motor_left_init
 * init left BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_left_init(uint8_t state);

/*******************************************************************************
 * menu_func_bldc_motor_left_min
 * set min value for left BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_left_min(uint8_t state);

/*******************************************************************************
 * menu_func_bldc_motor_left_max
 * set max value for left BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_left_max(uint8_t state);

/*******************************************************************************
  * menu_func_bldc_motor_right_init
 * init right BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_right_init(uint8_t state);

/*******************************************************************************
 * menu_func_bldc_motor_right_min
 * set min value for right BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_right_min(uint8_t state);

/*******************************************************************************
 * menu_func_bldc_motor_right_max
 * set max value for left BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_right_max(uint8_t state);

/*******************************************************************************
 * menu_func_restore
 * restore all parameter to default
 ******************************************************************************/
void menu_func_restore();

#endif /* SETTINGS_H_ */
