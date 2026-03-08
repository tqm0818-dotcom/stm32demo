#include "DMA.h"
#include "main.h"
#include "usart.h"
#include "dma.h"

extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;
extern uint8_t aRxBuffer[64]; //接收缓冲区，大小根据需要调整

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

HAL_StatusTypeDef UART1_DMA_RX_RECEIVE(uint8_t *rxbuffer, uint16_t length)
{
    return HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxbuffer, length);
}

//接收和发送都开启时不要使用这个回调函数，否则会死机
// void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if (huart->Instance == USART1) //如果是串口1
// 	{
// 		HAL_UART_DMAStop(&huart1);		//传输完成以后关闭串口DMA,缺了这一句会死机
// 	}
// }

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1) //如果是串口1
    {
        //接收完成后的处理代码，例如处理接收到的数据
        //Size参数表示实际接收到的数据长度，可以根据需要进行处理
        UART1_DMA_TX_SEND(aRxBuffer, Size); //示例：将接收到的数据通过DMA发送回去
        //处理完成后可以再次启动接收以继续接收数据
        UART1_DMA_RX_RECEIVE(aRxBuffer, sizeof(aRxBuffer)); //重新启动接收
        
    }
}

