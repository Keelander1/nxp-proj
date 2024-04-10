/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		drive.c							################
 ** Created on: 	04-12-2020						#####| |########  University of applied sciences
 ** Authors: 										#####| |########  Landshut, Germany
 ** 												#####| |########
 ** 												#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
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
//#include <stdio.h>
#include <math.h>
//#include <eigen/eigen.h>
/*******************************************************************************
 * Parameters

 ******************************************************************************/
//Variablen für die Geschwindigkeitssteuerung:
double speed_value_left=0; //Geschwindigkeit BLDC_Left
double speed_value_right=0; //Geschwindigkeit BLDC_rigth
int finished=0; //Ziellinie überschritten(==1)

/******************************************************************************/
int32_t BLDCTestValue=30;	//value for test purpose
int8_t Lenkfaktor = 5;

int32_t* servoMiddle= &((all_param_t*)&const_all_param)->motors.servo.init; 	//servo init value
int32_t* servoLeft= &((all_param_t*)&const_all_param)->motors.servo.min;		//servo min value
int32_t* servoRight= &((all_param_t*)&const_all_param)->motors.servo.max;		//servo max value


//extern volatile uint8_t edge_left;		//left Edge Coordinate
//extern volatile uint8_t edge_right;	 //Right Edge Coordinate
//extern volatile int16_t	edge_center_mm; //Edge Center Coordinate
//extern volatile uint8_t edge_left_found;
//extern volatile uint8_t edge_right_found;

extern volatile struct EdgeDetectionData edgeData[2];
const uint16_t track_width = 520; //Width of the track in mm
const uint16_t axle_distance = 176; //Distance between Axles in mm
const uint16_t camera_distance = 380; //Distance between Camera View and Car Center in mm



//Varaibeln für Regler
int16_t Y_ist = 0;					//Ausgang Regelschleife in mm
int16_t Y_soll = 0;					//Eingang Regelschleife in mm
int16_t Y_diff = 0;					//Differenz Soll-Ist Regler in mm
int16_t D = 180;					//Achsabstand des Autos --> muss noch nachgemessen werden
//int16_t u = 0;						// Stellgröße in mm vor Umrechnung in °
int16_t P_Regler = 1;				//P-Regelverstärkung
double X = 230.0;					//Sichtweite Kamera in mm



// Vars Für State Control

double y = 0;						//Reglerausgnag Querabweichung
double y_hat = 0;					//Schätzung der Ausgangsgröße
double g_hat = 0;					//Differenz zwischen y und y_hat
double u = 0;						//Stellgröße u
double x1_hat = 0;					// Schätzung der Zustandsgröße 1
double x2_hat = 0;					// Schätzung der Zustandsgröße 2
double x3_hat = 0;					// Schätzung der Zustandsgröße 3
double x4_hat = 0;					// Schätzung der Zustandsgröße 4
double x1_hat_dot = 0;				// Ableitund der Schätzung der Zustandsgröße 1
double x2_hat_dot = 0;				// Ableitund der Schätzung der Zustandsgröße 2
double x3_hat_dot = 0;				// Ableitund der Schätzung der Zustandsgröße 3
double x4_hat_dot = 0;				// Ableitund der Schätzung der Zustandsgröße 4
double i = 0;
double I = 0;
double u_old = 0;
double u_sat = 0;

int k_1 = 96;					// *100
int k_2 = 16;					// *100
int k_3 = -165;					// *100
int k_4 = -269;					// *100
int k_yi = -150;					// *100

int l_1 = 0;					// *100
int l_2 = 0;					// *100
int l_3 = 1440;					// *100
int l_4 = 1200;					// *100

int Speed_Param = 20;

float gamma = 0;					//Lenkwinkel



//State Control_Old
float Stellgroese_rad_u = 0;
float Ausgangsrueckfuehrung_KYP = -6.8948;
int16_t Const_Test_Speed = 20;
uint16_t Speed_Left_normiert = 0;
uint16_t Speed_Right_normiert = 0;
uint16_t SpeedValueLeft = 0;
uint16_t SpeedValueRight = 0;
float Querabweichung_m_y = 0;
double Spurweiter_m = 0.14;					//define
float ReglerSollgroeße_m = 0;				//define
float servo_Value = 0;
//int16_t gamma = 0;
float Stearing_Value = 0;
extern volatile USS_Distance;


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
	CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;		//Initialize MSR with BLDC_PWM_INIT_LOW_VALUE value (Left)
	//*************************************************************

	//*************************************************************
	//Configure Pin P[0][27] (J13 Pin12) (Right BLDC)
	CTIMER3-> MCR 		|= CTIMER_MCR_MR2RL_MASK;			//Reload MR2 with content of shadow register at timer overflow
	IOCON->PIO[0][27]	&= 0xFFFFFFF0;      				//Clear FUNC bits of P0.27
	IOCON->PIO[0][27]	|= 0x3;						//Set FUNC bits to CTIMER3_MAT2 function ALT3 P0.27
	GPIO->DIR[0]        |= 1<<27;        					//Set P0.27 to output
	CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - BLDC_PWM_INIT_LOW_VALUE;		//Initialize MSR with BLDC_PWM_INIT_LOW_VALUE value (Right)
	//*************************************************************

	//*******************************************************************************************************
	//Create ESC initialization task
	//if (xTaskCreate(ESC_Init_Task, "ESC_Init_Task", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL) != pdPASS)
	//{ LED3_ON(); } //LED3 is Error
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
		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - (*BLDCLeftMinValue);			//Stop motor
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - (*BLDCRightMinValue);		//Stop motor*/

		//***************************************************************

		vTaskSuspend(NULL);	//suspend Task
	}
}

