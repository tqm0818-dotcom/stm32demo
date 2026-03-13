#include "bsp_encode.h"
#include "main.h"
#include "tim.h"
#include "oled/bsp_oledfont.h"
#include "oled/bsp_oled.h"
/*
    ******************************************************************************
    * @file           : bsp_encode.c
    * @brief          : This file provides code for the configuration
    *                   of the TIM instances to be used as Encoder.
*/


/**
  * @brief  Start Encoder.
  * @retval None
  */
void Encode_Start(void)
{
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
}


 uint32_t Encode_count(void)
 {
        uint32_t count = __HAL_TIM_GET_COUNTER(&htim1);
        return count;
        

 }
