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
 ** Contains camera initialization functions
 **
 ** SCTimer/PWM: 	--> Event 0 is timer overflow (sets SCT0_OUT1 (CAM_CLK))
 **					--> Event 1 is for duty Cycle (clears SCT0_OUT1 (CAM_CLK))
 **					--> Event 2 is for Setting the Start Signal of the Camera (sets SCT0_OUT0 (CAM_SI))
 **					--> Event 3 is for Clearing the Start Signal of the Camera (clears SCT0_OUT0 (CAM_SI))
 **					--> Event 4 is the ADC Trigger
 **
 ** CAM_CLK (Camera Clock) at Pin P[3][27] (J13 Pin13) Clock Signal in range 5kHz to 8MHz --> here 3,63MHz
 ** CAM_SI (Camera Serial Input) at Pin P[3][26] (J13 Pin15) Serial Input for Start taking picture
 ** CAM_AO (Camera Analog Output) at Pin P[0][16] (J12 Pin2) Analog Output of camera (Pixel brightness)
 ************************************************************************************************************
 ***********************************************************************************************************/


#include "camera.h"

/*******************************************************************************
 * Parameters
 ******************************************************************************/
const ctimer_config_t TakeShots_config = {
		.mode = kCTIMER_TimerMode,   /* TC is incremented every rising APB bus clock edge */
		.prescale = 0                /*!< Prescale value 0 --> */
};


volatile uint8_t pixelCounter = 129;
volatile uint8_t pixelValues[128] = {0};

const int16_t VREFn = 0; 				//mV
const int16_t VREFp = 3300; 			//mV


/*******************************************************************************
 * Camera Main Initialization function
 ******************************************************************************/
void CAM_Init(void)
{
	//*******************************
	//Camera Initialization Functions
	CTIMER0_Init(); 				//CTIMER0 Initialization
	SCTimer_Clock_Config(); 		//SCTimer Clock Configuration
	SCTimer_CamCLK_Init();			//Initialize PWM Signal for Camera Clock (3,63MHz)
	SCTimer_SIEvents_Init();		//Initialize Start Signal for Camera (SI)
	SCTimer_ADCTrigger_Init();		//Initialize ADC Trigger Event
	ADC_Config();					//Configure ADC
	//*******************************

	SCT0->CTRL &= ~(1 << 2); //Unhalt SCT0 by clearing bit 2 of CTRL
}

/*******************************************************************************
 * CTIMER0 Initialization function
 ******************************************************************************/
void CTIMER0_Init(void)
{
	/* CTIMER0 peripheral initialization */
	CTIMER_Init(CTIMER0, &TakeShots_config);

	CTIMER0-> MCR  = 0;						//Delete current Configuration
	CTIMER0-> MCR |= (1<<0)|(1<<1)|(1<<24);	//Interrupt, Timer Counter reset and reload MR with MSR at Match0

	CTIMER0->MSR[0] = 220000;				//Initialize MSR0 with  220000 --> Timer overflow every 1ms
	//***********************************
	//ADC Interrupt configuration
	//CTIMER0-> INTEN |= (1<<0);
	NVIC_SetPriority(CTIMER0_IRQn, 0);		//Enable NVIC interrupt for sequence A.
	EnableIRQ(CTIMER0_IRQn);				//Enable ADC Sequence A Interrupt
	//Enabling NVIC will block DMA trigger!!!!
	//***********************************

	CTIMER_StartTimer(CTIMER0); //Start CTIMER0
}


/*******************************************************************************
 * SCTimer Clock Configuration
 ******************************************************************************/
void SCTimer_Clock_Config(void)
{
	//*******************************
	//Configure CLK for SCTimer/PWM
	SYSCON->SCTCLKSEL = 0x00; 			//Main Clock for SCTimer/PWM
	SYSCON->SCTCLKDIV = (5-1); 			//CLK Divider 5 -> 220MHz/5 = 44MHz  (SCMax = 100MHz)
	SYSCON->AHBCLKCTRL[1] |= 1<<2; 		//SCTimer/PWM SCT0 CLK Enable
	SYSCON->PRESETCTRLCLR[1] |= 1<<2; 	//Clear the SCTimer/PWM peripheral reset
	//*******************************

	//*****************************
	//SCT0 --> Use Configured CLK
	SCT0->CONFIG |= 0x3<<1;			//CLKMODE asynchronous with input (->CKSEL)
	SCT0->CONFIG |= 0xE<<3;			//CKSEL Input 7 Rising Edges
	SCT0->CONFIG |= 1<<17; 			//Auto limit (& two 16-bit timers)
	//*****************************
}


