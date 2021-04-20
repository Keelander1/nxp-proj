/*
 * hmi.h
 *
 *  Created on: 14.12.2020
 *      Author: chris
 */
#include "FreeRTOS.h"
#include "task.h"
#include "display/display.h"

#ifndef DISPLAY_HMI_HMI_H_
#define DISPLAY_HMI_HMI_H_


#define HMI_REFRESH_RATE 20


void HMI_Task(void* args);


#endif /* DISPLAY_HMI_HMI_H_ */
