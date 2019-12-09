/**
  ******************************************************************************
  * @file    motor.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-09
  * @brief   ����ײ�����
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "../BSP/motor/motor.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ��ʱ��2��ʼ��ΪPWMģʽ�����������
  * @param  None
  * @retval None
  */
static void TIM2_PWM_Init(uint16_t arr, uint16_t psc)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  RCC_APB1PeriphClockCmd(MOTOR_TIM_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(MOTOR_PWM_GPIO_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = L_MOTOR_PWM_PIN | R_MOTOR_PWM_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTOR_PWM_GPIO_PORT, &GPIO_InitStructure);
  
  TIM_TimeBaseInitStructure.TIM_Period = arr;  
  TIM_TimeBaseInitStructure.TIM_Prescaler = psc; 
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC3Init(TIM2, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);

  //TIM_CtrlPWMOutputs(TIM2,ENABLE);	//MOE �����ʹ��	

  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
  
  TIM_ARRPreloadConfig(TIM2, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
    
}

/**
  * @brief  ���������ʼ��
  * @param  None
  * @retval None
  */
void motor_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  TIM2_PWM_Init(999, 5);  //12KHz
  
  RCC_APB2PeriphClockCmd(MOTOR_IN_GPIO_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = L_MOTOR_IN1_PIN | L_MOTOR_IN2_PIN | R_MOTOR_IN1_PIN | R_MOTOR_IN2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTOR_IN_GPIO_PORT, &GPIO_InitStructure);
  GPIO_ResetBits(MOTOR_IN_GPIO_PORT, L_MOTOR_IN1_PIN | L_MOTOR_IN2_PIN | R_MOTOR_IN1_PIN | R_MOTOR_IN2_PIN); //���Ĭ������Ϊֹͣ״̬
}

/**
  * @brief  ���õ�������pwmֵ��0-1000��
  * @param  None
  * @retval None
  */
void motor_setPwm(MotorChoice_t choice, uint16_t pwm)
{
  switch(choice)
  {
    case LEFT_MOTOR :
      TIM_SetCompare4(MOTOR_TIM, pwm);
    case RIGHT_MOTOR :
      TIM_SetCompare3(MOTOR_TIM, pwm);
    default:
      return;
  }
}

/**
  * @brief  ���õ�����˶�������С���˶��ķ���Ϊǰ������
  * @param  None
  * @retval None
  * @attention ���ڵ�����Ƶ�·ΪH�ţ����¾���ܲ���ͬʱ��ͨ��Ϊ�˰�ȫ������������ת�ٷ���ʱ��Ҫ�ȹر��ٴ򿪼�����ִ�����Ÿ�λ��ִ��������λ��
  */
void motor_setDirection(MotorChoice_t choice, MotorDirection_t direction)
{
  switch(choice)
  {
    case LEFT_MOTOR :
    {
      switch (direction)
      {
        case FRONT :
        {
          GPIO_ResetBits(MOTOR_IN_GPIO_PORT, L_MOTOR_IN2_PIN);
          GPIO_SetBits(MOTOR_IN_GPIO_PORT, L_MOTOR_IN1_PIN);
        }
        case STOP :
        {
          GPIO_ResetBits(MOTOR_IN_GPIO_PORT, L_MOTOR_IN1_PIN | L_MOTOR_IN2_PIN);
        }
        case BACK :
        {
          GPIO_ResetBits(MOTOR_IN_GPIO_PORT, L_MOTOR_IN1_PIN);
          GPIO_SetBits(MOTOR_IN_GPIO_PORT, L_MOTOR_IN2_PIN);
        }
        default :
          return;
      }
    }
    case RIGHT_MOTOR :
    {
      switch (direction)
      {
        case FRONT :
        {
          GPIO_ResetBits(MOTOR_IN_GPIO_PORT, R_MOTOR_IN2_PIN);
          GPIO_SetBits(MOTOR_IN_GPIO_PORT, R_MOTOR_IN1_PIN);
        }
        case STOP :
        {
          GPIO_ResetBits(MOTOR_IN_GPIO_PORT, R_MOTOR_IN1_PIN | R_MOTOR_IN2_PIN);
        }
        case BACK :
        {
          GPIO_ResetBits(MOTOR_IN_GPIO_PORT, R_MOTOR_IN1_PIN);
          GPIO_SetBits(MOTOR_IN_GPIO_PORT, R_MOTOR_IN2_PIN);
        }
        default :
          return;
      }
    }
    default:
      return;
  }
}

