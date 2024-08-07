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
#include "screen.h"
#include <stdlib.h> //für abs() Funktion
#include <math.h>

/*******************************************************************************
 * Parameters
 ******************************************************************************/
const ctimer_config_t TakeShots_config = {
		.mode = kCTIMER_TimerMode,   /* TC is incremented every rising APB bus clock edge */
		.prescale = 0                /*!< Prescale value 0 --> */
};


volatile uint8_t pixelCounter = 129;			// PixelCounter for camera 1
volatile uint8_t pixelValues[128] = {0};		// PixelValue Array for camera 1
volatile uint8_t pixelValuesUC[128] = {0};		// Uncalibrated PixelValue Array for camera 1
volatile int8_t *calibrationCamera = calibrationCamera_storage[0]; // Calibration Array for camera 1
volatile uint8_t pixelCounter2 = 129;			// PixelCounter for camera 2
volatile uint8_t pixelValues2[128] = {0};		// PixelValue Array for camera 2
volatile uint8_t pixelValues2UC[128] = {0};		// Uncalibrated PixelValue Array for camera 2
volatile int8_t *calibrationCamera2 = calibrationCamera_storage[1]; // Calibration Array for camera 2


struct EdgeDetectionData edgeData[2] = {
		{
			.edges = {0},
			.edgesMiddle = {0},
			.edge_left_found = 0,
			.edge_right_found = 0,
			.edge_left = 0,
			.edge_right = 0,
			.edge_center = 0,
			.edge_center_mm = 0,
			.detection_mode = 0,
			.track_state = 0,
			.finish_detected = 0
		},
		{
			.edges = {0},
			.edgesMiddle = {0},
			.edge_left_found = 0,
			.edge_right_found = 0,
			.edge_left = 0,
			.edge_right = 0,
			.edge_center = 0,
			.edge_center_mm = 0,
			.detection_mode = 0,
			.track_state = 0,
			.finish_detected = 0
		}
    };

volatile uint8_t edges[128] = {0};
volatile uint8_t edgesMiddle[128] = {0}; //all detected edge
volatile uint8_t edge_left_found = 0;	//1 if found
volatile uint8_t edge_right_found = 0;	//1 if found
volatile uint8_t edge_left = 0;			//left Edge Coordinate
volatile uint8_t edge_right = 0;		//Right Edge Coordinate
volatile int16_t edge_center = 0;
volatile int16_t edge_center_mm =0;
volatile uint8_t detection_mode = 0;
volatile uint8_t track_state = 0;

//Array zum Speichern der Pixelwerte nach der 1. Ableitung (nur Werte von 0-255):
volatile float edge_gradient1[128];
volatile float edge_gefiltert[128];

//volatile uint8_t camera_distance = 0;	//TODO: implement


volatile uint32_t exposure_time=5000000;
volatile uint32_t exposure_time2=5000000;


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
	CTIMER4_Init();					//CTIMER4 Initialization
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
	CTIMER0-> MCR |= (1<<0)|(1<<1)|(1<<24);	//Interrupt when MR0 = value in TC, Timer Counter reset and reload MR with MSR at Match0

	CTIMER0->MSR[0] = exposure_time;				//Initialize MSR0 with  5000000 --> Timer overflow every 22.73ms  ((1/220MHZ)*5000000)
	CTIMER0->PR=1;									//Prescaler (/2)
	//***********************************
	//ADC Interrupt configuration
	//CTIMER0-> INTEN |= (1<<0);
	//NVIC_SetPriority(CTIMER0_IRQn, 0);
	EnableIRQ(CTIMER0_IRQn);
	//Enabling NVIC will block DMA trigger!!!!
	//***********************************

	CTIMER_StartTimer(CTIMER0); //Start CTIMER0
}

/*******************************************************************************
 * CTIMER1 Initialization function
 ******************************************************************************/
