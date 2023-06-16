/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		menu_data.h						################
 ** Created on: 	04-23-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	MS		|04-24-2021	| imported menu_data.h														|
 ** 2	|	MS		|06-07-2021	| implemented test function for BLDC and servo								|
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
 ** Header file for HMI functions:
 **
 ** contains menu entries
 **
 ** ENC_A (Encoder signal A) at Pin P[3][20] (J9 Pin9)
 ** ENC_B (Encoder signal B) at Pin P[3][22] (J9 Pin11)
 ** ENC_SW (Encoder switch) at Pin P[3][21] (J9 Pin13)
 **
 ** SW (switch) at Pin P[3][30] (J9 Pin15)s
 ************************************************************************************************************
 ***********************************************************************************************************/
#ifndef HMI_MENU_DATA_H_
#define HMI_MENU_DATA_H_

#include "display/display.h"
#include "screen.h"
#include "display/menu/design/menu_list.h"
#include "param.h"
#include "settings.h"
//#include "camera/camera.h"

/****************************************************
 * handle for main menu
 ***************************************************/
extern menu_rtos_handle_t menu_main_handle;

/****************************************************
 * handle for drive menu
 ***************************************************/
extern menu_rtos_handle_t menu_main_drive_handle;

/****************************************************
 * handle for hardware menu
 ***************************************************/
extern menu_rtos_handle_t menu_main_hardware_handle;

/****************************************************
 * handle for camera menu
 ***************************************************/
extern menu_rtos_handle_t menu_hardware_camera_handle;

/****************************************************
 * handle for camera parameter menu
 ***************************************************/
extern menu_rtos_handle_t menu_hardware_camera_parameter_handle;

/****************************************************
 * handle for motor menu
 ***************************************************/
extern menu_rtos_handle_t menu_hardware_ppm_handle;

/****************************************************
 * handle for servo motor menu
 ***************************************************/
extern menu_rtos_handle_t menu_ppm_servo_motor_handle;

/****************************************************
 * handle for left BLDC menu
 ***************************************************/
extern menu_rtos_handle_t menu_ppm_motor_left_handle;

/****************************************************
 * handle for right BLDC menu
 ***************************************************/
extern menu_rtos_handle_t menu_ppm_motor_right_handle;

/****************************************************
 * handle for motor restore menu
 ***************************************************/
extern menu_rtos_handle_t menu_ppm_restore_handle;


#endif /* HMI_MENU_DATA_H_ */
