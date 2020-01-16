/**
  ******************************************************************************
  * @file    motor_control.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   ����������
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "motorControl.h"
#include <math.h>
#include "config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ��������������
  * @param  None
  * @retval None
  */
void motorOutput(float leftMotorOut, float rightMotorOut)
{ 
  /*�������ΪH�ŵ�·���¾���ܲ���ͬʱ��ͨ�����ӵ��ת�ٷ�����������е��������������Ա������������ �����Կ��к�����ӣ�*/  
//   if(leftMotorOut > 0 && g_fCarSpeed < 0 || leftMotorOut < 0 && g_fCarSpeed > 0)
//   {
//     motor_setPwm(LEFT_MOTOR, (uint16_t) 0);
//   }
//   if(rightMotorOut > 0 && g_fCarSpeed < 0 || rightMotorOut < 0 && g_fCarSpeed > 0)
//   {
//     motor_setPwm(RIGHT_MOTOR, (uint16_t) 0);
//   }

  //���ֵ��
  if(leftMotorOut > 0)
  {
    motor_setDirection(LEFT_MOTOR, FRONT);
    leftMotorOut = leftMotorOut + LEFT_MOTOR_OUT_DEAD_ZONE; // +
  }
  else
  {
    motor_setDirection(LEFT_MOTOR, BACK);
    leftMotorOut = leftMotorOut - LEFT_MOTOR_OUT_DEAD_ZONE; // +
  }
  //���ֵ��
  if(rightMotorOut > 0)
  {
    motor_setDirection(RIGHT_MOTOR, FRONT);
    rightMotorOut = rightMotorOut + RIGHT_MOTOR_OUT_DEAD_ZONE; // +
  }
  else
  {
    motor_setDirection(RIGHT_MOTOR, BACK);
    rightMotorOut = rightMotorOut - RIGHT_MOTOR_OUT_DEAD_ZONE; // +
  }
    
  //�������޷�
  leftMotorOut = constrain(leftMotorOut, MIN_MOTOR_OUT, MAX_MOTOR_OUT);
  rightMotorOut = constrain(rightMotorOut, MIN_MOTOR_OUT, MAX_MOTOR_OUT);
  
  motor_setPwm(LEFT_MOTOR, (uint16_t) fabs(leftMotorOut));
  motor_setPwm(RIGHT_MOTOR, (uint16_t) fabs(rightMotorOut)); 
}