void CTIMER4_Init(void)
{
	/* CTIMER3 peripheral initialization */
	CTIMER_Init(CTIMER4, &TakeShots_config);

	CTIMER4-> MCR  = 0;						//Delete current Configuration
	CTIMER4-> MCR |= (1<<0)|(1<<1)|(1<<24);	//Interrupt when MR0 = value in TC, Timer Counter reset and reload MR with MSR at Match0

	CTIMER4->MSR[0] = exposure_time2;		//Initialize MSR0 with  5000000 --> Timer overflow every 22.73ms  ((1/220MHZ)*5000000)
	CTIMER4->PR=1;							//Prescaler (/2)
	//***********************************
	//ADC Interrupt configuration
	//CTIMER0-> INTEN |= (1<<0);
	//NVIC_SetPriority(CTIMER4_IRQn, 0);
	EnableIRQ(CTIMER4_IRQn);
	//Enabling NVIC will block DMA trigger!!!!
	//***********************************

	CTIMER_StartTimer(CTIMER4); //Start CTIMER4
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
	//NVIC_SetPriority(SCT0_IRQn, 0);
	EnableIRQ(SCT0_IRQn);				// Enable Interrupt for SC-Timer Events							//17.09.23 Martin Fürstberger
	//Camera 1
	//Configure Pin P[3][27] (J13 Pin13) (CAM_CLK/SCT0_OUT1)
	IOCON->PIO[3][27] &= 0xFFFFFFF0; 	//Clear FUNC bits of P3.27
	IOCON->PIO[3][27] |= 0x02;     		//Set FUNC bits to SCT0_OUT1 function FUNC2 P3.27
	GPIO->DIR[3]      |= 1<<27;         //Set PIO3_27 (SCT0_OUT1) to output

	//Camera 2
	//Configure Pin P[0][28] (J13 Pin14) (CAM_CLK/SCT0_OUT7)
	IOCON->PIO[3][29] &= 0xFFFFFFF0; 	//Clear FUNC bits of P0.28
	IOCON->PIO[3][29] |= 0x02;			//Set FUNC bits to SCT0_OUT3 function FUNC2 P3.29
	GPIO->DIR[3]	  |= 1<<29;			//Set PIO3_29 (SCT0_OUT3) to output


	//**********************************

	//***************************************************
	//Set PWM at PIO3_27 to 3,676MHz (Cam_CLK_frequency max=8MHz) // old numbers (review later)
	//***************************************************
	//Event 0 for Counter Limit
	SCT0->MATCHREL[0] = (500-1); 				//Match 0 @ 12/44MHz = 272,72ns Limit Counter		//Changed to 26 (590,90 ns)
	SCT0->EV[0].STATE = 0xFFFFFFFF; 			//Event 0 happens in all states
	SCT0->EV[0].CTRL = (1 << 12); 				//Match 0 condition only

	//Camera 1
	SCT0->OUT[1].SET = (1 << 0); 				//Event 0 will set SCT0_OUT1
	//Camera 2
	SCT0->OUT[3].SET = (1 << 0);				//Event 0 will set SCT0_OUT3


	//Event 1 for PWM Duty Cycle
	SCT0->MATCHREL[1] = (250-1); 				//Match 1 @ 6/44MHz = 136,36ns						//Changed to 13 (295,45 ns)
	SCT0->EV[1].STATE = 0xFFFFFFFF; 			//Event 1 happens in all states
	SCT0->EV[1].CTRL = (1 << 0) | (1 << 12); 	//Match 1 condition only

	//Camera 1
	SCT0->OUT[1].CLR = (1 << 1); 				//Event 1 will clear SCT0_OUT1
	//Camera 2
	SCT0->OUT[3].CLR = (1 << 1);				//Event 1 will clear SCT0_OUT3
	//***************************************************
}


/*******************************************************************************
 * Initialize Start Signal for Camera (SI)
 ******************************************************************************/
