/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		menu.h							################
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
 ** 1	|	MS		|04-23-2021	| imported menu.h															|
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
 ** contains menu functions
 **
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/

#ifndef DISPLAY_MENU_MENU_H_
#define DISPLAY_MENU_MENU_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * contain status informations
 * param MENU_LOAD:
 * param MENU_REFR:
 * param MENU_DEACT:
 * param MENU_ACT:
 * param MENU_UP:
 * param MENU_DOWN:
 * param MENU_POS:
 ******************************************************************************/
enum {
	MENU_LOAD = 0,
	MENU_REFR,
	MENU_DEACT,
	MENU_ACT,
	MENU_UP,
	MENU_DOWN,
	MENU_POS,
};

/*******************************************************************************
 * menu_entry_type_t
 * contains menu entry types
 * param MENU_LINK:		noninverted color mode
 * param MENU_CHECK:	inverted color mode
 * param MENU_VALUE:	noninverted color mode
 * param MENU_PAGE:		inverted color mode
 ******************************************************************************/
typedef enum {
	MENU_LINK = 0,
	MENU_CHECK,
	MENU_VALUE,
	MENU_PAGE,
} menu_entry_type_t;

/*******************************************************************************
 * menu_entry_t
 * contains menu entry parameters
 * param str:		menu entry text
 * param type:		menu entry type
 * param link:		inverted color mode
 * param act:		noninverted color mode
 * param val:		menu entry value
 * param func:		menu entry function
 * param min_val:	menu entry min value
 * param max_val:	menu entry max value
 * param en:		menu entry clickable or not
 ******************************************************************************/
typedef struct {
	char str[18];
	menu_entry_type_t type;	//menu entry type
	union {
		void (*link)();
		uint32_t *act;
		int32_t *val;
	};
	void (*func)(uint8_t);
	int32_t min_val;
	int32_t max_val;
	uint8_t en: 1;
} menu_entry_t;

/*******************************************************************************
 * menu_entry_type_t
 * contains menu handle
 * param entry_cnt:		number of entries
 * param entry_list:	entries
 * param pos:			actual positon
 * param active_entry:	selected entry
 * param dirty:			should display be updeated or not
 * param draw:			draw function for this entry
 ******************************************************************************/
typedef struct _menu_handle_t {
	uint8_t entry_cnt;
	menu_entry_t *entry_list;
	uint8_t pos;
	uint8_t active_entry: 1;
	uint8_t dirty: 1;
	void (*draw)(struct _menu_handle_t *);
} menu_handle_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * menu_init
 * init menu
 * param handle:		menu handle
 ******************************************************************************/
void menu_init(menu_handle_t *handle);

/*******************************************************************************
 * menu_reset
 * reset menu
 * param handle:		menu handle
 ******************************************************************************/
void menu_reset(menu_handle_t *handle);

/*******************************************************************************
 * menu_click
 * menu entry clicked
 * param handle:		menu handle
 ******************************************************************************/
void menu_click(menu_handle_t *handle);

/*******************************************************************************
 * menu_up
 * scroll uo
 * param handle:		menu handle
 * param incr:
 ******************************************************************************/
void menu_up(menu_handle_t *handle, uint16_t incr);

/*******************************************************************************
 * menu_down
 * scroll down
 * param handle:		menu handle
 * param decr:
 ******************************************************************************/
void menu_down(menu_handle_t *handle, uint16_t decr);

/*******************************************************************************
 * menu_set_pos
 * set courser position
 * param handle:		menu handle
 * param pos
 ******************************************************************************/
void menu_set_pos(menu_handle_t *handle, uint16_t pos);

/*******************************************************************************
 * menu_refresh
 * refresh menu
 * param handle:		menu handle
 ******************************************************************************/
void menu_refresh(menu_handle_t *handle);

/*******************************************************************************
 * menu_task
 * param handle:		menu handle
 ******************************************************************************/
void menu_task(menu_handle_t *handle);

#endif /* DISPLAY_MENU_MENU_H_ */
