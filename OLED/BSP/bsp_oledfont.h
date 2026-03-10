#ifndef __BSP_OLED_H__
#define __BSP_OLED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#define textsize_6x8 6
#define textsize_8x16 8
#define textsize_16x16 16

void OLED_ShowChinese(uint8_t y, uint8_t x,uint8_t n,const uint8_t data_cn[][16]);
void OLED_ShowChinese_16x16(uint8_t line, uint8_t offset,uint8_t n);
void OLED_ShowChar(uint8_t y, uint8_t x, uint8_t chardata, uint8_t textsize);
void OLED_ShowString(uint8_t y, uint8_t x, uint8_t *str, uint8_t textsize);
void OLED_ShowString_8x16(uint8_t line, uint8_t offset, uint8_t *str);


#ifdef __cplusplus
}
#endif
#endif
