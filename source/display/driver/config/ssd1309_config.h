/************************************************************************************************************
 ************************************************************************************************************
 ** Filename: 		ssd1309_config.h				################
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
 ** 1	|	MS		|04-23-2021	| imported ssd1309_config.h														|
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
 ** Contains function for bit banding (toggle bit in RAM)
 **
 ** LCD_SCL (LCD Clock) at Pin P[3][24] (J9 Pin1) Clock Signal with 400kHz
 ** LCD_SDA (LCD Serial Data) at Pin P[3][23] (J Pin3) I2C serial data
 ************************************************************************************************************
 ***********************************************************************************************************/
#ifndef SSD1309_SSD1309_CONFIG_H_
#define SSD1309_SSD1309_CONFIG_H_

// If device supports bitbanding it is recommend to enable it.
//#define USE_BITBAND

#ifdef USE_BITBAND
#define SRAM_U_BITBAND_REGADDR(Reg,Bit) (0x22000000u + (32u*((uint32_t)&(Reg) - (uint32_t)0x20000000u)) + (4u*((uint32_t)(Bit))))
#define SRAM_U_BITBAND_REG32(Reg,Bit) (*((uint32_t volatile*)(SRAM_U_BITBAND_REGADDR((Reg),(Bit)))))
#define SRAM_U_BITBAND_REG(Reg,Bit) (SRAM_U_BITBAND_REG32((Reg),(Bit)))

#define BITBAND_ACCESS(reg,bit,val) (SRAM_U_BITBAND_REG(reg,bit)=val)

#define BIT_ACCESS(reg,bit,val) BITBAND_ACCESS(reg,bit,val)
#else
#define BIT_ACCESS(reg,bit,val) (reg=(val?reg|(1<<bit):reg&~(1<<bit)))
#endif

#endif /* SSD1309_SSD1309_CONFIG_H_ */
