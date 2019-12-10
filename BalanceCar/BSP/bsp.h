/**
  ******************************************************************************
  * @file    bsp.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   板级支持包
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "usart1.h"
#include "usart3.h"
#include "systick.h"
#include "encoder.h"
#include "motor.h"
#include "i2c.h"
#include "mpu6050.h"
#include "tim.h"
#include "led.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void bsp_init(void);


#ifdef __cplusplus
}
#endif

#endif /* __BSP_H */
