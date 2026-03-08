#ifndef __USERDMA_H__
#define __USERDMA_H__

#include "stm32f1xx_hal.h"

void UART1_DMA_TX_SEND(uint8_t *buffer, uint16_t length);

#endif
