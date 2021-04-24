/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		hmi.c							################
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
 ** 1	|	MS		|04-24-2021	| imported hmi.c															|
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
 ** contains HMI functions
 **
 ** ENC_A (Encoder signal A) at Pin P[3][20] (J9 Pin9)
 ** ENC_B (Encoder signal B) at Pin P[3][22] (J9 Pin11)
 ** ENC_SW (Encoder switch) at Pin P[3][21] (J9 Pin13)
 **
 ** SW (switch) at Pin P[3][30] (J9 Pin15)s
 ************************************************************************************************************
 ***********************************************************************************************************/

#include <hmi/hmi.h>
#include <hmi/input_devices/input_devices.h>

typedef enum {
	SCREEN_START,
	SCREEN_MAIN,
	SCREEN_MENU,
	SCREEN_DRIVE,
} screen_state_t;

static screen_state_t screen_state = SCREEN_START;
menu_rtos_handle_t *curr_menu_handle;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*void screen_state_machine(uint8_t b0, uint8_t b1, int8_t e) {

	if (curr_menu_handle)
		menu_lock(curr_menu_handle);

	switch(screen_state) {
		case SCREEN_MAIN:
			if (b1) {
				screen_state = SCREEN_MENU;
				menu_open_main();
			} else if (last_cam_state != cam_state) {
				screen_main();
			}
			if (curr_menu_handle) {
				if (b0)
					menu_click(&curr_menu_handle->drv_handle);
				if (e < 0)
					menu_down(&curr_menu_handle->drv_handle, 1);
				else if (e > 0)
					menu_up(&curr_menu_handle->drv_handle, 1);

				menu_task(&curr_menu_handle->drv_handle);
			}
			break;
		case SCREEN_MENU:
			if (b1) {
				screen_state = SCREEN_MAIN;
				menu_close();
			}
			if (curr_menu_handle) {
				if (b0)
					menu_click(&curr_menu_handle->drv_handle);
				if (e < 0)
					menu_down(&curr_menu_handle->drv_handle, 1);
				else if (e > 0)
					menu_up(&curr_menu_handle->drv_handle, 1);

				menu_task(&curr_menu_handle->drv_handle);
			}
			break;
		case SCREEN_DRIVE:
			if (b0 || (cam_state < 3)) {
				stop_drive();
				screen_state = SCREEN_MAIN;
				screen_main();
			}
			break;
		default:
			break;
	}



	// Check for handle again because menu_task can manipulate it
	if (curr_menu_handle)
		menu_unlock(curr_menu_handle);

	last_cam_state = cam_state;
}

void extern_drive_start() {

	screen_state = SCREEN_DRIVE;

	menu_rtos_switch_handle(&curr_menu_handle, NULL);

	start_drive();
}

void hmi_task(void *args) {

	const TickType_t freq = pdMS_TO_TICKS(HMI_FG_REFR_RATE);
	TickType_t last_wake_time;
	last_wake_time = xTaskGetTickCount();

	uint32_t counter = 0;

	uint8_t b0;
	uint8_t b1;
	int32_t e;

	// Init
	//RIT_StartTimer(RIT);

	display_init();

	// Main screen handles
	menu_rtos_init(&menu_main_screen_handle);

	// Menu handles
	menu_rtos_init(&menu_main_handle);
	menu_rtos_init(&menu_main_drive_handle);
	menu_rtos_init(&menu_main_hardware_handle);
	menu_rtos_init(&menu_hardware_camera_handle);
	menu_rtos_init(&menu_hardware_ppm_handle);
	menu_rtos_init(&menu_ppm_servo_motor_handle);
	menu_rtos_init(&menu_ppm_motor_left_handle);
	menu_rtos_init(&menu_ppm_motor_right_handle);
	menu_rtos_init(&menu_ppm_restore_handle);

	// Start screen
	ssd1309_rtos_lock(&g_disp_0);
	ssd1309_draw_img(&g_disp_0.disp_obj, 0, 0, img_nxp, ON);
	ssd1309_rtos_unlock(&g_disp_0);

	vTaskDelay(pdMS_TO_TICKS(1000U));

	ssd1309_rtos_lock(&g_disp_0);
	ssd1309_draw_img(&g_disp_0.disp_obj, 0, 0, img_haw, ON);
	ssd1309_rtos_unlock(&g_disp_0);

	vTaskDelay(pdMS_TO_TICKS(1000U));

	screen_state = SCREEN_MAIN;

	screen_bg();
	screen_main();

	id_init();

	while (1) {


		uint32_t state = id_get_button_state();

		b0 = state & 0x01;
		b1 = (state >> 1) & 0x01;

		e = id_get_encoder_state();

		screen_state_machine(b0, b1, e);

		if (HMI_BG_REFR >= counter++) {
			counter = 0;
			screen_update_header();
		}

		vTaskDelayUntil(&last_wake_time, freq);
	}
}


 */

void hmi_task(void *args)
{
	const TickType_t ticks_to_next_wakeup = pdMS_TO_TICKS(HMI_FG_REFR_RATE);
	TickType_t ticks_since_last_wakeup;
	ticks_since_last_wakeup = xTaskGetTickCount();

	uint32_t counter = 0;

	uint8_t b0;
	uint8_t b1;
	int32_t e;


	DISPLAY_Init();

	// Main screen handles
	menu_rtos_init(&menu_main_screen_handle);

	// Menu handles
	menu_rtos_init(&menu_main_handle);
	menu_rtos_init(&menu_main_drive_handle);
	menu_rtos_init(&menu_main_hardware_handle);
	menu_rtos_init(&menu_hardware_camera_handle);
	menu_rtos_init(&menu_hardware_ppm_handle);
	menu_rtos_init(&menu_ppm_servo_motor_handle);
	menu_rtos_init(&menu_ppm_motor_left_handle);
	menu_rtos_init(&menu_ppm_motor_right_handle);
	menu_rtos_init(&menu_ppm_restore_handle);

	// Start screen
	ssd1309_rtos_lock(&g_disp_0);
	ssd1309_draw_img(&g_disp_0.disp_obj, 0, 0, img_nxp, ON);
	ssd1309_rtos_unlock(&g_disp_0);

	vTaskDelay(pdMS_TO_TICKS(100U));//1000

	ssd1309_rtos_lock(&g_disp_0);
	ssd1309_draw_img(&g_disp_0.disp_obj, 0, 0, img_haw, ON);
	ssd1309_rtos_unlock(&g_disp_0);

	vTaskDelay(pdMS_TO_TICKS(100U));

	screen_state = SCREEN_MAIN;
	screen_bg();
	screen_main();

	id_init();

	while (1)
	{
		uint32_t state = id_get_button_state();

		b0 = state & 0x01;
		b1 = (state >> 1) & 0x01;

		e = id_get_encoder_state();

		//screen_state_machine(b0, b1, e);

		if (HMI_BG_REFR >= counter++) {
			counter = 0;
			screen_update_header();
		}

		//vTaskDelayUntil(&last_wake_time, freq);
		LED2_TOGGLE();
		vTaskDelayUntil(&ticks_since_last_wakeup, ticks_to_next_wakeup);
	}
}
