#ifndef __OLED_H
#define __OLED_H
#define OLED_I2C_ADDR    (0x3C)
#define IIC_TIMEOUT       100
#define OLED_CMD         (0x00)
#define OLED_DATA        (0x40)
#define OLED_APPAY_SIZE(A) (sizeof(A) / sizeof(A[0]))  
#include "stm32f1xx_hal.h"
ErrorStatus OLED_CheckDevice(uint8_t slave_addr);
ErrorStatus OLED_WriteByte(uint8_t cmd, uint8_t byte);
ErrorStatus OLED_WriteBuffer(uint8_t cmd, uint8_t* buffer, uint32_t num);
void OLED_Fill(uint8_t fill_data);
void OLED_CLear(void);
void OLED_FillWhite(void);
void OLED_Init(void);
void OLED_SetPos(uint8_t y, uint8_t x);
#endif
