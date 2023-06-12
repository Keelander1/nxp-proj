/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		main_gustl.c					################
 ** Created on: 	04-20-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	EC/SM	|2020		| Created main_gustl.c														|
 ** 2	|	EC		|04-20-2021 | Commented the Code and outsourced the Includes, Prototypes and Functions	|
 ** 	|			|			| to board_func.c and board_func.h											|
 ** 3	|			|			| 																			|
 ** 4	|			|			| 																			|
 ** 5	|			|			| 																			|
 ** 6	|			|			| 																			|
 ** 7	|			|			| 																			|
 ** 8	|			|			| 																			|
 ** 9	|			|			| 																			|
 ** 10	|			|			| 																			|
 ************************************************************************************************************
 ***********************************************************************************************************/


#include "main_gustl.h"

/*******************************************************************************
 * Code: Main Entry
 ******************************************************************************/
int main(void)
{
	//*******************************************
	//Initialize board hardware.

	CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH); //attach 12 MHz clock to FLEXCOMM0 (debug console)
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitLEDsPins();
    //*******************************************

    //***********************************************
    //Own initialization of car peripherals and board
    Board_Init(); 	//Initialize board peripherals
    PARAM_Init();	//Initialize EEPROM
    CAM_Init();		//Initialize line-Camera
    RPMMEAS_Init();	//Initialize speed-measurement
    SERVO_Init();	//Initialize servo-drive
    BLDC_Init();	//Initialize BLDC-drives
    //***********************************************

    //***********************************************************************************************
    //Create LED blinking task (Board running feedback)
    if (xTaskCreate(LED_Blink_Task, "LED_Blink_Task", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL) != pdPASS)
    { LED3_ON(); } //LED3 is Error
    //***********************************************************************************************

    //Create HMI task
    if (xTaskCreate(hmi_task, "hmi_task", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL) !=
              pdPASS)
          {
          	LED3_ON();
          }

    //Create Camera_Exposure_time task
    if (xTaskCreate(Camera_Exposure_time_task,"Camera_Exposure_time_task", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL) != pdPASS)
    {
    	LED3_ON();
    }
//    void Camera_Exposure_time(void)
//    {
//    	uint16_t pixel_Values_sum = 0;
//
//    	for(uint8_t x=0;x<128;x++){
//    		pixel_Values_sum = pixel_Values_sum + pixelValues[x];
//    	}
//    	if(pixel_Values_sum < 16384){
//    		exposure_time = exposure_time+10000;
//    	}else{
//    		if(pixel_Values_sum > 16384){
//    			exposure_time = exposure_time-10000;
//    		}
//    	}
//    	CTIMER0->MSR[0] = exposure_time;
//    }
    //***********************************************************************************************


    //********************
    //Start task scheduler
    vTaskStartScheduler();
    //********************

    while(1){}
    return 0;
}

