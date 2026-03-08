3.8 串口DMA发送数据  
     HAL_UART_Transmit_DMA(&huart1, buffer, length);  
     注意：cubemx配置时需要开启串口全局中断，否则死机  
     串口DMA接收数据（采用空闲中断+DMA）  
     HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxbuffer, length);  
     注意：回调函数void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)  
     在处理完相关业务逻辑后要再次开启DMA接收  