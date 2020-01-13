/**
  ******************************************************************************
  * @file    serial.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2020-01-13
  * @brief   串口数据处理
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "serial.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ring_buffer rx_buffer = {{0},0,0};
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void serial_storeChar(unsigned char c)
{
  int i = (unsigned int)(rx_buffer.head + 1) % RX_BUFFER_SIZE;
  // if we should be storing the received character into the location  //如果我们应该存储的接收到的字符的位置刚好
  // just before the tail (meaning that the head would advance to the  //在尾端的前面（意味着头部将要进入尾端的
  // current location of the tail), we're about to overflow the buffer //当前位置），这样将会溢出缓冲区，因此我们
  // and so we don't write the character or advance the head.          //不该存入这个字符或使这个头前进
  if (i != rx_buffer.tail)  //缓冲区没有存满 
	{
    rx_buffer.buffer[rx_buffer.head] = c;
    rx_buffer.head = i;
  }
}

void serial_checkRx(void)  //检测接收到的数据
{ 
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收中断
	{ 
		unsigned char c = USART_ReceiveData(USART3);
		unsigned int i = (unsigned int)(rx_buffer.head + 1) % RX_BUFFER_SIZE;
		// if we should be storing the received character into the location
		// just before the tail (meaning that the head would advance to the
		// current location of the tail), we're about to overflow the buffer
		// and so we don't write the character or advance the head.
		if (i != rx_buffer.tail) 
		{
			rx_buffer.buffer[rx_buffer.head] = c;
			rx_buffer.head = i;
		}
	}
}

unsigned int serial_available(void)  //返回串口缓存区中数据的个数
{
   return (unsigned int)(RX_BUFFER_SIZE + rx_buffer.head - rx_buffer.tail) % RX_BUFFER_SIZE;
}

int serial_read(void)  //按存入顺序逐个读取缓冲区的数据
{ 
	uint8_t c;
  // if the head isn't ahead of the tail, we don't have any characters //如果头不是在尾的前面，将收不到任何字符
  if (rx_buffer.head == rx_buffer.tail) 
	{
    return -1;
  } 
	else 
	{
    c = rx_buffer.buffer[rx_buffer.tail];
    rx_buffer.tail = (unsigned int)(rx_buffer.tail + 1) % RX_BUFFER_SIZE;
    return c;
  }
}
void serial_flush(void)  //清空串口数据
{
  // RX
	// don't reverse this or there may be problems if the RX interrupt  //不要颠倒这个否则可能会有一些问题，如果接收中断
  // occurs after reading the value of rx_buffer_head but before writing  //发生在读取rx_buffer_head之后但在写入rx_buffer_tail之前
  // the value to rx_buffer_tail; the previous value of rx_buffer_head  //之前的rx_buffer_head值可能被写到rx_buffer_tail
  // may be written to rx_buffer_tail, making it appear as if the buffer  //使它呈现缓冲区是满的而非空的状态
  // were full, not empty.
  rx_buffer.head = rx_buffer.tail;
}		

/**
  * @brief  USART3串口中断服务函数
  * @param  none
  * @retval none
  */ 
void usart3_irq(void)
{
	uint8_t rec;
  
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		rec = USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
    serial_storeChar(rec);
  } 
} 

