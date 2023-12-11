/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		board_func.c					################
 ** Created on: 	04-20-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	CE		|04-20-2021	| Created board_func.c														|
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
 ** C-file for board initialization functions:
 **
 ** Contains board initialization functions
 ************************************************************************************************************
 ***********************************************************************************************************/

#include "board_func.h"


/*******************************************************************************
 * LED1 blinking task (Board running feedback)
 ******************************************************************************/
void LED_Blink_Task(void *pvParameters) //f = 1Hz
{
    while(1){
    	LED1_TOGGLE();
        vTaskDelay(500);
    }
}


/*******************************************************************************
 * Initialize board peripherals
 ******************************************************************************/
void Board_Init(void)
{
	//**********
	//Reset LEDs
	LED1_OFF();	//Turn LED1 off
	LED2_OFF(); //Turn LED2 off
	LED3_OFF(); //Turn LED3 off
	//**********

	CLKOUT_Init(); //Initialize Port 3 Pin 12 for CLKOUT Function (J12 Pin 9)

	//Config Buzzer
	IOCON->PIO[2][1] &= 0xFFFFFFFF0; 	//Clear FUNC bits of P2.1 Func 0 is GPIO-Pin
	GPIO->DIR[2]      |= (1 << 1);    	//Set PIO2_1  to output
}


/*******************************************************************************
 * Initialize Port 3 Pin 12 for CLKOUT Function (J12 Pin 9)
 ******************************************************************************/
void CLKOUT_Init(void)
{
	//***********************************
	//Set Port3 Pin 12 to CLKOUT Function
	IOCON->PIO[3][12]       &= 0xFFFFFFF0;      //clear FUNC bits of P3.12
	IOCON->PIO[3][12]     	|= 0x5;           	//set FUNC bits to CLKOUT function FUNC5 P3.12
	GPIO->DIR[3]           	|= 1<<12;         	//set CLKOUT pin to output (sec. 12.3.2)
	SYSCON->CLKOUTSELA 		= 0x04;            	//sec. 3.5.21
	SYSCON->CLKOUTDIV    	= 219;              //set divider to 220 --> sees 1MHz (sec. 3.5.23)
	//***********************************
}
