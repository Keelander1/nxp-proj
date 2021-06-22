/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		camera.h						################
 ** Created on: 	04-15-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	CE		|04-15-2021	| Created camera.h															|
 ** 2	|	CE		|04-20-2021	| Commentated the code														|
 ** 3	|	CE		|05-21-2021	| Coded ADC_Config															|
 ** 4	|	CE		|05-31-2021	| Coded function SI for CLK and rearranged functions for better readability	|
 ** 5	|	CE		|06-07-2021	| Coded ADC with hardware trigger from SCTIMER (Voltage & Logical Values not|
 **  	|			|			| right yet)																|
 ** 6	|	CE		|06-17-2021	| Finished camera.c and camera.h											|
 ** 7	|			|			|																			|
 ** 8	|			|			|																			|
 ** 9	|			|			|																			|
 ** 10	|			|			|																			|
 ************************************************************************************************************
 **
 **	Description
 ************************************************************************************************************
 ** Header file for camera initialization functions:
 **
 ** Contains includes and function prototypes
 **
 ** SCTimer/PWM: 	--> Event 0 is timer overflow (sets SCT0_OUT1 (CAM_CLK))
 **					--> Event 1 is for duty Cycle (resets SCT0_OUT1 (CAM_CLK))
 **					--> Event 2 is for Setting the Start Signal of the Camera (sets SCT0_OUT0 (CAM_SI))
 **					--> Event 3 is for Clearing the Start Signal of the Camera (clears SCT0_OUT0 (CAM_SI))
 **					--> Event 4 is the ADC Trigger
 **
 ** CAM_CLK (Camera Clock) at Pin P[3][27] (J13 Pin13) Clock Signal in range 5kHz to 8MHz --> here 4MHz
 ** CAM_SI (Camera Serial Input) at Pin P[3][26] (J13 Pin15) Serial Input for Start taking picture
 ** CAM_AO (Camera Analog Output) at Pin P[0][16] (J12 Pin2) Analog Output of camera (Pixel brightness)
 ************************************************************************************************************
 ***********************************************************************************************************/


#ifndef CAMERA_CAMERA_H_
#define CAMERA_CAMERA_H_

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


/*******************************************************************************
 * Defines
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void CAM_Init(void);
void SCTimer_Clock_Config(void);
void SCTimer_CamCLK_Init(void);
void SCTimer_SIEvents_Init(void);
void SCTimer_ADCTrigger_Init(void);
void ADC_Config(void);
void ADC_Calibration(void);
void CTIMER0_Init(void);

void Calculate_PixelValues_Task(void *pvParameters);

#endif /* CAMERA_CAMERA_H_ */
