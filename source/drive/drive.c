/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		drive.c							################
 ** Created on: 	04-12-2020						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	EC/MS	|04-12-2020	| Created drive.c															|
 ** 2	|	EC		|04-20-2021	| Commented the Code														|
 ** 3	|	MS		|06-07-2021	| implemented test function for BLDC and servo								|
 ** 4	|			|			| 																			|
 ** 5	|			|			| 																			|
 ** 6	|			|			| 																			|
 ** 7	|			|			| 																			|
 ** 8	|			|			| 																			|
 ** 9	|			|			| 																			|
 ** 10	|			|			| 																			|
 ************************************************************************************************************
 **
 **	Description
 ************************************************************************************************************
 ** C-file for initialization of the BLDC-drives:
 **
 ** Contains initialization functions
 **
 ** CTIMER3 PWM 0/2 Channel --> Write to CTIMER1->MSR[2] for PWM (Right drive)
 **							--> Write to CTIMER1->MSR[0] for PWM (Right drive)
 **                      	--> Use only values in Range 240000-418000 !!!!
 **
 ** Left  BLDC PWM at Pin P[3][10] (J13 Pin7)
 ** Right BLDC PWM at Pin P[0][27] (J13 Pin12)
 ************************************************************************************************************
 ***********************************************************************************************************/


#include "drive.h"

/*******************************************************************************
 * Parameters

 ******************************************************************************/
int32_t BLDCTestValue=10;	//value for test purpose
int8_t Lenkfaktor = 5;

int32_t* servoMiddle= &((all_param_t*)&const_all_param)->motors.servo.init; 	//servo init value
int32_t* servoLeft= &((all_param_t*)&const_all_param)->motors.servo.min;		//servo min value
int32_t* servoRight= &((all_param_t*)&const_all_param)->motors.servo.max;		//servo max value


extern volatile uint8_t edge_left;		//left Edge Coordinate
extern volatile uint8_t edge_right;	//Right Edge Coordinate

const ctimer_config_t BLDC_config = {
		.mode = kCTIMER_TimerMode,   /* TC is incremented every rising APB bus clock edge */
		.input = kCTIMER_Capture_0,  /*!< Timer capture channel 0 */
		.prescale = 0                /*!< Prescale value 0 --> */
};

int32_t* BLDCLeftInitValue= &((all_param_t*)&const_all_param)->motors.BLDCLeft.init; 	//left BLDC init value
int32_t* BLDCLeftMinValue= &((all_param_t*)&const_all_param)->motors.BLDCLeft.min;	//left BLDC min value
int32_t* BLDCLeftMaxValue= &((all_param_t*)&const_all_param)->motors.BLDCLeft.max;	//left BLDC max value

int32_t* BLDCRightInitValue= &((all_param_t*)&const_all_param)->motors.BLDCRight.init; 	//right BLDC init value
int32_t* BLDCRightMinValue= &((all_param_t*)&const_all_param)->motors.BLDCRight.min;	//right BLDC min value
int32_t* BLDCRightMaxValue=  &((all_param_t*)&const_all_param)->motors.BLDCRight.max;	//right BLDC max value


/*******************************************************************************
 * BLDC Main Initialization function
 ******************************************************************************/
void BLDC_Init(void)
{

	CTIMER3_Init(); //TIMER3 Initialization

	//*************************************************************
	//Configure Pin P[3][10] (J13 Pin7) (Left BLDC)
	CTIMER3-> MCR 		|= CTIMER_MCR_MR0RL_MASK;			//Reload MR0 with content of shadow register at timer overflow
	IOCON->PIO[3][10]	&= 0xFFFFFFF0;      				//Clear FUNC bits of P3.10
	IOCON->PIO[3][10]	|= 0x3;						//Set FUNC bits to CTIMER3_MAT0 function ALT3 P3.10
	GPIO->DIR[3]        |= 1<<10;        					//Set P3.10 to output
	CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;		//Initialize MSR with BLDC_PWM_INIT_LOW_VALUE value
	//*************************************************************

	//*************************************************************
	//Configure Pin P[0][27] (J13 Pin12) (Right BLDC)
	CTIMER3-> MCR 		|= CTIMER_MCR_MR2RL_MASK;			//Reload MR2 with content of shadow register at timer overflow
	IOCON->PIO[0][27]	&= 0xFFFFFFF0;      				//Clear FUNC bits of P0.27
	IOCON->PIO[0][27]	|= 0x3;						//Set FUNC bits to CTIMER3_MAT2 function ALT3 P0.27
	GPIO->DIR[0]        |= 1<<27;        					//Set P0.27 to output
	CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;		//Initialize MSR with BLDC_PWM_INIT_LOW_VALUE value
	//*************************************************************

	//*******************************************************************************************************
	//Create ESC initialization task
	if (xTaskCreate(ESC_Init_Task, "ESC_Init_Task", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL) != pdPASS)
	{ LED3_ON(); } //LED3 is Error
	//*******************************************************************************************************
}