//
//
///*************************************************************************
//First Test Drive:
//FUNTCTIONALITY:
//--- Test road detection
//--- Test steering with P-Regler
//DESCRIPTION:
//--- Fix speed value
//**************************************************************************/
//void Camera_Test_Drive (uint8_t state)
//{
//	menu_page_pixel_display_camera(1);
//	int16_t testValueSpeed;							//calculated test value for min to max speed in µs
//	int16_t Stearing_Value;
//	//int16_t x = camera_distance;
//	int16_t y = edgeData->edge_center_mm - 63;
//	int16_t servo_Value = 0; //between -100 and 100
//	if(state==MENU_DEACT)
//		{
//			//calculate timer register value for stop
//			CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - *BLDCLeftInitValue*CTIMER3_PWM_PERIOD/20000;//Linker Motor aus
//			CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - *BLDCRightInitValue*CTIMER3_PWM_PERIOD/20000;//Rechter Motor aus
//			CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD -(*servoMiddle)*CTIMER1_PWM_PERIOD/20000;	//Servo Mitte
//			return;
//		}
//	//calculate test value for selected speed
//	testValueSpeed= *BLDCLeftMinValue + BLDCTestValue*(*BLDCLeftMaxValue-*BLDCLeftMinValue)/100;// Fester Wert definiert--> evtl. noch dynamisch machen!!!
//
//	//calculate timer register value for test speed
//	CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - testValueSpeed * CTIMER3_PWM_PERIOD / 20000;
//	CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - testValueSpeed * CTIMER3_PWM_PERIOD / 20000;
//
//
//	servo_Value = y;
//
//
//	if((edgeData->edge_left_found == 0) && (edgeData->edge_right_found == 0))
//		servo_Value = 0;
//
//	servo_Value = servo_Value * Lenkfaktor;
//	if (servo_Value >= 100) servo_Value = 100;
//	if (servo_Value <= -100) servo_Value = -100;
//
//	if(servo_Value <=0)
//	{
//		//calculate test value for left steering
//		Stearing_Value= *servoMiddle+ servo_Value*(*servoMiddle-*servoLeft)/100;
//	}
//	else
//	{
//		//calculate test value for right steering
//		Stearing_Value= *servoMiddle +servo_Value*(*servoRight-*servoMiddle)/100;
//	}
//	//set test value to timer register
//	CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - Stearing_Value * CTIMER1_PWM_PERIOD / 20000;
//
//}
///*********************************************************************
// Second Test Drive:
// FUNTKIONALITY:
// ---Test if the vehicle stays on the track with higher speed
// ---Test calculation for speed reduction of the inner wheel, speed raise of the outer wheel
// 	based on the one-wheel model
// DESCRIPTION:
// ---Variable speed value
// 	[given through SW: proportional decreasing speed with higher steering value
// 	and different speed of the inner wheel]
// ---Steering control with P-Regler (same control as of the first test drive)
// ---Select camera input values depending on the Edge detection for steering control
// *********************************************************************/


void stear(float angle){
	int16_t direction = 0;
	uint16_t pwmDelta;
	uint16_t pwm = ((all_param_t*)&const_all_param)->motors.servo.init;

	if(angle < 0){
		direction = left_corner;
		angle = angle * -1;
	}
	else {
		direction = right_corner;
	};

	pwmDelta = (uint16_t) 1478*angle*angle + 574*angle;
	pwm += pwmDelta*direction;

	if(pwm > all_param->motors.servo.max){
			pwm = all_param->motors.servo.max;
		}

	if(pwm < all_param->motors.servo.min){
		pwm = all_param->motors.servo.min;
	}

	CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - pwm * CTIMER1_PWM_PERIOD / 20000;
}


