/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		camera.h						################
 ** Created on: 	29-10-2023						#####| |########  University of applied sciences
 ** Authors: 		Fürstberger Martin,				#####| |########  Landshut, Germany
 ** 				Simonis Leon,					#####| |########
 ** 				Schütz Tobias,					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				Wagner Martina,                 #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	MF		|04-15-2021	| created ultrasonic.c																|
 ** 2	|			|			| 																			|
 ** 3	|			|			| 																			|
 ** 4	|			|			| 																			|
 ** 5	|			|			| 																			|
 **  	|			|			| 																			|
 ** 6	|			|			|																			|
 ** 7	|			|			|																			|
 ** 8	|			|			|																			|
 ** 9	|			|			|																			|
 ** 10	|			|			|																			|
 ************************************************************************************************************
 **
 **	Description
 ************************************************************************************************************/

#include "ultrasonic.h"
#include "screen.h"

/*******************************************************************************
 * Parameters
 ******************************************************************************/
/*******************************************************************************
 * Ultrasonic Main Initialization function
 ******************************************************************************/
void Ultrasonic_Init(void)
{
	//*******************************
	//Camera Initialization Functions
	MRTIMER0_Init(); 				//MRTIMER0 Initialization
	PIN_INT_INIT();
}
void MRTIMER0_Init(void)
{
	SYSCON->AHBCLKCTRL[1]|=1<<0;		//Enable Multi-Rate-Timer
	SYSCON->PRESETCTRL[1]|=1<<0;		//Clear MRT-Reset
	EnableIRQ(MRT0_IRQn);				//Enable Interrupt
	MRT0->CHANNEL[0].CTRL=1;			//MRT0 Interrupt enabled and Repeat Interrupt Mode used
	MRT0->CHANNEL[0].INTVAL= 110000;	//Timer Interrupt every 110000/220000 = 0,5 ms
}

void MRT0_IRQHandler(uint32_t flags)
{
	MRT0->CHANNEL[0].STAT|=1<<0;			//Clear Interrupt-Flag
}

void PIN_INT_INIT(void)
{

}

