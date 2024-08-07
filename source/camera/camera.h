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
#include "param.h"


/*******************************************************************************
 * Defines
 ******************************************************************************/
enum position {
	right = 1,
    left = 2
};

enum mode {
	init = 0,
	trace = 1
};

enum track {
	track = 0,
	finish = 1,
	tree_stribes = 2,
	four_stribes = 3
};
#define PVsumW 16384				//target value for PixelValue Sum in exposure Time calculation  16384 = 128 * ADC-Medium-Value (0 ... 256)
#define ExposureGainCam	5			//Gain for exposure Time calculation of fist camera
#define ExposureGainCam2 5			//Gain for exposure Time calculation of second camera
#define MaxDiffExpCams 55000		//Maximum in- or decrease value for Exposure time in Timer register 55000 == 0,5 ms
#define MaxExposureTime 22000000 	//Maximum exposure time for cameras is 100 ms

#define RoadWidth 510.0				//Breite der Fahrbahn in mm
/*******************************************************************************
 * Structures
 ******************************************************************************/

struct EdgeDetectionData {
	uint8_t edges[128];
	uint8_t edgesMiddle[128]; 	//all detected edge
	uint8_t edge_left_found;	//1 if found
	uint8_t edge_right_found;	//1 if found
	uint8_t edge_left;			//left Edge Coordinate
	uint8_t edge_right;			//Right Edge Coordinate
	int16_t edge_center;
	int16_t edge_center_mm;
	uint8_t detection_mode;
	uint8_t track_state;
	uint8_t finish_detected; //1 if finish line detected
//	uint8_t camera_distance;	//TODO: implement
};


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
void CTIMER4_Init(void);
void Calculate_PixelValues_Task(void *pvParameters);
void Camera_Exposure_time_task(void *pvParameters);

void Edge_Detection(struct EdgeDetectionData *edgeData, volatile uint8_t *pixelVal); //Ermittlung steigender Flanken (Übergang von schwarz auf weis) und fallender Flanken (Übergang von weiß auf schwarz) anhand der Kameradaten (Pixelwerte)
void Finish_Line(struct EdgeDetectionData *edgeData); //Erkennung der Ziellinie

void menu_func_edge_calibration(void);
void Edge_Detection_Prewitt(struct EdgeDetectionData *edgeData, volatile uint8_t *pixelVal);


#endif /* CAMERA_CAMERA_H_ */

