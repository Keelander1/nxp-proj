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
#include "menu_data.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
int32_t servoTestValue=0;		//value for test purpose
int32_t BLDCLeftTestValue=0;	//value for test purpose
int32_t BLDCRightTestValue=0;	//value for test purpose

/****************************************************
 * Entry for drive menu
 * menu for all submenus
 ***************************************************/
const static menu_entry_t menu_main_entries[] = {
		{
				/****************************************************
				 * 	Entry for drive menu
				 ***************************************************/
				.str = "Drive",						//display "Drive"
				.type = MENU_LINK,					//type is MENU_LiNK
				.link = menu_open_main_drive,
				//.link = menu_close,
				.en = true,							//element is clickable
		},
		{
				/****************************************************
				 * 	Entry for hardware menu
				 ***************************************************/
				.str = "Hardware",					//display "Hardware"
				.type = MENU_LINK,					//type is MENU_LINK
				.link = menu_open_main_hardware,	//link to hardware menu handler
				.en = true,							//element is clickable
		},
		{
				/****************************************************
				 * 	Entry for expert menu
				 ***************************************************/
				.str = "Expert",					//display "Expert"
				.type = MENU_LINK,					//type is MENU_LINK
				//.link = NULL,
				.link=menu_close,
				.en = false,						//element is not clickable
		},
		{
				/****************************************************
				 * 	Entry for about menu
				 ***************************************************/
				.str = "About",						//display "About"
				.type = MENU_PAGE,					//type is MENU_PAGE
				.func = menu_page_main_about,
				//.func = 0,
				.en = true,							//element is clickable
		},
		{
				/****************************************************
				 * 	Entry for hide menu
				 ***************************************************/
				.str = "Hide Menu",					//display "Hide Menu"
				.type = MENU_LINK,					//type is MENU_LINK
				.link = menu_close,					//link to main screen
				.en = true,							//element is clickable
		},
};

/****************************************************
 * 	Entry for drive menu
 * 	menu for drive purpose
 ***************************************************/
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
				.link = menu_open_main,
				//.link = 0,
				.en = true,
		},
};
/****************************************************
 * 	Entry for hardware menu
 * 	menu for each hardware component
 ***************************************************/
const static menu_entry_t menu_main_hardware_entries[] = {
		{
				.str = "Camera",					//display "Camera"
				.type = MENU_LINK,					//type is MENU_LINK
				//.link =0,
				.link = menu_open_hardware_camera,	//link to camera menu
				.en = true,							//element is clickable
		},
		{
				.str = "Motors",					//display "Motors"
				.type = MENU_LINK,					//type is MENU_LINK
				.link = menu_open_hardware_ppm,		//link to PPM handler
				.en = true,							//element is clickable
		},
		{
				.str = "Buzzer",					//display "Buzzer"
				.type = MENU_CHECK,					//type is MENU_CHECK
				.act = &((all_param_t*)&const_all_param)->system.buzzer,
				.en = true,							//element is clickable
		},
		{
				.str = "Back",						//display "Back"
				.type = MENU_LINK,					//type is menu link
				.link = menu_open_main,				//link to main menu
				.en = true,							//element clickable
		},
};

/****************************************************
 * 	Entry for camera menu
 * 	menu to change and view camera parameters
 ***************************************************/
const static menu_entry_t menu_hardware_camera_entries[] = {
		{
				.str = "Info",
				.type = MENU_PAGE,
				//.func = menu_page_camera_info,
				.func = 0,
				.en = true,
		},

		{
				.str = "Parameter",
				.type = MENU_LINK,
				.func = NULL,
				.en = true,
		},

		{
				.str = "Back",
				.type = MENU_LINK,
				.link = menu_open_main_hardware,
				.en = true,
		},
};

/****************************************************
 * 	Entry for motor menu
 * 	menu for each motor type
 ***************************************************/
