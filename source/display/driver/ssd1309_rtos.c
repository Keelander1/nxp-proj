/*
 * ssd1309_rtos.c
 *
 *  Created on: 29 Jan 2020
 *      Author: arnekulinna
 */

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
static void disp_refresh(void* args);

/*******************************************************************************
 * Code
 ******************************************************************************/
void ssd1309_rtos_init(ssd1309_rtos_t *obj, uint8_t id, uint8_t height, uint8_t width, uint8_t *buffer) {

	obj->disp_obj.config.id = id;
	obj->disp_obj.config.height = height;
	obj->disp_obj.config.width = width;
	obj->disp_obj.config.byte_cnt = (uint16_t)((height * width) >> 3);
	obj->disp_obj.config.orientation_x = LEFT;
	obj->disp_obj.config.orientation_y = UP;
	obj->disp_obj.config.color_type = STD;

	obj->disp_obj.pos.x = 0x00;
	obj->disp_obj.pos.y = 0x00;

	obj->disp_obj.buffer = buffer;

	obj->mutex = xSemaphoreCreateMutex();
	if (obj->mutex == NULL)
		while(1);

	obj->disp_obj.user_data = (void*)obj->mutex;

	obj->disp_obj.dirty = false;

	ssd1309_init(&obj->disp_obj);

	if (xTaskCreate(disp_refresh, "DISP_REFR", (configMINIMAL_STACK_SIZE + 100), (void*)obj, SSD1309_REFR_TASK_PRIO, NULL) != pdPASS)
		while(1);
}

static void disp_refresh(void* args) {

	ssd1309_rtos_t *obj = (ssd1309_rtos_t*)args;
	TickType_t last_wake_time;
	const TickType_t freq = pdMS_TO_TICKS(1000U / SSD1309_REFR_RATE);

	last_wake_time = xTaskGetTickCount();

	while(1) {

		ssd1309_rtos_lock(obj);

		ssd1309_update(&obj->disp_obj);

		ssd1309_rtos_unlock(obj);

		vTaskDelayUntil(&last_wake_time, freq);
	}
}
