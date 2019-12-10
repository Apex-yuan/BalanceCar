/**
  ******************************************************************************
  * @file    systick.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   基于SysTick的系统时间和延时函数实现
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTICK_H
#define __SYSTICK_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h" 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define millis getCurrentMillis
#define micros getCurrentMicros
#define delay delay_ms

/* Exported functions ------------------------------------------------------- */
void systick_init(void);  
void systick_reset(void);
uint32_t getCurrentMillis(void);
uint32_t getCurrentMicros(void);
void delay_ms(uint32_t ms); 
void delay_us(uint32_t us);


#ifdef __cplusplus
}
#endif

#endif /* __SYSTICK_H */

