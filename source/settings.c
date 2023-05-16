/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		settings.c						################
 ** Created on: 	04-28-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	MS		|04-xx-2021	| imported settings.c															|
 ** 2	|			|			|																			|
 ** 3	|			|			|																			|
 ** 4	|			|			|																			|
 ** 5	|			|			|																			|
 ** 6	|			|			|																			|
 ** 7	|			|			|																			|
 ** 8	|			|			|																			|
 ** 9	|			|			|																			|
 ** 10	|			|			|																			|
 ************************************************************************************************************
 **
 **	Description
 ************************************************************************************************************
 ** Header file for display functions:
 **
 ** Contains setting transfer functions for peripheral
 **
 ************************************************************************************************************
 ***********************************************************************************************************/

#include "settings.h"
extern int32_t* servoMiddleValue;		//Ton in µs for servo in middle position
extern int32_t* servoLeftValue;			//Ton in µs for servo in left position
extern int32_t* servoRightValue;		//Ton in µs for servo in right position
extern int32_t servoTestValue;			//Ton in µs for testing servo

extern int32_t* BLDCLeftInitValue;		//Ton in µs for left BLDC init speed
extern int32_t* BLDCLeftMinValue;		//Ton in µs for left BLDC min speed
extern int32_t* BLDCLeftMaxValue;		//Ton in us for left BLDC max speed
extern int32_t BLDCLeftTestValue;		//Ton in µs for testing left BLDC

extern int32_t* BLDCRightInitValue;		//Ton in µs for	right BLDC init speed
extern int32_t* BLDCRightMinValue;		//Ton in µs for right BLDC min speed
extern int32_t* BLDCRightMaxValue;		//Ton in µs for right BLDC max speed
extern int32_t BLDCRightTestValue;		//Ton in µs for testing right BLDC

