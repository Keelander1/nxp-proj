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
volatile uint32_t USS_Distance=0;
volatile uint32_t USS_Distance_Counter=0;
volatile uint8_t USS_Count=0;


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
	SYSCON->PRESETCTRL[1]&=~(1<<0);		//Clear MRT-Reset
	EnableIRQ(MRT0_IRQn);				//Enable Interrupt
	MRT0->CHANNEL[0].CTRL=1;			//MRT0 Interrupt enabled and Repeat Interrupt Mode used
	MRT0->CHANNEL[0].INTVAL= 11000;	//Timer Interrupt every 11000/220000 = 50 us
}

void MRT0_IRQHandler(uint32_t flags)
{
	if((MRT0->CHANNEL[0].STAT & 1) == 1){
		if(USS_Count==1){					//PIN is High
		USS_Distance_Counter++;				//Timer counts up
		}
		MRT0->CHANNEL[0].STAT|=1<<0;		//Clear Interrupt-Flag
	}
	if((MRT0->CHANNEL[1].STAT & 1) == 1){
		GPIO->NOT[2] |=(1<<1);				//Toggle Buzzer
		MRT0->CHANNEL[1].STAT|=1<<0;		//Clear Interrupt-Flag
	}
}

void PIN_INT_INIT(void)
{
	SYSCON->AHBCLKCTRL[0] |= 1<<18;		//Enable clock for pin-Interrupt
	//Configure Pin P[0][23] (J13 Pin8) (Pin-Interrupt)
	IOCON->PIO[0][23] &= 0xFFFFFFFF0; 	//Clear FUNC bits of P0.16 Func 0 is GPIO-Pin
	GPIO->DIR[0]      &= ~(1 << 23);    //Set PIO0_23  to input
	SYSCON->AHBCLKCTRL[0] |= (1<<11);	//Enable clock for Input MUX
	INPUTMUX->PINTSEL[0]=0*32+23;		//Connect Pin 0_23 ==> (0)*32+(23) with Pin Interrupt 0
	SYSCON->AHBCLKCTRL[0] &= ~(1<<11);	//Disable clock for Input MUX
	PINT->ISEL &= ~(1<<0);				//Interrupt 0 is edge Sensitive
	PINT->SIENR	|= 1<<0;				//Rinsing Edge Interrupt enabled
	PINT->SIENF |= 1<<0;				//Falling Edge Interrupt enabled
	EnableIRQ(PIN_INT0_IRQn);			//Enable pin Interrupt
	PINT->IST|=1<<0;					//Clear Interrupt Flags
}
void PIN_INT0_IRQHandler(uint32_t flags)
{
	if ((PINT->RISE & 1)==1){			//Rising Edge detected
		USS_Distance=USS_Distance_Counter;	//Distance is stored in USS_Distance
		USS_Count=0;					//Timer stops counting up
		PINT->RISE|=(1<<0);				//Reset Rising edge Interrupt
	}
	if ((PINT->FALL & 1)==1){				//Falling Edge detected
		USS_Count=1;						//Timer starts counting up
		MRT0->CHANNEL[0].INTVAL|= 1<<31;	//Timer resets to 0
		USS_Distance_Counter=0;				//Reset Distance Counter
		PINT->FALL|=(1<<0);					//Reset Falling edge Interrupt
	}
}
