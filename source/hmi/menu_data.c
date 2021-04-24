/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		menu_data.c						################
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
 ** 1	|	MS		|04-24-2021	| imported menu_data.c														|
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
 ** contains menu entries
 **
 ** ENC_A (Encoder signal A) at Pin P[3][20] (J9 Pin9)
 ** ENC_B (Encoder signal B) at Pin P[3][22] (J9 Pin11)
 ** ENC_SW (Encoder switch) at Pin P[3][21] (J9 Pin13)
 **
 ** SW (switch) at Pin P[3][30] (J9 Pin15)s
 ************************************************************************************************************
 ***********************************************************************************************************/
#include "menu_data.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
const static menu_entry_t menu_main_entries[] = {
		{
				.str = "Drive",
				.type = MENU_LINK,
				//.link = menu_open_main_drive,
				.link = NULL,
				.en = true,
		},
		{
				.str = "Hardware",
				.type = MENU_LINK,
				//.link = menu_open_main_hardware,
				.link = NULL,
				.en = true,
		},
		{
				.str = "Expert",
				.type = MENU_LINK,
				.link = NULL,
				.en = false,
		},
		{
				.str = "About",
				.type = MENU_PAGE,
				//.func = menu_page_main_about,
				.func = 0,
				.en = true,
		},
		{
				.str = "Hide Menu",
				.type = MENU_LINK,
				//.link = menu_close,
				.link = 0,
				.en = true,
		},
};

const static menu_entry_t menu_main_drive_entries[] = {
		{
				.str = "Speed",
				.type = MENU_VALUE,
				//.val = &((all_param_t*)&const_all_param)->drive.speed,
				.val =0,
				.min_val = 0,
				.max_val = 100,
				.en = true,
		},
		{
				.str = "Stop",
				.type = MENU_CHECK,
				//.act = &((all_param_t*)&const_all_param)->drive.stop,
				.act =0,
				.en = true,
		},
		{
				.str = "Limit",
				.type = MENU_CHECK,
				//.act = &((all_param_t*)&const_all_param)->drive.limit,
				.act =0,
				.en = true,
		},
		{
				.str = "Object",
				.type = MENU_VALUE,
				//.val = &((all_param_t*)&const_all_param)->drive.object,
				.val =0,
				.min_val = 0,
				.max_val = 2,
				.en = true,
		},
		{
				.str = "Back",
				.type = MENU_LINK,
				//.link = menu_open_main,
				.link = 0,
				.en = true,
		},
};

const static menu_entry_t menu_main_hardware_entries[] = {
		{
				.str = "Camera",
				.type = MENU_LINK,
				.link =0,
				//.link = menu_open_hardware_camera,
				.en = true,
		},
		{
				.str = "PPM",
				.type = MENU_LINK,
				.link = 0,
				//.link = menu_open_hardware_ppm,
				.en = true,
		},
		{
				.str = "Buzzer",
				.type = MENU_CHECK,
				//.act = &((all_param_t*)&const_all_param)->system.buzzer,
				.link =0,
				.en = true,
		},
		{
				.str = "Back",
				.type = MENU_LINK,
				//.link = menu_open_main,
				.link =0,
				.en = true,
		},
};

const static menu_entry_t menu_hardware_camera_entries[] = {
		{
				.str = "Info",
				.type = MENU_PAGE,
				//.func = menu_page_camera_info,
				.func =NULL,
				.en = true,
		},
		/*
		{
				.str = "Parameter",
				.type = MENU_LINK,
				.func = NULL,
				.en = true,
		},
		 */
		{
				.str = "Back",
				.type = MENU_LINK,
				//.link = menu_open_main_hardware,
				.link = NULL,
				.en = true,
		},
};

const static menu_entry_t menu_hardware_ppm_entries[] = {
		{
				.str = "Servo-motor",
				.type = MENU_LINK,
				.link = NULL,
				//.link = menu_open_ppm_servo_motor,
				.en = true,
		},
		{
				.str = "Motor left",
				.type = MENU_LINK,
				.link =NULL,
				//.link = menu_open_ppm_motor_left,
				.en = true,
		},
		{
				.str = "Motor right",
				.type = MENU_LINK,
				.link =NULL,
				//.link = menu_open_ppm_motor_right,
				.en = true,
		},
		{
				.str = "Restore def.",
				.type = MENU_LINK,
				.link =NULL,
				//.link = menu_open_ppm_restore,
				.en = true,
		},
		{
				.str = "Back",
				.type = MENU_LINK,
				.link =NULL,
				//.link = menu_open_main_hardware,
				.en = true,
		},
};

const static menu_entry_t menu_ppm_servo_motor_entries[] = {
		{
				.str = "Init",
				.type = MENU_VALUE,
				.val = 0,
				//.val = &((all_param_t*)&const_all_param)->ppm.chnl0.init,
				.min_val = 500,
				.max_val = 2500,
				.func = NULL,
				//.func = menu_func_servo_motor_init,
				.en = true,
		},
		{
				.str = "Min",
				.type = MENU_VALUE,
				.val = 0,
				//.val = &((all_param_t*)&const_all_param)->ppm.chnl0.min,
				.min_val = 500,
				.max_val = 2500,
				.func = NULL,
				//.func = menu_func_servo_motor_min,
				.en = true,
		},
		{
				.str = "Max",
				.type = MENU_VALUE,
				.val = 0,
				//.val = &((all_param_t*)&const_all_param)->ppm.chnl0.max,
				.min_val = 500,
				.max_val = 2500,
				.func = NULL,
				//.func = menu_func_servo_motor_max,
				.en = true,
		},
		{
				.str = "Back",
				.type = MENU_LINK,
				.link =NULL,
				//.link = menu_open_hardware_ppm,
				.en = true,
		},
};

