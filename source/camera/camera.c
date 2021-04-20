/*
 * camera.c
 *
 *  Created on: 12.04.2021
 *      Author: Ecker Christian
 */

#include "camera.h"

struct _camData camData;

void CAM_Init(void)
{
	camData.rawImage = NULL;

	//ADC16_EnableDMA(ADC16_2_PERIPHERAL, true); //
	//ADC16_SetChannelConfig(ADC16_2_PERIPHERAL, 0, ADC16_2_channelsConfig);



	//ADC Trigger EVENT TEST TEST TEST TEST
	//IOCON->PIO[3][26] &= 0xFFFFFFF0; 	//clear FUNC bits of P3.12
	//IOCON->PIO[3][26] |= 0x02;     		//set FUNC bits to SCT0_OUT0 function FUNC P3.27
	//GPIO->DIR[3]      |= 1<<26;         //set PIO3_27 (SCT0_OUT1) to output
	//SCT0->OUT[0].CLR = (1 << 0); 		//Event 0 will clear SCTx_OUT0
	//SCT0->OUT[0].SET = (1 << 2); 		//Event 2 will set SCTx_OUT0

	//Configure PIO3_27 for SCT0_OUT1
	IOCON->PIO[3][27] &= 0xFFFFFFF0; 	//clear FUNC bits of P3.12
	IOCON->PIO[3][27] |= 0x02;     		//set FUNC bits to SCT0_OUT1 function FUNC2 P3.27
	GPIO->DIR[3]      |= 1<<27;         //set PIO3_27 (SCT0_OUT1) to output

	//CLK Config for SCTimer/PWM
	SYSCON->SCTCLKSEL = 0x00; 			//Main Clock for SCTimer/PWM
	SYSCON->SCTCLKDIV = (5-1); 			//CLK Divider 5 -> 220MHz/5 = 44MHz  (SCMax = 100MHz)
	SYSCON->AHBCLKCTRL[1] |= 1<<2; 		//SCTimer/PWM SCT0 CLK Enable
	SYSCON->PRESETCTRLCLR[1] |= 1<<2; 	//Clear the SCTimer/PWM peripheral reset

	//SCT0 --> Use Configured CLK
	SCT0->CONFIG |= 0x3<<1;				//CLKMODE Asynchron with input (->CKSEL)
	SCT0->CONFIG |= 0xE<<3;				//CKSEL Input 7 Rising Edges
	SCT0->CONFIG |= 1<<17; 				//Auto limit (& two 16-bit timers)

	//***************************************************
	//Set PWM at PIO3_27 to 4MHz (Cam_frequency max=8MHz)
	//***************************************************
	//Match 0 for Counter Limit
	SCT0->MATCHREL[0] = (11-1); 		//Match 0 @ 11/44MHz = 250ns Limit Counter
	SCT0->EV[0].STATE = 0xFFFFFFFF; 	//Event 0 happens in all states
	SCT0->EV[0].CTRL = (1 << 12); 		//Match 0 condition only
	SCT0->OUT[1].SET = (1 << 0); 		//Event 0 will set SCT0_OUT1
	//Match 1 for PWM Duty Cycle
	SCT0->MATCHREL[1] = (5-1); 			//Match 1 @ 5/44MHz = 113,64ns
	SCT0->EV[1].STATE = 0xFFFFFFFF; 	//Event 1 happens in all states
	SCT0->EV[1].CTRL = (1 << 0) | (1 << 12); //Match 1 condition only
	SCT0->OUT[1].CLR = (1 << 1); 		//Event 1 will set SCT0_OUT1
	//***************************************************

	//Match 2 for ADC Trigger Event
	SCT0->MATCHREL[2] = (6-1); 			//Match 2 @ 6/44MHz = 136,36ns (Cam_AO settlingTime Min120ns)
	SCT0->EV[2].STATE = 0xFFFFFFFF; 	//Event 2 happens in all states
	SCT0->EV[2].CTRL = (2 << 0) |(1 << 12); //Match 2 condition only

	SCT0->CTRL &= ~(1 << 2); 			//Unhalt it by clearing bit 2 of CTRL

}
