/**
  ******************************************************************************
  * @file    tim.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   定时器初始化
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "tim.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  定时器1初始化为定时器模式
  * @param  arr  自动重装载初值 
  * @param  psc  预分频因子
  * @retval None
  */
static void TIM1_TIMER_Init(uint16_t arr, uint16_t psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  TIM_TimeBaseInitStructure.TIM_Period = arr;
  TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
  
  //TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
  TIM_ITConfig(  //使能或者失能指定的TIM1中断
		TIM1, //TIM1
		TIM_IT_Update  |  //TIM1 中断源
		TIM_IT_Trigger,   //TIM1 触发中断源 
		ENABLE  //使能
		);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  TIM_Cmd(TIM1, DISABLE);
}

/**
  * @brief  定时器1初始化为定时器模式
  * @param  arr  自动重装载初值 
  * @param  psc  预分频因子
  * @retval None
  */
void timer_init(uint16_t arr, uint16_t psc)
{
  TIM1_TIMER_Init(arr, psc);  
}

/* 该中断服务函数置于应用层实现 */
////中断服务函数
//void TIM1_UP_IRQHandler(void) 
//{
//  if(TIM_GetFlagStatus(TIM1, TIM_IT_Update) != RESET)
//  {  
//    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//    //中断服务程序：
//    
//  }
//}

