#include "bsp_W25Q128.h"
#include "spi.h"


/**
 * @brief  读取W25Q128的制造商和设备ID
 * @retval 16位ID值，成功返回ID，失败返回0
 */
uint16_t W25Q128_ReadID(void)
{
    uint8_t tx_buf[6] = {0};
    uint8_t rx_buf[6] = {0};
    uint16_t ID = 0;
    tx_buf[0] = W25Q128_MANUFACTURER_DEVICE_ID;
    tx_buf[1] = (uint8_t)(W25Q128_MANUFACTDEVICEID_ADDRESS >> 16 & 0xFF) ;
    tx_buf[2] = (uint8_t)(W25Q128_MANUFACTDEVICEID_ADDRESS >> 8 & 0xFF);
    tx_buf[3] = (uint8_t)(W25Q128_MANUFACTDEVICEID_ADDRESS & 0xFF);
    tx_buf[4] = 0x00;
    tx_buf[5] = 0x00;

    CS_LOW();
   if( HAL_SPI_TransmitReceive(&hspi2, tx_buf, rx_buf, 6, HAL_MAX_DELAY) != HAL_OK)
    {
        CS_HIGH();
        printf("W25Q128_ReadID error\r\n");
        return 0;
    }
   CS_HIGH();  
   ID = ((uint16_t)rx_buf[4] << 8) | rx_buf[5];
    return ID;
}

/**
 * @brief  读取W25Q128的JEDEC ID
 * @retval 24位ID值，成功返回ID，失败返回0
 */
uint32_t W25Q128_ReadJEDECID(void)
{
    uint8_t tx_buf[4] = {W25Q128_JEDEC_ID, 0x00, 0x00, 0x00};
    uint8_t rx_buf[4] = {0};
    uint32_t ID = 0;

    CS_LOW();
    if( HAL_SPI_TransmitReceive(&hspi2, tx_buf, rx_buf, 4, HAL_MAX_DELAY) != HAL_OK)
    {
        CS_HIGH();
        printf("W25Q128_ReadJEDECID error\r\n");
        return 0;
    }
   CS_HIGH(); 
   ID = ((uint32_t)rx_buf[1] << 16) | ((uint32_t)rx_buf[2] << 8) | rx_buf[3];
    return ID;
}

void W25Q128_WriteEnable(void)
{
    uint8_t cmd = W25Q128_WRITE_ENABLE;
    CS_LOW();
    if( HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        printf("W25Q128_WriteEnable error\r\n");
        
    }
   CS_HIGH(); 
}

/**
 * @brief  等待W25Q128完成写入操作
 * @retval 无
 */