void SCTimer_SIEvents_Init(void)
{
	//**********************************
	//Camera 1
	//Configure Pin P[3][26] (J13 Pin15) (CAM_SI/SCT0_OUT0)
	IOCON->PIO[3][26] &= 0xFFFFFFF0; 	//clear FUNC bits of P3.26
	IOCON->PIO[3][26] |= 0x02;     		//Set FUNC bits to SCT0_OUT0 function FUNC2 P3.26
	GPIO->DIR[3]      |= 1<<26;       	//set PIO3_26 (SCT0_OUT0) to output
	//**********************************

	//Camera 2
	//Configure Pin P[3][28] (J13 Pin16) (CAM_SI/SCT0_OUT2)
	IOCON->PIO[3][28] &= 0xFFFFFFF0; 	//clear FUNC bits of P3.28
	IOCON->PIO[3][28] |= 0x02;     		//Set FUNC bits to SCT0_OUT2 function FUNC2 P3.28
	GPIO->DIR[3]      |= 1<<28;       	//set PIO3_28 (SCT0_OUT2) to output


	//**************************************
	//Event 2 for SI Set Event Camera1
	SCT0->MATCHREL[2] = (480-1); 			//Match 2 @ 11/44MHz = 250ns							//Changed to 24 (545,45 ns)
	SCT0->EV[2].STATE = 0; 					//Event 2 happens only in State 0
	SCT0->EV[2].CTRL = (2 << 0)|(1 << 12); 	//Match 2 condition only
	//Camera 1
	SCT0->OUT[0].SET = (1 << 2); 			//Event 2 will set SCT0_OUT0

	SCT0->EVEN |= (1 << 2);					//Event 2 will interrupt								17.09.23 Martin Fürstberger
	//**************************************

	//Event 5 for SI Set Event Camera2
	SCT0->MATCHREL[5]=(480-1);				//Match 5 @ 24/44MHz = 545,45 ns
	SCT0->EV[5].STATE = 0;					//Event 5 happens only in State 0
	SCT0->EV[5].CTRL = (5 << 0)|(1 << 12);	//Match 5 condition only
	//Camera 2
	SCT0->OUT[2].SET = (1 << 5); 			//Event 5 will set SCT0_OUT2

	SCT0->EVEN |= (1 << 5);					//Event 5 will interrupt								17.09.23 Martin Fürstberger
	//**************************************
	//Event 3 for SI reset Event
	SCT0->MATCHREL[3] = (20-1); 				//Match 3 @ 1/44MHz = 22,727ns							//Changed to 2 (45,454ns)
	SCT0->EV[3].STATE = 0xFFFFFFF; 			//Event 3 happens in every state
	SCT0->EV[3].CTRL = (3 << 0)|(1 << 12);	//Match 3 condition only
	//Camera 1
	SCT0->OUT[0].CLR = (1 << 3); 			//Event 3 will clear SCT0_OUT0
	//Camera 2
	SCT0->OUT[2].CLR = (1 << 3); 			//Event 3 will clear SCT0_OUT2
	//**************************************
}


/*******************************************************************************
 * Allow Start Signal for Camera (SI) to happen
 *    Allows Event 2 to happen once every interrupt (Set and Clear SI)
 ******************************************************************************/
void CTIMER0_IRQHandler(uint32_t flags)
{
	SCT0->EV[2].STATE = 0xFFFFFFF; 		//Event 2 happens in all states
	SCT0->CTRL &= ~(1 << 2); 			//Unhalt SCT0 by clearing bit 2 of CTRL
	CTIMER_ClearStatusFlags(CTIMER0,kCTIMER_Match0Flag);
}

/*******************************************************************************
 * Allow Start Signal for Camera (SI) to happen
 *    Allows Event 2 to happen once every interrupt (Set and Clear SI)
 ******************************************************************************/
void CTIMER4_IRQHandler(uint32_t flags)
{
	SCT0->EV[5].STATE = 0xFFFFFFF; 		//Event 5 happens in all states
	SCT0->CTRL &= ~(1 << 2); 			//Unhalt SCT0 by clearing bit 2 of CTRL
	CTIMER_ClearStatusFlags(CTIMER4,kCTIMER_Match0Flag);
}


/*******************************************************************************
 * Initialize Trigger Event for ADC
 ******************************************************************************/
