/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		screen.h						################
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
 ** 1	|	MS		|04-24-2021	| imported screen.h															|
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
 ** Header file for HMI functions:
 **
 ** contains screen layouts and helper functions
 **
 ** ENC_A (Encoder signal A) at Pin P[3][20] (J9 Pin9)
 ** ENC_B (Encoder signal B) at Pin P[3][22] (J9 Pin11)
 ** ENC_SW (Encoder switch) at Pin P[3][21] (J9 Pin13)
 **
 ** SW (switch) at Pin P[3][30] (J9 Pin15)s
 ************************************************************************************************************
 ***********************************************************************************************************/

#ifndef HMI_SCREEN_H_
#define HMI_SCREEN_H_
#include "menu_main_screen.h"
#include "menu_data.h"

extern menu_rtos_handle_t *curr_menu_handle;

void menu_open_main();
void menu_open_main_drive();
void menu_open_main_hardware();
void menu_page_main_about(uint8_t refresh);
void menu_open_hardware_ppm();
void menu_open_ppm_servo_motor();
void menu_open_ppm_motor_left();
void menu_open_ppm_motor_right();
void menu_open_ppm_restore();
void menu_open_hardware_camera();
void menu_open_hardware_parameter_camera();
void menu_page_pixel_display_camera(uint8_t refresh);
void menu_page_pixel_display_camera2(uint8_t refresh);
void menu_page_camera_info(uint8_t refresh);
void menu_close();
void screen_main();
void screen_update_header();
void screen_bg();
#endif /* HMI_SCREEN_H_ */