/*******************************************************************************
 * Initialize PWM Signal for Camera Clock (3,676MHz)
 ******************************************************************************/
void SCTimer_CamCLK_Init(void)
{
	//**********************************
	//Configure Pin P[3][27] (J13 Pin13) (CAM_CLK/SCT0_OUT1)
	IOCON->PIO[3][27] &= 0xFFFFFFF0; 	//Clear FUNC bits of P3.27
	IOCON->PIO[3][27] |= 0x02;     		//Set FUNC bits to SCT0_OUT1 function FUNC2 P3.27
	GPIO->DIR[3]      |= 1<<27;         //Set PIO3_27 (SCT0_OUT1) to output
	//**********************************

	//***************************************************
	//Set PWM at PIO3_27 to 3,676MHz (Cam_CLK_frequency max=8MHz)
	//***************************************************
	//Event 0 for Counter Limit
	SCT0->MATCHREL[0] = (12-1); 				//Match 0 @ 12/44MHz = 272,72ns Limit Counter
	SCT0->EV[0].STATE = 0xFFFFFFFF; 			//Event 0 happens in all states
	SCT0->EV[0].CTRL = (1 << 12); 				//Match 0 condition only
	SCT0->OUT[1].SET = (1 << 0); 				//Event 0 will set SCT0_OUT1
	//Event 1 for PWM Duty Cycle
	SCT0->MATCHREL[1] = (6-1); 					//Match 1 @ 6/44MHz = 136,36ns
	SCT0->EV[1].STATE = 0xFFFFFFFF; 			//Event 1 happens in all states
	SCT0->EV[1].CTRL = (1 << 0) | (1 << 12); 	//Match 1 condition only
	SCT0->OUT[1].CLR = (1 << 1); 				//Event 1 will clear SCT0_OUT1
	//***************************************************
}


/*******************************************************************************
 * Initialize Start Signal for Camera (SI)
 ******************************************************************************/
void SCTimer_SIEvents_Init(void)
{
	//**********************************
	//Configure Pin P[3][26] (J13 Pin15) (CAM_SI/SCT0_OUT0)
	IOCON->PIO[3][26] &= 0xFFFFFFF0; 	//clear FUNC bits of P3.26
	IOCON->PIO[3][26] |= 0x02;     		//Set FUNC bits to SCT0_OUT0 function FUNC2 P3.26
	GPIO->DIR[3]      |= 1<<26;       	//set PIO3_26 (SCT0_OUT0) to output
	//**********************************

	//**************************************
	//Event 2 for SI Set Event
	SCT0->MATCHREL[2] = (11-1); 			//Match 2 @ 11/44MHz = 250ns
	SCT0->EV[2].STATE = 0; 					//Event 2 happens only in State 0
	SCT0->EV[2].CTRL = (2 << 0)|(1 << 12); 	//Match 2 condition only
	SCT0->OUT[0].SET = (1 << 2); 			//Event 2 will set SCT0_OUT0
	//**************************************

	//**************************************
	//Event 3 for SI reset Event
	SCT0->MATCHREL[3] = (1-1); 				//Match 3 @ 1/44MHz = 22,727ns
	SCT0->EV[3].STATE = 0xFFFFFFF; 			//Event 3 happens in every state
	SCT0->EV[3].CTRL = (3 << 0)|(1 << 12);	//Match 3 condition only
	SCT0->OUT[0].CLR = (1 << 3); 			//Event 3 will clear SCT0_OUT0
	//**************************************
}


/*******************************************************************************
 * Allow Start Signal for Camera (SI) to happen
 *    Allows Event 2 to happen once every interrupt (Set and Clear SI)
 ******************************************************************************/
