#include "servo.h"


const ctimer_config_t SERVO_config = {
		.mode = kCTIMER_TimerMode,   /* TC is incremented every rising APB bus clock edge */
		.input = kCTIMER_Capture_0,  /*!< Timer capture channel 0 */
		.prescale = 0                /*!< Prescale value 0 --> */
};

void SERVO_Init(void)
{

	CTIMER1_Init(); //Timer init function
	CTIMER1-> MCR 		|= CTIMER_MCR_MR2RL_MASK;
		//Reload MR2 with the contents of the shadow register when the timer counter (TC) is reset to 0
	IOCON->PIO[3][2]	&= 0xFFFFFFF0;      		//clear FUNC bits of P3.2
	IOCON->PIO[3][2]	|= 0x4;						//set FUNC bits to CTIMER1_MAT2 function ALT4 P3.2
	GPIO->DIR[3]        |= 1<<2;        			//set CTIMER1_MAT2 pin to output
	CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - SERVO_PWM_MiddleValue;
	if (xTaskCreate(SERVO_Demo, "SERVO_Demo", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL) !=
			pdPASS)
	{
		LED3_ON();
	}
}

void CTIMER1_Init(void)
{
	/* CTIMER1 peripheral initialization */
	CTIMER_Init(CTIMER1, &SERVO_config); //(CTIMER_Type, ctimer_config_t)
	/* PWM channel 2 of CTIMER1 peripheral initialization */
	CTIMER_SetupPwmPeriod(CTIMER1, CTIMER1_PWM_0_CHANNEL, CTIMER1_PWM_PERIOD, CTIMER1_PWM_PERIOD - CTIMER1_PWM_0_DUTY, false);
		//(CTIMER_Type, ctimer_match_t,uint32_t,uint32_t)
	/* Start the timer */
	CTIMER_StartTimer(CTIMER1); //Timer starten
}

void SERVO_Demo(void *pvParameters)
{
	while(1)
	{
		//Steer left
		CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - SERVO_PWM_LeftValue;
		vTaskDelay(500);
		//Steer right
		CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - SERVO_PWM_RightValue;
		vTaskDelay(800);
		//Steering angle zero degree
		CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - SERVO_PWM_MiddleValue;
		vTaskSuspend(NULL);
	}
}