/*void Real_Drive (uint8_t state)
{

	//menu_page_pixel_display_camera(1);
	int16_t servo_Value = 0; //between -100 and 100
	int16_t Stearing_Value = 0;
	int16_t Speed_Left_normiert = 0;		// Speed Value Left normiert auf 0 bis 100
	int16_t Speed_Right_normiert = 0;		// Speed Value Right normiert auf 0 bis 100
	int16_t Speed = 0;
	int16_t Max_Speed = 30;
	int16_t Min_Speed = 10;
	int16_t SpeedValueLeft = 0;
	int16_t SpeedValueRight = 0;
	int16_t Spurweite = 135;				// muss noch Richtig abgemessen werden
	// Lenkregler

	// aktuell 0 ist alte
	// 1 ist neue
	for(uint8_t i = 1; i <2; i++){
		switch(edgeData[i].edge_left_found + (edgeData[i].edge_right_found << 1)){
			case 0:	//no Edge found
				break;

			case 1:	//left Edge found
				break;

			case 2:	//right Edge found
				break;

			case 3:	//left & right Edge found

				break;
		}
	}

	Y_ist = edgeData[0].edge_center_mm;


	//Bestimmung Regelfehler
	Y_ist = edgeData[0].edge_center_mm;
	Y_soll = 0;
	Y_diff = edgeData[0].edge_center_mm - Y_soll;
	//printf("Y_diff: %d \n edge_center_mm: %d", Y_diff, edge_center_mm);
	// Krümmung in Variable k
	//k = (2*Y) / (X^2 + Y_diff^2);

	// Stellgröße u = Regelfehler * Regler --> vorerst P mit Verstärkung 1

	u = Y_diff * P_Regler;


	// Umrechnung von Stellgröße u in mm auf Lenkwinkel in Grad

	gamma = atan((2.0*D*u) / (X*X + u*u));
	gamma = gamma * 180 / 3.14;				//Umrechnung Lenkwinkel in Grad


	//Begrenzung Lenkwinkel auf +- 60°
	if (gamma <= -60)
	{
		gamma = -60;
	}
	else if (gamma >= 60)
	{
		gamma = 60;
	}

	//Skalierung auf +- 100 --> 60 ° entspricht +100
	gamma = gamma * 10 / 6;
	servo_Value = (int16_t)gamma;

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
		Stearing_Value= *servoMiddle + servo_Value*(*servoRight-*servoMiddle)/100;
	}

	//Schreiben des Umgerechneten PWM Werts ins Register für die Lenkwinkelsteuerung
	CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - Stearing_Value * CTIMER1_PWM_PERIOD / 20000;

	//*********************Beginn Antreibssteuerung

	//****************************************************
	//*****************Geschwindigkeit********************
	//****************************************************

	Speed = abs(servo_Value);
	Speed = 100 - Speed;			// da Max Geschwindigkeit, bei Geradeausfahrt

	Speed = Speed * Max_Speed / 100;			// So wird die Maximale Geschwindigkeit erstmal auf 30% beschränkt--> ziel ist natürlich 100%
	if (Speed <= 10)
	{
		Speed = Min_Speed;
	}


	// In Var. Speed soll die Mittleregeschwindigkeit beschrieben werden.
	// Da jedoch in Kurvenfahrten die Geschwindigkeit des inneren und es äußeren Rades abweichen, werden
	// im Folgenden für die Motren Rechts und Links die Geschwindkeit angepasst


	Speed_Left_normiert  = Speed * (1 + ((Spurweite * Y_diff) / (X*X + Y_diff*Y_diff)));
	Speed_Right_normiert = Speed * (1 - ((Spurweite * Y_diff) / (X*X + Y_diff*Y_diff)));


	//calculate test value for selected speed
	SpeedValueLeft= *BLDCLeftMinValue + Speed_Left_normiert*(*BLDCLeftMaxValue-*BLDCLeftMinValue)/100;// Fester Wert definiert--> evtl. noch dynamisch machen!!!
	SpeedValueRight= *BLDCRightMinValue + Speed_Right_normiert*(*BLDCRightMaxValue-*BLDCRightMinValue)/100;// Fester Wert definiert--> evtl. noch dynamisch machen!!!

	//Berechnung Wert des Registers für Rechten und Linken Motor
	CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - SpeedValueLeft * CTIMER3_PWM_PERIOD / 20000;	// (Left)
	CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - SpeedValueRight * CTIMER3_PWM_PERIOD / 20000; //(Right)



//	//menu_page_pixel_display_camera(1);
//	int16_t servo_Value = 0; //between -100 and 100
//	int16_t Stearing_Value = 0;
//	int16_t Speed_Left_normiert = 0;		// Speed Value Left normiert auf 0 bis 100
//	int16_t Speed_Right_normiert = 0;		// Speed Value Right normiert auf 0 bis 100
//	int16_t Speed = 0;
//	int16_t Max_Speed = 30;
//	int16_t Min_Speed = 10;
//	int16_t SpeedValueLeft = 0;
//	int16_t SpeedValueRight = 0;
//	int16_t Spurweite = 135;				// muss noch Richtig abgemessen werden
//	// Lenkregler
//
//	// aktuell 0 ist alte
//	// 1 ist neue
//	for(uint8_t i = 1; i <2; i++){
//		switch(edgeData[i].edge_left_found + (edgeData[i].edge_right_found << 1)){
//			case 0:	//no Edge found
//				break;
//
//			case 1:	//left Edge found
//				break;
//
//			case 2:	//right Edge found
//				break;
//
//			case 3:	//left & right Edge found
//
//				break;
//		}
//	}
//
//	Y_ist = edgeData[0].edge_center_mm;
//
//
//	//Bestimmung Regelfehler
//	Y_ist = edgeData[0].edge_center_mm;
//	Y_soll = 0;
//	Y_diff = edgeData[0].edge_center_mm - Y_soll;
//	//printf("Y_diff: %d \n edge_center_mm: %d", Y_diff, edge_center_mm);
//	// Krümmung in Variable k
//	//k = (2*Y) / (X^2 + Y_diff^2);
//
//	// Stellgröße u = Regelfehler * Regler --> vorerst P mit Verstärkung 1
//
//	u = Y_diff * P_Regler;
//
//
//	// Umrechnung von Stellgröße u in mm auf Lenkwinkel in Grad
//
//	gamma = atan((2.0*D*u) / (X*X + u*u));
//	gamma = gamma * 180 / 3.14;				//Umrechnung Lenkwinkel in Grad
//
//
//	//Begrenzung Lenkwinkel auf +- 60°
//	if (gamma <= -60)
//	{
//		gamma = -60;
//	}
//	else if (gamma >= 60)
//	{
//		gamma = 60;
//	}
//
//	//Skalierung auf +- 100 --> 60 ° entspricht +100
//	gamma = gamma * 10 / 6;
//	servo_Value = (int16_t)gamma;
//
//	if (servo_Value >= 100) servo_Value = 100;
//	if (servo_Value <= -100) servo_Value = -100;
//
//	if(servo_Value <=0)
//	{
//		//calculate test value for left steering
//		Stearing_Value= *servoMiddle+ servo_Value*(*servoMiddle-*servoLeft)/100;
//	}
//	else
//	{
//		//calculate test value for right steering
//		Stearing_Value= *servoMiddle + servo_Value*(*servoRight-*servoMiddle)/100;
//	}
//
//	//Schreiben des Umgerechneten PWM Werts ins Register für die Lenkwinkelsteuerung
//	CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - Stearing_Value * CTIMER1_PWM_PERIOD / 20000;
//
//	//*********************Beginn Antreibssteuerung
//
//
//
//	Speed = abs(servo_Value);
//	Speed = 100 - Speed;			// da Max Geschwindigkeit, bei Geradeausfahrt
//
//	Speed = Speed * Max_Speed / 100;			// So wird die Maximale Geschwindigkeit erstmal auf 30% beschränkt--> ziel ist natürlich 100%
//	if (Speed <= 10)
//	{
//		Speed = Min_Speed;
//	}
//
//
//	// In Var. Speed soll die Mittleregeschwindigkeit beschrieben werden.
//	// Da jedoch in Kurvenfahrten die Geschwindigkeit des inneren und es äußeren Rades abweichen, werden
//	// im Folgenden für die Motren Rechts und Links die Geschwindkeit angepasst
//
//
//	Speed_Left_normiert  = Speed * (1 + ((Spurweite * Y_diff) / (X*X + Y_diff*Y_diff)));
//	Speed_Right_normiert = Speed * (1 - ((Spurweite * Y_diff) / (X*X + Y_diff*Y_diff)));
//
//
//	//calculate test value for selected speed
//	SpeedValueLeft= *BLDCLeftMinValue + Speed_Left_normiert*(*BLDCLeftMaxValue-*BLDCLeftMinValue)/100;// Fester Wert definiert--> evtl. noch dynamisch machen!!!
//	SpeedValueRight= *BLDCRightMinValue + Speed_Right_normiert*(*BLDCRightMaxValue-*BLDCRightMinValue)/100;// Fester Wert definiert--> evtl. noch dynamisch machen!!!
//
//	//Berechnung Wert des Registers für Rechten und Linken Motor
//	CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - SpeedValueLeft * CTIMER3_PWM_PERIOD / 20000;	// (Left)
//	CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - SpeedValueRight * CTIMER3_PWM_PERIOD / 20000; //(Right)
//
//
//
}
//

*/


/*********************************************************************
 Third Test Drive:
 FUNTKIONALITY:
 ---Test if the vehicle stays on the track with constant speed
 ---Zustandsregler mit Ausgangsrückführung
 DESCRIPTION:
 ---Constant speed value
 ---Steering control with P-Control (Based on Matlab_Model
 *********************************************************************/
