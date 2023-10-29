/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		screen.c						################
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
 ** 1	|	MS		|04-24-2021	| imported screen.c															|
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

#include "screen.h"

#include "hmi.h"
/*
#include "pixy2_func.h"

#include "pixy2.h"
#include "usb_host_main.h"

#include "param.h"
 */

extern volatile uint8_t pixelValues[128];
extern volatile uint8_t pixelValuesUC[128];
extern volatile int8_t calibrationCamera[128];
extern volatile uint8_t pixelValues2[128];
extern volatile uint8_t pixelValues2UC[128];
extern volatile int8_t calibrationCamera2[128];
extern volatile struct EdgeDetectionData edgeData[2];
int32_t camSelect = cam1;

//extern volatile uint8_t edges[128];
//extern volatile uint8_t edgesMiddle[128];
//extern volatile uint8_t edge_right;
//extern volatile uint8_t edge_left;
//extern volatile uint8_t edge_center;
//extern volatile uint8_t edge_right_found;
//extern volatile uint8_t edge_left_found;
//extern volatile uint8_t track_state;

//volatile uint32_t edge_throttle;


void screen_main();
/*******************************************************************************
 * Code
 ******************************************************************************/
/*******************************************************************************
 * helper_uint_to_2digit
 * convert value to 2 ASCII charakters and save them to str
 * param value:		integer value
 * param str:		pointer to char array
 ******************************************************************************/
static void helper_uint_to_2digit(uint32_t value, char* str) {

	str[0] = ((value / 10) % 10) + '0'; //calculate tenths of value and convert it to ASCII
	str[1] = (value % 10) + '0';		//calculate ones of value and convert it to ASCII
}

/*******************************************************************************
 * menu_open_main
 * open main menu
 ******************************************************************************/
void menu_open_main() {
	//?
	menu_rtos_switch_handle(&curr_menu_handle, &menu_main_screen_handle);
	menu_main_screen_handle.drv_handle.pos = 2;
	//**********************************************************************
	//switch to main menu
	//**********************************************************************
	menu_rtos_switch_handle(&curr_menu_handle, &menu_main_handle);	//switch menu handler
	menu_reset(&curr_menu_handle->drv_handle);						//reset menu handle
}

void menu_open_main_drive() {

	menu_rtos_switch_handle(&curr_menu_handle, &menu_main_drive_handle);
	menu_reset(&curr_menu_handle->drv_handle);
}

void menu_open_main_hardware() {

	menu_rtos_switch_handle(&curr_menu_handle, &menu_main_hardware_handle);
	menu_reset(&curr_menu_handle->drv_handle);
}
void menu_open_hardware_abaut() {

	menu_rtos_switch_handle(&curr_menu_handle, &menu_hardware_ppm_handle);
	menu_reset(&curr_menu_handle->drv_handle);
}

void menu_page_main_about(uint8_t refresh) {

	ssd1309_rtos_lock(&g_disp_0);

	ssd1309_draw_rect(&g_disp_0.disp_obj, 0, 13, 127, 63, true, OFF);
	ssd1309_set_pos(&g_disp_0.disp_obj, 8, 18);
	ssd1309_write_str(&g_disp_0.disp_obj, "Made for NXP Cup", ssd1309_font_6x8, false, ON);
	ssd1309_set_pos(&g_disp_0.disp_obj, 11, 34);
	ssd1309_write_str(&g_disp_0.disp_obj, "By Students", ssd1309_font_6x8, false, ON);
	ssd1309_set_pos(&g_disp_0.disp_obj, 29, 50);
	ssd1309_write_str(&g_disp_0.disp_obj, "June 2023", ssd1309_font_6x8, false, ON);

	ssd1309_rtos_unlock(&g_disp_0);
}

void menu_open_hardware_ppm() {

	menu_rtos_switch_handle(&curr_menu_handle, &menu_hardware_ppm_handle);
	menu_reset(&curr_menu_handle->drv_handle);
}



