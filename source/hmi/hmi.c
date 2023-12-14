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

#include <hmi.h>
#include <hmi/input_devices/input_devices.h>
#include <screen.h>

/*******************************************************************************
 * screen_state_t
 * type of screen
 * SCREEN_START:	start screen
 * SCREEN_MAIN:		main screen
 * SCREEN_MENU:		menu screen
 * SCREEN_DRIVE:	drive screen
 ******************************************************************************/
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
/*******************************************************************************
 * screen_state_machine
 * state machine for switching between menus
 * param b0:		encoder switch state
 * param b1:		push button state
 * param e:			encoder state
 ******************************************************************************/
void screen_state_machine(uint8_t b0, uint8_t b1, int8_t e) {

	//if curr_menu_handle exists
	if (curr_menu_handle)
		menu_lock(curr_menu_handle);	//take menu semaphore

	switch(screen_state) {
	//**********************************************************************
	//main screen query
	//**********************************************************************
	case SCREEN_MAIN:
		//**********************************************************************
		//push button pressed?
		//**********************************************************************
		if (b1) {
			screen_state = SCREEN_MENU;		//switch to menu
			menu_open_main();				//open main menu
		}
		// if menu handle exists
		if (curr_menu_handle) {
			//**********************************************************************
			//rotary encoder pressed?
			//**********************************************************************
			if (b0)
				menu_click(&curr_menu_handle->drv_handle);		//open next menu
			//**********************************************************************
			//rotary encoder turned left?
			//**********************************************************************
			if (e < 0)
				menu_down(&curr_menu_handle->drv_handle, 1);	//scroll down
			else if (e > 0)
				//**********************************************************************
				//rotary encoder turned right?
				//**********************************************************************
				menu_up(&curr_menu_handle->drv_handle, 1);		//scroll up

			//**********************************************************************
			//draw menu
			//**********************************************************************
			menu_task(&curr_menu_handle->drv_handle);
		}
		break;

		//**********************************************************************
		//menu screen query
		//**********************************************************************
	case SCREEN_MENU:
		//**********************************************************************
		//push button pressed?
		//**********************************************************************
		if (b1) {
			screen_state = SCREEN_MAIN;		//return to main screen
			menu_close();					//close menu
		}
		// if menu handle exists
		if (curr_menu_handle) {
			//**********************************************************************
			//rotary encoder pressed?
			//**********************************************************************
			if (b0)
				menu_click(&curr_menu_handle->drv_handle);		//open next menu
			//**********************************************************************
			//rotary encoder turned left?
			//**********************************************************************
			if (e < 0)
				menu_down(&curr_menu_handle->drv_handle, 1);	//scroll down
			//**********************************************************************
			//rotary encoder turned right?
			//**********************************************************************
			else if (e > 0)
				menu_up(&curr_menu_handle->drv_handle, 1);		//scroll up

			//**********************************************************************
			//draw menu
			//**********************************************************************
			menu_task(&curr_menu_handle->drv_handle);
		}
		break;
		/*case SCREEN_DRIVE:
			if (b0 || (cam_state < 3)) {
				stop_drive();
				screen_state = SCREEN_MAIN;
				screen_main();
			}
			break;*/
	default:
		break;
	}



	// Check for handle again because menu_task can manipulate it
	if (curr_menu_handle)
		menu_unlock(curr_menu_handle);	//give menu semaphore
}
/*
void extern_drive_start() {

	screen_state = SCREEN_DRIVE;

	menu_rtos_switch_handle(&curr_menu_handle, NULL);

	start_drive();
}


/*******************************************************************************
 * hmi_task
 * human machine interface task
 ******************************************************************************/
void hmi_task(void *args)
{
	//**********************************************************************
	//tick calculation for 20ms refresh time
	//**********************************************************************
	const TickType_t ticks_to_next_wakeup = pdMS_TO_TICKS(HMI_FG_REFR_RATE);	//calculate ticks until next wakeup
	TickType_t ticks_since_last_wakeup;
	ticks_since_last_wakeup = xTaskGetTickCount();								//calculate ticks since last wakeup

	uint32_t counter = 0;

	//**********************************************************************
	//button and encoder states
	//**********************************************************************
	uint8_t b0;				//encoder switch state
	uint8_t b1;				//push button state
	int32_t e;				//rotary encoder state

	//**********************************************************************
	//initialize LCD
	//**********************************************************************
	DISPLAY_Init();


	//**********************************************************************
	//menue display handles
	//**********************************************************************
	// Main screen handles
	menu_rtos_init(&menu_main_screen_handle);

	// Menu handles
	menu_rtos_init(&menu_main_handle);				//main menu handle
	menu_rtos_init(&menu_main_drive_handle);		//drive menu handle
	menu_rtos_init(&menu_main_hardware_handle);		//hardware menu handle
	menu_rtos_init(&menu_hardware_camera_handle);	//camera menu handle
	menu_rtos_init(&menu_hardware_camera_parameter_handle);	//camera menu handle
	menu_rtos_init(&menu_hardware_ppm_handle);		//motors menu handle
	menu_rtos_init(&menu_ppm_servo_motor_handle);	//servo menu handle
	menu_rtos_init(&menu_ppm_motor_left_handle);	//left BLDC menu handle
	menu_rtos_init(&menu_ppm_motor_right_handle);	//right BLDC menu handle
	menu_rtos_init(&menu_ppm_restore_handle);		//restore settings menu handle
	menu_rtos_init(&menu_drive_StateControl_handle);
	menu_rtos_init(&menu_main_musicmode_handle);	//music mode handle

	//**********************************************************************
	//display NXP logo
	//**********************************************************************
	ssd1309_rtos_lock(&g_disp_0);								//take LCD semaphore for LCD
	ssd1309_draw_img(&g_disp_0.disp_obj, 0, 0, img_nxp, ON);	//set image on LCD
	ssd1309_rtos_unlock(&g_disp_0);								//give LCD semaphore

	vTaskDelay(pdMS_TO_TICKS(100U));//1000						//wait

	//**********************************************************************
	//display HAW Landshut logo
	//**********************************************************************
	ssd1309_rtos_lock(&g_disp_0);								//take LCD semaphore
	ssd1309_draw_img(&g_disp_0.disp_obj, 0, 0, img_haw, ON);	//set image on LCD
	ssd1309_rtos_unlock(&g_disp_0);								//give LCD semaphor

	vTaskDelay(pdMS_TO_TICKS(100U));							//wait

	//**********************************************************************
	//build up main screen
	//**********************************************************************
	screen_state = SCREEN_MAIN;		//set current screen to main screen
	screen_bg();					//display top header
	screen_main();					//draw main screen

	//**********************************************************************
	//initalize rotary encoder and push button
	//**********************************************************************
	id_init();

	while (1)
	{
		//**********************************************************************
		//read push button and encoder states
		//**********************************************************************
		uint32_t state = id_get_button_state(); //get button state

		b0 = state & 0x01;						//get encoder button state
		b1 = (state >> 1) & 0x01;				//get push button state

		e = id_get_encoder_state();				//get encoder state

		screen_state_machine(b0, b1, e);		//screen state machine

		if (HMI_BG_REFR >= counter++) {			//if counter reached HMI_BG_REFR
			counter = 0;						//reset counter
			screen_update_header();				//update screen
		}

		//vTaskDelayUntil(&last_wake_time, freq);
		vTaskDelayUntil(&ticks_since_last_wakeup, ticks_to_next_wakeup);	//wait until next wakeup
	}
}
