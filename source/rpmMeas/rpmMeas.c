/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		rpmMeas.c						################
 ** Created on: 	04-19-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	| CE/SM/AM  |04-19-2021	| Created rpmMeas.c															|
 ** 2	|	CE		|04-20-2021	| Commentated the code																			|
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
 ** C-file for speed-measurement initialization functions:
 **
 ** Contains speed-measurement initialization functions
 **
 ** CTIMER2 with Register Callback (Capture Channels 0 and 1)
 **
 ** Left  BLDC Phase 1 Input at Pin P[0][24] (J13 Pin6) Capture Channel 0
 ** Right BLDC Phase 1 Input at Pin P[0][25] (J13 Pin4) Capture Channel 1
 ************************************************************************************************************
 ***********************************************************************************************************/


#include "rpmMeas.h"

/*******************************************************************************
 * Parameters
 ******************************************************************************/
const ctimer_config_t RPMMEAS_config = {
		.mode = kCTIMER_TimerMode,   /* TC is incremented every rising APB bus clock edge */
		.input = kCTIMER_Capture_0,  /*!< Timer capture channel 0 */
		.prescale = 0                /*!< Prescale value 0 --> */
};

//CTIMER callback table contains functions for Match Event 0 to 3 and Capture Event 0 to 3
ctimer_callback_t ctimer_callback_table[] = {NULL, NULL, NULL, NULL, rpmMeas_Left_ISR, rpmMeas_Right_ISR, NULL, NULL};


/*******************************************************************************
 * Speed measurement main initialization function
 ******************************************************************************/
void RPMMEAS_Init()
{
	CTIMER2_Init();
	//*************************************************************
	//Configure Pin P[0][24] (J13 Pin6) (Left BLDC Phase 1)
	IOCON->PIO[0][24]       &= 0xFFFFFFF0;      //clear FUNC bits of P0.24
	IOCON->PIO[0][24]     	|= 0x3;           	//set FUNC bits to CTIMER2_CAP0 function FUNC3 P0.24
	GPIO->DIR[0]           	&= ~1<<12;         	//set P0.24 pin to input
	//*************************************************************

	//*************************************************************
	//Configure Pin P[0][25] (J13 Pin4) (Right BLDC Phase 1)
	IOCON->PIO[0][25]       &= 0xFFFFFFF0;      //clear FUNC bits of P0.25
	IOCON->PIO[0][25]     	|= 0x3;           	//set FUNC bits to CTIMER2_CAP1 function FUNC3 P0.25
	GPIO->DIR[0]           	&= ~1<<12;         	//set P0.25 pin to input
	//*************************************************************

}


/*******************************************************************************
 * TIMER2 Initialization function
 ******************************************************************************/
void CTIMER2_Init(void)
{
	/* CTIMER1 peripheral initialization */
	CTIMER_Init(CTIMER2, &RPMMEAS_config);

	/* Set Callback functions */
	CTIMER_RegisterCallBack(CTIMER2, &ctimer_callback_table[0], kCTIMER_MultipleCallback);

	//Setup Capture Left Drive Capture Channel 0
	CTIMER_SetupCapture(CTIMER2, CTIMER2_RPMMEAS_LEFT_CHANNEL, kCTIMER_Capture_RiseEdge, 1);

	//Setup Capture Right Drive Capture Channel 1
	CTIMER_SetupCapture(CTIMER2, CTIMER2_RPMMEAS_RIGHT_CHANNEL, kCTIMER_Capture_RiseEdge, 1);

	/* Start the timer */
	CTIMER_StartTimer(CTIMER2);
}


/*******************************************************************************
 * Interrupt Service Routine for rising edge of Phase 1 of left BLDC
 ******************************************************************************/
void rpmMeas_Left_ISR(uint32_t flags)
{

}


/*******************************************************************************
 * Interrupt Service Routine for rising edge of Phase 1 of right BLDC
 ******************************************************************************/
void rpmMeas_Right_ISR(uint32_t flags)
{

}