void menu_open_ppm_servo_motor() {

	menu_rtos_switch_handle(&curr_menu_handle, &menu_ppm_servo_motor_handle);
	menu_reset(&curr_menu_handle->drv_handle);
}

void menu_open_ppm_motor_left() {

	menu_rtos_switch_handle(&curr_menu_handle, &menu_ppm_motor_left_handle);
	menu_reset(&curr_menu_handle->drv_handle);
}

void menu_open_ppm_motor_right() {

	menu_rtos_switch_handle(&curr_menu_handle, &menu_ppm_motor_right_handle);
	menu_reset(&curr_menu_handle->drv_handle);
}

void menu_open_ppm_restore() {
	menu_rtos_switch_handle(&curr_menu_handle, &menu_ppm_restore_handle);
	menu_reset(&curr_menu_handle->drv_handle);
}

void menu_open_hardware_camera() {

	menu_rtos_switch_handle(&curr_menu_handle, &menu_hardware_camera_handle);
	menu_reset(&curr_menu_handle->drv_handle);
}

void menu_open_hardware_parameter_camera() {

	menu_rtos_switch_handle(&curr_menu_handle, &menu_hardware_camera_parameter_handle);
	menu_reset(&curr_menu_handle->drv_handle);
}

void menu_page_pixel_display_camera(uint8_t refresh)    {								// new Martin Fürstberger 27.05.23
		uint8_t x=0;
		uint8_t y=0;
		volatile struct EdgeDetectionData *edgeDat;
		volatile uint8_t *pixelVal;
		char time_string[15] = "Exp.Time[ms]:";

		switch(camSelect){
			case cam1:
				sprintf(&time_string[13], "%d", CTIMER0->MSR[0]/110000);	//Exposure time in ms
				edgeDat = &edgeData[0];
				pixelVal = pixelValues;
				break;
			case cam2:
				sprintf(&time_string[13], "%d", CTIMER4->MSR[0]/110000);	//Exposure time in ms				edgeDat = &edgeData[1];
				edgeDat = &edgeData[1];
				pixelVal = pixelValues2;
				break;
			}
		ssd1309_rtos_lock(&g_disp_0);
		//x1,y1, x2,  y2
		ssd1309_draw_rect(&g_disp_0.disp_obj, 0, 13, 127, 63, true, OFF);

		//Print Left & Right Edge

		for(x=0;x<128;x++){
			y = 64-(pixelVal[x]/5);						//Scale Pixel Values to Display Size
			ssd1309_set_pixel(&g_disp_0.disp_obj,x, y, ON);	//Print Pixel Values

//			Print all found Edges
//			if(edges[x] == 1)
//				ssd1309_draw_rect(&g_disp_0.disp_obj, x, 13, x, 18, true, ON);		//Draw Right Edges
			if(edgeDat->edgesMiddle[x] == 1)
				ssd1309_draw_rect(&g_disp_0.disp_obj, x, 13, x, 30, true, ON);		//Draw Right Edges
//			if(edges[x] == 2)
//				ssd1309_draw_rect(&g_disp_0.disp_obj, x, 13, x, 18, true, ON);	//Draw Left Edges
			if(edgeDat->edgesMiddle[x] == 2)
				ssd1309_draw_rect(&g_disp_0.disp_obj, x, 13, x, 30, true, ON);	//Draw Left Edges
		}

		if(edgeDat->edge_left_found){
			ssd1309_draw_rect(&g_disp_0.disp_obj, edgeDat->edge_left, 13, edgeDat->edge_left, 63, true, ON); //Draw Left Edge
			ssd1309_set_pos(&g_disp_0.disp_obj, edgeDat->edge_left +1, 50);
			ssd1309_write_str(&g_disp_0.disp_obj, "L" , ssd1309_font_6x8, false, ON);
		}

		if(edgeDat->edge_right_found){
			ssd1309_draw_rect(&g_disp_0.disp_obj, edgeDat->edge_right, 13, edgeDat->edge_right, 63, true, ON);	//Draw Right Edge
			ssd1309_set_pos(&g_disp_0.disp_obj, edgeDat->edge_right - 7, 50);
			ssd1309_write_str(&g_disp_0.disp_obj, "R" , ssd1309_font_6x8, false, ON);
		}

		if(edgeDat->edge_right_found || edgeDat->edge_left_found){
			uint8_t center = (uint8_t) edgeDat->edge_center;
			if (edgeDat->edge_center < 0)
				center =0 ;
			if (edgeDat->edge_center > 127)
				center = 127;
			ssd1309_draw_rect(&g_disp_0.disp_obj, center, 43, center, 63, true, ON);	//Draw Right Edge
			ssd1309_set_pos(&g_disp_0.disp_obj, center - 7, 50);
			ssd1309_write_str(&g_disp_0.disp_obj, "C" , ssd1309_font_6x8, false, ON);
		}

		//Print Exposure Time
		ssd1309_rtos_unlock(&g_disp_0);
		if(((all_param_t*)&const_all_param)->camera.exposure_show){
			ssd1309_set_pos(&g_disp_0.disp_obj, 0, 18);
			ssd1309_write_str(&g_disp_0.disp_obj, time_string , ssd1309_font_6x8, false, ON);	//Print Exposure time
		}

		//Printing Track State
//		ssd1309_set_pos(&g_disp_0.disp_obj, 64, 50);
//		switch(track_state){
//			case 0:
//				ssd1309_write_str(&g_disp_0.disp_obj, "T" , ssd1309_font_6x8, false, ON); break; //T: Track
//			case 1:
//				ssd1309_write_str(&g_disp_0.disp_obj, "F" , ssd1309_font_6x8, false, ON); break; //F: Finish
//			case 3:
//				ssd1309_write_str(&g_disp_0.disp_obj, "3" , ssd1309_font_6x8, false, ON); break; //3: tree_stribes
//			case 4:
//				ssd1309_write_str(&g_disp_0.disp_obj, "4" , ssd1309_font_6x8, false, ON); break; //4: four_stribes
//
//		}
	}