void SCTimer_ADCTrigger_Init(void)
{
	//**************************************
	//Event 4 for ADC Trigger Event
	SCT0->MATCHREL[4] = (260-1); 			//Match 4 @ 9/44MHz = 204,54ns (Cam_AO settlingTime Min120ns)	//Changed to 22 (500,00 ns)
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

	//*******************************
	//Configure Pin P[0][31] (J12 Pin4) (ADC0IN5)
	IOCON->PIO[0][31] &= 0xFFFFFFFF0; 	//Clear FUNC bits of P0.31 Func 0 is ADC0_5
	IOCON->PIO[0][31] &= ~(1 << 8);		//Disable DIGIMODE --> Analog input
	GPIO->DIR[0]      &= ~(1 << 31);    //Set PIO0_31 (ADC0_5) to input
	//*******************************

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
	ADC0->SEQ_CTRL[0] |= (1 << 5); 		//Select Channel Input 5 for ADC Conversion in Sequence A
	ADC0->SEQ_CTRL[0] |= (3 << 12); 	//SCTIMER Output 4 Trigger SCT0_OUT4
	ADC0->SEQ_CTRL[0] |= (1 << 18); 	//TRIGPOL positive Edge
	ADC0->SEQ_CTRL[0] |= (1 << 19); 	//Bypass Trigger Synchronization
	ADC0->SEQ_CTRL[0] |= (1 << 30);	//Mode: 0(Rst.Value)=End of Conversion / 1=End of Sequence
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
	if(pixelCounter<128) //Save Pixel Values Camera 1
	{
		pixelValuesUC[pixelCounter] = (ADC0->DAT[4] >> 8)/* + calibrationCamera[pixelCounter]*/;		//Reading current pixel and Adding Calibration Array for Camera 1
		pixelCounter++;										//Next ISR is next pixel
	}

	if(pixelCounter2<128)//Save Pixel Values Camera 2
	{
		pixelValues2UC[pixelCounter2] = (ADC0->DAT[5] >> 8)/* + calibrationCamera2[pixelCounter2]*/;	//Reading current pixel and Adding Calibration Array for Camera 2
		pixelCounter2++;									//Next ISR is next pixel
	}

	ADC0->FLAGS = (1<<28);									//Delete interrupt flags
	SDK_ISR_EXIT_BARRIER;
}
void SCT0_IRQHandler(void)
{
	if((SCT0->EVFLAG & (1 << 2)) != 0){			//Wenn Bit 2 (Event 2) 1 ist
		pixelCounter=0;							//new picture start at pixel 0
		SCT0->EV[2].STATE = 0; 				//Event 2 happens only in State 0
		SCT0->CTRL &= ~(1 << 2); 			//Unhalt SCT0 by clearing bit 2 of CTRL

	}
	if((SCT0->EVFLAG & (1 << 5)) != 0){			//Wenn Bit 5 (Event 5) 1 ist
		pixelCounter2=0;						//new picture start at pixel 0
		SCT0->EV[5].STATE = 0; 					//Event 5 happens only in State 0
		SCT0->CTRL &= ~(1 << 2); 				//Unhalt SCT0 by clearing bit 2 of CTRL
	}
	SCT0->EVFLAG |= (1 << 2) | (1 << 5);		//Reset InterruptFlags
	SDK_ISR_EXIT_BARRIER;
}