const static menu_entry_t menu_hardware_ppm_entries[] = {
		{
				/****************************************************
				 * 	Entry for Servo-motor menu
				 ***************************************************/
				.str = "Servo-motor",				//display "Servo-motor"
				.type = MENU_LINK,					//type is MENU_LINK
				.link = menu_open_ppm_servo_motor,	//link to servo menu
				.en = true,							//element is clickable
		},
		{
				/****************************************************
				 * 	Entry for BLDC left menu
				 ***************************************************/
				.str = "BLDC left",					//display "BLDC left"
				.type = MENU_LINK,					//type is MENU_LINK
				.link = menu_open_ppm_motor_left,	//link to left motor menu
				.en = true,							//element is clickable
		},
		{
				/****************************************************
				 * 	Entry for BLDC right menu
				 ***************************************************/
				.str = "BLDC right",				//display "BLDC right"
				.type = MENU_LINK,					//type is MENU_LINK
				.link = menu_open_ppm_motor_right,	//link to right motor menu
				.en = true,							//element clickable
		},
		{
				/****************************************************
				 * 	Entry for Restore def menu
				 ***************************************************/
				.str = "Restore def.",				//display "Restore def."
				.type = MENU_LINK,					//type is MENU_LINK
				.link = menu_open_ppm_restore,		//link to resore menu
				.en = true,							//element is clickable
		},
		{
				/****************************************************
				 * 	Entry for Back
				 ***************************************************/
				.str = "Back",						//display "Back"
				.type = MENU_LINK,					//type is MENU_LINK
				.link = menu_open_main_hardware,	//link to main hardware menu
				.en = true,							//element is clickable
		},
};
/****************************************************
 * 	Entry for servo motor menu
 * 	menu to change and view servo motor parameters
 ***************************************************/
const static menu_entry_t menu_ppm_servo_motor_entries[] = {
		{
				/****************************************************
				 * display GPIO number
				 ****************************************************/
				.str = "P3.02, J13.11",										//display Pin number
				.type = MENU_LINK,											//type is menu MENU_LINK
				.link = 0,													//link is not necessary
				.en = false,												//element is not clickable
		},
		{
				/****************************************************
				 * servo Test function
				 ****************************************************/
				.str = "Test",													//display "Test"
				.type = MENU_VALUE,												//type is MENU_VALUE value
				.val = &servoTestValue,											//set test value
				.min_val = -100,												//min value for servo test value
				.max_val = 100,													//max value for servo test value
				.func = menu_func_servo_motor_test,								//write value to timer for signal generation
				.en = true,														//element is changeable
		},
		{
				/****************************************************
				 * servo init value (middle steering angle)
				 ****************************************************/
				.str = "Init",													//display "Init"
				.type = MENU_VALUE,												//type is MENU_VALUE value
				.val = &((all_param_t*)&const_all_param)->motors.servo.init,	//get saved servo init value
				.min_val = 500,													//min value for servo init value
				.max_val = 2500,												//max value for servo init value
				.func = menu_func_servo_motor_init,								//write value to timer for signal generation
				.en = true,														//element is changeable
		},
		{
				/****************************************************
				 * servo min value (max left steering angle)
				 ****************************************************/
				.str = "Left",												//display "Left"
				.type = MENU_VALUE,											//type is MENU_VALUE
				.val = &((all_param_t*)&const_all_param)->motors.servo.min,	//get saved min value
				.min_val = 500,												//min value for servo min value
				.max_val = 2500,											//max value for servo min value
				.func = menu_func_servo_motor_left,							//write value to timer for signal generation
				.en = true,													//element is changeable
		},
		{
				/****************************************************
				 * servo max value (max right steering angle)
				 ****************************************************/
				.str = "Right",												//display "Right"
				.type = MENU_VALUE,											//type is MENU_VALUE
				.val = &((all_param_t*)&const_all_param)->motors.servo.max,	//get saved max value
				.min_val = 500,												//min value for servo max value
				.max_val = 2500,											//max value for servo max value
				.func = menu_func_servo_motor_right,						//write value to timer for signal generation
				.en = true,													//element is changeable
		},
		{
				/****************************************************
				 * 	Entry for Back
				 ***************************************************/
				.str = "Back",												//display "Back"
				.type = MENU_LINK,											//type is MENU_LINK
				.link = menu_open_hardware_ppm,								//link to hardware menu
				.en = true,													//element is clickable
		},
};

/****************************************************
 * 	Entry for left BLDC menu
 * 	menu to change and view left BLDC parameters
 ***************************************************/
