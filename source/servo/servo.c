/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		servo.c							################
 ** Created on: 	11-20-2020						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	EC/SM	|11-20-2020	| Created servo.c															|
 ** 2	|	EC		|04-20-2021	| Commented the Code														|
 ** 3	|			|			| 																			|
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
 ** C-file for initialization of the servo-drive (Steering):
 **
 ** Contains initialization functions and demo-task
 **
 ** CTIMER1 PWM 2 Channel  --> Write to CTIMER1->MSR[2] for alternating steering angle
 **                        --> Use only values in Range 200000-459000 !!!!
 **                        --> 200000 < x < 327000 -> steer left
 **                        --> 327000 < x < 459000 -> steer right
 **
 ** Servo-drive PWM at Pin P[3][2] (J13 Pin11)
 ************************************************************************************************************
 ***********************************************************************************************************/


#include "servo.h"


/*******************************************************************************
 * Parameters
 ******************************************************************************/
const ctimer_config_t SERVO_config = {
		.mode = kCTIMER_TimerMode,   /* TC is incremented every rising APB bus clock edge */
		.input = kCTIMER_Capture_0,  /*!< Timer capture channel 0 */
		.prescale = 0                /*!< Prescale value 0 --> */
};

int32_t* servoMiddleValue= &((all_param_t*)&const_all_param)->motors.servo.init;
int32_t* servoLeftValue= &((all_param_t*)&const_all_param)->motors.servo.min;
int32_t* servoRightValue= &((all_param_t*)&const_all_param)->motors.servo.max;



/*******************************************************************************
 * Servo Main Initialization function
 ******************************************************************************/
void SERVO_Init(void)
{
	CTIMER1_Init(); //TIMER1 Initialization

	//*************************************************************
	//Configure Pin P[3][2] (J13 Pin11) (Servo PWM)
	CTIMER1-> MCR 		|= CTIMER_MCR_MR2RL_MASK;					//Reload MR2 with content of shadow register at timer overflow
	IOCON->PIO[3][2]	&= 0xFFFFFFF0;      						//Clear FUNC bits of P3.2
	IOCON->PIO[3][2]	|= 0x4;										//Cet FUNC bits to CTIMER1_MAT2 function ALT4 P3.2
	GPIO->DIR[3]        |= 1<<2;        							//Set P3.2 pin to output
	CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD -(*servoMiddleValue)*CTIMER1_PWM_PERIOD/20000;	//Initialize MSR with SERVO_PWM_Middle_Value value
	//*************************************************************

	//*******************************************************************************************************
	//Create SERVO_Demo task
	if (xTaskCreate(SERVO_Demo, "SERVO_Demo", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL) != pdPASS)
	{ LED3_ON(); } //LED3 is Error
	//*******************************************************************************************************
}


/*******************************************************************************
 * TIMER1 Initialization function
 ******************************************************************************/
void CTIMER1_Init(void)
{
	/* CTIMER1 peripheral initialization */
	CTIMER_Init(CTIMER1, &SERVO_config);

	/* PWM channel 2 of CTIMER1 peripheral initialization */
	CTIMER_SetupPwmPeriod(CTIMER1, CTIMER1_PWM_0_CHANNEL, CTIMER1_PWM_PERIOD, CTIMER1_PWM_PERIOD - CTIMER1_PWM_0_DUTY, false);

	/* Start the timer */
	CTIMER_StartTimer(CTIMER1); //Timer starten
}


/*******************************************************************************
 * Servo Steering Demonstration Task
 ******************************************************************************/
void SERVO_Demo(void *pvParameters)
{

	while(1)
	{
		//***************************************************************
		//Demo Sequence for Servo-drive
		CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - SERVO_PWM_Left_Value;	//Steer left
		vTaskDelay(500);
		CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - SERVO_PWM_Right_Value;	//Steer right
		vTaskDelay(800);
		CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - SERVO_PWM_Middle_Value;	//Steering angle zero degree
		//***************************************************************

		vTaskSuspend(NULL);	//suspend Task
	}

}


