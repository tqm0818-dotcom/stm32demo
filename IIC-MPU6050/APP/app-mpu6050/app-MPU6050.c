#include "bsp-mpu6050/bsp-MPU6050.h"
#include "delay.h"
#include "app-MPU6050.h"
void MPU6050_Task(void)
{
    if(mpu6050_int_flag)
    {
       
    
        mpu6050_int_flag = 0; // Clear the software interrupt flag atomically
    
        // Handle MPU6050 interrupt (e.g., read sensor data, update status, etc.)
        short accdata[3]={0};
        short gyrodata[3]={0};
        short rawtempdata=0;
        float tempdata=0.0f;
        MPU6050_GetAcc(accdata);
        MPU6050_GetGyro(gyrodata);
        MPU6050_GetTemp(&rawtempdata);
        MPU6050_ReturnTemp(&tempdata);

        printf("Accelerometer: X=%d, Y=%d, Z=%d\r\n", accdata[0], accdata[1], accdata[2]);
        printf("Gyroscope: X=%d, Y=%d, Z=%d\r\n", gyrodata[0], gyrodata[1], gyrodata[2]);
        printf("Temperature: %.2f\r\n", tempdata);
        printf("MPU6050_INT_FLAG: %d\r\n", mpu6050_int_flag);
        MPU6050_ClearInterrupt(); // First clear device interrupt to release latched INT pin
    }
}
