/*****************************************************************************************************/
/*****************************************************************************************************/
/*****             Erstellt von Summer Matthias und Ecker Christian am 20.11.2020                *****/
/*****                                                                                           *****/
/***** Funktionen zur Initialisierung des ServoMotors für die Lenkung                            *****/
/***** CTIMER1 PWM 2 Channel   --> Zum Lenken in CTIMER1->MSR[2] schreiben                       *****/
/*****                         --> Nur Werte von 200000 bis 459000 verwenden!!!                  *****/
/*****                         --> 200000 < x < 327000 -> Links Lenken                           *****/
/*****                         --> 327000 < x < 459000 -> Rechts lenken                          *****/
/***** Enthält auch DemoTask mit Links-Rechts-Mitte Lenkeinschlag                                *****/
/*****                                                                                           *****/
/***** Servo PWM an Pin P[3][2] und J13 Pin11 auf dem Board                                      *****/
/*****************************************************************************************************/
/*****************************************************************************************************/


#include "fsl_ctimer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "board.h"



/* Definition of channel 2 duty  --> on ticks*/
#define CTIMER1_PWM_0_DUTY 0

/* Definition of PWM period --> whole period ticks*/
#define CTIMER1_PWM_PERIOD 4400000

/* Definition of channel 2 ID */
#define CTIMER1_PWM_0_CHANNEL kCTIMER_Match_2

//max left steering value //Attention!! If less than 200000 -> 0,91ms--> hardware crash
#define SERVO_PWM_LeftValue 200000

//max right steering value //Attention!! If more than 459000 -> 2,08ms --> hardware crash
#define SERVO_PWM_RightValue 459000

//zero degree steering angle
#define SERVO_PWM_MiddleValue 327000


void SERVO_Init(void);
void CTIMER1_Init(void) ;
void SERVO_Demo(void *pvParameters);





