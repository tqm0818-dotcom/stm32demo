#include "delay.h"
#include "main.h"

uint16_t g_fac_us = 0;


void delay_ms(uint32_t ms)
{
    uint32_t start = HAL_GetTick();
    while ((HAL_GetTick() - start) < ms);
}

void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 使能 DWT 访问
    DWT->CYCCNT = 0;                                 // 清零计数器
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;             // 使能 CYCCNT 计数
}

void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000); // 每微秒 CPU 周期数
    while ((DWT->CYCCNT - start) < ticks);
}
