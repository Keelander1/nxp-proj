/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		menu.c							################
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
 ** 1	|	MS		|04-**-2021	| imported menu.c															|
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
#include "menu.h"

/*******************************************************************************
 * menu_init
 * init menu
 * param handle:		menu handle
 ******************************************************************************/
void menu_init(menu_handle_t *handle) {

	handle->pos = 0;
	handle->active_entry = 0;
	handle->dirty = 1;
}

/*******************************************************************************
 * menu_reset
 * reset menu
 * param handle:		menu handle
 ******************************************************************************/
void menu_reset(menu_handle_t *handle) {

	handle->pos = 0;
	handle->active_entry = 0;
	handle->dirty = 1;
}

/*******************************************************************************
 * menu_click
 * menu entry clicked
 * param handle:		menu handle
 ******************************************************************************/
void menu_click(menu_handle_t *handle) {

	menu_entry_t *entry;

	entry = &handle->entry_list[handle->pos];

	if (!entry->en)
		return;

	handle->dirty = 1;

	if (entry->type == MENU_CHECK) {
		*entry->act += 1;
	} else if (entry->type == MENU_LINK)
		entry->link();
	else if (entry->type == MENU_PAGE) {
		if (handle->active_entry)
			handle->active_entry = 0;
		else {
			handle->active_entry = 1;
			entry->func(MENU_LOAD);
			return;
		}
	} else
		handle->active_entry ^= 1U;
	if (entry->func)
		entry->func(MENU_DEACT + handle->active_entry);
}

/*******************************************************************************
 * menu_up
 * scroll uo
 * param handle:		menu handle
 * param incr:
 ******************************************************************************/
void menu_up(menu_handle_t *handle, uint16_t incr) {

	menu_entry_t *entry;

	entry = &handle->entry_list[handle->pos];

	if (handle->active_entry) {
		if (entry->type == MENU_PAGE)
			return;
		if(entry->type == MENU_VALUE) {
			if ((*entry->val + incr) <= entry->max_val) {
				*entry->val += incr;
				handle->dirty = 1;
			}
		}
		if (entry->func)
			entry->func(MENU_UP);
	} else {
		if ((uint16_t)(handle->pos + incr) >= handle->entry_cnt)
			handle->pos = handle->entry_cnt - 1;
		else
			handle->pos += incr;
		handle->dirty = 1;
	}
}

/*******************************************************************************
 * menu_down
 * scroll down
 * param handle:		menu handle
 * param decr:
 ******************************************************************************/
void menu_down(menu_handle_t *handle, uint16_t decr) {

	menu_entry_t *entry;

	entry = &handle->entry_list[handle->pos];

	if (handle->active_entry) {
		if (entry->type == MENU_PAGE)
			return;
		if(entry->type == MENU_VALUE) {
			if ((*entry->val - decr) >= entry->min_val) {
				*entry->val -= decr;
				handle->dirty = 1;
			}
		}
		if (entry->func)
			entry->func(MENU_DOWN);
	} else {
		if ((int16_t)(handle->pos - decr) < 0)
			handle->pos = 0;
		else
			handle->pos -= decr;
		handle->dirty = 1;
	}
}

/*******************************************************************************
 * menu_set_pos
 * set courser position
 * param handle:		menu handle
 * param pos
 ******************************************************************************/
void menu_set_pos(menu_handle_t *handle, uint16_t pos) {

	menu_entry_t *entry;

	entry = &handle->entry_list[handle->pos];

	if (handle->active_entry) {
		if (entry->type == MENU_PAGE)
			return;
		if(entry->type == MENU_VALUE) {
			if (pos >= entry->min_val && pos <= entry->max_val) {
				*entry->val = pos;
				handle->dirty = 1;
			}
		}
		if (entry->func)
			entry->func(MENU_POS);
	} else {
		if (pos >= 0 && pos < handle->entry_cnt) {
			handle->pos = pos;
			handle->dirty = 1;
		}
	}
}

/*******************************************************************************
 * menu_refresh
 * refresh menu
 * param handle:		menu handle
 ******************************************************************************/
void menu_refresh(menu_handle_t *handle) {

	menu_entry_t *entry;

	entry = &handle->entry_list[handle->pos];

	entry->func(MENU_REFR);
}

/*******************************************************************************
 * menu_task
 * param handle:		menu handle
 ******************************************************************************/
void menu_task(menu_handle_t *handle) {

	if (handle == NULL)
		return;

	handle->draw(handle);
}