/*
void StateControl_old(uint8_t state)
{

	//menu_page_pixel_display_camera(1);

	//if(state==MENU_DEACT)
	//{
// Set the Speed -- here is not the Speed decision!!!
		Speed_Left_normiert  = Const_Test_Speed; //* (1 + ((Spurweite * Y_diff) / (X*X + Y_diff*Y_diff)));
		Speed_Right_normiert = Const_Test_Speed; //* (1 - ((Spurweite * Y_diff) / (X*X + Y_diff*Y_diff)));
		//calculate test value for selected speed
		SpeedValueLeft= *BLDCLeftMinValue + Speed_Left_normiert*(*BLDCLeftMaxValue-*BLDCLeftMinValue)/100;// Fester Wert definiert--> evtl. noch dynamisch machen!!!
		SpeedValueRight= *BLDCRightMinValue + Speed_Right_normiert*(*BLDCRightMaxValue-*BLDCRightMinValue)/100;// Fester Wert definiert--> evtl. noch dynamisch machen!!!
//Berechnung und Setzen der Werte der Register für Rechten und Linken Motor
		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - SpeedValueLeft * CTIMER3_PWM_PERIOD / 20000;	// (Left)
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - SpeedValueRight * CTIMER3_PWM_PERIOD / 20000; //(Right)



//Bestimmen der Stellgröße

		Querabweichung_m_y = (float)edgeData[0].edge_center_mm / 1000.0;
		Stellgroese_rad_u = ReglerSollgroeße_m - Ausgangsrueckfuehrung_KYP*Querabweichung_m_y;

		gamma = (int16_t)(Stellgroese_rad_u*0.5*180/3.14);		// 30°Lenkwinkel Entsprechen 60° beim Servo

		stear(Stellgroese_rad_u);

//		//Faktor Lenkwinkel zu Servo-Winkel
//		if (gamma <= -60)
//			{
//				gamma = -60;
//			}
//			else if (gamma >= 60)
//			{
//				gamma = 60;
//			}
//			//Skalierung auf +- 100 --> 60 ° entspricht +100
//			gamma = gamma * 10 / 6;
//			servo_Value = (int16_t)gamma;
//
//			if (servo_Value >= 100) servo_Value = 100;
//			if (servo_Value <= -100) servo_Value = -100;
//
//			if(servo_Value <=0)
//			{
//				//calculate test value for left steering
//				Stearing_Value= *servoMiddle+ servo_Value*(*servoMiddle-*servoLeft)/100;
//			}
//			else
//			{
//				//calculate test value for right steering
//				Stearing_Value= *servoMiddle + servo_Value*(*servoRight-*servoMiddle)/100;
//			}
//
//			//Schreiben des Umgerechneten PWM Werts ins Register für die Lenkwinkelsteuerung
//			CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - Stearing_Value * CTIMER1_PWM_PERIOD / 20000;
//
//
//
//
//
	}
*/
/*******************************************************************************
 * Regleung mit Beobachter
 *
 *
 *
 ******************************************************************************/