void W25Q128_WaitForWriteEnd(void)
{
    uint8_t cmd = W25Q128_READ_STATUS_REGISTER_1;
    uint8_t status = 0;

    CS_LOW();
    if( HAL_SPI_TransmitReceive(&hspi2, &cmd, &status, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        printf("W25Q128 Send read status command error!!\r\n");
    }
    do
    {
        if( HAL_SPI_Receive(&hspi2, &status, 1, HAL_MAX_DELAY) != HAL_OK)
        {
            
            printf("W25Q128 receive status error!!\r\n");
            
        }
    } while ((status & WIP_FLAG) == SET);
    CS_HIGH();
}


/**
 * @brief  使W25Q128进入掉电模式
 * @retval 无
 */
void W25Q128_PowerDown(void)
{
    uint8_t cmd = W25Q128_POWER_DOWN;
    CS_LOW();
    if( HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        printf("W25Q128_PowerDown error\r\n");
        
    }
   CS_HIGH(); 
}

/**
 * @brief  唤醒W25Q128，使其从掉电模式恢复
 * @retval 无
 */
void W25Q128_WakeUp(void)
{
    uint8_t cmd = W25Q128_RELEASE_POWER_DOWN_HPM_DEVICE_ID;

    CS_LOW();
    if( HAL_SPI_Transmit(&hspi2, &cmd,  1, HAL_MAX_DELAY) != HAL_OK)
    {
        printf("W25Q128_WakeUp Send command error\r\n");
        
    }
   CS_HIGH(); 
}

/**
 * @brief  擦除W25Q128的全部数据
 * @retval 无
 */
void W25Q128_BulkErase(void)
{
    uint8_t cmd = W25Q128_CHIP_ERASE;
    W25Q128_WriteEnable();

    CS_LOW();
    if( HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        printf("W25Q128 BulkErase commend error\r\n");
        
    }
   CS_HIGH(); 
   W25Q128_WaitForWriteEnd();
}

/**
 * @brief  擦除W25Q128的指定扇区
 * @param  SectorAddr: 扇区地址，必须是4KB对齐的地址
 * @retval 无
 */
void W25Q128_SectorErase(uint32_t SectorAddr)
{
    uint8_t tx_buf[4] = {0};
    tx_buf[0] = W25Q128_SECTOR_ERASE_4KB;
    tx_buf[1] = (uint8_t)(SectorAddr >> 16 & 0xFF);
    tx_buf[2] = (uint8_t)(SectorAddr >> 8 & 0xFF);
    tx_buf[3] = (uint8_t)(SectorAddr & 0xFF);

    W25Q128_WriteEnable();

    CS_LOW();
    if( HAL_SPI_Transmit(&hspi2, tx_buf, 4, HAL_MAX_DELAY) != HAL_OK)
    {
        printf("W25Q128 EraseSector commend error\r\n");
        
    }
   CS_HIGH(); 
   W25Q128_WaitForWriteEnd();
}


/**
 * @brief  向W25Q128的指定地址写入数据，写入前必须确保目标区域已擦除
 * @param  WriteAddr: 写入地址，必须是页起始地址
 * @param  pBuffer: 指向要写入数据的缓冲区指针
 * @param  size: 要写入的数据大小，单位字节，必须不超过页大小
 * @retval 无
 */
void W25Q128_WritePage( uint32_t WriteAddr,uint8_t* pBuffer, uint16_t size)
{
    if(size +(WriteAddr % W25Q128_SPI_FLASH_PAGESIZE) > W25Q128_SPI_FLASH_PAGESIZE)
    {
        printf("W25Q128 WritePage error: size exceeds page boundary\r\n");
        
    }
    
    uint8_t tx_buf[4] = {0};
    tx_buf[0] = W25Q128_PAGE_PROGRAM;
    tx_buf[1] = (uint8_t)(WriteAddr >> 16 & 0xFF);
    tx_buf[2] = (uint8_t)(WriteAddr >> 8 & 0xFF);
    tx_buf[3] = (uint8_t)(WriteAddr & 0xFF);

    W25Q128_WriteEnable();
    CS_LOW();
    if( HAL_SPI_Transmit(&hspi2, tx_buf, 4, HAL_MAX_DELAY) != HAL_OK)
    {
        printf("W25Q128 WritePage Send command error\r\n");
        
        
    }
    if( HAL_SPI_Transmit(&hspi2, pBuffer, size, HAL_MAX_DELAY) != HAL_OK)
    {
        printf("W25Q128 WritePage Send data error\r\n");
        
    }
    CS_HIGH();
    W25Q128_WaitForWriteEnd();

}

/**
 * @brief  向W25Q128的指定地址连续写入数据，写入前必须确保目标区域已擦除
 * @param  WriteAddr: 写入地址，可以是任意地址
 * @param  pBuffer: 指向要写入数据的缓冲区指针
 * @param  size: 要写入的数据大小，单位字节
 * @retval 无
 */
void W25Q128_WriteBuffer( uint32_t WriteAddr,uint8_t* pBuffer, uint16_t size)
{
    uint32_t write_num = size;
    uint32_t current_addr = WriteAddr;
    uint32_t offset = WriteAddr % W25Q128_SPI_FLASH_PAGESIZE;

    if(offset !=0)
    {
        uint32_t page_remain = W25Q128_SPI_FLASH_PAGESIZE - offset;
        if(write_num > page_remain)
        {
            W25Q128_WritePage(current_addr, pBuffer, page_remain);
            current_addr += page_remain;
            pBuffer += page_remain;
            write_num -= page_remain;
        }
        else
        {
            W25Q128_WritePage(current_addr, pBuffer, write_num);
            return;
        }

    }
    while(write_num > 0)
    {
        uint16_t write_size = (write_num > W25Q128_SPI_FLASH_PAGESIZE) ? W25Q128_SPI_FLASH_PAGESIZE : write_num;
        W25Q128_WritePage(current_addr, pBuffer, write_size);
        current_addr += write_size;
        pBuffer += write_size;
        write_num -= write_size;
    }
}

/**
 * @brief  从W25Q128的指定地址读取数据
 * @param  ReadAddr: 读取地址，可以是任意地址
 * @param  pBuffer: 指向存储读取数据的缓冲区指针
 * @param  size: 要读取的数据大小，单位字节
 * @retval 无
 */
void W25Q128_ReadBuffer(uint32_t ReadAddr, uint8_t* pBuffer, uint16_t size)
{
    uint8_t tx_buf[4] = {0};
    tx_buf[0] = W25Q128_READ_DATA;
    tx_buf[1] = (uint8_t)(ReadAddr >> 16 & 0xFF);
    tx_buf[2] = (uint8_t)(ReadAddr >> 8 & 0xFF);
    tx_buf[3] = (uint8_t)(ReadAddr & 0xFF);

    CS_LOW();
    if( HAL_SPI_Transmit(&hspi2, tx_buf, 4, HAL_MAX_DELAY) != HAL_OK)
    {
        printf("W25Q128 ReadBuffer Send command error\r\n");
        
    }
    if( HAL_SPI_Receive(&hspi2, pBuffer, size, HAL_MAX_DELAY) != HAL_OK)
    {
        printf("W25Q128 ReadBuffer Receive data error\r\n");
        
    }
   CS_HIGH(); 
}
