/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		ssd1309.h						################
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
 ** 1	|	MS		|04-**-2021	| imported ssd1309.h													|
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
 ** contains functions to init the LCD and set pixels on the LCD
 **
 ** Use this data in ssd1309_write_char to draw the char
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/

#ifndef SSD1309_H_
#define SSD1309_H_

#include <stdint.h>
#include <stdbool.h>

#include "ssd1309_font.h"		//include fonts
#include "ssd1309_img.h"		//include images

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/


//******************************************************************************
/*******************************************************************************
 * structs to init LCD
 ******************************************************************************/

/*******************************************************************************
 * color type of LCD
 ******************************************************************************/
typedef enum {
	STD = 0xA6,	//noninverted color mode
	INV = 0xA7,	//inverted color mode
} ssd1309_color_type_t;

/*******************************************************************************
 * orientation of LCD
 ******************************************************************************/
typedef enum {
	UP    = 0xC0,	//y orientation from top to bottom
	DOWN  = 0xC8,	//y orientation from bottom to top
	LEFT  = 0xA0,	//x orientation from left to right
	RIGHT = 0xA1,	//x orientation from right to left
} ssd1309_orientation_t;

/*******************************************************************************
 * configuration of LCD
 ******************************************************************************/
typedef struct {
	uint8_t id;								//I2C address
	uint8_t height;							//LCD height in pixels
	uint8_t width;							//LCD width in pixels
	uint16_t byte_cnt;						//pixel count in bytes
	ssd1309_orientation_t orientation_y;	//y orientation
	ssd1309_orientation_t orientation_x;	//x orinetaton
	ssd1309_color_type_t color_type;		//color type
} ssd1309_config_t;

/*******************************************************************************
 * courser position
 ******************************************************************************/
typedef struct {
	uint8_t x;	//x position
	uint8_t y;	//y position
} ssd1309_pos_t;

/*******************************************************************************
 * draw or delete pixel
 ******************************************************************************/
typedef enum {
	OFF = 0,	//draw pixel
	ON = 1,		//delete pixel
} ssd1309_color_t;

/*******************************************************************************
 * data or command
 ******************************************************************************/
typedef enum {
	CMD = 0,	//data is command
	DATA = 1,	//data is data
} ssd1309_data_t;

/*******************************************************************************
 * LCD object
 ******************************************************************************/
typedef struct {
	ssd1309_config_t config;	//LCD configuration
	ssd1309_pos_t pos;			//courser position
	uint8_t *buffer;			//data buffer
	void *user_data;			//user data
	bool dirty;					// should display be updated or not
} ssd1309_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
* ssd1309_init
* init LCD
* param obj: 		LCD object
******************************************************************************/
void ssd1309_init(ssd1309_t *obj);

/*******************************************************************************
 * ssd1309_fill
 * fill whole LCD
 * param obj: 		LCD object,
 * param color:		set (ON) or delete (OFF) whole LCD
 ******************************************************************************/
void ssd1309_fill(ssd1309_t *obj, ssd1309_color_t color);

/*******************************************************************************
 * ssd1309_set_pixel
 * fill whole LCD
 * param obj: 		LCD object,
 * param x:			x position
 * param y:			y position
 * param color:		set (ON) or delete (OFF) pixel
 ******************************************************************************/
void ssd1309_set_pixel(ssd1309_t *obj, uint8_t x, uint8_t y, ssd1309_color_t color);

/*******************************************************************************
 * ssd1309_draw_rect
 * draw filled or unfilled rectangle on position x1, y1 with height y2-y1 and width x2-x1
 * param handle:	LCD object,
 * param x1:		x1 position
 * param y1:		y1 position
 * param x2:		x2 position
 * param y2:		y2 position
 * param fill:		filled (1) or unfilled (0) rectangle
 * param bw:		noninverted (ON) or inverted (OFF) rectangel
 ******************************************************************************/
void ssd1309_draw_rect(ssd1309_t *obj, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool fill, ssd1309_color_t color);

/*******************************************************************************
 * ssd1309_write_char
 * write char
 * param obj:		LCD object
 * param chr:		char
 * param font:		font type
 * param bg:
 * param color:		noninverted (1) or inverted (0) text
 ******************************************************************************/
void ssd1309_write_char(ssd1309_t *obj, char chr, ssd1309_font_t font, bool bg, ssd1309_color_t color);

/*******************************************************************************
 * ssd1309_write_string
 * write string at position x,y
 * param obj:		LCD object
 * param str:		string
 * param font:		font type
 * param bg:
 * param color:		noninverted (1) or inverted (0) text
 ******************************************************************************/
void ssd1309_write_str(ssd1309_t *obj, char *str, ssd1309_font_t font, bool bg, ssd1309_color_t color);

/*******************************************************************************
 * ssd1309_draw_img
 * write string at position x,y
 * param obj:		LCD object
 * param chr:		char
 * param font:		font type
 * param bg:
 * param color:		noninverted (1) or inverted (0) text
 ******************************************************************************/
void ssd1309_draw_img(ssd1309_t *obj, uint8_t x, uint8_t y, ssd1309_img_t img, ssd1309_color_t color);

/*******************************************************************************
 * ssd1309_update
 * update LCD
 * param obj:		LCD object
 ******************************************************************************/
void ssd1309_update(ssd1309_t *obj);

/*******************************************************************************
 * ssd1309_send
 * init LCD with RTOS operation
 * param obj: 		LCD object,
 * param d_type:	is data a command (0) or data (1)
 * param data:		data
 * param size:		size of data
 ******************************************************************************/
void ssd1309_send(ssd1309_t *obj, ssd1309_data_t d_type, uint8_t *data, uint32_t size) __attribute__((weak));

/*******************************************************************************
 * ssd1309_set_pos
 * set courser positon
 * param obj: 		LCD object,
 * param x:			x position
 * param y:			y positon
 ******************************************************************************/
static inline void ssd1309_set_pos(ssd1309_t *obj, uint8_t x, uint8_t y) {

	obj->pos.x = x;
	obj->pos.y = y;
}

#endif /* SSD1309_H_ */