void StateControl(uint8_t state)
{
	SpeedControl();

	double lenkeinschlag = 0;
	double k1 = k_1/100.0;
	double k2 = k_2/100.0;
	double k3 = k_3/100.0;
	double k4 = k_4/100.0;
	double KYI = k_yi/100.0;

	double l1 = l_1/100.0;
	double l2 = l_2/100.0;
	double l3 = l_3/100.0;
	double l4 = l_4/100.0;


	controlParam->testParam = 0;	//Testparameter für EEPROM


	double sleeptime = t/1000.0;
	//menu_page_pixel_display_camera(1);

	//_________________________________
	// stop if Uss detect an object
	/*
	if (USS_Distance <= 40)
		Const_Test_Speed = 0;
	else
		Const_Test_Speed = Speed_Param;
	*/

	//__________________________________

	int cam = 1;
		switch(edgeData[cam].edge_left_found + (edgeData[cam].edge_right_found << 1))
		{
			case 0:	//no Edge found
				lenkeinschlag = 0;

				break;

			case 1:	//left Edge found
				lenkeinschlag = KYI;
				// Input Data for funktion
				Querabweichung_m_y = (float)edgeData[cam].edge_center_mm / 1000.0;
				y = Querabweichung_m_y;
				break;

			case 2:	//right Edge found
				lenkeinschlag = KYI;
				// Input Data for funktion
				Querabweichung_m_y = (float)edgeData[cam].edge_center_mm / 1000.0;
				y = Querabweichung_m_y;
				break;

			case 3:	//left & right Edge found
				lenkeinschlag = KYI;
				Querabweichung_m_y = (float)edgeData[cam].edge_center_mm / 1000.0;
				y = Querabweichung_m_y;

				break;
		}
		//______________________
	//___________________________________________________________
	// Input Data for funktion
	//Querabweichung_m_y = (float)edgeData[0].edge_center_mm / 1000.0;
	//y = Querabweichung_m_y;

	//_____________________________________________________________

	//Reglergleichungen
	// Gleichung I-Anteil
	//i = 0 - y + (u_sat - u_old) * Windup;
	//I = I +  i * sleeptime;
	// Gleichung 1
	//u = 0 + I*KYI - (k1*x1_hat + k2*x2_hat + k3*x3_hat + k4*x4_hat);
	//u = 0 - (k1*x1_hat + k2*x2_hat + k3*x3_hat + k4*x4_hat);			// ohne I anteil
	u = 0 - lenkeinschlag*y;
	//Begrenzung u
	if (u <= -0.54)
		{
			u_old = u;
			u = -0.54;
			u_sat = u;

		}
	else if (u>=0.54)
		{
			u_old = u;
			u = 0.54;
			u_sat = u;
		}
	else
	{
		u_old = u;
		u_sat = u;
	}

	//Gleichung2
	g_hat = y - y_hat;
	//Gleichung 3
	x1_hat_dot = l1*g_hat + B1*u + a11*x1_hat + a12*x2_hat + a13*x3_hat + a14*x4_hat;
	x2_hat_dot = l2*g_hat + B2*u + a21*x1_hat + a22*x2_hat + a23*x3_hat + a24*x4_hat;
	x3_hat_dot = l3*g_hat + B3*u + a31*x1_hat + a32*x2_hat + a33*x3_hat + a34*x4_hat;
	x4_hat_dot = l4*g_hat + B4*u + a41*x1_hat + a42*x2_hat + a43*x3_hat + a44*x4_hat;
	//Gleichung4
	x1_hat = x1_hat + x1_hat_dot * sleeptime;
	x2_hat = x2_hat + x2_hat_dot * sleeptime;
	x3_hat = x3_hat + x3_hat_dot * sleeptime;
	x4_hat = x4_hat + x4_hat_dot * sleeptime;
	//Gleichung 5
	y_hat = c1*x1_hat + c2*x2_hat + c3*x3_hat + c4*x4_hat;



	stear(u);
//
//	gamma = (int16_t)(u*2*180/3.14);		// 30°Lenkwinkel Entsprechen 60° beim Servo
//	//Faktor Lenkwinkel zu Servo-Winkel
//	if (gamma <= -60)
//	{
//		gamma = -60;
//	}
//	else if (gamma >= 60)
//	{
//		gamma = 60;
//	}
//
//	//Skalierung auf +- 100 --> 60 ° entspricht +100
//	gamma = gamma * 10 / 6;
//	servo_Value = (int16_t)gamma;
//
//	if (servo_Value >= 100) servo_Value = 100;
//	if (servo_Value <= -100) servo_Value = -100;
//	if(servo_Value <=0)
//	{
//	//calculate test value for left steering
//	Stearing_Value= *servoMiddle+ servo_Value*(*servoMiddle-*servoLeft)/100;
//	}
//	else
//	{
//	//calculate test value for right steering
//	Stearing_Value= *servoMiddle + servo_Value*(*servoRight-*servoMiddle)/100;
//	}
////	//Schreiben des Umgerechneten PWM Werts ins Register für die Lenkwinkelsteuerung
	//CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - Stearing_Value * CTIMER1_PWM_PERIOD / 20000;

	// Delay the Task --> for integration over time
	//vTaskDelay(t);
/*
	//Set the Speed -- here is not the Speed decision!!!
			Speed_Left_normiert  = Const_Test_Speed;// * (1 + ((Spurweiter_m * y) / (X*X + y*y)));
			Speed_Right_normiert = Const_Test_Speed;// * (1 - ((Spurweiter_m * y) / (X*X + y*y)));
			//calculate test value for selected speed
			SpeedValueLeft= *BLDCLeftMinValue + Speed_Left_normiert*(*BLDCLeftMaxValue-*BLDCLeftMinValue)/100;// Fester Wert definiert--> evtl. noch dynamisch machen!!!
			SpeedValueRight= *BLDCRightMinValue + Speed_Right_normiert*(*BLDCRightMaxValue-*BLDCRightMinValue)/100;// Fester Wert definiert--> evtl. noch dynamisch machen!!!
	//Berechnung und Setzen der Werte der Register für Rechten und Linken Motor
			if (Const_Test_Speed == 0)
			{
			CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - (SpeedValueLeft-10) * CTIMER3_PWM_PERIOD / 20000;	// (Left)
			CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - (SpeedValueRight-10) * CTIMER3_PWM_PERIOD / 20000; //(Right)
			}
			else
			{
				CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - SpeedValueLeft * CTIMER3_PWM_PERIOD / 20000;	// (Left)
				CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - SpeedValueRight * CTIMER3_PWM_PERIOD / 20000; //(Right)
			}
*/


	}
/**********************************************************************************************
 * Speed Control:
 * FUNKTIONALITY: Set Speed-Value according to route section
 * DESCRIPTION:
 **********************************************************************************************/

