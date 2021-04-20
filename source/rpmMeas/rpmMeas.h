#ifndef RPMMEAS_RPMMEAS_H_
#define RPMMEAS_RPMMEAS_H_

#include "fsl_ctimer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "board.h"

#define CTIMER2_RPMMEAS_LEFT_CHANNEL kCTIMER_Capture_0
#define CTIMER2_RPMMEAS_RIGHT_CHANNEL kCTIMER_Capture_1


void RPMMEAS_Init(void);
void rpmMeas_Left_ISR(uint32_t);
void rpmMeas_Right_ISR(uint32_t);





#endif /* RPMMEAS_RPMMEAS_H_ */