const static menu_entry_t menu_ppm_motor_left_entries[] = {
		{
				.str = "Init",
				.type = MENU_VALUE,
				.val = 0,
				//.val = &((all_param_t*)&const_all_param)->ppm.chnl1.init,
				.min_val = 500,
				.max_val = 2500,
				.func = NULL,
				//.func = menu_func_motor_left_init,
				.en = true,
		},
		{
				.str = "Min",
				.type = MENU_VALUE,
				.val = 0,
				//.val = &((all_param_t*)&const_all_param)->ppm.chnl1.min,
				.min_val = 500,
				.max_val = 2500,
				.func = NULL,
				//.func =  menu_func_motor_left_min,
				.en = true,
		},
		{
				.str = "Max",
				.type = MENU_VALUE,
				.val = 0,
				//.val = &((all_param_t*)&const_all_param)->ppm.chnl1.max,
				.min_val = 500,
				.max_val = 2500,
				.func = NULL,
				//.func = menu_func_motor_left_max,
				.en = true,
		},
		{
				.str = "Back",
				.type = MENU_LINK,
				.link =NULL,
				//.link = menu_open_hardware_ppm,
				.en = true,
		},
};

const static menu_entry_t menu_ppm_motor_right_entries[] = {
		{
				.str = "Init",
				.type = MENU_VALUE,
				.val = 0,
				//.val = &((all_param_t*)&const_all_param)->ppm.chnl2.init,
				.min_val = 500,
				.max_val = 2500,
				.func = NULL,
				//.func = menu_func_motor_right_init,
				.en = true,
		},
		{
				.str = "Min",
				.type = MENU_VALUE,
				.val = 0,
				//.val = &((all_param_t*)&const_all_param)->ppm.chnl2.min,
				.min_val = 500,
				.max_val = 2500,
				.func = NULL,
				//.func =  menu_func_motor_right_min,
				.en = true,
		},
		{
				.str = "Max",
				.type = MENU_VALUE,
				.val = 0,
				//.val = &((all_param_t*)&const_all_param)->ppm.chnl2.max,
				.min_val = 500,
				.max_val = 2500,
				.func = NULL,
				//.func = menu_func_motor_right_max,
				.en = true,
		},
		{
				.str = "Back",
				.type = MENU_LINK,
				.link =NULL,
				//.link = menu_open_hardware_ppm,
				.en = true,
		},
};

const static menu_entry_t menu_ppm_restore_entries[] = {
		{
				.str = "Restore?",
				.type = MENU_LINK,
				.link = NULL,
				.en = false,
		},
		{
				.str = "Yes",
				.type = MENU_LINK,
				.link =NULL,
				//.link = menu_func_restore,
				.en = true,
		},
		{
				.str = "No",
				.type = MENU_LINK,
				.link =NULL,
				//.link = menu_open_hardware_ppm,
				.en = true,
		},
};

menu_rtos_handle_t menu_main_handle = {
		.drv_handle = {
				.entry_cnt = 5,
				.entry_list = (menu_entry_t*)menu_main_entries,
				.draw = menu_list_draw,
		},
};

menu_rtos_handle_t menu_main_drive_handle = {
		.drv_handle = {
				.entry_cnt = 5,
				.entry_list = (menu_entry_t*)menu_main_drive_entries,
				.draw = menu_list_draw,
		},
};

menu_rtos_handle_t menu_main_hardware_handle = {
		.drv_handle = {
				.entry_cnt = 4,
				.entry_list = (menu_entry_t*)menu_main_hardware_entries,
				.draw = menu_list_draw,
		},
};

menu_rtos_handle_t menu_hardware_camera_handle = {
		.drv_handle = {
				.entry_cnt = 2,
				.entry_list = (menu_entry_t*)menu_hardware_camera_entries,
				.draw = menu_list_draw,
		},
};

menu_rtos_handle_t menu_hardware_ppm_handle = {
		.drv_handle = {
				.entry_cnt = 5,
				.entry_list = (menu_entry_t*)menu_hardware_ppm_entries,
				.draw = menu_list_draw,
		},
};

menu_rtos_handle_t menu_ppm_servo_motor_handle = {
		.drv_handle = {
				.entry_cnt = 4,
				.entry_list = (menu_entry_t*)menu_ppm_servo_motor_entries,
				.draw = menu_list_draw,
		},
};

menu_rtos_handle_t menu_ppm_motor_left_handle = {
		.drv_handle = {
				.entry_cnt = 4,
				.entry_list = (menu_entry_t*)menu_ppm_motor_left_entries,
				.draw = menu_list_draw,
		},
};

menu_rtos_handle_t menu_ppm_motor_right_handle = {
		.drv_handle = {
				.entry_cnt = 4,
				.entry_list = (menu_entry_t*)menu_ppm_motor_right_entries,
				.draw = menu_list_draw,
		},
};

menu_rtos_handle_t menu_ppm_restore_handle = {
		.drv_handle = {
				.entry_cnt = 3,
				.entry_list = (menu_entry_t*)menu_ppm_restore_entries,
				.draw = menu_list_draw,
		},
};


