#include "i2c.h"
#include "bsp-MPU6050.h"
#include "delay.h"
#include "led.h"

 uint8_t mpu6050_int_flag = 0; // Flag to indicate MPU6050 interrupt occurrence

/*
 * Function: MPU6050_WriteReg
 * Description: Write a byte to a specific register of the MPU6050
 * Parameters:
 *   reg: The register address to write to
 *   byte: The byte value to write
 * Return: HAL_StatusTypeDef indicating the success or failure of the operation
 */
HAL_StatusTypeDef MPU6050_WriteReg(uint8_t reg, uint8_t byte)
{
    uint8_t data[2];
    data[0] = reg;
    data[1] = byte;
    if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, MPU6050_AD0D_SLAVER_ARRD << 1, data, 2, 1000))
    {
        return HAL_OK;
    }
    {}
    return HAL_ERROR;
}

/*
 * Function: MPU6050_ReadReg
 * Description: Read a byte from a specific register of the MPU6050
 * Parameters:
 *   reg: The register address to read from
 * Return: The byte value read from the register, or 0 if an error occurred
 */
uint8_t MPU6050_ReadReg(uint8_t reg)
{
    uint8_t data=0;
    if(HAL_OK != HAL_I2C_Master_Transmit(&hi2c1, MPU6050_AD0D_SLAVER_ARRD << 1, &reg, 1, 1000))
    {
        return 0;
    }
    if(HAL_OK != HAL_I2C_Master_Receive(&hi2c1, MPU6050_AD0D_SLAVER_ARRD << 1, &data, 1, 1000))
    {
        return 0;
    }
    return data;
}

/*
 * Function: MPU6050_ReadBuffer
 * Description: Read multiple bytes from a specific register of the MPU6050
 * Parameters:
 *   reg: The register address to read from
 *   buffer: Pointer to the buffer where the read data will be stored
 *   length: The number of bytes to read
 * Return: HAL_StatusTypeDef indicating the success or failure of the operation
 */
HAL_StatusTypeDef MPU6050_ReadBuffer(uint8_t reg, uint8_t *buffer, uint16_t length)
{
    if(HAL_OK != HAL_I2C_Master_Transmit(&hi2c1, MPU6050_AD0D_SLAVER_ARRD << 1, &reg, 1, 1000))
    {
        return HAL_ERROR;
    }
    if(HAL_OK != HAL_I2C_Master_Receive(&hi2c1, MPU6050_AD0D_SLAVER_ARRD << 1, buffer, length, 1000))
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

/*
 * Function: MPU6050_WriteBuffer
 * Description: Write multiple bytes to a specific register of the MPU6050
 * Parameters:
 *   reg: The register address to write to
 *   buffer: Pointer to the buffer containing the data to be written
 *   length: The number of bytes to write
 * Return: HAL_StatusTypeDef indicating the success or failure of the operation
 */
HAL_StatusTypeDef MPU6050_WriteBuffer(uint8_t reg, uint8_t *buffer, uint16_t length)
{
    uint8_t data[length+1];
    if(data == NULL)
    {
        return HAL_ERROR;
    }
    data[0] = reg;
    memcpy(&data[1], buffer, length);
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, MPU6050_AD0D_SLAVER_ARRD << 1, data, length+1, 1000);
    free(data); 
    return status; 
}

/*
 * Function: MPU6050_ReadID
 * Description: Read the device ID from the MPU6050
 * Return: The device ID read from the MPU6050, or 0 if an error occurred
 */
uint8_t MPU6050_ReadID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I );
}

/*
 * Function: MPU6050_ClearInterrupt
 * Description: Clear the interrupt status of the MPU6050 by reading the interrupt status register
 */
void MPU6050_ClearInterrupt(void)
{
    uint8_t status = MPU6050_ReadReg(MPU6050_RA_INT_STATUS);
} 

