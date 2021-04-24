/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		menu_adapter.c					################
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
 ** 1	|	MS		|04-23-2021	| imported menu_adapter.c														|
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
 ** Contains function which sets pixels in menu object
 **
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/

#include "display.h"

/*******************************************************************************
 * menu_set_pixel
 * set or delete pixel on poit x,y
 * param handle	menue object,
 * param x:			x position
 * param y:			y position
 * param bw:		set (1) or delete (0) pixel
 ******************************************************************************/
void menu_set_pixel(menu_handle_t *handle, uint8_t x, uint8_t y, bool bw) {

	ssd1309_set_pixel(&g_disp_0.disp_obj, x, y, (ssd1309_color_t)bw);
}

/*******************************************************************************
 * menu_draw_rect
 * draw filled or unfilled rectangle on position x1, y1 with height y2-y1 and width x2-x1
 * param handle:	menue object,
 * param x1:		x1 position
 * param y1:		y1 position
 * param x2:		x2 position
 * param y2:		y2 position
 * param fill:		filled (1) or unfilled (0) rectangle
 * param bw:		noninverted (1) or inverted (0) rectangel
 ******************************************************************************/
void menu_draw_rect(menu_handle_t *handle, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool fill, bool bw) {

	ssd1309_draw_rect(&g_disp_0.disp_obj, x1, y1, x2, y2, fill, (ssd1309_color_t)bw);
}

/*******************************************************************************
 * menu_write_str
 * write string at position x,y
 * param handle:	menue object,
 * param x:			x position
 * param y:			y position
 * param str:		text
 * param bw:		noninverted (1) or inverted (0) text
 ******************************************************************************/
void menu_write_str(menu_handle_t *handle, uint8_t x, uint8_t y, char* str, bool bw) {

	ssd1309_set_pos(&g_disp_0.disp_obj, x, y); //set cursor position
	//write string to cursor position
	ssd1309_write_str(&g_disp_0.disp_obj, str, ssd1309_font_6x8, false, (ssd1309_color_t)bw);
}


/*******************************************************************************
 * menu_disp_lock
 * take display semaphore
 * param handle:	menue object,
 ******************************************************************************/
void menu_disp_lock(menu_handle_t *handle) {

	ssd1309_rtos_lock(&g_disp_0);
}

/*******************************************************************************
 * menu_disp_unlock
 * give display semaphore
 * param handle:	menue object,
 ******************************************************************************/
void menu_disp_unlock(menu_handle_t *handle) {

	ssd1309_rtos_unlock(&g_disp_0);
}