void menu_page_calibration_camera(uint8_t refresh)    {
	uint8_t x=0;
//	uint8_t y=0;
	volatile uint8_t *pixelVal;
	volatile int8_t *calibrationCam;

	switch(camSelect){
		case cam1:
			pixelVal = pixelValuesUC;
			calibrationCam = calibrationCamera_storage[0];
			break;
		case cam2:
			pixelVal = pixelValues2UC;
			calibrationCam = calibrationCamera_storage[1];
			break;
	}

	for(x=0; x<128; x++){
		calibrationCam[x] = (128 - pixelVal[x]); //Calibrating Camera
	}

	param_save();

	menu_rtos_switch_handle(&curr_menu_handle, &menu_main_hardware_handle);
	menu_reset(&curr_menu_handle->drv_handle);
}

//void menu_page_pixel_display_camera2(uint8_t refresh)    {								// new Martin Fürstberger 27.05.23
//		uint8_t x=0;
//		uint8_t y=0;
//		char time_string[15] = "Exp.Time[ms]:";
//		sprintf(&time_string[13], "%d", CTIMER4->MSR[0]/220000);	//Exposure time in ms
//
//		ssd1309_rtos_lock(&g_disp_0);
//		//x1,y1, x2,  y2
//		ssd1309_draw_rect(&g_disp_0.disp_obj, 0, 13, 127, 63, true, OFF);
//
//		//Print Left & Right Edge
//
//
//
//		for(x=0;x<128;x++){
//			y = 64-(pixelValues2[x]/5);						//Scale Pixel Values to Display Size
//			ssd1309_set_pixel(&g_disp_0.disp_obj,x, y, ON);	//Print Pixel Values
//
////			Print all found Edges
////			if(edges[x] == 1)
////				ssd1309_draw_rect(&g_disp_0.disp_obj, x, 13, x, 18, true, ON);		//Draw Right Edges
//			if(edgeData->edgesMiddle[x] == 1)
//				ssd1309_draw_rect(&g_disp_0.disp_obj, x, 13, x, 30, true, ON);		//Draw Right Edges
////			if(edges[x] == 2)
////				ssd1309_draw_rect(&g_disp_0.disp_obj, x, 13, x, 18, true, ON);	//Draw Left Edges
//			if(edgeData->edgesMiddle[x] == 2)
//				ssd1309_draw_rect(&g_disp_0.disp_obj, x, 13, x, 30, true, ON);	//Draw Left Edges
//		}
//
//		if(edgeData->edge_left_found){
//			ssd1309_draw_rect(&g_disp_0.disp_obj, edgeData->edge_left, 13, edgeData->edge_left, 63, true, ON); //Draw Left Edge
//			ssd1309_set_pos(&g_disp_0.disp_obj, edgeData->edge_left +1, 50);
//			ssd1309_write_str(&g_disp_0.disp_obj, "L" , ssd1309_font_6x8, false, ON);
//		}
//
//		if(edgeData->edge_right_found){
//			ssd1309_draw_rect(&g_disp_0.disp_obj, edgeData->edge_right, 13, edgeData->edge_right, 63, true, ON);	//Draw Right Edge
//			ssd1309_set_pos(&g_disp_0.disp_obj, edgeData->edge_right - 7, 50);
//			ssd1309_write_str(&g_disp_0.disp_obj, "R" , ssd1309_font_6x8, false, ON);
//		}
//
//		if(edgeData->edge_right_found || edgeData->edge_left_found){
//			uint8_t center = (uint8_t) edgeData->edge_center;
//			if (edgeData->edge_center < 0)
//				center =0 ;
//			if (edgeData->edge_center > 127)
//				center = 127;
//			ssd1309_draw_rect(&g_disp_0.disp_obj, center, 43, center, 63, true, ON);	//Draw Right Edge
//			ssd1309_set_pos(&g_disp_0.disp_obj, center - 7, 50);
//			ssd1309_write_str(&g_disp_0.disp_obj, "C" , ssd1309_font_6x8, false, ON);
//		}
//
//		//Print Exposure Time
//		ssd1309_rtos_unlock(&g_disp_0);
//		if(((all_param_t*)&const_all_param)->camera.exposure_show){
//			ssd1309_set_pos(&g_disp_0.disp_obj, 0, 18);
//			ssd1309_write_str(&g_disp_0.disp_obj, time_string , ssd1309_font_6x8, false, ON);	//Print Exposure time
//		}

		//Printing Track State
