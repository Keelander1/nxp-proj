/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		ssd1309_rtos.c					################
 ** Created on: 	04-22-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	MS		|04-23-2021	| imported ssd1309_rtos.c													|
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
 ** Contains funktions to operate the display with RTOS
 **
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/
#include "ssd1309_rtos.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * diso_refresh
 * LCD refresh task
 ******************************************************************************/
static void disp_refresh(void* args);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * ssd1309_rtos_init
 * init LCD with RTOS operation
 * param obj: 		LCD_RTOS object,
 * param id:		LCD I2C address
 * param height:	LCD height in pixels
 * param width:		LCD width in pixels
 * param buffer:	LCD buffer
 ******************************************************************************/
void ssd1309_rtos_init(ssd1309_rtos_t *obj, uint8_t id, uint8_t height, uint8_t width, uint8_t *buffer) {

	//*******************************************
	//configure LCD
	//*******************************************
	obj->disp_obj.config.id = id; 			//set display I2C address
	obj->disp_obj.config.height = height; 	//set display height in pixel
	obj->disp_obj.config.width = width; 	//set display width in pixel
	obj->disp_obj.config.byte_cnt = (uint16_t)((height * width) >> 3);//calculate number of pixels in byte

	//set origin at Top Left corner
	obj->disp_obj.config.orientation_x = LEFT;
	obj->disp_obj.config.orientation_y = UP;

	obj->disp_obj.config.color_type = STD; 	//set color type to not invertet

	//set coursor to origin
	obj->disp_obj.pos.x = 0x00;
	obj->disp_obj.pos.y = 0x00;

	obj->disp_obj.buffer = buffer; 			//set buffer pointer

	//*******************************************
	//LCD RTOS operation init
	//*******************************************
	obj->mutex = xSemaphoreCreateMutex();			//create Mutex
	if (obj->mutex == NULL)							//stop if creation failed
		while(1);

	obj->disp_obj.user_data = (void*)obj->mutex;	//save mutex in LCD handler

	obj->disp_obj.dirty = false;					//display is up to date

	ssd1309_init(&obj->disp_obj);					//send init to display

	//create disp_refresh task
	if (xTaskCreate(disp_refresh, "DISP_REFR", (configMINIMAL_STACK_SIZE + 100), (void*)obj, SSD1309_REFR_TASK_PRIO, NULL) != pdPASS)
		while(1);
}

/*******************************************************************************
 * disp_refresh task
 * refresh LCD
 ******************************************************************************/
static void disp_refresh(void* args) {

	ssd1309_rtos_t *obj = (ssd1309_rtos_t*)args;
	TickType_t last_wake_time;
	const TickType_t freq = pdMS_TO_TICKS(1000U / SSD1309_REFR_RATE); //calculate display refresh time for 30Hz

	last_wake_time = xTaskGetTickCount();		//get actual operation time

	while(1) {
		//*******************************************
		//update LCD with RTOS operation
		//*******************************************
		ssd1309_rtos_lock(obj);					//take semaphore
		ssd1309_update(&obj->disp_obj); 		//update display
		ssd1309_rtos_unlock(obj);				//give semaphore

		vTaskDelayUntil(&last_wake_time, freq);	//wait until refresh time is reached
	}
}
