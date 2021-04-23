/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		menu_list.c						################
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
 ** 1	|	MS		|04-**-2021	| imported menu_list.c														|
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
 ** contains function which displays the menu
 **
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/

#include "menu_list.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/*******************************************************************************
 * menu_list_draw
 * draw menu
 * param handle: 		menu handle,
 ******************************************************************************/
void menu_list_draw(menu_handle_t *handle) {

	uint8_t page;
	uint8_t l_pos;
	uint8_t page_entries;
	uint8_t slider;
	int32_t value;
	uint16_t temp_val;
	char temp_str[6];
	menu_entry_t *entry;

	if (!handle->dirty) {
		return;
	}

	if (handle->active_entry && handle->entry_list[handle->pos].type == MENU_PAGE) {
		menu_refresh(handle);
		return;
	}

	if (handle->pos < 0 || handle->pos >= handle->entry_cnt)
		handle->pos = 0;

	page = handle->pos / 5;
	l_pos = handle->pos % 5;
	page_entries = (handle->entry_cnt / 5) == page ? (handle->entry_cnt % 5) : 5;

	menu_disp_lock(handle);

	menu_draw_rect(handle, 0, 13, 127, 63, true, false);

	for (uint8_t i = 0; i < 5; i++) {
		menu_draw_rect(handle, 0, (i * 10) + 18, 0, (i * 10) + 19, true, (i == l_pos));
		menu_draw_rect(handle, 1, (i * 10) + 17, 1, (i * 10) + 20, true, (i == l_pos));
		menu_draw_rect(handle, 2, (i * 10) + 16, 2, (i * 10) + 21, true, (i == l_pos));
		menu_draw_rect(handle, 3, (i * 10) + 15, 3, (i * 10) + 22, true, (i == l_pos));
		menu_draw_rect(handle, 4, (i * 10) + 14, 122, (i * 10) + 23, true, (i == l_pos));
		if (i < page_entries) {
			entry = &handle->entry_list[(page * 5) + i];
			menu_write_str(handle, 5, (i * 10) + 15, entry->str, !(i == l_pos));
			if (entry->type == MENU_VALUE) {
				if (handle->active_entry)
					menu_draw_rect(handle, 84, (i * 10) + 14, 120, (i * 10) + 23, true, false);
				value = *entry->val;
				if (value > 9999) {
					temp_str[0] = '>';
					value = 9999;
				} else if (value < -9999) {
					temp_str[0] = '<';
					value = 9999;
				} else if (value < 0) {
					temp_str[0] = '-';
					value *= -1;
				}
				else
					temp_str[0] = ' ';
				if (temp_str[0] == '<' && value == 9999)
					temp_str[1] = '-';
				else {
					temp_val = value / 1000;
					temp_str[1] = temp_val ? temp_val + '0' : ' ';
				}
				temp_val = (value / 100) % 10;
				temp_str[2] = temp_val ? temp_val + '0' : (value < 100 ? ' ' : '0');
				temp_val = (value / 10) % 10;
				temp_str[3] = temp_val ? temp_val + '0' : (value < 100 ? ' ' : '0');
				temp_val = value % 10;
				temp_str[4] = temp_val + '0';
				temp_str[5] = 0x00;
				menu_write_str(handle, 85, (i * 10) + 15, temp_str, (!(i == l_pos) || handle->active_entry));
			} else if (entry->type == MENU_CHECK) {
				if (i == l_pos)
					menu_draw_rect(handle, 112, (i * 10) + 14, 121, (i * 10) + 23, true, false);
				menu_draw_rect(handle, 113, (i * 10) + 15, 120, (i * 10) + 22, false, true);
				if (*entry->val & 0x01) {
					menu_set_pixel(handle, 115, (i * 10) + 17, true);
					menu_set_pixel(handle, 116, (i * 10) + 18, true);
					menu_set_pixel(handle, 117, (i * 10) + 19, true);
					menu_set_pixel(handle, 118, (i * 10) + 20, true);
					menu_set_pixel(handle, 115, (i * 10) + 20, true);
					menu_set_pixel(handle, 116, (i * 10) + 19, true);
					menu_set_pixel(handle, 117, (i * 10) + 18, true);
					menu_set_pixel(handle, 118, (i * 10) + 17, true);
				}
			}
		}
	}

	// Slider
	slider = 45 / handle->entry_cnt;
	menu_draw_rect(handle, 125, 14, 127, 14, false, true);
	menu_draw_rect(handle, 125, ((handle->entry_cnt - 1) * slider) + slider + 18, 127, ((handle->entry_cnt - 1) * slider) + slider + 18, false, true);
	menu_draw_rect(handle, 125, 16, 127, ((handle->entry_cnt - 1) * slider) + slider + 16, true, false);
	menu_draw_rect(handle, 125, (handle->pos * slider) + 16, 127, (handle->pos * slider) + slider + 16, true, true);

	menu_disp_unlock(handle);

	handle->dirty = 0;
}

