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

/* Exported variables ------------------------------------------------------- */
extern float g_fBTSpeedSet;
extern float g_fBTDirectionSet;

/* Exported functions ------------------------------------------------------- */
void usart3_irq(void);
void protocol_process(void);

#ifdef __cplusplus
}
#endif

#endif /* __PROTOCOL_H */


