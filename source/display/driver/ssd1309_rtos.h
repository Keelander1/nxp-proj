/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		ssd1309_rtos.h					################
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
 ** 1	|	MS		|04-**-2021	| imported ssd1309_rtos.h													|
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
 ** Contains contains arrays with font in hex format; each bit represents one pixel
 **
 ** Use this data in ssd1309_write_char to draw the char
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/

#ifndef SSD1309_RTOS_H_
#define SSD1309_RTOS_H_

#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "ssd1309.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SSD1309_REFR_RATE		30U /* images per s */
#define SSD1309_REFR_TASK_PRIO	(configMAX_PRIORITIES - 1U)

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * LCD RTOS struct with LCD object and mutex for RTOS operation
 ******************************************************************************/
typedef struct {
	ssd1309_t disp_obj;
	SemaphoreHandle_t mutex;
} ssd1309_rtos_t;

/*******************************************************************************
 * Prototypes
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
void ssd1309_rtos_init(ssd1309_rtos_t *obj, uint8_t id, uint8_t height, uint8_t width, uint8_t *buffer);


/*******************************************************************************
 * ssd1309_rtos_lock
 * take semaphore
 * param obj: 		LCD_RTOS object,
 ******************************************************************************/
static inline void ssd1309_rtos_lock(ssd1309_rtos_t *obj) {

	xSemaphoreTake(obj->mutex, portMAX_DELAY);
}

/*******************************************************************************
 * ssd1309_rtos_lock
 * give semaphore
 * param obj: 		LCD_RTOS object,
 ******************************************************************************/
static inline void ssd1309_rtos_unlock(ssd1309_rtos_t *obj) {

	xSemaphoreGive(obj->mutex);
}

#endif /* SSD1309_RTOS_H_ */
