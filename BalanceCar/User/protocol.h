/**
  ******************************************************************************
  * @file    virtual_oscilloscope.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-09
  * @brief   虚拟示波器，基于山外虚拟示波器的客户端
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "usart3.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define BYTE0(dwTemp) (*(char *)(&dwTemp))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))

#define BUFFER_SIZE 4
#define MAX_PACK_SIZE 80

/* Exported variables ------------------------------------------------------- */
extern float g_fBTSpeedSet;
extern float g_fBTDirectionSet;

extern bool g_bStartBitFlag; //接收到起始字符的标志位
extern char cmdBuffer[BUFFER_SIZE][MAX_PACK_SIZE];
extern int bufindr;
extern int bufindw;
extern int buflen;
extern int serial_count;

/* Exported functions ------------------------------------------------------- */
void usart3_irq(void);
void protocol_process(void);

#ifdef __cplusplus
}
#endif

#endif /* __PROTOCOL_H */