/*******************************************************************************
 * menu_func_servo_motor_test
 * test servo motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_servo_motor_test(uint8_t state) {

	int16_t testValue;	//calculated test value for max left to max right steering in µs

	//exit test menu
	if(state==MENU_DEACT)
	{
		//calculate timer register value for middle steering value
		CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - *servoMiddleValue*CTIMER1_PWM_PERIOD/20000;
		return;
	}


	//if left steering is tested
	if(servoTestValue <=0)
	{
		//calculate test value for left steering
		testValue= *servoMiddleValue + servoTestValue*(*servoMiddleValue-*servoLeftValue)/100;
	}
	else
	{
		//calculate test value for right steering
		testValue= *servoMiddleValue +servoTestValue*(*servoRightValue-*servoMiddleValue)/100;
	}
	//set test value to timer register
	CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - testValue * CTIMER1_PWM_PERIOD / 20000;
}

/*******************************************************************************
 * menu_func_servo_motor_init
 * init servo motor to middle positoion
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_servo_motor_init(uint8_t state) {

	//set middle value for servo
	CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD -(*servoMiddleValue)*CTIMER1_PWM_PERIOD/20000;
}

/*******************************************************************************
 * menu_func_servo_motor_left
 * set max left steeringangle
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_servo_motor_left(uint8_t state) {

	//exit left steering menu
	if (state == MENU_DEACT) {
		//calculate timer register value for middle steering value
		CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD -(*servoMiddleValue)*CTIMER1_PWM_PERIOD/20000;
		return;
	}
	//calculate timer register value for left steering value
	CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - *servoLeftValue*CTIMER1_PWM_PERIOD/20000;

}

/*******************************************************************************
 * menu_func_servo_motor_right
 * set max right steering angle
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_servo_motor_right(uint8_t state) {

	//exit right steering menu
	if (state == MENU_DEACT) {
		//calculate timer register value for middle steering value
		CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD -(*servoMiddleValue)*CTIMER1_PWM_PERIOD/20000;
		return;
	}
	//calculate timer register value for right steering value
	CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD - *servoRightValue*CTIMER1_PWM_PERIOD/20000;
}

/*******************************************************************************
 * menu_func_servo_motor_test
 * test servo motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldcLeft_test(uint8_t state) {

	int16_t testValue;	//calculated test value for min to max speed in µs

	//exit test menu
	if(state==MENU_DEACT)
	{
		//calculate timer register value for stop
		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - *BLDCLeftInitValue*CTIMER3_PWM_PERIOD/20000;
		return;
	}
	//calculate test value for selected speed
	testValue= *BLDCLeftMinValue + BLDCLeftTestValue*(*BLDCLeftMaxValue-*BLDCLeftMinValue)/100;

	//calculate timer register value for test speed
	CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD - testValue * CTIMER3_PWM_PERIOD / 20000;

	//for Debugging
	printf("TestValueLeft: %i \t TimerRegister: %i \n", testValue, CTIMER3->MSR[0]);
}


/*******************************************************************************
 * menu_func_bldc_motor_left_init
 * init left BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_left_init(uint8_t state) {

	//calculate timer register value for init
	CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD -(*BLDCLeftInitValue)*CTIMER3_PWM_PERIOD/20000;		//set init value for left BLDC
}

/*******************************************************************************
 * menu_func_bldc_motor_left_min
 * set min value for left BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_left_min(uint8_t state) {

	//exit min speed menu
	if (state == MENU_DEACT) {
		//calculate timer register value for stop
		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD -(*BLDCLeftInitValue)*CTIMER3_PWM_PERIOD/20000;		//set init value for left BLDC
		return;
	}
	//calculate timer register value for min speed
	CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD -(*BLDCLeftMinValue)*CTIMER3_PWM_PERIOD/20000;		//set init value for left BLDC
}

/*******************************************************************************
 * menu_func_bldc_motor_left_max
 * set max value for left BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_left_max(uint8_t state) {

	//exit max speed menu
	if (state == MENU_DEACT) {
		//calculate timer register value for stop
		CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD -(*BLDCLeftInitValue)*CTIMER3_PWM_PERIOD/20000;		//set init value for left BLDC
		return;
	}
	//calculate timer register value for max speed
	CTIMER3->MSR[0] = CTIMER3_PWM_PERIOD -(*BLDCLeftMaxValue)*CTIMER3_PWM_PERIOD/20000;		//set init value for left BLDC
}


/*******************************************************************************
 * menu_func_servo_motor_test
 * test servo motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldcRight_test(uint8_t state) {

	int16_t testValue;


	if(state==MENU_DEACT)
	{
		//calculate timer register value for stop
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - *BLDCRightInitValue*CTIMER3_PWM_PERIOD/20000;
		return;
	}
	//calculate test value for selected speed
	testValue= *BLDCRightMinValue + BLDCRightTestValue*(*BLDCRightMaxValue-*BLDCRightMinValue)/100;
	//calculate timer register value for test speed
	CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD - testValue * CTIMER3_PWM_PERIOD / 20000;

	//for Debugging
	printf("TestValueRight: %i \t TimerRegister: %i \n", testValue, CTIMER3->MSR[2]);
}


/*******************************************************************************
 * menu_func_bldc_motor_right_init
 * init right BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_right_init(uint8_t state) {

	//calculate timer register value for init
	CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD -(*BLDCRightInitValue)*CTIMER3_PWM_PERIOD/20000;
}

/*******************************************************************************
 * menu_func_bldc_motor_right_min
 * set min value for right BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_right_min(uint8_t state) {

	//exit min speed menu
	if (state == MENU_DEACT) {
		//calculate timer register value for stop
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD -(*BLDCRightInitValue)*CTIMER3_PWM_PERIOD/20000;
		return;
	}
	//calculate timer register value for min speed
	CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD -(*BLDCRightMinValue)*CTIMER3_PWM_PERIOD/20000;
}

/*******************************************************************************
 * menu_func_bldc_motor_right_max
 * set max value for left BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_right_max(uint8_t state) {

	//exit max speed menu
	if (state == MENU_DEACT) {
		//calculate timer register value for stop
		CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD -(*BLDCRightInitValue)*CTIMER3_PWM_PERIOD/20000;
		return;
	}
	//calculate timer register value for max speed
	CTIMER3->MSR[2] = CTIMER3_PWM_PERIOD -(*BLDCRightMaxValue)*CTIMER3_PWM_PERIOD/20000;
}

/*******************************************************************************
 * menu_func_restore
 * restore all parameter to default
 ******************************************************************************/
void menu_func_restore() {

	all_param->motors.servo.init = 1500;
	all_param->motors.servo.min = 1110;
	all_param->motors.servo.max = 2130;
	all_param->motors.servo.fac = 16384;
	all_param->motors.BLDCLeft.init = 1000;
	all_param->motors.BLDCLeft.min = 1100;
	all_param->motors.BLDCLeft.max = 1900;
	all_param->motors.BLDCLeft.fac = 11195;
	all_param->motors.BLDCRight.init = 1000;
	all_param->motors.BLDCRight.min = 1100;
	all_param->motors.BLDCRight.max = 1900;
	all_param->motors.BLDCRight.fac = 11195;

	menu_open_hardware_ppm();
}
