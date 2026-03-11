#include "bsp_oled.h"
#include "i2c.h"
#include "delay.h"
#include "main.h"

/**
 * @brief  OLED设备检查
 * @param  slave_addr OLED设备的I2C地址
 * @retval 成功返回SUCCESS，失败返回ERROR
 */
ErrorStatus OLED_CheckDevice(uint8_t slave_addr)
{
    uint8_t res = 0;
    if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, (slave_addr << 1), &res, 0, IIC_TIMEOUT))
    {
        return SUCCESS;
    }

     return ERROR;
}

/**
 * @brief  向OLED写入一个字节数据
 * @param  cmd 数据类型，OLED_CMD表示命令，OLED_DATA表示数据
 * @param  byte 要写入的数据字节
 * @retval 成功返回SUCCESS，失败返回ERROR
 */
ErrorStatus OLED_WriteByte(uint8_t cmd, uint8_t byte)
{
    uint8_t buf[2] = {cmd, byte};
    if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, (OLED_I2C_ADDR << 1), buf, 2, IIC_TIMEOUT))
    {
        return SUCCESS;
    }
    printf("oled write byte failed:cmd = 0x%02X , byte = 0x%02X\r\n", cmd, byte);
    return ERROR;
}

/**
 * @brief  向OLED写入数据缓冲区
 * @param  cmd 数据类型，OLED_CMD表示命令，OLED_DATA表示数据
 * @param  buffer 要写入的數據缓冲区
 * @param  num 要写入的数据字节数
 * @retval 成功返回SUCCESS，失败返回ERROR
 */
ErrorStatus OLED_WriteBuffer(uint8_t cmd, uint8_t* buffer, uint32_t num)
{
    uint8_t buf[256];
    if(num > sizeof(buf) - 1)
    {
        printf("oled write buffer failed: num = %lu is too large\r\n", num);
        return ERROR;
    }
    buf[0] = cmd;
    memcpy(&buf[1], buffer, num);
    if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, (OLED_I2C_ADDR << 1), buf, num + 1, IIC_TIMEOUT))
    {
        return SUCCESS;
    }
    printf("oled write buffer failed: num = %lu\r\n", num);
    return ERROR;
}

/**
 * @brief  设置OLED显示位置
 * @param  y OLED的页地址，范围0-7
 * @param  x OLED的列地址，范围0-127
 * @retval None
 */
void OLED_SetPos(uint8_t y, uint8_t x)
{
    uint8_t pos_buf[3]={
        0xB0 + y, //设置页地址
        ((x & 0xF0) >> 4) | 0x10, //设置列地址高4位
        (x & 0x0F)  //设置列地址
    };
    OLED_WriteBuffer(OLED_CMD, pos_buf, sizeof(pos_buf));
}

/**
 * @brief  OLED屏幕填充
 * @param  fill_data 要填充的数据字节，通常为0x00表示全黑，0xFF表示全白
 * @retval None
 */
void OLED_Fill(uint8_t fill_data)
{
    uint8_t data_buffer_temp[128] = {0};
    memset(data_buffer_temp, fill_data, 128); //填充数据
    for(uint8_t page = 0; page < 8; page++)
    {
        OLED_SetPos(page, 0); //设置页地址和列地址
        OLED_WriteBuffer(OLED_DATA, data_buffer_temp, sizeof(data_buffer_temp)); //写入数据
    }
}

/**
 * @brief  OLED屏幕清除
 * @retval None
 */
void OLED_CLear(void)
{
    OLED_Fill(0x00); //填充全黑
}

/**
 * @brief  OLED屏幕全白
 * @retval None
 */
void OLED_FillWhite(void)
{
    OLED_Fill(0xFF); //填充全白
}

void OLED_Init(void)
{
    delay_ms(100); //等待OLED上电稳定
    while(OLED_CheckDevice(OLED_I2C_ADDR) != SUCCESS);

    OLED_WriteByte(OLED_CMD, 0xAF); //打开显示

    OLED_WriteByte(OLED_CMD, 0x20); //设置内存寻址模式
    OLED_WriteByte(OLED_CMD, 0x02); //水平寻址模式

    OLED_WriteByte(OLED_CMD, 0xB0); //设置页地址
    
    //输出扫描方向
    OLED_WriteByte(OLED_CMD,0xA1); //设置左右方向，0xA0为正常，0xA1为左右反转
    OLED_WriteByte(OLED_CMD,0xC8); //设置上下方向，0xC8为正常，0xC0为上下反转

    //页内列起始地址
    OLED_WriteByte(OLED_CMD, 0x00); //设置列地址低4位
    OLED_WriteByte(OLED_CMD, 0x10); //设置列地址高4位

    //页内行起始地址
    OLED_WriteByte(OLED_CMD, 0x40); //设置行地址起始地址
    
    //设置对比度
    OLED_WriteByte(OLED_CMD, 0x01); //设置对比度命令
    OLED_WriteByte(OLED_CMD, 0x7F); //设置对比度值，范围0x00-0xFF

    //设置显示方向
    OLED_WriteByte(OLED_CMD,0xA1);
    OLED_WriteByte(OLED_CMD, 0xA6); //设置正常显示，0xA6为正常，0xA7为反相显示

    //设置多路复率
    OLED_WriteByte(OLED_CMD, 0xA8); //设置多路复
    OLED_WriteByte(OLED_CMD, 0x3F); //设置多路复率值，范围0x00-0xFF

    //全屏显示
    OLED_WriteByte(OLED_CMD, 0xA4); //设置全屏显示，0xA4为正常显示，0xA5为全屏点亮

    //设置显示偏移
    OLED_WriteByte(OLED_CMD, 0xD3); //设置显示偏移
    OLED_WriteByte(OLED_CMD, 0x00); //设置显示偏移值

    //设置时钟分频
    OLED_WriteByte(OLED_CMD, 0xD5); //设置时钟分频
    OLED_WriteByte(OLED_CMD, 0xf0); //设置分割比率，范围0x00-0xFF

    //设置预充电周期
    OLED_WriteByte(OLED_CMD, 0xD9); //设置预充电
    OLED_WriteByte(OLED_CMD, 0x22); //设置预充电周期值，范围0x00-0xFF

    //设置COM引脚配置
    OLED_WriteByte(OLED_CMD, 0xDA); //设置COM引脚配置
    OLED_WriteByte(OLED_CMD, 0x12); //设置COM引脚配置

    //设置VCOMH电压倍率
    OLED_WriteByte(OLED_CMD, 0xDB); //设置VCOMH电压倍率
    OLED_WriteByte(OLED_CMD, 0x20); //设置VCOMH电压倍率值，范围0x00-0xFF

    OLED_WriteByte(OLED_CMD, 0x8D); //设置DCDC使能
    OLED_WriteByte(OLED_CMD, 0x14); //设置DCDC使能值，0x00为关闭，0x14为开启

    //显示初始化完成后清屏
    OLED_WriteByte(OLED_CMD, 0xAF); //打开显示
    OLED_CLear(); //清屏
    
}