void CTIMER0_IRQHandler(uint32_t flags)
{
	pixelCounter = 0; //new picture start at pixel 0

	//**********************************
	SCT0->EV[2].STATE = 0xFFFFFFF; 		//Event 2 happens in all states
	SCT0->CTRL &= ~(1 << 2); 			//Unhalt SCT0 by clearing bit 2 of CTRL
	SCT0->EV[2].STATE = 0; 				//Event 2 happens only in State 0
	SCT0->CTRL &= ~(1 << 2); 			//Unhalt SCT0 by clearing bit 2 of CTRL
	//**********************************

	CTIMER_ClearStatusFlags(CTIMER0,kCTIMER_Match0Flag);
}


/*******************************************************************************
 * Initialize Trigger Event for ADC
 ******************************************************************************/
void SCTimer_ADCTrigger_Init(void)
{
	//**************************************
	//Event 4 for ADC Trigger Event
	SCT0->MATCHREL[4] = (9-1); 				//Match 4 @ 9/44MHz = 204,54ns (Cam_AO settlingTime Min120ns)
	SCT0->EV[4].STATE = 0xFFFFFFF;			//Event 4 happens in all states
	SCT0->EV[4].CTRL = (4 << 0)|(1 << 12); 	//Match 4 condition only

	//SCT0_OUT4 is ADC hardware trigger
	SCT0->OUT[4].SET = (1 << 4); 			//Event 4 will set SCTx_OUT4
	SCT0->OUT[4].CLR = (1 << 0); 			//Event 0 will clear SCTx_OUT4
	//**************************************
}


/*******************************************************************************
 * ADC Calibration Sequence
 ******************************************************************************/
void ADC_Calibration(void)
{
	uint32_t tmp = ADC0->CTRL;					//Save current ADC configurations

	ADC0->CTRL |= ADC_CTRL_CLKDIV(2U); 			//Clock Divider	--> 220MHz/3 = 73,33MHz (80MHz max.)
	ADC0->STARTUP = ADC_STARTUP_ADC_ENA_MASK;	// Start ADC module

	if (0UL == (ADC0->STARTUP & ADC_STARTUP_ADC_ENA_MASK)) {
		return;	// ADC is not powered up.
	}


	//***********************************************************
	//Calibration frequency and cycle (max. 30MHz!!!!!!)
	ADC0->CTRL |= ADC_CTRL_CLKDIV(7U);									//Clock Divider --> 220MHz/8 = 27,5MHz (30MHz max.)
	ADC0->CALIB = ADC_CALIB_CALIB_MASK;									// Launch calibration cycle.
	SDK_DelayAtLeastUs(100U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY); 	// Wait for at least 81 ADC clock cycles.
	if (ADC_CALIB_CALIB_MASK == (ADC0->CALIB & ADC_CALIB_CALIB_MASK)) {
		return;	// Calibration timed out.
	}
	//***********************************************************


	ADC0->CTRL = tmp;							//Rewrite ADC Configurations
}


/*******************************************************************************
 * Configure ADC
 ******************************************************************************/