void SpeedControl(void) //Beide Kameras
{

	//Identifikation des Streckenabschnittes(mit Kamera1 und Kamera2):
	/*
	unsigned int straight_line=edgeData[0].edge_left_found&&edgeData[0].edge_right_found&&edgeData[1].edge_left_found&&edgeData[1].edge_right_found; //Gerade Strecke
	unsigned int crossover1=(edgeData[0].edge_left_found||edgeData[0].edge_right_found)&&edgeData[1].edge_left_found&&edgeData[1].edge_right_found;  //Kreuzung
	unsigned int crossover2=!edgeData[0].edge_left_found&&!edgeData[0].edge_right_found&&edgeData[1].edge_left_found&&edgeData[1].edge_right_found;	 //Kreuzung
	unsigned int right_curve=edgeData[0].edge_left_found&&edgeData[0].edge_right_found&&edgeData[1].edge_left_found&&!edgeData[1].edge_right_found;  //Rechte Kurve
	unsigned int left_curve=edgeData[0].edge_left_found&&edgeData[0].edge_right_found&&!edgeData[1].edge_left_found&&edgeData[1].edge_right_found;	 //Linke Kurve
	*/

	//Identifikation des Streckenabschnittes (nur mit Kamera1):
	unsigned int straight_line=edgeData[0].edge_left_found&&edgeData[0].edge_right_found; //Gerade Strecke
	unsigned int crossover1=!edgeData[0].edge_left_found&&!edgeData[0].edge_right_found; //Kreuzung
	unsigned int right_curve=edgeData[0].edge_left_found&&!edgeData[0].edge_right_found; //Rechtskurve
	unsigned int left_curve=!edgeData[0].edge_left_found&&edgeData[0].edge_right_found; //Linkskurve


	double r=0; //Radius
	double winkel_v=0; //Winkelgeschwindigkeit

	double pulse_right=0;
	double pulse_left=0;

	double distance=0; //Abstand Ultraschallsensor (cm)

	if((USS_Distance<=40)&&(finished==0)) //Falls die Ziellinie nicht erkannt wurde (und Geschwindigkeit nicht reduziert wurde) soll 30 cm vor dem Hinderniss bereits auf Null abgebremst werden
	{
		//Stillstand:
		pulse_right=(*BLDCRightMinValue)*0.001-0.1;
		pulse_left=(*BLDCLeftMinValue)*0.001-0.1;

		//Neuer Geschwindikeitswert ins Timer-Register schreiben:
		CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
		CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;

	}
	else if (USS_Distance<=40) //Reduzierung der Geschwindigkeit wenn Hinderniss erkannt
	{
		distance=USS_Distance;

		if(distance<=15) //Stillstand
		{
			pulse_right=(*BLDCRightMinValue)*0.001-0.1;
			pulse_left=(*BLDCLeftMinValue)*0.001-0.1;

			//Neuer Geschwindikeitswert ins Timer-Register schreiben:
			CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
			CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;
		}
		else //Reduzierung der Geschwindigkeit mit Bremsfaktor
		{
			speed_value_right=sqrt(2.0*BRAKE_FAKTOR*(40.0-distance)*0.01+SPEED_AFTER_FINISH*SPEED_AFTER_FINISH);
			speed_value_left=sqrt(2.0*BRAKE_FAKTOR*(40.0-distance)*0.01+SPEED_AFTER_FINISH*SPEED_AFTER_FINISH);

			pulse_right=(*BLDCRightMinValue)*0.001+(speed_value_right/SPEED_TO_PULSE);
			pulse_left=(*BLDCLeftMinValue)*0.001+(speed_value_left/SPEED_TO_PULSE);

			//Neuer Geschwindikeitswert ins Timer-Register schreiben:
			CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
			CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;
		}
	}
	else if(edgeData[0].finish_detected==1)	//Reduzierung der Geschwindigkeit wenn Ziel erkannt:
	{
		/*
		//Test (Stillstand bei Erkennung der Ziellinie):
		pulse_right=(*BLDCRightMinValue)*0.001-0.1;
		pulse_left=(*BLDCLeftMinValue)*0.001-0.1;

		//Neuer Geschwindikeitswert ins Timer-Register schreiben:
		CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
		CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;
		*/

		speed_value_right=SPEED_AFTER_FINISH;
		speed_value_left=SPEED_AFTER_FINISH;


		pulse_right=(*BLDCRightMinValue)*0.001+(speed_value_right/SPEED_TO_PULSE);
		pulse_left=(*BLDCLeftMinValue)*0.001+(speed_value_left/SPEED_TO_PULSE);

		//Neuer Geschwindikeitswert ins Timer-Register schreiben:
		CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
		CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;


		finished=1;

	}
	else if(finished==0) //Ziellinie noch nicht überschritten (Wenn Ziellinie überschritten keine Reduzierung der Geschwindigkeit in der Kurve, da gering genung)
	{
		if(straight_line||crossover1) //Fahren mit maximaler Geschwindigkeit [Gerade,Kreuzung,unebener Fahrbahnrand]
		{
			speed_value_right=SPEED_MAX;
			speed_value_left=SPEED_MAX;

			pulse_right=(*BLDCRightMinValue)*0.001+(speed_value_right/SPEED_TO_PULSE);
			pulse_left=(*BLDCLeftMinValue)*0.001+(speed_value_left/SPEED_TO_PULSE);

			//Neuer Geschwindikeitswert ins Timer-Register schreiben:
			CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
			CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;

		}
		else if(left_curve) //Abbremsen bei Erkennung einer Links-Kurve [unterschiedliche Ansteuerung der Reifen]
		{
			r=RADIUS+edgeData[0].edge_center_mm;
			winkel_v=SPEED_CURVE*r;

			speed_value_right=winkel_v/(r+0.5*WHEELBASE);
			speed_value_left=winkel_v/(r-0.5*WHEELBASE);

			pulse_right=(*BLDCRightMinValue)*0.001+(speed_value_right/SPEED_TO_PULSE);
			pulse_left=(*BLDCLeftMinValue)*0.001+(speed_value_left/SPEED_TO_PULSE);

			//Neuer Geschwindikeitswert ins Timer-Register schreiben:
			CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
			CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;
		}
		else if(right_curve) //Abbremsen bei Erkennung einer Rechts-Kurve [unterschiedliche Ansteuerung der Reifen]
		{
			r=RADIUS-edgeData[0].edge_center_mm;
			winkel_v=SPEED_CURVE*r;

			speed_value_right=winkel_v/(r-0.5*WHEELBASE);
			speed_value_left=winkel_v/(r+0.5*WHEELBASE);

			pulse_right=(*BLDCRightMinValue)*0.001+(speed_value_right/SPEED_TO_PULSE);
			pulse_left=(*BLDCLeftMinValue)*0.001+(speed_value_left/SPEED_TO_PULSE);

			//Neuer Geschwindikeitswert ins Timer-Register schreiben:
			CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
			CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;
		}
		else //Falls gar nichts erkannt wird soll stehen geblieben werden
		{
			pulse_right=(*BLDCRightMinValue)*0.001-0.1;
			pulse_left=(*BLDCLeftMinValue)*0.001-0.1;

			//Neuer Geschwindikeitswert ins Timer-Register schreiben:
			CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
			CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;
		}
	}
}