const static menu_entry_t menu_ppm_motor_left_entries[] = {
		{
				/****************************************************
				 * display GPIO number
				 ****************************************************/
				.str = "P3.10, J13.7",											//display pin number
				.type = MENU_LINK,												//type is MENU_LINK
				.link = 0,														//not necessary
				.en = false,													//element is not clickable
		},
		{
				/****************************************************
				 * BLDC left test function
				 ****************************************************/
				.str = "Test",													//display "Test"
				.type = MENU_VALUE,												//type is MENU_VALUE value
				.val = &BLDCLeftTestValue,										//set left BLDC test value
				.min_val = 0,													//min value for left BLDC test value
				.max_val = 100,													//max value for left BLDC test value
				.func = menu_func_bldcLeft_test,								//write value to timer for signal generation
				.en = true,														//element is changeable
		},
		{
				/****************************************************
				 * left motor init value
				 ****************************************************/
				.str = "Init",													//display "Init"
				.type = MENU_VALUE,												//type is MENU_VALUE
				.val = &((all_param_t*)&const_all_param)->motors.BLDCLeft.init,	//get saved init value
				.min_val = 500,													//min value for left BLDC init value
				.max_val = 2500,												//max value for left BLDC init value
				.func = menu_func_bldc_motor_left_init,							//write value to timer for signal generation
				.en = true,														//element is changeable
		},
		{
				/****************************************************
				 * left motor min value
				 ****************************************************/
				.str = "Min",													//display "Min"
				.type = MENU_VALUE,                                             //type is MENU_VALUE
				.val = &((all_param_t*)&const_all_param)->motors.BLDCLeft.min,  //get saved min value
				.min_val = 500,                                                 //min value for left BLDC min value
				.max_val = 2500,                                                //max value for left BLDC min value                                                   //write value to timer for signal generation
				.func =  menu_func_bldc_motor_left_min,                       //element is changeable
				.en = true,
		},
		{
				/****************************************************
				 * left motor max value
				 ****************************************************/
				.str = "Max",                                                  	//display "MAX"
				.type = MENU_VALUE,                                            	//type is MENU_VALUE
				.val = &((all_param_t*)&const_all_param)->motors.BLDCLeft.max, 	//get saved max value
				.min_val = 500,                                                	//min value for left BLDC max value
				.max_val = 2500,                                               	//max value for left BLDC max value
				.func = NULL,                                                  	//write value to timer for signal generation
				//.func = menu_func_bldc_motor_left_max,
				.en = true,														//element is changeable
		},
		{
				/****************************************************
				 * 	Entry for Back
				 ***************************************************/
				.str = "Back",                                                   //display "Back"
				.type = MENU_LINK,                                               //type is MENU_LINK
				.link = menu_open_hardware_ppm,                                  //link to hardware menu
				.en = true,                                                      //element is clickable
		},
};

/****************************************************
 * 	Entry for left BLDC menu
 * 	menu to change and view right BLDC parameters
 ***************************************************/
const static menu_entry_t menu_ppm_motor_right_entries[] = {
		{
				/****************************************************
				 * display GPIO number
				 ****************************************************/
				.str = "P0.27, J13.12",                                          	//display pin number
				.type = MENU_LINK,                                               	//type is MENU_LINK
				.link = menu_open_ppm_motor_right,                               	//not necessary
				.en = false,                                                     	//element is not clickable
		},
		{
				/****************************************************
				 * right BLDC test function
				 ****************************************************/
				.str = "Test",													//display "Test"
				.type = MENU_VALUE,												//type is MENU_VALUE value
				.val = &BLDCRightTestValue,										//set BLDC right test value
				.min_val = 0,													//min value for right BLDC test value
				.max_val = 100,													//max value for right BLDC test value
				.func = menu_func_bldcRight_test,								//write value to timer for signal generation
				.en = true,														//element is changeable
		},
		{
				/****************************************************
				 * right motor init value
				 ****************************************************/
				.str = "Init",                                                    	//display "Init"
				.type = MENU_VALUE,                                               	//type is MENU_VALUE
				.val = &((all_param_t*)&const_all_param)->motors.BLDCRight.init,  	//get saved init value
				.min_val = 500,                                                   	//min value for right BLDC init value
				.max_val = 2500,                                                  	//max value for right BLDC init value
				.func = NULL,                                                     	//write value to timer for signal generation
				//.func = menu_func_bldc_motor_right_init,
				.en = true,															//element is changeable
		},
		{
				/****************************************************
				 * right motor min value
				 ****************************************************/
				.str = "Min",                                                     	//display "Min"
				.type = MENU_VALUE,                                               	//type is MENU_VALUE
				.val = &((all_param_t*)&const_all_param)->motors.BLDCRight.min,    	//get saved min value
				.min_val = 500,                                                    	//min value for right BLDC min value
				.max_val = 2500,                                                   	//max value for right BLDC min value
				.func = NULL,                                                      	//write value to timer for signal generation
				//.func =  menu_func_bldc_motor_right_min,
				.en = true,															//element is changeable
		},
		{
				/****************************************************
				 * right motor max value
				 ****************************************************/
				.str = "Max",                                                       //display "MAX"
				.type = MENU_VALUE,                                                 //type is MENU_VALUE
				.val = &((all_param_t*)&const_all_param)->motors.BLDCRight.max,     //get saved max value
				.min_val = 500,                                                     //min value for right BLDC max value
				.max_val = 2500,                                                    //max value for right BLDC max value
				.func = NULL,                                                       //write value to timer for signal generation
				//.func = menu_func_bldc_motor_right_max,
				.en = true,                                                         //element is changeable
		},
		{
				/****************************************************
				 * 	Entry for Back
				 ***************************************************/
				.str = "Back",                                                      //display "Back"
				.type = MENU_LINK,                                                  //type is MENU_LINK
				.link = menu_open_hardware_ppm,                                     //link to hardware menu
				.en = true,                                                         //element is clickable
		},
};

