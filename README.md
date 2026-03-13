3.8（1）串口DMA发送数据
     HAL_UART_Transmit_DMA(&huart1, buffer, length);
     注意：cubemx配置时需要开启串口全局中断，否则死机
     串口DMA+空闲中断接收数据    
     HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxbuffer, length)    
     在用户回调函数void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)    
     中处理完业务逻辑后需要再次打开接收  
    （2）IIC通信
     硬件初始化哪些由cubemx配置  
     需要调用HAL_I2C_Master_Transmit(&hi2c1, (slave_addr << 1), &res, 0, IIC_TIMEOUT))    
     来发送定义函数用于发送数据  
3.13 定时器编码器计数  
     


     