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
#define CTIMER3_PWM_LEFT_CHANNEL kCTIMER_Match_0 	//Define channel 0 as match register for the left BLDC
#define CTIMER3_PWM_RIGHT_CHANNEL kCTIMER_Match_2	//Define channel 2 as match register for the right BLDC

/* BLDC PWM initialization value definitions */
#define BLDC_PWM_INIT_HIGH_VALUE 330000	//Init High value Sequence 1,5ms
#define BLDC_PWM_INIT_LOW_VALUE 220000	//Init Low value Sequence 1,0ms
#define CTIMER3_PWM_0_DUTY 0            //Definition of initialization duty
#define CTIMER3_PWM_PERIOD 4400000			//Definition of PWM period

enum direction{
	left_corner = -1,
	right_corner = 1,
};

/*******************************************************************************
 * Control System
 ******************************************************************************/
#define t 20.0

#define  Cv 25
#define  Ch  40
#define  m  2.5
#define  V  1.5
#define  Lh 0.175
#define  Lv 0.175
#define  J 0.087
#define  D_ 0.45
	//ReglermatrixA
#define a11 ((-Cv-Ch) / (m*V))
#define a12 ((-Cv*Lv+Ch*Lh)/(m*V*V)-1)
#define a13 0.0
#define a14 0.0

#define a21 (-Cv*Lv+Ch*Lh)/J
#define a22 -(Lv*Lv*Cv+Lh*Lh*Ch)/(J*V)
#define a23 0.0
#define a24 0.0

#define a31 0.0
#define a32 -1.0
#define a33 0.0
#define a34 0.0

#define a41 -V
#define a42 -D_
#define a43 V
#define a44 0.0


	//Reglermatrix B
#define B1 Cv / (m*V)
#define B2 (Cv*Lv) / J
#define B3 0.0
#define B4 0.0

	//Reglermatrix C
#define c1 0.0
#define c2 0.0
#define c3 0.0
#define c4 1.0

	//Beobachter L
/*
#define l1 -2.1993
#define l2 48.6350
#define l3 21.2867
#define l4 12.6026

//Regler K
#define k1 1.6289
#define k2 0.2159
#define k3 -2.6515
#define k4 -10.4811
#define KYI -20.4
*/
#define Windup -0.1;
extern int k_1;
extern int k_2;
extern int k_3;
extern int k_4;
extern int k_yi;
extern int l_1;
extern int l_2;
extern int l_3;
extern int l_4;
extern int Speed_Param;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BLDC_Init(void);
void CTIMER3_Init(void);
void delay_ms(uint32_t value);
void ESC_Init_Task(void *pvParameters);
void stear(float angle);		//angle in rad
void Camera_Test_Drive (uint8_t state);
void Real_Drive (uint8_t state);
void StateControl(uint8_t state);
#endif /* DRIVE_DRIVE_H_ */


