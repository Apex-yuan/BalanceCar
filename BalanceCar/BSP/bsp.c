/**
  ******************************************************************************
  * @file    bsp.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   板级支持包，包括驱动层和HAL层（目前两部分是合在一起的没有进一步分开）
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "bsp.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  初始化底层硬件驱动
  * @param  None
  * @retval None
  */
void bsp_init(void)
{
  // __disable_irq();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断优先级分组
  systick_init();
  usart1_init(115200);
  usart3_init(9600);
  
  timer_init(999,71);
  motor_init();
  encoder_init();
  while(MPU_DMP_Init());
  // delay_ms(10);
  // MPU_DMP_Init();
  // while(1)
  // {
  // uint8_t res = MPU_DMP_Init();
  // if(res == 0)
  // {
  //     break;
  // }
  // else
  // {
  //     printf("res=%d",res);
  // }  
  // }
  // __enable_irq();
}





