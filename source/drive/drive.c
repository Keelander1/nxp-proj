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
//#include <stdio.h>
#include <math.h>
/*******************************************************************************
 * Parameters

 ******************************************************************************/
int32_t BLDCTestValue=20;	//value for test purpose
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
double gamma = 0;					//Lenkwinkel
int16_t Y_ist = 0;					//Ausgang Regelschleife in mm
int16_t Y_soll = 0;					//Eingang Regelschleife in mm
int16_t Y_diff = 0;					//Differenz Soll-Ist Regler in mm
int16_t D = 180;					//Achsabstand des Autos --> muss noch nachgemessen werden
int16_t u = 0;						// Stellgröße in mm vor Umrechnung in °
int16_t P_Regler = 1;				//P-Regelverstärkung
int16_t X = 230;					//Sichtweite Kamera in mm



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

void Camera_Test_Drive (uint8_t state)
{
	menu_page_pixel_display_camera(1);
	int16_t testValueSpeed;							//calculated test value for min to max speed in µs
	int16_t Stearing_Value;
	//int16_t x = camera_distance;
	int16_t y = edgeData->edge_center_mm - 63;
	int16_t servo_Value = 0; //between -100 and 100
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


	servo_Value = y;


	if((edgeData->edge_left_found == 0) && (edgeData->edge_right_found == 0))
		servo_Value = 0;

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

void Real_Drive (uint8_t state)
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



}
