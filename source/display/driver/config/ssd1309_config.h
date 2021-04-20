/*
 * ssd1309_config.h
 *
 *  Created on: 16.03.2020
 *      Author: s-akulin
 */

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