void Camera_Exposure_time_task(void *pvParameters)
{
	while(1){
		//Camera 1
		int32_t pixel_Values_sum = 0;
		int32_t exposureTimediff = 0;													// Value gets added to current exposure time at every call of task

		for(uint8_t x=0;x<128;x++){
			pixel_Values_sum = pixel_Values_sum + pixelValuesUC[x];						// Sum of all 128 ADC-Camera-Values
			pixelValues[x]=pixelValuesUC[x]+calibrationCamera[x];						// Calibration Camera 1
		}
		exposureTimediff = (PVsumW-pixel_Values_sum)*ExposureGainCam;					// Value gets added to current exposure time at every call of task
																						// Gain = 5
																						// 16384 = 128 * ADC-Medium-Value (0 ... 256)
		if(exposureTimediff>=MaxDiffExpCams){											// max increment of exposure time every ms is 0,5ms
			exposureTimediff=MaxDiffExpCams;
		}
		if(exposureTimediff<=-MaxDiffExpCams){													// max decrement of exposure time every ms is 0,5ms
			exposureTimediff=-MaxDiffExpCams;
		}
		exposure_time = exposure_time + exposureTimediff;
		if(exposure_time>=MaxExposureTime){													// max exposure time for camera is 100 ms
			exposure_time=MaxExposureTime;
		}
		CTIMER0->MSR[0] = exposure_time;												// Exposure Time in s = exposure_time/110 MHz

		//Camera 2
		int32_t pixel_Values_sum2 = 0;
		int32_t exposureTimediff2 = 0;													// Value gets added to current exposure time at every call of task

		for(uint8_t x=0;x<128;x++){
		pixel_Values_sum2 = pixel_Values_sum2 + pixelValues2UC[x];						// Sum of all 128 ADC-Camera-Values
		pixelValues2[x]=pixelValues2UC[x]+calibrationCamera2[x];						// Calibration Camera 2
		}
		exposureTimediff2 = (PVsumW-pixel_Values_sum2)*ExposureGainCam2;					// Value gets added to current exposure time at every call of task
																							// Gain = 5
																							// 16384 = 128 * ADC-Medium-Value (0 ... 256)
		if(exposureTimediff2>=MaxDiffExpCams){													// max increment of exposure time 2 every ms is 0,5ms
			exposureTimediff2=MaxDiffExpCams;
		}
		if(exposureTimediff2<=-MaxDiffExpCams){													// max decrement of exposure time 2 every ms is 0,5ms
			exposureTimediff2=-MaxDiffExpCams;
		}


		exposure_time2 = exposure_time2 + exposureTimediff2;
		if(exposure_time2>=MaxExposureTime){													// max exposure time for camera is 100 ms
			exposure_time2=MaxExposureTime;
		}
		CTIMER4->MSR[0] = exposure_time2;												// Exposure Time in s = exposure_time2/110 MHz

//		IOCON->PIO[2][1] &= 0xFFFFFFFF0; 	//Clear FUNC bits of P2.2 Func 0 is GPIO-Pin
//		GPIO->DIR[2]      |= (1 << 1);    //Set PIO2_1  to output
//		GPIO->SET[2] |=(1<<1);


		//Kantenerkennung in dem Kamerabild von Kamera1:
		//Edge_Detection(&edgeData[0], pixelValues);
		Edge_Detection_Prewitt(&edgeData[0], pixelValues);

		//Kantenerkennung in dem Kamerabild von Kamera2:
		//Edge_Detection(&edgeData[1], pixelValues2);
		Edge_Detection_Prewitt(&edgeData[1], pixelValues2);

		//Erkennung der Ziellinie mittels Daten der Kamera1:
		Finish_Line(&edgeData[0]);

		vTaskDelay(1);
	}
}
//	*********************************************



void menu_func_edge_calibration() {

	((all_param_t*)&const_all_param)->camera.edge_distance[camSelect-1] = edgeData[camSelect-1].edge_right - edgeData[camSelect-1].edge_left;

}


/*******************************************************************************
 * Edge Detection
 ******************************************************************************/

