/**
  ******************************************************************************
  * @file    tim.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   ��ʱ����ʼ��
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
  * @brief  ��ʱ��1��ʼ��Ϊ��ʱ��ģʽ
  * @param  arr  �Զ���װ�س�ֵ 
  * @param  psc  Ԥ��Ƶ����
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
  TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM1�ж�
		TIM1, //TIM1
		TIM_IT_Update  |  //TIM1 �ж�Դ
		TIM_IT_Trigger,   //TIM1 �����ж�Դ 
		ENABLE  //ʹ��
		);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  TIM_Cmd(TIM1, DISABLE);
}

/**
  * @brief  ��ʱ��1��ʼ��Ϊ��ʱ��ģʽ
  * @param  arr  �Զ���װ�س�ֵ 
  * @param  psc  Ԥ��Ƶ����
  * @retval None
  */
void timer_init(uint16_t arr, uint16_t psc)
{
  TIM1_TIMER_Init(arr, psc);  
}

/* ���жϷ���������Ӧ�ò�ʵ�� */
////�жϷ�����
//void TIM1_UP_IRQHandler(void) 
//{
//  if(TIM_GetFlagStatus(TIM1, TIM_IT_Update) != RESET)
//  {  
//    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
//    //�жϷ������
//    
//  }
//}

