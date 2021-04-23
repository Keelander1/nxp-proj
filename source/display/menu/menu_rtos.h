/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		menu_rtos.h						################
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
 ** 1	|	MS		|04-**-2021	| imported menu_rtos.h														|
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
 ** contains menu init with RTOS operation
 **
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/

#ifndef DISPLAY_MENU_MENU_RTOS_H_
#define DISPLAY_MENU_MENU_RTOS_H_

#include "FreeRTOS.h"
#include "semphr.h"

#include "menu.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * menu RTOS handle
 ******************************************************************************/
typedef struct {
	menu_handle_t drv_handle; 	//menu handle
	SemaphoreHandle_t mutex;	//mutex
} menu_rtos_handle_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * menu_lock
 * take semaphore
 * param handle: 		menu RTOS handle
 ******************************************************************************/
static inline void menu_lock(menu_rtos_handle_t *handle) {
	xSemaphoreTake(handle->mutex, portMAX_DELAY);
}

/*******************************************************************************
 * menu_unlock
 * give semaphore
 * param handle: 		menu RTOAS handle
 ******************************************************************************/
static inline void menu_unlock(menu_rtos_handle_t *handle) {
	xSemaphoreGive(handle->mutex);
}

/*******************************************************************************
 * menu_rtos_init
 * init menu with RTOS operation
 * param handle:		menu RTOS handle
 ******************************************************************************/
void menu_rtos_init(menu_rtos_handle_t *handle);

/*******************************************************************************
 * menu_rtos_switch_handle
 * switch menu handle
 * param handle:		old menu RTOS handle
 * param to:			new menu RTOS handle
 ******************************************************************************/
static inline void menu_rtos_switch_handle(menu_rtos_handle_t **handle, menu_rtos_handle_t *to) {

	//if to is defined
	if (to)
		menu_lock(to);			//take to semaphore
	//if handle is defined
	if (*handle)
		menu_unlock(*handle);	//give handle semaphore
	*handle = to;				//switch handles
}



#endif /* DISPLAY_MENU_MENU_RTOS_H_ */