void Edge_Detection(struct EdgeDetectionData *edgeData, volatile uint8_t *pixelVal){
	//Parameter
	uint8_t threshold = 20;					//threshold for edge detection //Schwellwert 30 -> 20 bei Wettbewerb
	uint8_t edge_min_width = 1;
	uint8_t edge_min_hight = 30;
	uint8_t trace_offset = 7;
	uint8_t edge_distance = ((all_param_t*)&const_all_param)->camera.edge_distance[camSelect-1];


	uint8_t right_edge_hight_max = 0;
	uint8_t left_edge_hight_max = 0;
	uint8_t edgeWidth = 0;
	uint8_t edgeHight = 0;
	uint8_t edgeMiddle = 0;
	uint8_t right_edge_begin = 0;
	uint8_t right_edge_end = 0;
	uint8_t left_edge_begin = 0;
	uint8_t left_edge_end = 0;
	uint8_t xmin;
	uint8_t xmax;
	uint8_t right_edge_count = 0;
	uint8_t left_edge_count = 0;

	edgeData->edge_right_found = 0;
	edgeData->edge_left_found = 0;
	edgeData->finish_detected=0;

//	detection_mode = init; //for debugging

	for(uint8_t x=1;x<=126;x=x+1){
		edgeData->edgesMiddle[x] = 0;

		//search for falling Pixel
		if((pixelVal[x-1]-pixelVal[x+1])>threshold)
		{
			edgeData->edges[x] = right;	//1 = Right Edge (Falling Edge)
		}

		//search for rising Pixel
		else if((pixelVal[x+1]-pixelVal[x-1])>threshold)
		{
			edgeData->edges[x] = left;	//2 = Left Edge (Rising Edge)
		}

		//no rising or falling Pixel found
		else edgeData->edges[x] = 0;	//0 = No Edge


	}

	switch(detection_mode) {
		case init:
			xmin = 1; xmax = 126; break;
		case trace:
			xmin = edgeData->edge_right - trace_offset;
			xmax = edgeData->edge_right + trace_offset;
			if(xmin < 1)
				xmin = 1;
			if(xmax > 126)
				xmax = 126;
			break;
	}

	//Search for falling (right) Edge
	for(uint8_t x=1;x<=126;x=x+1){
		if(edgeData->edges[x] == right){
			right_edge_begin = x;
			while((edgeData->edges[x] == right) && (x <=126))
				x++;
			right_edge_end = x-1;
			edgeWidth = right_edge_end - right_edge_begin;
			edgeHight = pixelVal[right_edge_begin] - pixelVal[right_edge_end];

			if((edgeWidth >= edge_min_width) && (edgeHight >= edge_min_hight)){		//noise filter
				edgeMiddle = edgeWidth/2 + right_edge_begin;
				edgeData->edgesMiddle[edgeMiddle] = right;
				right_edge_count++;
				if(/*(edgeHight > right_edge_hight_max) && */ (edgeMiddle >= xmin) && (edgeMiddle <= xmax)){	//taking highest edge in area (Flanke nehmen die am weitesten rechts liegt)
					edgeData->edge_right = edgeMiddle;
					right_edge_hight_max = edgeHight;
					edgeData->edge_right_found = 1;
				}
			}
		}
	}

	switch(detection_mode) {
		case init:
			xmin = 1; xmax = 126; break;
		case trace:
			xmin = edgeData->edge_left - trace_offset;
			xmax = edgeData->edge_left + trace_offset;
			if(xmin < 1)
				xmin = 1;
			if(xmax > 126)
				xmax = 126;
			break;
	}

	//Search for rising (left) Edge
	for(uint8_t x=1;x<=125;x=x+1){
		if(edgeData->edges[x] == left){
			left_edge_begin = x;
			while((edgeData->edges[x] == left) && (x <=126))
				x++;
			left_edge_end = x-1;
			edgeWidth = left_edge_end - left_edge_begin;
			edgeHight = pixelVal[left_edge_end] - pixelVal[left_edge_begin];

			if((edgeWidth >= edge_min_width) && (edgeHight >= edge_min_hight)){		//noise filter
				edgeMiddle = edgeWidth/2 + left_edge_begin;
				edgeData->edgesMiddle[edgeMiddle] = left;
				left_edge_count++;
				if((edgeHight > left_edge_hight_max) && (edgeMiddle >= xmin) && (edgeMiddle <= xmax) && (left_edge_hight_max == 0)){	//taking highest edge in area (Flanke nehmen die am weitesten links liegt)
					edgeData->edge_left = edgeMiddle;
					left_edge_hight_max = edgeHight;
					edgeData->edge_left_found = 1;
				}
			}
		}
	}

	if( ((edgeData->edge_left_found == 0) && (edgeData->edge_right_found == 0)) || (abs(edgeData->edge_right - edgeData->edge_left) < 20) || (edgeData->edge_left > edgeData->edge_right) )
		edgeData->detection_mode = init;
	else if((edgeData->edge_left_found == 0) && (edgeData->edge_right_found == 1))
		edgeData->edge_center = edgeData->edge_right - edge_distance/2;
	else if((edgeData->edge_left_found == 1) && (edgeData->edge_right_found == 0))
		edgeData->edge_center = edgeData->edge_left + edge_distance/2;
	else if((edgeData->edge_left_found == 1) && (edgeData->edge_right_found == 1)){
		edgeData->detection_mode = trace;
		edgeData->edge_center = (edgeData->edge_right + edgeData->edge_left)/2;
	}
	//Convert Pixel to mm
	edgeData->edge_center_mm = (int)(edgeData->edge_center - 62) * (RoadWidth/edge_distance);



/*
	if((right_edge_count >=2 ) || (left_edge_count >= 2)){
		edgeData->finish_detected = 1;
//		GPIO->SET[2] |=(1<<1);
	}
	else{
		edgeData->finish_detected = 0;
//		GPIO->CLR[2] |=(1<<1);
	}

*/

//	if((right_edge_count == 5) && (left_edge_count == 5)){
//		edgeData->track_state = four_stribes;
//	}
//
//	else if((right_edge_count == 4) && (left_edge_count == 4)){
//		edgeData->track_state = tree_stribes;
//	}
//
//	else if((right_edge_count == 3) && (left_edge_count == 3)){
//		edgeData->track_state = finish;
//	}
//	else
//		edgeData->track_state = track;
}