/********************************************************************************************************************
 * First_Try: Fast_Mode (beim ersten Umfahren der Strecke soll mit schnellerer Geschwindigkeit gefahren werden)
********************************************************************************************************************/
void StateControl2(uint8_t state)
{
	SpeedControl2();

	double lenkeinschlag;
	double k1 = k_1/100.0;
	double k2 = k_2/100.0;
	double k3 = k_3/100.0;
	double k4 = k_4/100.0;
	double KYI = k_yi/100.0;

	double l1 = l_1/100.0;
	double l2 = l_2/100.0;
	double l3 = l_3/100.0;
	double l4 = l_4/100.0;


	controlParam->testParam = 0;	//Testparameter für EEPROM


	double sleeptime = t/1000.0;
	//menu_page_pixel_display_camera(1);

	//_________________________________
	// stop if Uss detect an object
	/*
	if (USS_Distance <= 40)
		Const_Test_Speed = 0;
	else
		Const_Test_Speed = Speed_Param;
	*/

	//__________________________________
	int cam = 1;
	switch(edgeData[cam].edge_left_found + (edgeData[cam].edge_right_found << 1))
	{
		case 0:	//no Edge found
			lenkeinschlag = 0;

			break;

		case 1:	//left Edge found
			lenkeinschlag = KYI;
			// Input Data for funktion
			Querabweichung_m_y = (float)edgeData[cam].edge_center_mm / 1000.0;
			y = Querabweichung_m_y;
			break;

		case 2:	//right Edge found
			lenkeinschlag = KYI;
			// Input Data for funktion
			Querabweichung_m_y = (float)edgeData[cam].edge_center_mm / 1000.0;
			y = Querabweichung_m_y;
			break;

		case 3:	//left & right Edge found
			lenkeinschlag = KYI;
			Querabweichung_m_y = (float)edgeData[cam].edge_center_mm / 1000.0;
			y = Querabweichung_m_y;

			break;
	}
	//___________________________________________________________
	// Input Data for funktion
	//Querabweichung_m_y = (float)edgeData[0].edge_center_mm / 1000.0;
	//y = Querabweichung_m_y;

	//_____________________________________________________________

	//Reglergleichungen
	// Gleichung I-Anteil
	//i = 0 - y + (u_sat - u_old) * Windup;
	//I = I +  i * sleeptime;
	// Gleichung 1
	//u = 0 + I*KYI - (k1*x1_hat + k2*x2_hat + k3*x3_hat + k4*x4_hat);
	//u = 0 - (k1*x1_hat + k2*x2_hat + k3*x3_hat + k4*x4_hat);			// ohne I anteil
	u = 0 - lenkeinschlag*y;
	//Begrenzung u
	if (u <= -0.54)
		{
			u_old = u;
			u = -0.54;
			u_sat = u;

		}
	else if (u>=0.54)
		{
			u_old = u;
			u = 0.54;
			u_sat = u;
		}
	else
	{
		u_old = u;
		u_sat = u;
	}

	//Gleichung2
	g_hat = y - y_hat;
	//Gleichung 3
	x1_hat_dot = l1*g_hat + B1*u + a11*x1_hat + a12*x2_hat + a13*x3_hat + a14*x4_hat;
	x2_hat_dot = l2*g_hat + B2*u + a21*x1_hat + a22*x2_hat + a23*x3_hat + a24*x4_hat;
	x3_hat_dot = l3*g_hat + B3*u + a31*x1_hat + a32*x2_hat + a33*x3_hat + a34*x4_hat;
	x4_hat_dot = l4*g_hat + B4*u + a41*x1_hat + a42*x2_hat + a43*x3_hat + a44*x4_hat;
	//Gleichung4
	x1_hat = x1_hat + x1_hat_dot * sleeptime;
	x2_hat = x2_hat + x2_hat_dot * sleeptime;
	x3_hat = x3_hat + x3_hat_dot * sleeptime;
	x4_hat = x4_hat + x4_hat_dot * sleeptime;
	//Gleichung 5
	y_hat = c1*x1_hat + c2*x2_hat + c3*x3_hat + c4*x4_hat;



	stear(u);
//
//	gamma = (int16_t)(u*2*180/3.14);		// 30°Lenkwinkel Entsprechen 60° beim Servo
//	//Faktor Lenkwinkel zu Servo-Winkel
//	if (gamma <= -60)
//	{
//		gamma = -60;
//	}
//	else if (gamma >= 60)
//	{
//		gamma = 60;
//	}
//
//	//Skalierung auf +- 100 --> 60 ° entspricht +100
//	gamma = gamma * 10 / 6;
//	servo_Value = (int16_t)gamma;
//
//	if (servo_Value >= 100) servo_Value = 100;
//	if (servo_Value <= -100) servo_Value = -100;
//	if(servo_Value <=0)
//	{
//	//calculate test value for left steering
//	Stearing_Value= *servoMiddle+ servo_Value*(*servoMiddle-*servoLeft)/100;
//	}
//	else
//	{
//	//calculate test value for right steering
//	Stearing_Value= *servoMiddle + servo_Value*(*servoRight-*servoMiddle)/100;
//	}
////	//Schreiben des Umgerechneten PWM Werts ins Register für die Lenkwinkelsteuerung
	//CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - Stearing_Value * CTIMER1_PWM_PERIOD / 20000;

	// Delay the Task --> for integration over time
	//vTaskDelay(t);
/*
	//Set the Speed -- here is not the Speed decision!!!
			Speed_Left_normiert  = Const_Test_Speed;// * (1 + ((Spurweiter_m * y) / (X*X + y*y)));
			Speed_Right_normiert = Const_Test_Speed;// * (1 - ((Spurweiter_m * y) / (X*X + y*y)));
			//calculate test value for selected speed
			SpeedValueLeft= *BLDCLeftMinValue + Speed_Left_normiert*(*BLDCLeftMaxValue-*BLDCLeftMinValue)/100;// Fester Wert definiert--> evtl. noch dynamisch machen!!!
			SpeedValueRight= *BLDCRightMinValue + Speed_Right_normiert*(*BLDCRightMaxValue-*BLDCRightMinValue)/100;// Fester Wert definiert--> evtl. noch dynamisch machen!!!
	//Berechnung und Setzen der Werte der Register für Rechten und Linken Motor
			if (Const_Test_Speed == 0)
			{
			CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - (SpeedValueLeft-10) * CTIMER3_PWM_PERIOD / 20000;	// (Left)
			CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - (SpeedValueRight-10) * CTIMER3_PWM_PERIOD / 20000; //(Right)
			}
			else
			{
				CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - SpeedValueLeft * CTIMER3_PWM_PERIOD / 20000;	// (Left)
				CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - SpeedValueRight * CTIMER3_PWM_PERIOD / 20000; //(Right)
			}
*/


	}
/**********************************************************************************************
 * Speed Control:
 * FUNKTIONALITY: Set Speed-Value according to route section
 * DESCRIPTION:
 **********************************************************************************************/

