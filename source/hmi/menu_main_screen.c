/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		menu_main_screen.c				################
 ** Created on: 	04-23-2021						#####| |########  University of applied sciences
 ** Authors: 		Ecker Christian,				#####| |########  Landshut, Germany
 ** 				Summer Matthias,				#####| |########
 ** 				Ambrosch Markus					#####|  __ |####  Am Lurzenhof 1, 84036 Landshut
 ** 				                                #####| |##||####
 **													#####|_|##|_|###
 **	ChangeLog:										################
 ************************************************************************************************************
 **		| Authors	| Date 		| Commit																	|
 **	----|-----------|-----------|---------------------------------------------------------------------------|
 ** 1	|	MS		|04-24-2021	| imported menu_main_screen.c												|
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
 ** Header file for HMI functions:
 **
 ** contains menu main screen layoutS
 **
 ** ENC_A (Encoder signal A) at Pin P[3][20] (J9 Pin9)
 ** ENC_B (Encoder signal B) at Pin P[3][22] (J9 Pin11)
 ** ENC_SW (Encoder switch) at Pin P[3][21] (J9 Pin13)
 **
 ** SW (switch) at Pin P[3][30] (J9 Pin15)s
 ************************************************************************************************************
 ***********************************************************************************************************/

#include "menu_main_screen.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void draw(menu_handle_t *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const uint8_t img_data_frame[] = {
	0xFC, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0xFC, 0xFF, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0x01, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x02, 0x01,
};

static const ssd1309_img_t img_frame = {
	.height = 27,
	.width = 27,
	.data = img_data_frame,
};

static const uint8_t img_data_footer_middle_active[] = {
	0x60, 0x70, 0x78, 0x7C, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
	0x7F, 0x7F, 0x7F, 0x7F, 0x51, 0x55, 0x45, 0x7F, 0x7D, 0x41,
	0x7D, 0x7F, 0x41, 0x75, 0x41, 0x7F, 0x41, 0x75, 0x65, 0x51,
	0x7F, 0x7D, 0x41, 0x7D, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
	0x7F, 0x7F, 0x7F, 0x7E, 0x7C, 0x78, 0x70, 0x60,
};

static const ssd1309_img_t img_footer_middle_active = {
	.height = 7,
	.width = 48,
	.data = img_data_footer_middle_active,
};

static const uint8_t img_data_footer_middle_inactive[] = {
	0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x5D, 0x55, 0x75, 0x01, 0x05, 0x7D,
	0x05, 0x01, 0x7D, 0x15, 0x7D, 0x01, 0x7D, 0x15, 0x35, 0x5D,
	0x01, 0x05, 0x7D, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
};

static const ssd1309_img_t img_footer_middle_inactive = {
	.height = 7,
	.width = 48,
	.data = img_data_footer_middle_inactive,
};

static const menu_entry_t entries[] = {
		{
				.type = MENU_VALUE,
				//.val = &((all_param_t*)&const_all_param)->drive.speed,
				.val = 0,
				.min_val = 0,
				.max_val = 100,
				.en = true,
		},
		{
				.type = MENU_CHECK,
				//.act = &((all_param_t*)&const_all_param)->drive.stop,
				.val =0,
				.en = true,
		},
		{
				.type = MENU_LINK,
				//.link = extern_drive_start,
				.link = 0,
				.en = true,
		},
		{
				.type = MENU_CHECK,
				//.act = &((all_param_t*)&const_all_param)->drive.limit,
				.act = 0,
				.en = true,
		},
		{
				.type = MENU_CHECK,
				//.act = (uint32_t*)&((all_param_t*)&const_all_param)->drive.object,
				.act =0,
				.en = true,
		},
};

menu_rtos_handle_t menu_main_screen_handle = {
		.drv_handle = {
				.entry_cnt = 5,
				.entry_list = (menu_entry_t*)entries,
				.draw = draw,
				.pos = 2,
		},
};
/*******************************************************************************
 * Code
 ******************************************************************************/