/*******************************************************************************
 * Finish_Line
 ******************************************************************************/
void Finish_Line(struct EdgeDetectionData *edgeData)
{
	//Notwendige Variablen:
	//Abstand zwischen linken und rechten Fahrbahnrand in mm:
	uint8_t edge_distance = ((all_param_t*)&const_all_param)->camera.edge_distance[camSelect-1];
	//Hypothenuse (entspricht Abstand rechter - linker Fahrbahnrand in mm):
	int h=(int)(edgeData->edge_right-edgeData->edge_left)*(RoadWidth/edge_distance);
	//Winkel in dem das Fahrzeug zum Fahrbahnmittelpunkt steht:
	double alpha_rad=acos(RoadWidth/h); //Winkel im Bogenmaß

	int H=0;
	int diff=0; //Abstand der Flanke zum Mittelpunkt in mm (Horizontaler Abstand)
	int cnt=0;

	//Berechnung des tatsächlichen Abstandes der detektierten Flanken vom Fahrbahnmittelpunkt: (aus dem Winkel)
	for (uint8_t x=1;x<=126;x++)
	{
		if (((edgeData->edgesMiddle[x]==right)||(edgeData->edgesMiddle[x]==left))&&(x>edgeData->edge_left)&&(x<edgeData->edge_right)) //Ermittlung aller Edges zwischen dem rechten und linken Fahrbahnrand
		{
			if(x<edgeData->edge_center) //Flanke befindet sich LINKS vom Mittelpunkt
			{
				H=(int)(edgeData->edge_center-x)*(RoadWidth/edge_distance);
				diff=(int)(cos(alpha_rad)*H);
			}
			else if(x>edgeData->edge_center) //Flanke befindet sich RECHTS vom Mittelpunkt
			{
				H=(int)(x-edgeData->edge_center)*(RoadWidth/edge_distance);
				diff=(int)(cos(alpha_rad)*H);
			}

			//Sobald eine Flanke innerhalb des Bereichs des Zielstreifens detektiert wurde, ist die Ziellinie überschritten:
			if((30<=diff)&&(diff<=140))
			{
				cnt=cnt+1;
				if (cnt>=2)
				{
					edgeData->finish_detected=1; //Änderung: muss eigentlich 1 sein, damit Zielerkennung funktioniert!!!
					break;
				}

			}
		}
	}

}

/*******************************************************************************
 * EdgeDetection_Prewitt (Überarbeitung der EdgeDetection: Anwendung des Prewitt-Filters)
 ******************************************************************************/