void ADC_Config(void)
{
	//*******************************
	//Configure Pin P[0][16] (J12 Pin2) (ADC0IN4)
	IOCON->PIO[0][16] &= 0xFFFFFFFF0; 	//Clear FUNC bits of P0.16 Func 0 is ADC0_4
	IOCON->PIO[0][16] &= ~(1 << 8);		//Disable DIGIMODE --> Analog input
	GPIO->DIR[0]      &= ~(1 << 16);    //Set PIO0_16 (ADC0_4) to input
	//******************************

	//*********************************************
	//Power up ADC0 peripheral in normal-power mode
	SYSCON->PDRUNCFG[0] &= ~(1 << 10); 				//Power Up ADC (PDEN_ADC0)
	SYSCON->PDRUNCFG[0] &= ~(1 << 9);				//Power Up Analog Supply for ADC (PDEN_VD2_ANA)
	SYSCON->PDRUNCFG[0] &= ~(1 << 19);				//VDDA to ADC (PDEN_VDDA)
	SYSCON->PDRUNCFG[0] &= ~(1 << 23);				//VREFP to ADC (PDEN_VREFP)
	//*********************************************


	//**************************************************************
	//Clock Enable and Peripheral Reset
	SDK_DelayAtLeastUs(20U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);	//Wait at least 20us after Power Up !!!!!!!!
	SYSCON->AHBCLKCTRL[0] 		|= (1 << 27); 							//ADC0 CLK Enable
	SYSCON->PRESETCTRLCLR[0] 	|= (1 << 27); 							//Clear ADC0 peripheral reset
	//**************************************************************

	ADC_Calibration();	//ADC Calibration Sequence

	//*********************************************
	//ADC fundamental configuration
	ADC0->CTRL |= 3-1; 								//CLK Divider 220MHz/3 = 73,33MHz (80MHz max)
	//Synchronous Mode System Clock is Reset value
	ADC0->CTRL &= ~ADC_CTRL_RESOL_MASK; 			//Delete Resolution bits
	ADC0->CTRL |= (0b01 << ADC_CTRL_RESOL_SHIFT); 	//Resolution 8bit (12bit is max)
	ADC0->CTRL |= (0b010 << 12); 					//Sampling Time is 4.5x ADC Clock Cycle --> 61,366ns
	//*********************************************

	//*********************************
	//ADC Sequence A configuration
	ADC0->SEQ_CTRL[0] &= ~(1 << 31); 	//Sequence A Disable for Configuration
	ADC0->SEQ_CTRL[0] |= (1 << 4); 		//Select Channel Input 4 for ADC Conversion in Sequence A
	ADC0->SEQ_CTRL[0] |= (3 << 12); 	//SCTIMER Output 4 Trigger SCT0_OUT4
	ADC0->SEQ_CTRL[0] |= (1 << 18); 	//TRIGPOL positive Edge
	ADC0->SEQ_CTRL[0] |= (1 << 19); 	//Bypass Trigger Synchronization
	//ADC0->SEQ_CTRL[0] |= (1 << 30);	//Mode: 0(Rst.Value)=End of Conversion / 1=End of Sequence
	ADC0->SEQ_CTRL[0] |= (1 << 31); 	//Sequence A Enable
	//*********************************

	//***********************************
	//ADC Interrupt configuration
	ADC0->INTEN |= (1<<0); 					//Interrupt Enable ADC0 Sequence A
	NVIC_SetPriority(ADC0_SEQA_IRQn, 0);	//Enable NVIC interrupt for sequence A.
	EnableIRQ(ADC0_SEQA_IRQn);				//Enable ADC Sequence A Interrupt
	//Enabling NVIC will block DMA trigger!!!!
	//***********************************
}


/*******************************************************************************
 * ADC0 Sequence A conversion finished ISR (Get pixel charge values)
 ******************************************************************************/
void ADC0_SEQA_IRQHandler(void)
{
	if(pixelCounter<128) //Save Pixel Values
	{
		pixelValues[pixelCounter] = ADC0->SEQ_GDAT[0] >> 8;	//Reading current pixel
		pixelCounter++;										//Next ISR is next pixel
	}

	ADC0->FLAGS = (1<<28);									//Delete interrupt flags
	SDK_ISR_EXIT_BARRIER;
}


/*Transfer ADC result values to voltages and logical values
uint8_t transferCounter;											//Start with result 0
float cFactor = (float)1/256*(VREFp-VREFn);							//Precalculate the calculation factor for calculation of voltages
for (transferCounter = 0; transferCounter<129; transferCounter++)	//Go through all results
{
	pixelVoltages[transferCounter] = (int16_t)(((float)pixelValues[transferCounter]*cFactor)+VREFn);	//Calculate Pixel Voltages
	if(pixelValues[transferCounter] >= 128)
	{
		pixelLogicValues[transferCounter] = 1; 						//Write logical 1 for "Bright"-Value
	}
	else
	{
		pixelLogicValues[transferCounter] = 0;							//Write logical 0 for "Dark"-Value
	}
}*/
