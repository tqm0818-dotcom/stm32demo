#include "key.h"
#include "main.h"

// 按键引脚定义
#define KEY_GPIO_PORT    GPIOE
#define KEY_GPIO_PIN     GPIO_PIN_4

// 按键状态宏定义
#define KEY_PRESSED      0   // 上拉按键按下时为低电平
#define KEY_RELEASED     1   // 松开时为高电平

/**
 * @brief  按键消抖检测函数
 * @param  mode: 0-只检测一次按下（按一次返回一次）；1-持续检测（按住一直返回按下）
 * @retval uint8_t: 1-按键按下；0-按键未按下
 */
uint8_t KEY_Scan(uint8_t mode)
{
    static uint8_t key_flag = 1; // 按键松开标志（静态变量，保持上次状态）
    uint8_t key_state = 0;

    // 读取按键当前状态
    if(HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_GPIO_PIN) == KEY_PRESSED)
    {
        HAL_Delay(20); // 消抖延时20ms（过滤机械抖动）
        // 再次确认按键确实按下
        if(HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_GPIO_PIN) == KEY_PRESSED)
        {
            // mode=0：按一次只返回一次按下（松手后才能再次触发）
            if(mode == 0 && key_flag == 1)
            {
                key_flag = 0;
                key_state = 1;
            }
            // mode=1：持续按下持续返回
            else if(mode == 1)
            {
                key_state = 1;
            }
        }
    }
    // 按键松开，重置标志
    else if(HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_GPIO_PIN) == KEY_RELEASED)
    {
        key_flag = 1;
    }

    return key_state;
}