//		ssd1309_set_pos(&g_disp_0.disp_obj, 64, 50);
//		switch(track_state){
//			case 0:
//				ssd1309_write_str(&g_disp_0.disp_obj, "T" , ssd1309_font_6x8, false, ON); break; //T: Track
//			case 1:
//				ssd1309_write_str(&g_disp_0.disp_obj, "F" , ssd1309_font_6x8, false, ON); break; //F: Finish
//			case 3:
//				ssd1309_write_str(&g_disp_0.disp_obj, "3" , ssd1309_font_6x8, false, ON); break; //3: tree_stribes
//			case 4:
//				ssd1309_write_str(&g_disp_0.disp_obj, "4" , ssd1309_font_6x8, false, ON); break; //4: four_stribes
//
//		}
//	}

//void menu_page_calibration_camera2(uint8_t refresh)    {
//	uint8_t x=0;
//	uint8_t y=0;
//	for(x=0; x<128; x++){
//		calibrationCamera2[x] = calibrationCamera2[x] + (128 - pixelValues2[x]); //Calibrating Camera
//	}
//	menu_rtos_switch_handle(&curr_menu_handle, &menu_main_hardware_handle);
//	menu_reset(&curr_menu_handle->drv_handle);
//}

/*
void menu_page_camera_info(uint8_t refresh) {

	uint8_t buffer[64];
	static uint8_t last = 0;

	if ((pixy2_is_initialized(&g_pixy2) ^ last) || !refresh) {

		ssd1309_rtos_lock(&g_disp_0);

		ssd1309_draw_rect(&g_disp_0.disp_obj, 0, 13, 127, 63, true, OFF);
		ssd1309_set_pos(&g_disp_0.disp_obj, 0, 18);

		if (pixy2_is_initialized(&g_pixy2)) {

			ssd1309_write_str(&g_disp_0.disp_obj, "Dev:  Pixy2", ssd1309_font_6x8, false, ON);

			pixy2_func_zero_arg(&g_pixy2, "version", 0x01, buffer, 64);
			sprintf((char*)(buffer+17), "HW:   %u.%u.%u", *(uint16_t*)(buffer+5), *(uint16_t*)(buffer+7), *(uint16_t*)(buffer+9));
			ssd1309_set_pos(&g_disp_0.disp_obj, 0, 28);
			ssd1309_write_str(&g_disp_0.disp_obj, (char*)(buffer + 17), ssd1309_font_6x8, false, ON);

			sprintf((char*)(buffer+17), "SW:   %u.%u.%u", *(uint16_t*)(buffer+11), *(uint16_t*)(buffer+13), *(uint16_t*)(buffer+15));
			ssd1309_set_pos(&g_disp_0.disp_obj, 0, 38);
			ssd1309_write_str(&g_disp_0.disp_obj, (char*)(buffer + 17), ssd1309_font_6x8, false, ON);

 *(uint32_t*)buffer = pixy2_func_zero_arg(&g_pixy2, "getUID", 0x00, NULL, 0);
			sprintf((char*)(buffer+17), "UID:  %08X", *(uint32_t*)(buffer));
			ssd1309_set_pos(&g_disp_0.disp_obj, 0, 48);
			ssd1309_write_str(&g_disp_0.disp_obj, (char*)(buffer + 17), ssd1309_font_6x8, false, ON);

		} else {
			ssd1309_write_str(&g_disp_0.disp_obj, "Cam:  None", ssd1309_font_6x8, false, ON);
		}

		ssd1309_rtos_unlock(&g_disp_0);

		last = pixy2_is_initialized(&g_pixy2);
	}
}







/*******************************************************************************
 * menu_clos
 * close men
 ******************************************************************************/
