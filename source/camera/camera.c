/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		camera.c						################
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
 ** 1	|	CE		|04-15-2021	| Created camera.c															|
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
 ** Header file for camera initialization functions:
 **
 ** Contains camera initialization functions
 **
 ** SCTimer/PWM: 	--> Event 0 is timer overflow (sets SCT0_OUT1 (CAM_CLK))
 **					--> Event 1 is for duty Cycle (resets SCT0_OUT1 (CAM_CLK))
 **					--> Event 2 is ADC trigger
 **
 ** CAM_CLK (Camera Clock) at Pin P[3][27] (J13 Pin13) Clock Signal in range 5kHz to 8MHz --> here 4MHz
 ** CAM_SI (Camera Serial Input) at Pin P[3][26] (J13 Pin15) Serial Input for Start taking picture
 ** CAM_AO (Camera Analog Output) at Pin P[0][16] (J12 Pin2) Analog Output of camera (Pixel brightness)
 ************************************************************************************************************
 ***********************************************************************************************************/


#include "camera.h"


/*******************************************************************************
 * Camera Main Initialization function
 ******************************************************************************/
void CAM_Init(void)
{
	//******************************
	//Configure Pin P[3][27] (J13 Pin13) (CAM_CLK/SCT0_OUT1)
	IOCON->PIO[3][27] &= 0xFFFFFFF0; 	//Clear FUNC bits of P3.12
	IOCON->PIO[3][27] |= 0x02;     		//Set FUNC bits to SCT0_OUT1 function FUNC2 P3.27
	GPIO->DIR[3]      |= 1<<27;         //Set PIO3_27 (SCT0_OUT1) to output
	//******************************

	//*******************************
	//Configure CLK for SCTimer/PWM
	SYSCON->SCTCLKSEL = 0x00; 			//Main Clock for SCTimer/PWM
	SYSCON->SCTCLKDIV = (5-1); 			//CLK Divider 5 -> 220MHz/5 = 44MHz  (SCMax = 100MHz)
	SYSCON->AHBCLKCTRL[1] |= 1<<2; 		//SCTimer/PWM SCT0 CLK Enable
	SYSCON->PRESETCTRLCLR[1] |= 1<<2; 	//Clear the SCTimer/PWM peripheral reset
	//*******************************

	//*******************************
	//SCT0 --> Use Configured CLK
	SCT0->CONFIG |= 0x3<<1;				//CLKMODE asynchronous with input (->CKSEL)
	SCT0->CONFIG |= 0xE<<3;				//CKSEL Input 7 Rising Edges
	SCT0->CONFIG |= 1<<17; 				//Auto limit (& two 16-bit timers)
	//*******************************

	//***************************************************
	//Set PWM at PIO3_27 to 4MHz (Cam_frequency max=8MHz)
	//***************************************************
	//Match 0 for Counter Limit
	SCT0->MATCHREL[0] = (11-1); 			//Match 0 @ 11/44MHz = 250ns Limit Counter
	SCT0->EV[0].STATE = 0xFFFFFFFF; 		//Event 0 happens in all states
	SCT0->EV[0].CTRL = (1 << 12); 			//Match 0 condition only
	SCT0->OUT[1].SET = (1 << 0); 			//Event 0 will set SCT0_OUT1
	//Match 1 for PWM Duty Cycle
	SCT0->MATCHREL[1] = (5-1); 				//Match 1 @ 5/44MHz = 113,64ns
	SCT0->EV[1].STATE = 0xFFFFFFFF; 		//Event 1 happens in all states
	SCT0->EV[1].CTRL = (1 << 0) | (1 << 12); //Match 1 condition only
	SCT0->OUT[1].CLR = (1 << 1); 			//Event 1 will set SCT0_OUT1
	//***************************************************

	//**************************************
	//Match 2 for ADC Trigger Event
	SCT0->MATCHREL[2] = (6-1); 				//Match 2 @ 6/44MHz = 136,36ns (Cam_AO settlingTime Min120ns)
	SCT0->EV[2].STATE = 0xFFFFFFFF; 		//Event 2 happens in all states
	SCT0->EV[2].CTRL = (2 << 0) |(1 << 12); //Match 2 condition only
	//**************************************

	SCT0->CTRL &= ~(1 << 2); 			//Unhalt SCT0 by clearing bit 2 of CTRL

}

	//ADC Trigger EVENT TEST TEST TEST TEST
	//IOCON->PIO[3][26] &= 0xFFFFFFF0; 	//clear FUNC bits of P3.12
	//IOCON->PIO[3][26] |= 0x02;     		//set FUNC bits to SCT0_OUT0 function FUNC P3.27
	//GPIO->DIR[3]      |= 1<<26;         //set PIO3_27 (SCT0_OUT1) to output
	//SCT0->OUT[0].CLR = (1 << 0); 		//Event 0 will clear SCTx_OUT0
	//SCT0->OUT[0].SET = (1 << 2); 		//Event 2 will set SCTx_OUT0
