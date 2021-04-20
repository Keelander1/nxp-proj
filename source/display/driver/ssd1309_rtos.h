/*
 * ssd1309_rtos.h
 *
 *  Created on: 29 Jan 2020
 *      Author: arnekulinna
 */

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
typedef struct {
	ssd1309_t disp_obj;
	SemaphoreHandle_t mutex;
} ssd1309_rtos_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void ssd1309_rtos_init(ssd1309_rtos_t *obj, uint8_t id, uint8_t height, uint8_t width, uint8_t *buffer);

static inline void ssd1309_rtos_lock(ssd1309_rtos_t *obj) {

	xSemaphoreTake(obj->mutex, portMAX_DELAY);
}

static inline void ssd1309_rtos_unlock(ssd1309_rtos_t *obj) {

	xSemaphoreGive(obj->mutex);
}

#endif /* SSD1309_RTOS_H_ */
