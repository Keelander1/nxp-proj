/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		param.c							################
 ** Created on: 	04-28-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	MS		|04-xx-2021	| imported param.c														|
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
 ** Contains EEPROM functions
 **
 ************************************************************************************************************
 ***********************************************************************************************************/

#include "param.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t const_all_param[FSL_FEATURE_EEPROM_PAGE_SIZE];	//all constants
all_param_t *all_param = (all_param_t*)const_all_param;

int8_t calibrationCamera_storage[2][128];

uint8_t state_control_storage[FSL_FEATURE_EEPROM_PAGE_SIZE];	//all constants
state_control_t *controlParam = (state_control_t*)state_control_storage;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*******************************************************************************
 * PARAM_init
 * init EEPROM functionalit
 ******************************************************************************/
void PARAM_Init() {

	eeprom_config_t config;			//EEPROM convig
	uint32_t src_clk;				//EEPROM clock

	src_clk = EEPROM_CLK_FREQ; 		//get EEPROM clock frequency

	EEPROM_GetDefaultConfig(&config);		//read EEPROM default configuration
	EEPROM_Init(EEPROM, &config, src_clk);	//init EEPROM

	//read EEPROM content to const_all_param,
	memcpy(const_all_param, (uint8_t*)(FSL_FEATURE_EEPROM_BASE_ADDRESS + PARAM_EEPROM_PAGE_NUM * FSL_FEATURE_EEPROM_PAGE_SIZE), FSL_FEATURE_EEPROM_PAGE_SIZE);

	memcpy(calibrationCamera_storage[0], (uint8_t*)(FSL_FEATURE_EEPROM_BASE_ADDRESS + 1 * FSL_FEATURE_EEPROM_PAGE_SIZE), FSL_FEATURE_EEPROM_PAGE_SIZE);
	memcpy(calibrationCamera_storage[1], (uint8_t*)(FSL_FEATURE_EEPROM_BASE_ADDRESS + 2 * FSL_FEATURE_EEPROM_PAGE_SIZE), FSL_FEATURE_EEPROM_PAGE_SIZE);

	memcpy(controlParam, (uint8_t*)(FSL_FEATURE_EEPROM_BASE_ADDRESS + 3 * FSL_FEATURE_EEPROM_PAGE_SIZE), FSL_FEATURE_EEPROM_PAGE_SIZE);
}

/*******************************************************************************
 * paramm_save
 * save parameters to EEPROM
 ******************************************************************************/
void param_save() {

	//copy const_all_param to EEPROM memory on page PARAM_EEPROM_PAGE_NUM
	if (0 != memcmp((uint8_t*)(FSL_FEATURE_EEPROM_BASE_ADDRESS + PARAM_EEPROM_PAGE_NUM * FSL_FEATURE_EEPROM_PAGE_SIZE), const_all_param, FSL_FEATURE_EEPROM_PAGE_SIZE))
		//if copy succeed write const_all_param to EEPROM
		EEPROM_WritePage(EEPROM, PARAM_EEPROM_PAGE_NUM, (uint32_t*)const_all_param);

	if (0 != memcmp((uint8_t*)(FSL_FEATURE_EEPROM_BASE_ADDRESS + 1 * FSL_FEATURE_EEPROM_PAGE_SIZE), calibrationCamera_storage[0], FSL_FEATURE_EEPROM_PAGE_SIZE))
		EEPROM_WritePage(EEPROM, (PARAM_EEPROM_PAGE_NUM + 1),(uint32_t*)calibrationCamera_storage[0]);

	if (0 != memcmp((uint8_t*)(FSL_FEATURE_EEPROM_BASE_ADDRESS + 2 * FSL_FEATURE_EEPROM_PAGE_SIZE), calibrationCamera_storage[1], FSL_FEATURE_EEPROM_PAGE_SIZE))
		EEPROM_WritePage(EEPROM, (PARAM_EEPROM_PAGE_NUM + 2),(uint32_t*)calibrationCamera_storage[1]);

	if (0 != memcmp((uint8_t*)(FSL_FEATURE_EEPROM_BASE_ADDRESS + 3 * FSL_FEATURE_EEPROM_PAGE_SIZE), controlParam, FSL_FEATURE_EEPROM_PAGE_SIZE))
			EEPROM_WritePage(EEPROM, (PARAM_EEPROM_PAGE_NUM + 3),(uint32_t*)controlParam);
}