void Edge_Detection_Prewitt(struct EdgeDetectionData *edgeData, volatile uint8_t *pixelVal)
{

	//Notwendige Variablen:
	float threshold=15.0;
	int Hx[3]={-1,0,1};
	uint8_t edge_distance = ((all_param_t*)&const_all_param)->camera.edge_distance[camSelect-1];

	//Array zum Speichern der Pixelwerte nach der 1. Ableitung (nur Werte von 0-255):
	//float edge_gradient1[128];

	edge_gradient1[0] = (float)(pixelVal[0] * Hx[0] + pixelVal[1] * Hx[2]);
	edge_gradient1[127] = (float)pixelVal[126] * Hx[0] + pixelVal[127] * Hx[2];
	float sum = 0.0; // Summe der Multiplikation des linke und rechten Pixels vom aktuellen Pixel

	//Array zum Speichern der Werte nach dem Noise-Filtern:
	//float edge_gefiltert[128];


	//Array zum Speichern der gefundenen Maxima(left) und Minima(right): edgeData->edgesMiddle[i]
	for (int i = 0; i < 128; i++)
	{
		edgeData->edgesMiddle[i]= 0;
	}

	//Inititalisierung:
	edgeData->edge_right_found = 0;
	edgeData->edge_left_found = 0;
	edgeData->finish_detected=0;


	//Kantenverstärkung (1.Ableitung durch Anwendung des Prewitt-Operators):
	for (int i = 1; i <= 126; i++) //Rechtes und linkes Pixel nicht berücksichtigen
	{
		sum = (float)(pixelVal[i - 1] * Hx[0] + pixelVal[i + 1] * Hx[2]);

		//Ergebnis Abspeichern:
		edge_gradient1[i] = sum;

	}

	//Noise-Filter (Eliminierung des Rauschen durch Schwellenwert:
	for (int i = 0; i <= 127; i++)
	{
		if ((edge_gradient1[i] > 0) && (edge_gradient1[i] <= threshold))
		{
			edge_gefiltert[i] = 0;
		}
		else if ((edge_gradient1[i] < 0) && (edge_gradient1[i] >= -threshold))
		{
			edge_gefiltert[i] = 0;
		}
		else
			edge_gefiltert[i] = edge_gradient1[i];
	}

	//Find Maxima(Left-Edge) and Minima(Right-Edge):
	int cnt = 0;
	while (cnt < 128)
	{
		if (edge_gefiltert[cnt] == 0)
		{
			cnt++;
		}
		else if (edge_gefiltert[cnt] > 0) //Linke Kante
		{
			while (edge_gefiltert[cnt] < edge_gefiltert[cnt+1])
			{
				cnt++;
			}
			edgeData->edgesMiddle[cnt] = left;
			while (edge_gefiltert[cnt] != 0)
			{
				cnt++;
			}
		}
		else if (edge_gefiltert[cnt] < 0) //Rechte Kante
		{
			while (edge_gefiltert[cnt] > edge_gefiltert[cnt+1])
			{
				cnt++;
			}
			edgeData->edgesMiddle[cnt] = right;

			while (edge_gefiltert[cnt] != 0)
			{
				cnt++;
			}
		}
	}

	//Rechten Fahrbahnrand festlegen (=Flanke die am weitesten Rechts liegt):
	//Rechten Fahrbahnrand finden:
	for (int i = 127; i >= 0; i--)
	{
		if (edgeData->edgesMiddle[i] == right)
		{
			edgeData->edge_right_found = 1;
			edgeData->edge_right=i;
			break;
		}
	}

	//Linken Fahrbahnrand festlegen(=Flanke, die am weitesten Links liegt):
	for (int i = 0; i <= 127; i++)
	{
		if (edgeData->edgesMiddle[i] == left)
		{
			edgeData->edge_left_found = 1;
			edgeData->edge_left=i;
			break;
		}
	}

	//Ermittlung des Fahrbahnmittelpunktes:
	if((edgeData->edge_left_found == 0) && (edgeData->edge_right_found == 1)) //Nur rechter Fahrbahnrand gefunden
		edgeData->edge_center = edgeData->edge_right - edge_distance/2;
	else if((edgeData->edge_left_found == 1) && (edgeData->edge_right_found == 0)) //Nur linker Fahrbahnrand gefunden
		edgeData->edge_center = edgeData->edge_left + edge_distance/2;
	else if((edgeData->edge_left_found == 1) && (edgeData->edge_right_found == 1)) //Linker und rechter Fahrbahnrand gefunden
		edgeData->edge_center = (edgeData->edge_right + edgeData->edge_left)/2;

	//Berechnung des aktuellen Abstandes des Fahrzeugs zum Fahrbahnmittelpunkt:
	edgeData->edge_center_mm = (int)(edgeData->edge_center - 62) * (RoadWith/edge_distance);
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