void SpeedControl2(void) //Beide Kameras
{

	//Identifikation des Streckenabschnittes(mit Kamera1 und Kamera2):
	/*
	unsigned int straight_line=edgeData[0].edge_left_found&&edgeData[0].edge_right_found&&edgeData[1].edge_left_found&&edgeData[1].edge_right_found; //Gerade Strecke
	unsigned int crossover1=(edgeData[0].edge_left_found||edgeData[0].edge_right_found)&&edgeData[1].edge_left_found&&edgeData[1].edge_right_found;  //Kreuzung
	unsigned int crossover2=!edgeData[0].edge_left_found&&!edgeData[0].edge_right_found&&edgeData[1].edge_left_found&&edgeData[1].edge_right_found;	 //Kreuzung
	unsigned int right_curve=edgeData[0].edge_left_found&&edgeData[0].edge_right_found&&edgeData[1].edge_left_found&&!edgeData[1].edge_right_found;  //Rechte Kurve
	unsigned int left_curve=edgeData[0].edge_left_found&&edgeData[0].edge_right_found&&!edgeData[1].edge_left_found&&edgeData[1].edge_right_found;	 //Linke Kurve
	*/

	//Identifikation des Streckenabschnittes (nur mit Kamera1):
	unsigned int straight_line=edgeData[0].edge_left_found&&edgeData[0].edge_right_found; //Gerade Strecke
	unsigned int crossover1=!edgeData[0].edge_left_found&&!edgeData[0].edge_right_found; //Kreuzung
	unsigned int right_curve=edgeData[0].edge_left_found&&!edgeData[0].edge_right_found; //Rechtskurve
	unsigned int left_curve=!edgeData[0].edge_left_found&&edgeData[0].edge_right_found; //Linkskurve


	double r=0; //Radius
	double winkel_v=0; //Winkelgeschwindigkeit

	double pulse_right=0;
	double pulse_left=0;

	double distance=0; //Abstand Ultraschallsensor (cm)

	if((USS_Distance<=50)&&(finished==0)) //Falls die Ziellinie nicht erkannt wurde (und Geschwindigkeit nicht reduziert wurde) soll 30 cm vor dem Hinderniss bereits auf Null abgebremst werden
	{
		//Stillstand:
		pulse_right=(*BLDCRightMinValue)*0.001-0.1;
		pulse_left=(*BLDCLeftMinValue)*0.001-0.1;

		//Neuer Geschwindikeitswert ins Timer-Register schreiben:
		CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
		CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;

	}
	else if (USS_Distance<=40) //Reduzierung der Geschwindigkeit wenn Hinderniss erkannt
	{
		distance=USS_Distance;

		if(distance<=15) //Stillstand
		{
			pulse_right=(*BLDCRightMinValue)*0.001-0.1;
			pulse_left=(*BLDCLeftMinValue)*0.001-0.1;

			//Neuer Geschwindikeitswert ins Timer-Register schreiben:
			CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
			CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;
		}
		else //Reduzierung der Geschwindigkeit mit Bremsfaktor
		{
			speed_value_right=sqrt(2.0*BRAKE_FAKTOR2*(40.0-distance)*0.01+SPEED_AFTER_FINISH2*SPEED_AFTER_FINISH2);
			speed_value_left=sqrt(2.0*BRAKE_FAKTOR2*(40.0-distance)*0.01+SPEED_AFTER_FINISH2*SPEED_AFTER_FINISH2);

			pulse_right=(*BLDCRightMinValue)*0.001+(speed_value_right/SPEED_TO_PULSE);
			pulse_left=(*BLDCLeftMinValue)*0.001+(speed_value_left/SPEED_TO_PULSE);

			//Neuer Geschwindikeitswert ins Timer-Register schreiben:
			CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
			CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;
		}
	}
	else if(edgeData[0].finish_detected==1)	//Reduzierung der Geschwindigkeit wenn Ziel erkannt:
	{
		/*
		//Test (Stillstand bei Erkennung der Ziellinie):
		pulse_right=(*BLDCRightMinValue)*0.001-0.1;
		pulse_left=(*BLDCLeftMinValue)*0.001-0.1;

		//Neuer Geschwindikeitswert ins Timer-Register schreiben:
		CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
		CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;
		*/

		speed_value_right=SPEED_AFTER_FINISH2;
		speed_value_left=SPEED_AFTER_FINISH2;


		pulse_right=(*BLDCRightMinValue)*0.001+(speed_value_right/SPEED_TO_PULSE);
		pulse_left=(*BLDCLeftMinValue)*0.001+(speed_value_left/SPEED_TO_PULSE);

		//Neuer Geschwindikeitswert ins Timer-Register schreiben:
		CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
		CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;


		finished=1;

	}
	else if(finished==0) //Ziellinie noch nicht überschritten (Wenn Ziellinie überschritten keine Reduzierung der Geschwindigkeit in der Kurve, da gering genung)
	{
		if(straight_line||crossover1) //Fahren mit maximaler Geschwindigkeit [Gerade,Kreuzung,unebener Fahrbahnrand]
		{
			speed_value_right=SPEED_MAX2;
			speed_value_left=SPEED_MAX2;

			pulse_right=(*BLDCRightMinValue)*0.001+(speed_value_right/SPEED_TO_PULSE);
			pulse_left=(*BLDCLeftMinValue)*0.001+(speed_value_left/SPEED_TO_PULSE);

			//Neuer Geschwindikeitswert ins Timer-Register schreiben:
			CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
			CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;

		}
		else if(left_curve) //Abbremsen bei Erkennung einer Links-Kurve [unterschiedliche Ansteuerung der Reifen]
		{
			r=RADIUS+edgeData[0].edge_center_mm;
			winkel_v=SPEED_CURVE2*r;

			speed_value_right=winkel_v/(r+0.5*WHEELBASE);
			speed_value_left=winkel_v/(r-0.5*WHEELBASE);

			pulse_right=(*BLDCRightMinValue)*0.001+(speed_value_right/SPEED_TO_PULSE);
			pulse_left=(*BLDCLeftMinValue)*0.001+(speed_value_left/SPEED_TO_PULSE);

			//Neuer Geschwindikeitswert ins Timer-Register schreiben:
			CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
			CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;
		}
		else if(right_curve) //Abbremsen bei Erkennung einer Rechts-Kurve [unterschiedliche Ansteuerung der Reifen]
		{
			r=RADIUS-edgeData[0].edge_center_mm;
			winkel_v=SPEED_CURVE2*r;

			speed_value_right=winkel_v/(r-0.5*WHEELBASE);
			speed_value_left=winkel_v/(r+0.5*WHEELBASE);

			pulse_right=(*BLDCRightMinValue)*0.001+(speed_value_right/SPEED_TO_PULSE);
			pulse_left=(*BLDCLeftMinValue)*0.001+(speed_value_left/SPEED_TO_PULSE);

			//Neuer Geschwindikeitswert ins Timer-Register schreiben:
			CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
			CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;
		}
		else //Falls gar nichts erkannt wird soll stehen geblieben werden
		{
			pulse_right=(*BLDCRightMinValue)*0.001-0.1;
			pulse_left=(*BLDCLeftMinValue)*0.001-0.1;

			//Neuer Geschwindikeitswert ins Timer-Register schreiben:
			CTIMER3->MSR[2]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_right;
			CTIMER3->MSR[0]=CTIMER3_PWM_PERIOD-TAKT_PER_MS*pulse_left;
		}

	}
}
