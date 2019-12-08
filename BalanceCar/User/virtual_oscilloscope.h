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
#ifndef __VIRTUAL_OSCILLOSCOPE_H
#define __VIRTUAL_OSCILLOSCOPE_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define CHANNEL_NUM  8
/* Exported variables --------------------------------------------------------*/
extern float g_fware[CHANNEL_NUM];

/* Exported functions ------------------------------------------------------- */
void vcan_sendware(uint8_t *wareaddr, uint32_t waresize);


#ifdef __cplusplus
}
#endif

#endif /* __VIRTUAL_OSCILLOSCOPE_H */


