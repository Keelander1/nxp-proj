/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		ssd1309.c						################
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
 ** 1	|	MS		|04-23-2021	| imported ssd1309.c													|
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
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/

#include "ssd1309_config.h"
#include "ssd1309.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * send these bytes to init the LCD
 ******************************************************************************/
const static uint8_t init_data[] __attribute__((aligned(4U))) = {
		0xAE,	// Set Display off (RESET)

		0x81,	// Set Contrast Control
		0x7F,

		0xA4, 	// Resume to RAM content display (RESET)

		0xA6,	// Set normal display

		0x20,	// Set Memory Addressing Mode
		0x00,	// 00: Horizontal

		0x21,	// Set Column Address
		0x00,	// Start
		0x7F,	// End

		0x22,	// Set Page Address
		0x00,	// Page start Address (RESET)
		0x07,	// Page end Address (RESET)

		0x40,	// Set Display Start Line (RESET)

		0xA1,	// Horizontal normal

		0xA8,	// Set Multiplex Ratio
		0x3F,	// RESET

		0xC8,	// Vertically normal

		0xD3,	// Set Display Offset
		0x00,	// RESET

		0xDA,	// Set COM Pins Hardware Configuration
		0x12,	// RESET

		0xDC,	// Set GPIO
		0x02,	// pin output LOW (RESET)

		0xD5,	// Set Display Clock Divide Ratio/Oscillator Frequency
		0xF0,	// 1111b

		0xD9,	// Set Pre-charge Period
		0x22,	// Phase 1 period 2h
		// Phase 2 period 2h

		0xDB,	// Set Vcomh Deselect Level
		0x34,	// 0.78 x Vcc (RESET)

		0xAF,	// Set Display on
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*******************************************************************************
 * ssd1309_init
 * init LCD
 * param obj: 		LCD object
 ******************************************************************************/
void ssd1309_init(ssd1309_t *obj) {

	//init display
	ssd1309_send(obj, CMD, (uint8_t*)init_data, sizeof(init_data) - 1);

	//set origin
	ssd1309_send(obj, CMD, &obj->config.orientation_y, 0x01);
	ssd1309_send(obj, CMD, &obj->config.orientation_x, 0x01);

	//set display to inverted or non inverted color
	ssd1309_send(obj, CMD, &obj->config.color_type, 0x01);

	//delete whole delete
	ssd1309_fill(obj, OFF);

	//update display
	ssd1309_update(obj);

	//set display on
	ssd1309_send(obj, CMD, (uint8_t*)&init_data[sizeof(init_data) - 1], 0x01);
}

/*******************************************************************************
 * ssd1309_fill
 * fill whole LCD
 * param obj: 		LCD object,
 * param color:		set (ON) or delete (OFF) whole LCD
 ******************************************************************************/
void ssd1309_fill(ssd1309_t *obj, ssd1309_color_t color) {
	//fill whole display
	for (uint16_t i = 0; i < obj->config.byte_cnt; i++)
		obj->buffer[i] = (uint8_t)(255 * (uint8_t)color);
	obj->dirty = true;
}

/*******************************************************************************
 * ssd1309_set_pixel
 * fill whole LCD
 * param obj: 		LCD object,
 * param x:			x position
 * param y:			y position
 * param color:		set (ON) or delete (OFF) pixel
 ******************************************************************************/
void ssd1309_set_pixel(ssd1309_t *obj, uint8_t x, uint8_t y, ssd1309_color_t color) {

	BIT_ACCESS(obj->buffer[x + (y / 8) * obj->config.width], y % 8, color);
	obj->dirty = true;
}

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
void ssd1309_draw_rect(ssd1309_t *obj, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool fill, ssd1309_color_t color) {
	for (uint8_t i = x1; i <= x2; i++) { 		//for every pixel between x1 and x2
		for (uint8_t j = y1; j <= y2; j++) {	//for every pixel between y1 and y2
			if (fill || (i == x1 || i == x2 || j == y1 || j == y2))		//if fill=true or index i or j is on an outline
				ssd1309_set_pixel(obj, i, j, color);	//draw pixel
		}
	}
}

/*******************************************************************************
 * ssd1309_write_char
 * write char
 * param obj:		LCD object
 * param chr:		char
 * param font:		font type
 * param bg:
 * param color:		noninverted (1) or inverted (0) text
 ******************************************************************************/
void ssd1309_write_char(ssd1309_t *obj, char chr, ssd1309_font_t font, bool bg, ssd1309_color_t color) {

	uint8_t lf;	//linefeed
	uint8_t b;
	int16_t h;
	uint8_t tmp;

	//is chr a printable ascii
	if (chr < 32 || chr > 126)
		return;
	//is the character within the LCD
	if ((obj->config.height < (obj->pos.y + font.height)) || (obj->config.width < (obj->pos.x + font.width)))
		return;

	//calculate line feed lf (max: 8 pixels because data is 8 Bit and every Bit stands for one pixel)
	//for example 0x81 (0b1000 0001) height:8 pixel 0 and pixel 7 is on
	lf = (font.height - 1U) / 8 + 1;

	for (uint8_t j = 0; j < font.width; j++) {				//for ever pixel smaller then width
		for (uint8_t i = 0; i < lf; i++) {					//for every line
			if (font.height % 8)							//if height != mult of 8
				h = (i == lf - 1) ? font.height % 8 : 8; 	//if(i==lf)h= height % 8 else h = 8
			else
				h = 8;
			b = font.data[(chr - 32) * font.width * lf + (lf * j) + i]; //calc index of data
			for (uint8_t k = 0; k < h; k++) { 							//for every pixel smaller then line height
				tmp = (ssd1309_color_t)((b >> k) & 0x01);				//temp = 1 if pixel should be set and 0 if not
				if (!tmp & !bg)
					continue;	//do nothing if pixel shouldn´t be set
				ssd1309_set_pixel(obj, obj->pos.x + j, obj->pos.y + (i << 3) + k, tmp ? color : !color);
			}
		}
	}

	obj->pos.x += font.width + 1;		//increment courser position

	obj->dirty = true; 					//LCD must be updated
}

/*******************************************************************************
 * ssd1309_write_string
 * write string at position x,y
 * param obj:		LCD object
 * param str:		string
 * param font:		font type
 * param bg:
 * param color:		noninverted (1) or inverted (0) text
 ******************************************************************************/
void ssd1309_write_str(ssd1309_t *obj, char *str, ssd1309_font_t font, bool bg, ssd1309_color_t color) {

	//print every character in str
	while(*str) {
		ssd1309_write_char(obj, *str, font, bg, color);
		str++;
	}
}

/*******************************************************************************
 * ssd1309_draw_img
 * write string at position x,y
 * param obj:		LCD object
 * param chr:		char
 * param font:		font type
 * param bg:
 * param color:		noninverted (1) or inverted (0) text
 ******************************************************************************/
void ssd1309_draw_img(ssd1309_t *obj, uint8_t x, uint8_t y, ssd1309_img_t img, ssd1309_color_t color) {

	uint8_t lf;
	uint8_t b;
	int16_t h;
	uint8_t tmp;

	//is the character within the LCD
	if ((obj->config.height < (y + img.height)) || (obj->config.width < (x + img.width))) {
		return;
	}

	//calculate line feed lf (max: 8 pixels because data is 8 Bit and every Bit stands for one pixel)
	//for example 0x81 (0b1000 0001) height:8 pixel 0 and pixel 7 is on
	lf = (img.height - 1U) / 8 + 1;

	for(uint8_t i = 0; i < lf; i++) {		//for every line
		if (img.height % 8)								//if height != multiple of 8
			h = (i == lf - 1) ? img.height % 8 : 8;		//if(i==lf)h= height % 8 else h = 8
		else
			h = 8;
		for (uint8_t j = 0; j < img.width; j++) {			//for every pixel smaller then width
			b = img.data[(i * img.width) + j];				//calc index of data
			for (uint8_t k = 0; k < h; k++) {				//for every pixel smaller then line height
				tmp = (ssd1309_color_t)((b >> k) & 0x01);	//temp = 1 if pixel should be set and 0 if not
				ssd1309_set_pixel(obj, x + j, y + (i << 3) + k, tmp ? color : !color);		//set current pixel
			}
		}

	}

	obj->dirty = true;
}

/*******************************************************************************
 * ssd1309_update
 * update LCD
 * param obj:		LCD object
 ******************************************************************************/
void ssd1309_update(ssd1309_t *obj) {

	//send data to LCD if it should be updated
	if (obj->dirty)
		ssd1309_send(obj, DATA, obj->buffer, obj->config.byte_cnt);
	obj->dirty = false;		//LCD is up to date
}