/****************************************************
 * 	Entry for restore menu
 * 	restore all motor parameters
 ***************************************************/
const static menu_entry_t menu_ppm_restore_entries[] = {
		{
				.str = "Restore?",              //display "Restore?"
				.type = MENU_LINK,              //type is MENU_LINK
				.link = NULL,                   //not necessary
				.en = false,                    //element is not clickable
		},
		{
				.str = "Yes",                    //display "Yes"
				.type = MENU_LINK,               //type is MENU_LINK
				.link = menu_func_restore,       //link to restore function
				.en = true,                      //element is clickable
		},
		{
				.str = "No",                     //display "No"
				.type = MENU_LINK,               //type is MENU_LINK
				.link = menu_open_hardware_ppm,  //link to hardware motor menu
				.en = true,                      //element is clickable
		},
};

/****************************************************
 * handle for main menu
 ***************************************************/
menu_rtos_handle_t menu_main_handle = {
		.drv_handle = {
				.entry_cnt = 5,												//5 entries
				.entry_list = (menu_entry_t*)menu_main_entries,				//list of all entries
				.draw = menu_list_draw,										//emtry draw function
		},
};

/****************************************************
 * handle for drive menu
 ***************************************************/
menu_rtos_handle_t menu_main_drive_handle = {
		.drv_handle = {
				.entry_cnt = 5,                                        		//5 entries
				.entry_list = (menu_entry_t*)menu_main_drive_entries,  		//list of all entries
				.draw = menu_list_draw,                                		//emtry draw function
		},
};

/****************************************************
 * handle for hardware menu
 ***************************************************/
menu_rtos_handle_t menu_main_hardware_handle = {
		.drv_handle = {
				.entry_cnt = 4,                                         	//4 entries
				.entry_list = (menu_entry_t*)menu_main_hardware_entries,	//list of all entries
				.draw = menu_list_draw,                                 	//emtry draw function
		},
};

/****************************************************
 * handle for camera menu
 ***************************************************/
menu_rtos_handle_t menu_hardware_camera_handle = {
		.drv_handle = {
				.entry_cnt = 3,                                             //2 entries
				.entry_list = (menu_entry_t*)menu_hardware_camera_entries,  //list of all entries
				.draw = menu_list_draw,                                     //emtry draw function
		},
};

/****************************************************
 * handle for motor menu
 ***************************************************/
menu_rtos_handle_t menu_hardware_ppm_handle = {
		.drv_handle = {
				.entry_cnt = 5,                                           	//5 entries
				.entry_list = (menu_entry_t*)menu_hardware_ppm_entries,   	//list of all entries
				.draw = menu_list_draw,                                   	//emtry draw function
		},
};

/****************************************************
 * handle for servo motor menu
 ***************************************************/
menu_rtos_handle_t menu_ppm_servo_motor_handle = {
		.drv_handle = {
				.entry_cnt = 6,                                             //6 entries
				.entry_list = (menu_entry_t*)menu_ppm_servo_motor_entries,  //list of all entries
				.draw = menu_list_draw,                                     //emtry draw function
		},
};

/****************************************************
 * handle for left BLDC menu
 ***************************************************/
menu_rtos_handle_t menu_ppm_motor_left_handle = {
		.drv_handle = {
				.entry_cnt = 6,                                             //6 entries
				.entry_list = (menu_entry_t*)menu_ppm_motor_left_entries,   //list of all entries
				.draw = menu_list_draw,                                     //emtry draw function
		},
};

/****************************************************
 * handle for right BLDC menu
 ***************************************************/
menu_rtos_handle_t menu_ppm_motor_right_handle = {
		.drv_handle = {
				.entry_cnt = 6,                                             //6 entries
				.entry_list = (menu_entry_t*)menu_ppm_motor_right_entries,  //list of all entries
				.draw = menu_list_draw,                                     //emtry draw function
		},
};

/****************************************************
 * handle for motor restore menu
 ***************************************************/
menu_rtos_handle_t menu_ppm_restore_handle = {
		.drv_handle = {
				.entry_cnt = 3,                                            	//3 entries
				.entry_list = (menu_entry_t*)menu_ppm_restore_entries,     	//list of all entries
				.draw = menu_list_draw,                                    	//emtry draw function
		},
};