void menu_close() {

	//drive_output_init_chnl(0, &all_param->ppm.chnl0);
	//drive_output_init_chnl(1, &all_param->ppm.chnl1);
	//drive_output_init_chnl(2, &all_param->ppm.chnl2);

	param_save();	//save parameters
	screen_main();	//draw main screen
}


/*******************************************************************************
 * screen main
 * draw main creen
 ******************************************************************************/
void screen_main() {

	ssd1309_rtos_lock(&g_disp_0);	//take LCD semaphore

	ssd1309_draw_rect(&g_disp_0.disp_obj, 0, 13, 127, 56, true, OFF);	//clear display except for top header
	/*
	if (pixy2_is_initialized(&g_pixy2)) {
		menu_rtos_switch_handle(&curr_menu_handle, &menu_main_screen_handle);
		menu_main_screen_handle.drv_handle.pos = 2;
		menu_main_screen_handle.drv_handle.active_entry = 0;
		menu_main_screen_handle.drv_handle.dirty = 1;
	} else {
		menu_rtos_switch_handle(&curr_menu_handle, NULL);
		ssd1309_draw_img(&g_disp_0.disp_obj, 39, 18, img_no_camera, ON);
	}

	 */
	menu_rtos_switch_handle(&curr_menu_handle, NULL);					//delete current menu handle
	ssd1309_draw_img(&g_disp_0.disp_obj, 39, 18, img_no_camera, ON);	//draw no camera symbol
	ssd1309_draw_img(&g_disp_0.disp_obj, 0, 57, img_footer_outer, ON);	//draw bottom header with menu and reset

	ssd1309_rtos_unlock(&g_disp_0);		//give LCD semaphore
}

