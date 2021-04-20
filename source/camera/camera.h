/*
 * camera.c
 *
 *  Created on: 12.04.2021
 *      Author: Ecker Christian
 *      */

#include "fsl_common.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef CHC_MODULE_H_
#define CHC_MODULE_H_



#define RAW_IMAGE_BUFFER_SIZE 128U

struct _camData {
	uint8_t *rawImage;
};

extern struct _camData _camData;

void CAM_Init(void);

#endif /* CHC_MODULE_H_ */