/*******************************************************************************
 * TIMER3 Initialization function
 ******************************************************************************/
void CTIMER3_Init(void)
{
	/* CTIMER1 peripheral initialization */
	CTIMER_Init(CTIMER3, &BLDC_config);

	/* PWM channel 0 of CTIMER3 peripheral initialization (Left BLDC)*/
	CTIMER_SetupPwmPeriod(CTIMER3, CTIMER3_PWM_LEFT_CHANNEL, CTIMER3_PWM_PERIOD, CTIMER3_PWM_PERIOD - CTIMER3_PWM_0_DUTY, false);

	/* PWM channel 2 of CTIMER3 peripheral initialization (Left BLDC)*/
	CTIMER_SetupPwmPeriod(CTIMER3, CTIMER3_PWM_RIGHT_CHANNEL, CTIMER3_PWM_PERIOD, CTIMER3_PWM_PERIOD - CTIMER3_PWM_0_DUTY, false);

	/* Start TIMER3 */
	CTIMER_StartTimer(CTIMER3);
}


/*******************************************************************************
 * ESC Initialization Task
 ******************************************************************************/
void ESC_Init_Task(void *pvParameters)
{
	while(1)
	{
		//***************************************************************
		//Initialize Sequence for BLDC-Motors
		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;		//Low throttle value
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;		//Low throttle value
		vTaskDelay(2000);

		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_HIGH_VALUE;	//Half throttle value
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_HIGH_VALUE;	//Half throttle value
		vTaskDelay(2000);

		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;		//Low throttle value
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;		//Low throttle value
		vTaskDelay(3500);
		//***************************************************************


		//***************************************************************
		//Testing Sequence for BLDC-Motors

		/*CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - (*BLDCLeftMaxValue);		//Set motor to full speed
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - (*BLDCRightMaxValue);		//Set motor to full speed
		vTaskDelay(1000);
		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - (*BLDCLeftMinValue);		//Stop motor
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - (*BLDCRightMinValue);		//Stop motor*/

		//***************************************************************

		vTaskSuspend(NULL);	//suspend Task
	}
}

void Camera_Test_Drive (uint8_t state)
{
	menu_page_pixel_display_camera(1);
	int16_t testValueSpeed;							//calculated test value for min to max speed in µs
	int16_t Stearing_Value;
	float servo_Value = 30;
	int16_t middleLeftRight = 0;
	if(state==MENU_DEACT)
		{
			//calculate timer register value for stop
			CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - *BLDCLeftInitValue*CTIMER3_PWM_PERIOD/20000;//Linker Motor aus
			CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - *BLDCRightInitValue*CTIMER3_PWM_PERIOD/20000;//Rechter Motor aus
			CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD -(*servoMiddle)*CTIMER1_PWM_PERIOD/20000;	//Servo Mitte
			return;
		}
	//calculate test value for selected speed
	testValueSpeed= *BLDCLeftMinValue + BLDCTestValue*(*BLDCLeftMaxValue-*BLDCLeftMinValue)/100;// Fester Wert definiert--> evtl. noch dynamisch machen!!!

	//calculate timer register value for test speed
	CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - testValueSpeed * CTIMER3_PWM_PERIOD / 20000;
	CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - testValueSpeed * CTIMER3_PWM_PERIOD / 20000;

	//if left steering is tested
	middleLeftRight = (edge_left + edge_right) /2;
	servo_Value = 1.5625 * middleLeftRight - 100;

	servo_Value = servo_Value * Lenkfaktor;
	if (servo_Value >= 100) servo_Value = 100;
	if (servo_Value <= -100) servo_Value = -100;

	if(servo_Value <=0)
	{
		//calculate test value for left steering
		Stearing_Value= *servoMiddle+ servo_Value*(*servoMiddle-*servoLeft)/100;
	}
	else
	{
		//calculate test value for right steering
		Stearing_Value= *servoMiddle +servo_Value*(*servoRight-*servoMiddle)/100;
	}
	//set test value to timer register
	CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - Stearing_Value * CTIMER1_PWM_PERIOD / 20000;

}
