/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		servo.h							################
 ** Created on: 	11-20-2020						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	EC/SM	|11-20-2020	| Created servo.h															|
 ** 2	|	EC		|04-20-2021	| Commented the Code														|
 ** 3	|			|			| 																			|
 ** 4	|			|			| 																			|
 ** 5	|			|			| 																			|
 ** 6	|			|			| 																			|
 ** 7	|			|			| 																			|
 ** 8	|			|			| 																			|
 ** 9	|			|			| 																			|
 ** 10	|			|			| 																			|
 ************************************************************************************************************
 **
 **	Description
 ************************************************************************************************************
 ** Header file for initialization of the servo-drive (Steering):
 **
 ** Contains includes, defines and function prototypes
 **
 ** CTIMER1 PWM 2 Channel  --> Write to CTIMER1->MSR[2] for alternating steering angle
 **                        --> Use only values in Range 200000-459000 !!!!
 **                        --> 200000 < x < 327000 -> steer left
 **                        --> 327000 < x < 459000 -> steer right
 **
 ** Servo-drive PWM at Pin P[3][2] (J13 Pin11)
 ************************************************************************************************************
 ***********************************************************************************************************/


#ifndef SERVO_SERVO_H_
#define SERVO_SERVO_H_


/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_ctimer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "board.h"
#include "param.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/* Channel definitions */
#define CTIMER1_PWM_0_CHANNEL kCTIMER_Match_2 	//Definition of channel 2 ID

/* Servo PWM initialization value definitions */
#define CTIMER1_PWM_0_DUTY 0					//Definition of channel 2 duty  --> on ticks
#define CTIMER1_PWM_PERIOD 4400000				//Definition of PWM period --> whole period ticks


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void SERVO_Init(void);
void CTIMER1_Init(void) ;
void SERVO_Demo(void *pvParameters);



#endif /* SERVO_SERVO_H_ */


