/**
  ******************************************************************************
  * @file    motor.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-09
  * @brief   电机底层驱动
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
  * @brief  定时器2初始化为PWM模式（驱动电机）
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

  //TIM_CtrlPWMOutputs(TIM2,ENABLE);	//MOE 主输出使能	

  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
  
  TIM_ARRPreloadConfig(TIM2, ENABLE);			 // 使能TIM3重载寄存器ARR
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
    
}

/**
  * @brief  电机驱动初始化
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
  GPIO_ResetBits(MOTOR_IN_GPIO_PORT, L_MOTOR_IN1_PIN | L_MOTOR_IN2_PIN | R_MOTOR_IN1_PIN | R_MOTOR_IN2_PIN); //电机默认设置为停止状态
}

/**
  * @brief  设置电机的输出pwm值（0-1000）
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
  * @brief  设置电机的运动方向（以小车运动的方向为前进方向）
  * @param  None
  * @retval None
  * @attention 由于电机控制电路为H桥，上下晶体管不能同时导通，为了安全起见，更换电机转速方向时，要先关闭再打开即：先执行引脚复位在执行引脚置位。
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

