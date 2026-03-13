#include "bsp_oled.h"
#include "bsp_oledfont.h"
#include "delay.h"
#include "main.h"
#include "font.h"
/**
 * @brief  OLED显示一个中文字符
 * @param  y 显示位置的页地址（0-7）
 * @param  x 显示位置的列地址（0-127）
 * @param  n 要显示的中文字符在字库中的索引
 * @param  data_cn 中文字符字库数据，格式为16x16点阵，每个字符占32字节
 * @retval None
 */
void OLED_ShowChinese(uint8_t y, uint8_t x,uint8_t n,const uint8_t data_cn[][16])
{
    OLED_SetPos(y, x); //设置显示位置
    for(uint8_t i = 0; i < 16; i++)
    {
        OLED_WriteByte(OLED_DATA, data_cn[n * 2][i]); //写入中文字符数据
    }
    OLED_SetPos(y + 1, x); //设置下一行显示位置
    for(uint8_t i = 0; i < 16; i++)
    {
        OLED_WriteByte(OLED_DATA, data_cn[n * 2 + 1][i]); //写入中文字符数据
    }

}

/**
 * @brief  OLED显示一个16x16的中文字符
 * @param  line 显示行号，范围0-3，每行显示两个16x16字符
 * @param  offset 显示列偏移，范围0-7，每个偏移显示一个16x16字符
 * @param  n 要显示的中文字符在字库中的索引
 * @retval None
 */
void OLED_ShowChinese_16x16(uint8_t line, uint8_t offset,uint8_t n)
{
    OLED_ShowChinese(line *2, offset *16, n, chinese_library_16x16);
}

/**
 * @brief  OLED显示一个英文字符
 * @param  y 显示位置的页地址（0-7）
 * @param  x 显示位置的列地址（0-127）
 * @param  chardata 要显示的英文字符ASCII码
 * @param  textsize 字体大小，6表示6x8点阵，8表示8x16点阵
 * @retval None
 */
void OLED_ShowChar(uint8_t y, uint8_t x, uint8_t char_data, uint8_t textsize)
{
    uint32_t addr = char_data - 32;  // 计算字符在点阵数组中的索引

    switch(textsize)
    {
        case textsize_6x8:   // 6x8点阵字体，宽6像素，高8像素
            OLED_SetPos(y, x);  // 设置光标位置
            for(uint8_t i = 0; i < 6; i++)  // 依次写入6字节点阵数据
            {
                OLED_WriteByte(OLED_DATA, (uint8_t)ascii_code_6x8[addr][i]);
            }
            break;

        case textsize_8x16:  // 8x16点阵字体，宽8像素，高16像素
            OLED_SetPos(y, x);  // 设置光标位置显示上半部分
            for(uint8_t i = 0; i < 8; i++)  // 写入8字节点阵数据（上半部分）
            {
                OLED_WriteByte(OLED_DATA, (uint8_t)ascii_code_8x16[addr][i]);
            }
            OLED_SetPos(y + 1, x);  // 设置下一页光标显示下半部分
            for(uint8_t i = 0; i < 8; i++)  // 写入剩余8字节点阵数据（下半部分）
            {
                OLED_WriteByte(OLED_DATA, (uint8_t)ascii_code_8x16[addr][i + 8]);
            }
            break;

        default:
            break;
    }
}
/**
 * @brief  OLED显示一个字符串
 * @param  y 显示位置的页地址（0-7）
 * @param  x 显示位置的列地址（0-127）
 * @param  str 要显示的字符串，以'\0'结尾
 * @param  textsize 字体大小，6表示6x8点阵，8表示8x16点阵
 * @retval None
 */
void OLED_ShowString(uint8_t y, uint8_t x, uint8_t *str, uint8_t textsize)
{
    for (uint8_t i = 0; *str != '\0'; i++)
    {
        OLED_ShowChar(y, x + i * textsize, *str++, textsize);
    }
}

/**
 * @brief  OLED显示一个字符串，使用8x16点阵字体
 * @param  line 显示行号，范围0-3，每行显示两个16x16字符
 * @param  offset 显示列偏移，范围0-7，每个偏移显示一个16x16字符
 * @param  str 要显示的字符串，以'\0'结尾
 * @retval None
 */
void OLED_ShowString_8x16(uint8_t line, uint8_t offset, uint8_t *str)
{
    OLED_ShowString(line * 2, offset * 8, str, textsize_8x16);
}