static void draw(menu_handle_t *handle) {

	menu_entry_t *entry;

	char temp_str[4];
	uint8_t digits = 0;

	if (!handle->dirty) {
		return;
	}

	ssd1309_rtos_lock(&g_disp_0);
	ssd1309_draw_rect(&g_disp_0.disp_obj, 0, 13, 127, 63, true, OFF);
	ssd1309_draw_img(&g_disp_0.disp_obj, 0, 57, img_footer_outer, ON);

	ssd1309_draw_img(&g_disp_0.disp_obj, 5, 19, img_frame, ON);
	ssd1309_draw_img(&g_disp_0.disp_obj, 35, 19, img_frame, ON);
	ssd1309_draw_img(&g_disp_0.disp_obj, 66, 19, img_frame, ON);
	ssd1309_draw_img(&g_disp_0.disp_obj, 96, 19, img_frame, ON);

	if (handle->pos == 2)
		ssd1309_draw_img(&g_disp_0.disp_obj, 40, 57, img_footer_middle_active, ON);
	else {
		if (handle->pos == 0)
			ssd1309_draw_rect(&g_disp_0.disp_obj, 6, 20, 30, 44, true, ON);
		else if (handle->pos == 1)
			ssd1309_draw_rect(&g_disp_0.disp_obj, 36, 20, 60, 44, true, ON);
		else if (handle->pos == 3)
			ssd1309_draw_rect(&g_disp_0.disp_obj, 67, 20, 91, 44, true, ON);
		else if (handle->pos == 4)
			ssd1309_draw_rect(&g_disp_0.disp_obj, 97, 20, 121, 44, true, ON);
		ssd1309_draw_img(&g_disp_0.disp_obj, 40, 57, img_footer_middle_inactive, ON);
	}

	entry = &handle->entry_list[0];

	if (*entry->val < 10UL) {
		temp_str[0] = (*entry->val % 10) + '0';
		temp_str[1] = 0x00;
		digits = 1;
	} else if (*entry->val < 100UL) {
		temp_str[0] = ((*entry->val / 10) % 10) + '0';
		temp_str[1] = (*entry->val % 10) + '0';
		temp_str[2] = 0x00;
		digits = 2;
	} else if (*entry->val < 1000UL) {
		temp_str[0] = ((*entry->val / 100) % 10) + '0';
		temp_str[1] = ((*entry->val / 10) % 10) + '0';
		temp_str[2] = (*entry->val % 10) + '0';
		temp_str[3] = 0x00;
		digits = 3;
	}

	ssd1309_draw_img(&g_disp_0.disp_obj, 8, 22, img_sign_speed, !(handle->pos == 0));
	ssd1309_set_pos(&g_disp_0.disp_obj, 13 + ((3 - digits) * 2), 30);
	ssd1309_write_str(&g_disp_0.disp_obj, temp_str, ssd1309_font_3x5, true, !(handle->pos == 0));

	entry = &handle->entry_list[1];
	if (*entry->act & 0x01)
		ssd1309_draw_img(&g_disp_0.disp_obj, 38, 22, img_sign_stop, !(handle->pos == 1));
	else
		ssd1309_draw_img(&g_disp_0.disp_obj, 38, 22, img_sign_right_of_way, !(handle->pos == 1));

	entry = &handle->entry_list[3];
	if (*entry->act & 0x01)
		ssd1309_draw_img(&g_disp_0.disp_obj, 69, 22, img_sign_unlimited, !(handle->pos == 3));
	else
		ssd1309_draw_img(&g_disp_0.disp_obj, 69, 22, img_sign_slow, !(handle->pos == 3));

	entry = &handle->entry_list[4];

	uint8_t temp = *entry->act % 3;

	switch(temp) {
		case 0:
			ssd1309_draw_img(&g_disp_0.disp_obj, 99, 22, img_sign_no_parking, !(handle->pos == 4));
			break;
		case 1:
			ssd1309_draw_img(&g_disp_0.disp_obj, 99, 22, img_sign_retoure, !(handle->pos == 4));
			break;
		case 2:
			ssd1309_draw_img(&g_disp_0.disp_obj, 99, 22, img_sign_blocking, !(handle->pos == 4));
			break;
	}

	ssd1309_rtos_unlock(&g_disp_0);

	handle->dirty = 0;
}