void screen_update_header() {

	static uint8_t cam_init = 0;

	char temp_str[5];

	uint32_t time = ((uint32_t)(xTaskGetTickCount()) / configTICK_RATE_HZ);	//get CPU runtime in seconds

	//calculate hours and convert to ASCII
	temp_str[0] = ((time / 3600) % 10) + '0';

	//calculate and convert minutes from uint to two digits and save them to temp_str[1] and temp_str[2]
	helper_uint_to_2digit(((time / 60) % 60), (char *)(temp_str + 1));

	//calculate and convert seconds from uint to two digits and save them to temp_str[3] and temp_str[4]
	helper_uint_to_2digit((time % 60), (char *)(temp_str + 3));


	//**********************************************************************
	//update screen
	//**********************************************************************
	ssd1309_rtos_lock(&g_disp_0);		//take LCD semaphore

	//draw runtime to the top left corner
	ssd1309_set_pos(&g_disp_0.disp_obj, 8, 0);											//set hour position
	ssd1309_write_char(&g_disp_0.disp_obj, temp_str[0], ssd1309_font_3x5, true, ON);	//draw hours
	ssd1309_set_pos(&g_disp_0.disp_obj, 14, 0);											//set left minute position
	ssd1309_write_char(&g_disp_0.disp_obj, temp_str[1], ssd1309_font_3x5, true, ON);	//draw tens of minute
	ssd1309_set_pos(&g_disp_0.disp_obj, 18, 0);											//set right minue position
	ssd1309_write_char(&g_disp_0.disp_obj, temp_str[2], ssd1309_font_3x5, true, ON);	//draw ones of minute
	ssd1309_set_pos(&g_disp_0.disp_obj, 24, 0);											//set left second position
	ssd1309_write_char(&g_disp_0.disp_obj, temp_str[3], ssd1309_font_3x5, true, ON);	//draw tens of second
	ssd1309_set_pos(&g_disp_0.disp_obj, 28, 0);											//set right second position
	ssd1309_write_char(&g_disp_0.disp_obj, temp_str[4], ssd1309_font_3x5, true, ON);	//draw ones of second

	// draw battery voltage to the top right corner
	ssd1309_set_pos(&g_disp_0.disp_obj, 97, 0);											//set tens voltage position
	ssd1309_write_char(&g_disp_0.disp_obj, '-', ssd1309_font_3x5, true, ON);			//draw tens volatage
	ssd1309_set_pos(&g_disp_0.disp_obj, 101, 0);										//set ones voltage position
	ssd1309_write_char(&g_disp_0.disp_obj, '-', ssd1309_font_3x5, true, ON);			//draw ones voltage
	ssd1309_set_pos(&g_disp_0.disp_obj, 109, 0);										//set tenths voltage position
	ssd1309_write_char(&g_disp_0.disp_obj, '-', ssd1309_font_3x5, true, ON);			//draw tenths voltage

	// Symbols
	/*
	// Pixy camera
	if (cam_init != pixy2_is_initialized(&g_pixy2)) {
		if (pixy2_is_initialized(&g_pixy2))
			ssd1309_draw_img(&g_disp_0.disp_obj, 40, 3, img_camera_symbol, OFF);
		else
			ssd1309_draw_rect(&g_disp_0.disp_obj, 40, 3, 50, 9, true, ON);
	}

	// Buzzer
	if (all_param->system.buzzer & 0x01)
		ssd1309_draw_img(&g_disp_0.disp_obj, 80, 3, img_speaker_symbol, OFF);
	else
		ssd1309_draw_rect(&g_disp_0.disp_obj, 80, 3, 87, 9, true, ON);
	 */
	ssd1309_rtos_unlock(&g_disp_0);		//give LCD semaphore

	//cam_init = pixy2_is_initialized(&g_pixy2);
}



