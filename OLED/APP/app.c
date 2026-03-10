#include "bsp_oledfont.h"
#include "delay.h"
#include "main.h"
#include "bsp_oled.h"
#include "app.h"
void OLED_TASK(void)
{
    OLED_ShowString(3,30,"hello world",8);
    delay_ms(1000);
    OLED_CLear();

    for(int i = 0; i < 10; i++)
    {
        OLED_ShowChar(0,0,i+'0',8);
        delay_ms(1000);
        OLED_CLear();
    }

}
