/**
  ******************************************************************************
  * @file    encoder.h 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-09
  * @brief   ±àÂëÆ÷µ×²ãÇý¶¯
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  LEFT_ENCODER = 0,
  RIGHT_ENCODER
}EncoderChoice_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define L_ENCODER_TIM  TIM3
#define L_ENCODER_TIM_CLK  RCC_APB1Periph_TIM3

#define L_ENCODER_A_PIN    GPIO_Pin_0
#define L_ENCODER_A_GPIO_PORT  GPIOA
#define L_ENCODER_A_GPIO_CLK  RCC_APB2Periph_GPIOA
#define L_ENCODER_A_TIM_CHANNEL TIM_Channel_1

#define L_ENCODER_B_PIN    GPIO_Pin_1
#define L_ENCODER_B_GPIO_PORT  GPIOA
#define L_ENCODER_B_GPIO_CLK  RCC_APB2Periph_GPIOA
#define L_ENCODER_B_TIM_CHANNEL  TIM_Channel_2


#define R_ENCODER_TIM  TIM3
#define R_ENCODER_TIM_CLK  RCC_APB1Periph_TIM3

#define R_ENCODER_A_PIN    GPIO_Pin_0
#define R_ENCODER_A_GPIO_PORT  GPIOA
#define R_ENCODER_A_GPIO_CLK  RCC_APB2Periph_GPIOA
#define R_ENCODER_A_TIM_CHANNEL TIM_Channel_1

#define R_ENCODER_B_PIN    GPIO_Pin_1
#define R_ENCODER_B_GPIO_PORT  GPIOA
#define R_ENCODER_B_GPIO_CLK  RCC_APB2Periph_GPIOA
#define R_ENCODER_B_TIM_CHANNEL  TIM_Channel_2

/* Exported functions ------------------------------------------------------- */
void encoder_init(void);
int16_t encoder_getPulse(EncoderChoice_t choice);
void encoder_resetPulse(void);

#endif /* __ENCODER_H */

