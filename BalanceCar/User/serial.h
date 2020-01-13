/**
  ******************************************************************************
  * @file    serial.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2020-01-13
  * @brief   串口数据处理
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SERIAL_H
#define __SERIAL_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported macro ------------------------------------------------------------*/
#define RX_BUFFER_SIZE 64  //定义接收缓冲区的大小
   
/* Exported types ------------------------------------------------------------*/
typedef struct ring_buffer
{
  unsigned char buffer[RX_BUFFER_SIZE];
  int head;
  int tail;
}ring_buffer;

extern  ring_buffer rx_buffer;	
/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void usart3_irq(void);
unsigned int serial_available(void);
int serial_read(void);
void serial_flush(void);
void serial_checkRx(void);

#ifdef __cplusplus
}
#endif

#endif /* __SERIAL_H */


