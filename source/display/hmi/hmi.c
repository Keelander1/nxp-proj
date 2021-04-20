/*
 * hmi.c
 *
 *  Created on: 14.12.2020
 *      Author: chris
 */
#include "hmi.h"

void HMI_Task(void *args)
{
	const TickType_t ticks_to_next_wakeup = pdMS_TO_TICKS(HMI_REFRESH_RATE);
	TickType_t ticks_since_last_wakeup;
	ticks_since_last_wakeup = xTaskGetTickCount();

	DISPLAY_Init();

	while (1)
	{

		LED2_TOGGLE();
		vTaskDelayUntil(&ticks_since_last_wakeup, ticks_to_next_wakeup);
	}
}
