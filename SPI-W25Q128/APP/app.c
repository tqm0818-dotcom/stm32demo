#include "bsp_oledfont.h"
#include "delay.h"
#include "main.h"
#include "bsp_oled.h"
#include "app.h"
#include "bsp_W25Q128.h"
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

void W25Q128_TASK(void)
{
    W25Q128_BulkErase();
    uint16_t id = W25Q128_ReadID();
    uint32_t jedec_id = W25Q128_ReadJEDECID();
    printf("W25Q128 ID: 0x%04X\r\n", id);
    printf("W25Q128 JEDEC ID: 0x%06X\r\n", jedec_id);
    OLED_ShowString(0,0,"W25Q128 ID:",8);
    OLED_ShowChar(0,80,(id >> 8) & 0xFF,8);
    OLED_ShowChar(0,88,id & 0xFF,8);

    OLED_ShowString(2,0,"JEDEC ID:",8);
    OLED_ShowChar(2,80,(jedec_id >> 16) & 0xFF,8);
    OLED_ShowChar(2,88,(jedec_id >> 8) & 0xFF,8);
    OLED_ShowChar(2,96,jedec_id & 0xFF,8);

    uint8_t write_data[15] = "Hello W25Q128";
    W25Q128_WriteBuffer(0x00,write_data, 15);
    uint8_t read_data[15] = {0};
    W25Q128_ReadBuffer(0x00, read_data, 15);
    printf("Read data from W25Q128: %s\r\n", read_data);
    OLED_ShowString(4,0,"Read:",8);
    OLED_ShowString(4,40,read_data,8);


}