/*
 * Function: MPU6050_Init
 * Description: Initialize the MPU6050 sensor with default settings
 * This function checks if the MPU6050 is connected and then configures it for operation.
 */
void MPU6050_Init(void)
{
    delay_ms(1000);

    while (MPU6050_ReadID() != MPU6050_ID )
    {
        printf("MPU6050 not found, please check the connection.\r\n");
        delay_ms(500);
    }
    printf("MPU6050 found successfully.\r\n");
    MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);    // 解除休眠
    MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV, 0x07);    // 设置采样率为1kHz/(7+1)=125Hz
    MPU6050_WriteReg(MPU6050_RA_CONFIG, 0x06);        // 配置低通滤波，带宽5Hz
    MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG, 0x00);  // 加速度计2G模式
    MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);   // 陀螺仪±2000°/s
	MPU6050_WriteReg(MPU6050_RA_INT_ENABLE, 0x01); 		// 使能数据准备好中断
	MPU6050_WriteReg(MPU6050_RA_INT_PIN_CFG, 0xA0);		// 配置中断引脚：低电平有效，推挽输出，保持模式，读取中断清除
    printf("MPU6050 initialization complete.\r\n");
    printf("MPU6050_INT_FLAG: %d\r\n", mpu6050_int_flag);
}

/*
 * Function: MPU6050_GetAcc
 * Description: Read the accelerometer data from the MPU6050 and store it in the provided buffer
 * Parameters:
 *   accData: Pointer to a buffer where the accelerometer data will be stored (3 elements for X, Y, Z)
 */
void MPU6050_GetAcc(short *accData)
{
    uint8_t buffer[6];
    if (MPU6050_ReadBuffer(MPU6050_ACC_OUT, buffer, 6) == HAL_OK)
    {
        accData[0] = (buffer[0] << 8) | buffer[1]; // X-axis
        accData[1] = (buffer[2] << 8) | buffer[3]; // Y-axis
        accData[2] = (buffer[4] << 8) | buffer[5]; // Z-axis
    }
}

/*
 * Function: MPU6050_GetGyro
 * Description: Read the gyroscope data from the MPU6050 and store it in the provided buffer
 * Parameters:
 *   gyroData: Pointer to a buffer where the gyroscope data will be stored (3 elements for X, Y, Z)
 */
void MPU6050_GetGyro(short *gyroData)
{
    uint8_t buffer[6];
    if (MPU6050_ReadBuffer(MPU6050_GYRO_OUT, buffer, 6) == HAL_OK)
    {
        gyroData[0] = (buffer[0] << 8) | buffer[1]; // X-axis
        gyroData[1] = (buffer[2] << 8) | buffer[3]; // Y-axis
        gyroData[2] = (buffer[4] << 8) | buffer[5]; // Z-axis
    }
}

void MPU6050_GetTemp(short *tempData)
{
    uint8_t buffer[2];
    if (MPU6050_ReadBuffer(MPU6050_RA_TEMP_OUT_H, buffer, 2) == HAL_OK)
    {
        *tempData = (buffer[0] << 8) | buffer[1]; // Temperature
    }
}

void MPU6050_ReturnTemp(float *tempData)
{
    short rawTemp;
    MPU6050_GetTemp(&rawTemp);
    *tempData = (float)rawTemp / 340.0f + 36.53f; // Convert raw temperature to degrees Celsius
}

/*
 * Function: HAL_GPIO_EXTI_Callback
 * Description: Callback function for handling GPIO external interrupts
 * Parameters:
 *   GPIO_Pin: The pin number that triggered the interrupt
 * This function checks if the interrupt was triggered by the MPU6050 interrupt pin and sets a flag accordingly.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == MPU6050_INT_PIN) // Assuming the interrupt pin is connected to GPIO_PIN_0
    {
        mpu6050_int_flag = 1; // Set the interrupt flag when the MPU6050 interrupt occurs
        HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_5); // Toggle an LED to indicate the interrupt (optional)
    }
}


