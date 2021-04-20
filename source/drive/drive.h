/*****************************************************************************************************/
/*****************************************************************************************************/
/*****             Erstellt von Summer Matthias und Ecker Christian am 04.12.2020                *****/
/*****                                                                                           *****/
/***** Funktionen zur Initialisierung der BLDC-Motoren für den Antrieb                           *****/
/***** CTIMER3 PWM 0/2 Channel 	--> Zum Fahren CTIMER1->MSR[2](Right Wheel) schreiben			 *****/
/***** 							--> und CTIMER1->MSR[0](Left Wheel) schreiben                    *****/
/*****                         	--> Nur Werte von 240000 bis 418000 verwenden!!!                 *****/
/*****                                                                                           *****/
/***** left BLDC PWM an Pin P[3][10] und J13 Pin7 auf dem Board    								 *****/
/***** right PWM an Pin P[0][27] und J13 Pin12 auf dem Board                                     *****/
/*****************************************************************************************************/
/*****************************************************************************************************/


#ifndef DRIVE_DRIVE_H_
#define DRIVE_DRIVE_H_

#include "fsl_ctimer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "board.h"


#define SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_CoreSysClk)

/* Definition of channel 2 duty  --> on ticks*/
#define CTIMER3_PWM_0_DUTY 0

/* Definition of PWM period --> whole period ticks*/
#define CTIMER3_PWM_PERIOD 4400000

/* Definition of channel 0 ID */
#define CTIMER3_PWM_LEFT_CHANNEL kCTIMER_Match_0

/* Definition of channel 2 ID */
#define CTIMER3_PWM_RIGHT_CHANNEL kCTIMER_Match_2

//Init High value for BLDC Init Sequence 1,5ms
#define BLDC_PWM_INIT_HIGH_VALUE 330000

//Init Low value for BLDC Init Sequence 1,0ms
#define BLDC_PWM_INIT_LOW_VALUE 220000

//Stop Throttle value (N = 0rpm) below 1,1ms
#define BLDC_PWM_STOPTHROTTLE 240000

//fULL Throttle value (N = MAX rpm) 1,9ms
#define BLDC_PWM_FULLTHROTTLE 418000


void BLDC_Init(void);
void CTIMER3_Init(void);
void delay_ms(uint32_t value);
void BLDC_Init_Task(void *pvParameters);



#endif /* DRIVE_DRIVE_H_ */
