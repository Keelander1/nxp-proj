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

/*******************************************************************************
 * Includes
 ******************************************************************************/

#ifndef HMI_SCREEN_H_
#define HMI_SCREEN_H_
#include "menu_main_screen.h"
#include "menu_data.h"

extern menu_rtos_handle_t *curr_menu_handle;

/*******************************************************************************
 * Defines
 ******************************************************************************/

enum camSel {
	cam1 = 1,
	cam2 = 2
};
extern int32_t camSelect;

//Notes for Music Mode
/********************************************************************************/
#define nC4 220000000/264
#define nD4 220000000/294
#define nE4 220000000/329
#define nF4 220000000/349
#define nG4 220000000/392
#define nA4 220000000/440
#define nB4 220000000/494
#define nC5 220000000/523
#define nD5 220000000/587
#define nE5 220000000/659
#define nF5 220000000/698
#define nG5 220000000/784
#define nA5 220000000/880
#define nB5 220000000/988
/********************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void menu_open_main();
void menu_open_main_drive();
void menu_open_main_hardware();
void menu_open_main_musicmode();
void menu_page_main_about(uint8_t refresh);
void menu_open_hardware_ppm();
void menu_open_ppm_servo_motor();
void menu_open_ppm_motor_left();
void menu_open_ppm_motor_right();
void menu_open_ppm_restore();
void menu_open_hardware_camera();
void menu_open_hardware_parameter_camera();
void menu_page_pixel_display_camera(uint8_t refresh);
void menu_page_calibration_camera(uint8_t refresh);
void menu_page_pixel_display_camera2(uint8_t refresh);
void menu_page_calibration_camera2(uint8_t refresh);
void menu_page_display_distance_USS(uint8_t refresh);
void menu_page_camera_info(uint8_t refresh);
void menu_page_musicmode_fcn(uint8_t refresh);
void menu_page_Pirates(uint8_t refresh);
//void menu_page_Champions(uint8_t refresh);		//Not enough memory for Function :(
void menu_page_Buzzer_stop(uint8_t refresh);
void menu_close();
void screen_main();
void screen_update_header();
void screen_bg();
void menu_open_drive_StateControl();
#endif /* HMI_SCREEN_H_ */


