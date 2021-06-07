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
 ** 1	|	MS		|04-23-2021	| imported menu_list.c														|
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

	uint8_t page;				//menu page
	uint8_t l_pos;				//menu active element pos
	uint8_t page_entries;		//number of entries on actual page
	uint8_t slider;				//slieder position
	int32_t value;				//value of menu element
	uint16_t temp_val;			//for digit calculation
	char temp_str[6];			//digits in ASCII
	menu_entry_t *entry;		//actual entry

	//return if menu shouldn´t be printed
	if (!handle->dirty) {
		return;
	}

	//if menu only has to be refreshed
	if (handle->active_entry && handle->entry_list[handle->pos].type == MENU_PAGE) {
		menu_refresh(handle);
		return;
	}
	//if courser pos is out of range
	if (handle->pos < 0 || handle->pos >= handle->entry_cnt)
		handle->pos = 0;


	page = handle->pos / 5;			//calculate actual page number
	l_pos = handle->pos % 5;		//calculate positon (0-4)
	page_entries = (handle->entry_cnt / 5) == page ? (handle->entry_cnt % 5) : 5;	//calculate number of entries on actual page

	menu_disp_lock(handle);		//get semaphore

	menu_draw_rect(handle, 0, 13, 127, 63, true, false);	//clear menu part of disply

	//draw select bar on active entry
	for (uint8_t i = 0; i < 5; i++) {
		menu_draw_rect(handle, 0, (i * 10) + 18, 0, (i * 10) + 19, true, (i == l_pos));
		menu_draw_rect(handle, 1, (i * 10) + 17, 1, (i * 10) + 20, true, (i == l_pos));
		menu_draw_rect(handle, 2, (i * 10) + 16, 2, (i * 10) + 21, true, (i == l_pos));
		menu_draw_rect(handle, 3, (i * 10) + 15, 3, (i * 10) + 22, true, (i == l_pos));
		menu_draw_rect(handle, 4, (i * 10) + 14, 122, (i * 10) + 23, true, (i == l_pos));

		//for every visible entry on menu page
		if (i < page_entries) {
			entry = &handle->entry_list[(page * 5) + i];	//get entries on current menu page
			menu_write_str(handle, 5, (i * 10) + 15, entry->str, !(i == l_pos));	//draw inverted text on select bar in noninverted text on non selected entry
			if (entry->type == MENU_VALUE) {	//if current entry is MENU_VALUE
				if (handle->active_entry)		//if entry is active
					menu_draw_rect(handle, 84, (i * 10) + 14, 120, (i * 10) + 23, true, false);		//delete rect in active MENU_VALUE for showing value
				value = *entry->val;	//get value
				//if value is greater then 9999 draw >9999
				if (value > 9999) {
					temp_str[0] = '>';	//draw >
					value = 9999;		//value is 9999
				}
				//if value is less then -9999 draw <-9999
				else if (value < -9999) {
					temp_str[0] = '<';	//draw <
					value = 9999;		//draw 9999
				}
				//if value is less then 0 draw -
				else if (value < 0) {
					temp_str[0] = '-'; 	//draw -
					value *= -1;		//value is value * -1
				}
				else
					temp_str[0] = ' ';
				//if value is <-9999 draw <-9999
				if (temp_str[0] == '<' && value == 9999)
					temp_str[1] = '-';
				//if value is greater then -9999 and lower then 9999
				else {
					temp_val = value / 1000;						//calculate thousands
					temp_str[1] = temp_val ? temp_val + '0' : ' ';	//display thousands or nothing if greater then -1000 or less then 1000
				}
				temp_val = (value / 100) % 10;											//calculate hundreds
				temp_str[2] = temp_val ? temp_val + '0' : (value < 100 ? ' ' : '0');	//display hundreds or nothing if greater then -100 or less then 100
				temp_val = (value / 10) % 10;											//calculate tens
				temp_str[3] = temp_val ? temp_val + '0' : (value < 100 ? ' ' : '0');	//display tens or nothing if greater then -10 or less then 10
				temp_val = value % 10;													//calculate ones
				temp_str[4] = temp_val + '0';											//display ones
				temp_str[5] = 0x00;														//Null-terminator
				menu_write_str(handle, 85, (i * 10) + 15, temp_str, (!(i == l_pos) || handle->active_entry));	//draw value to display
			} else if (entry->type == MENU_CHECK) {
				if (i == l_pos)
					menu_draw_rect(handle, 112, (i * 10) + 14, 121, (i * 10) + 23, true, false);	//draw black rect arround checkbox if entry is selected
				menu_draw_rect(handle, 113, (i * 10) + 15, 120, (i * 10) + 22, false, true);		//draw checkbox
				//if entry is checked, draw check cross in check box
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
	slider = 45 / handle->entry_cnt;	//calculate slider sice
	menu_draw_rect(handle, 125, 14, 127, 14, false, true);		//draw slider top sign
	menu_draw_rect(handle, 125, ((handle->entry_cnt - 1) * slider) + slider + 18, 127, ((handle->entry_cnt - 1) * slider) + slider + 18, false, true);	//draw slider bottom sign
	menu_draw_rect(handle, 125, 16, 127, ((handle->entry_cnt - 1) * slider) + slider + 16, true, false);												//delete last slider
	menu_draw_rect(handle, 125, (handle->pos * slider) + 16, 127, (handle->pos * slider) + slider + 16, true, true);									//draw slider

	menu_disp_unlock(handle);

	handle->dirty = 0;
}

