#include "DMA.h"
#include "main.h"
#include "usart.h"
#include "dma.h"

extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_HandleTypeDef huart1;

void UART1_DMA_TX_SEND(uint8_t *buffer, uint16_t length)
{
    //等待上一次的数据发送完毕
	while(HAL_DMA_GetState(&hdma_usart1_tx) != HAL_DMA_STATE_READY);
    //while(__HAL_DMA_GET_COUNTER(&hdma_usart1_tx));
	
    //关闭DMA
    __HAL_DMA_DISABLE(&hdma_usart1_tx);

    //开始发送数据
    HAL_UART_Transmit_DMA(&huart1, buffer, length);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1) //如果是串口1
	{
		HAL_UART_DMAStop(&huart1);		//传输完成以后关闭串口DMA,缺了这一句会死机
	}
}


