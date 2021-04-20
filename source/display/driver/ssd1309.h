/*
 * ssd1309.h
 *
 *  Created on: 29 Jan 2020
 *      Author: Arne Kulinna
 */

#ifndef SSD1309_H_
#define SSD1309_H_

#include <stdint.h>
#include <stdbool.h>

#include "resources/ssd1309_font.h"
#include "resources/ssd1309_img.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
typedef enum {
	STD = 0xA6,
	INV = 0xA7,
} ssd1309_color_type_t;

typedef enum {
	UP    = 0xC0,
	DOWN  = 0xC8,
	LEFT  = 0xA0,
	RIGHT = 0xA1,
} ssd1309_orientation_t;

typedef struct {
	uint8_t id;
	uint8_t height;
	uint8_t width;
	uint16_t byte_cnt;
	ssd1309_orientation_t orientation_y;
	ssd1309_orientation_t orientation_x;
	ssd1309_color_type_t color_type;
} ssd1309_config_t;

typedef struct {
	uint8_t x;
	uint8_t y;
} ssd1309_pos_t;

typedef enum {
	OFF = 0,
	ON = 1,
} ssd1309_color_t;

typedef enum {
	CMD = 0,
	DATA = 1,
} ssd1309_data_t;

typedef struct {
	ssd1309_config_t config;
	ssd1309_pos_t pos;
	uint8_t *buffer;
	void *user_data;
	bool dirty;
} ssd1309_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void ssd1309_init(ssd1309_t *obj);
void ssd1309_fill(ssd1309_t *obj, ssd1309_color_t color);
void ssd1309_set_pixel(ssd1309_t *obj, uint8_t x, uint8_t y, ssd1309_color_t color);
void ssd1309_draw_rect(ssd1309_t *obj, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool fill, ssd1309_color_t color);
void ssd1309_write_char(ssd1309_t *obj, char chr, ssd1309_font_t font, bool bg, ssd1309_color_t color);
void ssd1309_write_str(ssd1309_t *obj, char *str, ssd1309_font_t font, bool bg, ssd1309_color_t color);
void ssd1309_draw_img(ssd1309_t *obj, uint8_t x, uint8_t y, ssd1309_img_t img, ssd1309_color_t color);
void ssd1309_update(ssd1309_t *obj);
void ssd1309_send(ssd1309_t *obj, ssd1309_data_t d_type, uint8_t *data, uint32_t size) __attribute__((weak));

static inline void ssd1309_set_pos(ssd1309_t *obj, uint8_t x, uint8_t y) {

	obj->pos.x = x;
	obj->pos.y = y;
}

#endif /* SSD1309_H_ */
