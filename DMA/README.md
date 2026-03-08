3.8 串口DMA发送数据
     HAL_UART_Transmit_DMA(&huart1, buffer, length);
     注意：cubemx配置时需要开启串口全局中断，否则死机