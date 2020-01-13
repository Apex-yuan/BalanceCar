/**
  ******************************************************************************
  * @file    serial.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2020-01-13
  * @brief   �������ݴ���
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
  // if we should be storing the received character into the location  //�������Ӧ�ô洢�Ľ��յ����ַ���λ�øպ�
  // just before the tail (meaning that the head would advance to the  //��β�˵�ǰ�棨��ζ��ͷ����Ҫ����β�˵�
  // current location of the tail), we're about to overflow the buffer //��ǰλ�ã�����������������������������
  // and so we don't write the character or advance the head.          //���ô�������ַ���ʹ���ͷǰ��
  if (i != rx_buffer.tail)  //������û�д��� 
	{
    rx_buffer.buffer[rx_buffer.head] = c;
    rx_buffer.head = i;
  }
}

void serial_checkRx(void)  //�����յ�������
{ 
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //�����ж�
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

unsigned int serial_available(void)  //���ش��ڻ����������ݵĸ���
{
   return (unsigned int)(RX_BUFFER_SIZE + rx_buffer.head - rx_buffer.tail) % RX_BUFFER_SIZE;
}

int serial_read(void)  //������˳�������ȡ������������
{ 
	uint8_t c;
  // if the head isn't ahead of the tail, we don't have any characters //���ͷ������β��ǰ�棬���ղ����κ��ַ�
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
void serial_flush(void)  //��մ�������
{
  // RX
	// don't reverse this or there may be problems if the RX interrupt  //��Ҫ�ߵ����������ܻ���һЩ���⣬��������ж�
  // occurs after reading the value of rx_buffer_head but before writing  //�����ڶ�ȡrx_buffer_head֮����д��rx_buffer_tail֮ǰ
  // the value to rx_buffer_tail; the previous value of rx_buffer_head  //֮ǰ��rx_buffer_headֵ���ܱ�д��rx_buffer_tail
  // may be written to rx_buffer_tail, making it appear as if the buffer  //ʹ�����ֻ����������Ķ��ǿյ�״̬
  // were full, not empty.
  rx_buffer.head = rx_buffer.tail;
}		

/**
  * @brief  USART3�����жϷ�����
  * @param  none
  * @retval none
  */ 
void usart3_irq(void)
{
	uint8_t rec;
  
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
	{
		rec = USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
    serial_storeChar(rec);
  } 
} 