/*******************************************************************************
 * screen_bg
 * draw the top header with run time and voltage meassurement
 ******************************************************************************/
void screen_bg() {

	ssd1309_rtos_lock(&g_disp_0);	//take LCD semaphore

	ssd1309_draw_rect(&g_disp_0.disp_obj, 0, 0, 127, 63, true, OFF);	//Clear display

	ssd1309_draw_img(&g_disp_0.disp_obj, 0, 0, img_header_bar, ON);		//draw header bar



	ssd1309_draw_img(&g_disp_0.disp_obj, 0, 0, img_clock_symbol, ON);	//draw clock symbol to the left top corner

	//draw colon between hour and minutes
	ssd1309_set_pixel(&g_disp_0.disp_obj, 12, 1, ON);
	ssd1309_set_pixel(&g_disp_0.disp_obj, 12, 3, ON);
	//draw colon between minutes and secondes
	ssd1309_set_pixel(&g_disp_0.disp_obj, 22, 1, ON);
	ssd1309_set_pixel(&g_disp_0.disp_obj, 22, 3, ON);

	//draw runtime to the top left corner
	ssd1309_set_pos(&g_disp_0.disp_obj, 8, 0);									//set hour position
	ssd1309_write_char(&g_disp_0.disp_obj, '0', ssd1309_font_3x5, true, ON);	//draw hours
	ssd1309_set_pos(&g_disp_0.disp_obj, 14, 0);									//set left minute position
	ssd1309_write_char(&g_disp_0.disp_obj, '0', ssd1309_font_3x5, true, ON);	//draw tens of minute
	ssd1309_set_pos(&g_disp_0.disp_obj, 18, 0);									//set right minue position
	ssd1309_write_char(&g_disp_0.disp_obj, '0', ssd1309_font_3x5, true, ON);	//draw ones of minute
	ssd1309_set_pos(&g_disp_0.disp_obj, 24, 0);									//set left second position
	ssd1309_write_char(&g_disp_0.disp_obj, '0', ssd1309_font_3x5, true, ON);	//draw tens of second
	ssd1309_set_pos(&g_disp_0.disp_obj, 28, 0);									//set right second position
	ssd1309_write_char(&g_disp_0.disp_obj, '0', ssd1309_font_3x5, true, ON);	//draw ones of second

	//draw battery symbol on right top corner
	ssd1309_draw_img(&g_disp_0.disp_obj, 118, 1, img_battery_symbol, ON);

	//draw dot point for voltage level text
	ssd1309_set_pixel(&g_disp_0.disp_obj, 106, 4, ON);

	//draw 'V' for voltage beside voltage level text
	ssd1309_set_pos(&g_disp_0.disp_obj, 113, 0);
	ssd1309_write_char(&g_disp_0.disp_obj, 'V', ssd1309_font_3x5, false, ON);

	//drwa voltage level
	ssd1309_set_pos(&g_disp_0.disp_obj, 97, 0);									//set tens voltage position
	ssd1309_write_char(&g_disp_0.disp_obj, '-', ssd1309_font_3x5, true, ON);	//draw tens voltage
	ssd1309_set_pos(&g_disp_0.disp_obj, 101, 0);								//set ones voltage position
	ssd1309_write_char(&g_disp_0.disp_obj, '-', ssd1309_font_3x5, true, ON);	//draw ones voltage
	ssd1309_set_pos(&g_disp_0.disp_obj, 109, 0);								//set tenth voltage position
	ssd1309_write_char(&g_disp_0.disp_obj, '-', ssd1309_font_3x5, true, ON);	//draw tenths voltag

	ssd1309_rtos_unlock(&g_disp_0);		//give LCD semaphore
}

