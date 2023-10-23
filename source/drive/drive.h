/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		drive.c							################
 ** Created on: 	04-12-2020						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	EC/SM	|04-12-2020	| Created drive.c															|
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
 ** Header file for initialization of the BLDC-drives:
 **
 ** Contains includes, defines and function prototypes
 **
 ** CTIMER3 PWM 0/2 Channel --> Write to CTIMER1->MSR[2] for PWM (Right drive)
 **							--> Write to CTIMER1->MSR[0] for PWM (Right drive)
 **                      	--> Use only values in Range 240000-418000 !!!!
 **
 ** Left  BLDC PWM at Pin P[3][10] (J13 Pin7)
 ** Right BLDC PWM at Pin P[0][27] (J13 Pin12)
 ************************************************************************************************************
 ***********************************************************************************************************/


#ifndef DRIVE_DRIVE_H_
#define DRIVE_DRIVE_H_


/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_ctimer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "board.h"
#include "param.h"
#include "menu.h"
#include "servo/servo.h"
#include "camera/camera.h"
#include "hmi/screen.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_CoreSysClk) //Get Main-CLK Frequency

/* Channel definitions */
#define CTIMER3_PWM_LEFT_CHANNEL kCTIMER_Match_0 	//Definition of channel 0 ID
#define CTIMER3_PWM_RIGHT_CHANNEL kCTIMER_Match_2	//Definition of channel 2 ID

/* BLDC PWM initialization value definitions */
#define BLDC_PWM_INIT_HIGH_VALUE 330000	//Init High value Sequence 1,5ms
#define BLDC_PWM_INIT_LOW_VALUE 220000	//Init Low value Sequence 1,0ms
#define CTIMER3_PWM_0_DUTY 0            //Definition of initialization duty
#define CTIMER3_PWM_PERIOD 4400000			//Definition of PWM period

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BLDC_Init(void);
void CTIMER3_Init(void);
void delay_ms(uint32_t value);
void ESC_Init_Task(void *pvParameters);
void Camera_Test_Drive (uint8_t state);
void Real_Drive (uint8_t state);
void StateControl(uint8_t state);
#endif /* DRIVE_DRIVE_H_ */
