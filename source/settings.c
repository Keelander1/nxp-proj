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
extern int32_t* servoMiddleValue;		//Ton in us for servo in middle position

/*******************************************************************************
 * menu_func_servo_motor_init
 * init servo motor to middle positoion
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_servo_motor_init(uint8_t state) {

	CTIMER1->MSR[2] = CTIMER1_PWM_PERIOD -(*servoMiddleValue)*CTIMER1_PWM_PERIOD/20000;		//set middle value for servo
}

/*******************************************************************************
 * menu_func_servo_motor_left
 * set max left steeringangle
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_servo_motor_left(uint8_t state) {

	if (state == MENU_DEACT) {
		//drive_output_init_chnl(0, &all_param->ppm.chnl0);
		return;
	}

	//all_param->ppm.chnl0.fac = ((uint32_t)((all_param->ppm.chnl0.max - all_param->ppm.chnl0.min) << 12U) / 255U);
	//drive_output_set_chnl(0, 0, &all_param->ppm.chnl0);
}

/*******************************************************************************
 * menu_func_servo_motor_right
 * set max right steering angle
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_servo_motor_right(uint8_t state) {

	if (state == MENU_DEACT) {
		//drive_output_init_chnl(0, &all_param->ppm.chnl0);
		return;
	}

	//all_param->ppm.chnl0.fac = ((uint32_t)((all_param->ppm.chnl0.max - all_param->ppm.chnl0.min) << 12U) / 255U);
	//drive_output_set_chnl(0, 254, &all_param->ppm.chnl0);
}

/*******************************************************************************
 * menu_func_bldc_motor_left_init
 * init left BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_left_init(uint8_t state) {

	//drive_output_init_chnl(1, &all_param->ppm.chnl1);
}

/*******************************************************************************
 * menu_func_bldc_motor_left_min
 * set min value for left BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_left_min(uint8_t state) {

	if (state == MENU_DEACT) {
		//drive_output_init_chnl(1, &all_param->ppm.chnl1);
		return;
	}

	//all_param->ppm.chnl1.fac = ((uint32_t)((all_param->ppm.chnl1.max - all_param->ppm.chnl1.min) << 12U) / 255U);
	//drive_output_set_chnl(1, 0, &all_param->ppm.chnl1);
}

/*******************************************************************************
 * menu_func_bldc_motor_left_max
 * set max value for left BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_left_max(uint8_t state) {

	if (state == MENU_DEACT) {
		//drive_output_init_chnl(1, &all_param->ppm.chnl1);
		return;
	}

	//all_param->ppm.chnl1.fac = ((uint32_t)((all_param->ppm.chnl1.max - all_param->ppm.chnl1.min) << 12U) / 255U);
	//drive_output_set_chnl(1, 254, &all_param->ppm.chnl1);
}

/*******************************************************************************
  * menu_func_bldc_motor_right_init
 * init right BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_right_init(uint8_t state) {

	//drive_output_init_chnl(2, &all_param->ppm.chnl2);
}

/*******************************************************************************
 * menu_func_bldc_motor_right_min
 * set min value for right BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_right_min(uint8_t state) {

	if (state == MENU_DEACT) {
		//drive_output_init_chnl(2, &all_param->ppm.chnl2);
		return;
	}

	//all_param->ppm.chnl2.fac = ((uint32_t)((all_param->ppm.chnl2.max - all_param->ppm.chnl2.min) << 12U) / 255U);
	//drive_output_set_chnl(2, 0, &all_param->ppm.chnl2);
}

/*******************************************************************************
 * menu_func_bldc_motor_right_max
 * set max value for left BLDC motor
 * param state:		value, when function was called
 ******************************************************************************/
void menu_func_bldc_motor_right_max(uint8_t state) {

	if (state == MENU_DEACT) {
		//drive_output_init_chnl(2, &all_param->ppm.chnl2);
		return;
	}

	//all_param->ppm.chnl2.fac = ((uint32_t)((all_param->ppm.chnl2.max - all_param->ppm.chnl2.min) << 12U) / 255U);
	//drive_output_set_chnl(2, 254, &all_param->ppm.chnl2);
}

/*******************************************************************************
 * menu_func_restore
 * restore all parameter to default
 ******************************************************************************/
void menu_func_restore() {

	all_param->motors.servo.init = 1620;
	all_param->motors.servo.min = 1110;
	all_param->motors.servo.max = 2130;
	all_param->motors.servo.fac = 16384;
	all_param->motors.BLDCLeft.init = 1000;
	all_param->motors.BLDCLeft.min = 1135;
	all_param->motors.BLDCLeft.max = 1832;
	all_param->motors.BLDCLeft.fac = 11195;
	all_param->motors.BLDCRight.init = 1000;
	all_param->motors.BLDCRight.min = 1135;
	all_param->motors.BLDCRight.max = 1832;
	all_param->motors.BLDCRight.fac = 11195;

	menu_open_hardware_ppm();
}
