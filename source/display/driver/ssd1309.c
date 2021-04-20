/*
 * ssd1309.c
 *
 *  Created on: 29 Jan 2020
 *      Author: Arne Kulinna
 */

#include "display/driver/config/ssd1309_config.h"
#include "ssd1309.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Variables
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
void ssd1309_init(ssd1309_t *obj) {

	ssd1309_send(obj, CMD, (uint8_t*)init_data, sizeof(init_data) - 1);
	ssd1309_send(obj, CMD, &obj->config.orientation_y, 0x01);
	ssd1309_send(obj, CMD, &obj->config.orientation_x, 0x01);
	ssd1309_send(obj, CMD, &obj->config.color_type, 0x01);
	ssd1309_fill(obj, OFF);
	ssd1309_update(obj);
	ssd1309_send(obj, CMD, (uint8_t*)&init_data[sizeof(init_data) - 1], 0x01);
}


void ssd1309_fill(ssd1309_t *obj, ssd1309_color_t color) {

	for (uint16_t i = 0; i < obj->config.byte_cnt; i++)
		obj->buffer[i] = (uint8_t)(255 * (uint8_t)color);
	obj->dirty = true;
}

void ssd1309_set_pixel(ssd1309_t *obj, uint8_t x, uint8_t y, ssd1309_color_t color) {

	BIT_ACCESS(obj->buffer[x + (y / 8) * obj->config.width], y % 8, color);
	obj->dirty = true;
}

void ssd1309_draw_rect(ssd1309_t *obj, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool fill, ssd1309_color_t color) {

	for (uint8_t i = x1; i <= x2; i++) {
		for (uint8_t j = y1; j <= y2; j++) {
			if (fill || (i == x1 || i == x2 || j == y1 || j == y2))
				ssd1309_set_pixel(obj, i, j, color);
		}
	}
}

void ssd1309_write_char(ssd1309_t *obj, char chr, ssd1309_font_t font, bool bg, ssd1309_color_t color) {

	uint8_t lf;
	uint8_t b;
	int16_t h;
	uint8_t tmp;

    if (chr < 32 || chr > 126)
        return;

    if ((obj->config.height < (obj->pos.y + font.height)) || (obj->config.width < (obj->pos.x + font.width)))
    	return;

    lf = (font.height - 1U) / 8 + 1;

    for (uint8_t j = 0; j < font.width; j++) {
		for (uint8_t i = 0; i < lf; i++) {
			if (font.height % 8)
				h = (i == lf - 1) ? font.height % 8 : 8;
			else
				h = 8;
			b = font.data[(chr - 32) * font.width * lf + (lf * j) + i];
			for (uint8_t k = 0; k < h; k++) {
				tmp = (ssd1309_color_t)((b >> k) & 0x01);
				if (!tmp & !bg)
					continue;
				ssd1309_set_pixel(obj, obj->pos.x + j, obj->pos.y + (i << 3) + k, tmp ? color : !color);
			}
		}
    }

    obj->pos.x += font.width + 1;
    obj->dirty = true;
}

void ssd1309_write_str(ssd1309_t *obj, char *str, ssd1309_font_t font, bool bg, ssd1309_color_t color) {

	while(*str) {
		ssd1309_write_char(obj, *str, font, bg, color);
		str++;
	}
}

void ssd1309_draw_img(ssd1309_t *obj, uint8_t x, uint8_t y, ssd1309_img_t img, ssd1309_color_t color) {

	uint8_t lf;
	uint8_t b;
	int16_t h;
	uint8_t tmp;


    if ((obj->config.height < (y + img.height)) || (obj->config.width < (x + img.width))) {
    	return;
    }

    lf = (img.height - 1U) / 8 + 1;

	for(uint8_t i = 0; i < lf; i++) {
		if (img.height % 8)
			h = (i == lf - 1) ? img.height % 8 : 8;
		else
			h = 8;
		for (uint8_t j = 0; j < img.width; j++) {
			b = img.data[(i * img.width) + j];
			for (uint8_t k = 0; k < h; k++) {
				tmp = (ssd1309_color_t)((b >> k) & 0x01);
				ssd1309_set_pixel(obj, x + j, y + (i << 3) + k, tmp ? color : !color);
			}
		}

	}

	obj->dirty = true;
}

void ssd1309_update(ssd1309_t *obj) {

	if (obj->dirty)
		ssd1309_send(obj, DATA, obj->buffer, obj->config.byte_cnt);
	obj->dirty = false;
}
