/*
 * drive.c
 *
 *  Created on: 04.12.2020
 *      Author: Summer Matthias, Ecker Christian
 */


#include "drive.h"


const ctimer_config_t BLDC_config = {
		.mode = kCTIMER_TimerMode,   /* TC is incremented every rising APB bus clock edge */
		.input = kCTIMER_Capture_0,  /*!< Timer capture channel 0 */
		.prescale = 0                /*!< Prescale value 0 --> */
};

void BLDC_Init(void)
{

	CTIMER3_Init(); //Timer init function
	CTIMER3-> MCR 		|= CTIMER_MCR_MR0RL_MASK;
		//Reload MR0 with the contents of the shadow register when the timer counter (TC) is reset to 0
	IOCON->PIO[3][10]	&= 0xFFFFFFF0;      		//clear FUNC bits of P3.10
	IOCON->PIO[3][10]	|= 0x3;						//set FUNC bits to CTIMER3_MAT0 function ALT3 P3.10
	GPIO->DIR[3]        |= 1<<10;        			//set P3.10 to output
	//initalice MSR register with BLDC_INIT_LOW value (1.0 ms on time)
	CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;

	CTIMER3-> MCR 		|= CTIMER_MCR_MR2RL_MASK;
		//Reload MR2 with the contents of the shadow register when the timer counter (TC) is reset to 0
	IOCON->PIO[0][27]	&= 0xFFFFFFF0;      		//clear FUNC bits of P0.27
	IOCON->PIO[0][27]	|= 0x3;						//set FUNC bits to CTIMER3_MAT2 function ALT3 P0.27
	GPIO->DIR[0]        |= 1<<27;        			//set P0.27 to output
	//initalice MSR register with BLDC_INIT_LOW value (1.0 ms on time)
	CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;

	//crate initialication task for ESCs
	if (xTaskCreate(BLDC_Init_Task, "BLDC_Init_Task", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL) !=
			pdPASS)
	{
		LED3_ON();
	}
}

void CTIMER3_Init(void)
{
	/* CTIMER1 peripheral initialization */
	CTIMER_Init(CTIMER3, &BLDC_config); //(CTIMER_Type, ctimer_config_t)
	/* PWM channel 0 of CTIMER3 peripheral initialization */
	CTIMER_SetupPwmPeriod(CTIMER3, CTIMER3_PWM_LEFT_CHANNEL, CTIMER3_PWM_PERIOD, CTIMER3_PWM_PERIOD - CTIMER3_PWM_0_DUTY, false);
		//(CTIMER_Type, ctimer_match_t,uint32_t,uint32_t)
	/* PWM channel 2 of CTIMER3 peripheral initialization */
	CTIMER_SetupPwmPeriod(CTIMER3, CTIMER3_PWM_RIGHT_CHANNEL, CTIMER3_PWM_PERIOD, CTIMER3_PWM_PERIOD - CTIMER3_PWM_0_DUTY, false);
		//(CTIMER_Type, ctimer_match_t,uint32_t,uint32_t)
	/* Start the timer */
	CTIMER_StartTimer(CTIMER3); //Timer starten
}

//Initialize ESCs
void BLDC_Init_Task(void *pvParameters)
{
	while(1){

		//Initialize Sequence for BLDC-Motors
		//low throttle value
		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;
		vTaskDelay(2000);
		//half throttle value
		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_HIGH_VALUE;
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_HIGH_VALUE;
		vTaskDelay(2000);
		//low throttle value
		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;
		vTaskDelay(3500);

		//set motor to full speed
		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - BLDC_PWM_FULLTHROTTLE;
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - BLDC_PWM_FULLTHROTTLE;
		vTaskDelay(1000);
		//stop motor
		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - BLDC_PWM_STOPTHROTTLE;
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - BLDC_PWM_STOPTHROTTLE;

		//suspend Task
		vTaskSuspend(NULL);
	}
}



