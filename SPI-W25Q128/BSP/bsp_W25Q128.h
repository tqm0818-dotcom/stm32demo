#ifndef __BSP_W25Q128_H__
#define __BSP_W25Q128_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
	
#define CS_LOW()  HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET)
	

#define WFLASH_ID               0xEF4018                       // W25Q128芯片ID

#define W25Q128_SPI_FLASH_PAGESIZE       256                     // W25Q128页大小，单位字节
#define W25Q128_SPI_FLASH_SIZE           16777216U  // W25Q128总大小，单位字节

#define W25Q128_MANUFACTDEVICEID_ADDRESS 0x000000                 // 芯片制造商及设备ID地址
#define W25Q128_DUMMY_BYTE               0xFF                      // 通信中的无效字节

#define WIP_FLAG                       0x01                      // 写入中标志，FLASH内部正忙于写入


/********************************W25Q128命令定义*******************************/

#define W25Q128_WRITE_ENABLE                     0x06    // 写使能指令
#define W25Q128_WRITE_DISABLE                    0x04    // 写禁止指令
#define W25Q128_READ_STATUS_REGISTER_1           0x05    // 读状态寄存器1
#define W25Q128_READ_STATUS_REGISTER_2           0x35    // 读状态寄存器2
#define W25Q128_WRITE_STATUS_REGISTER            0x01    // 写状态寄存器
#define W25Q128_PAGE_PROGRAM                     0x02    // 页面编程指令
#define W25Q128_QUAD_PAGE_PROGRAM                0x32    // 四线页面编程
#define W25Q128_BLOCK_ERASE_64KB                 0xD8    // 擦除64KB块
#define W25Q128_BLOCK_ERASE_32KB                 0x52    // 擦除32KB块
#define W25Q128_SECTOR_ERASE_4KB                 0x20    // 擦除4KB扇区
#define W25Q128_CHIP_ERASE                       0xC7    // 全片擦除
#define W25Q128_ERASE_SUSPEND                    0x75    // 擦除暂停
#define W25Q128_ERASE_RESUME                     0x7A    // 擦除恢复
#define W25Q128_POWER_DOWN                       0xB9    // 进入掉电模式
#define W25Q128_HIGH_PERFORMANCE_MODE            0xA3    // 高性能模式
#define W25Q128_CONTINUOUS_READ_MODE_RESET       0xFF    // 连续读取模式复位
#define W25Q128_RELEASE_POWER_DOWN_HPM_DEVICE_ID 0xAB    // 释放掉电/高性能模式，并读取设备ID
#define W25Q128_MANUFACTURER_DEVICE_ID           0x90    // 读取制造商和设备ID
#define W25Q128_READ_UNIQUE_ID                   0x4B    // 读取唯一ID
#define W25Q128_JEDEC_ID                         0x9F    // 读取JEDEC ID
#define W25Q128_READ_DATA                        0x03    // 读取数据指令
#define W25Q128_FAST_READ                        0x0B    // 快速读取指令
#define W25Q128_FAST_READ_DUAL_OUTPUT            0x3B    // 双线快速读取输出
#define W25Q128_FAST_READ_DUAL_IO                0xBB    // 双线快速读取IO
#define W25Q128_FAST_READ_QUAD_OUTPUT            0x6B    // 四线快速读取输出
#define W25Q128_FAST_READ_QUAD_IO                0xEB    // 四线快速读取IO
#define W25Q128_OCTAL_WORD_READ_QUAD_IO          0xE3    // 八线字快速读取四线IO


uint16_t W25Q128_ReadID(void);
uint32_t W25Q128_ReadJEDECID(void);
void W25Q128_WriteEnable(void);
void W25Q128_WaitForWriteEnd(void);
void W25Q128_PowerDown(void);
void W25Q128_WakeUp(void);
void W25Q128_BulkErase(void);
void W25Q128_SectorErase(uint32_t SectorAddr);
void W25Q128_WritePage( uint32_t WriteAddr,uint8_t* pBuffer, uint16_t size);
void W25Q128_WriteBuffer( uint32_t WriteAddr,uint8_t* pBuffer, uint16_t size);
void W25Q128_ReadBuffer(uint32_t ReadAddr, uint8_t* pBuffer, uint16_t size);

#ifdef __cplusplus
}
#endif
#endif
