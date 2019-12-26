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
#include "motor_control.h"
#include "angle_control.h"
#include "speed_control.h"
#include "direction_control.h"
#include "motor.h"
#include <math.h>
#include "virtual_oscilloscope.h"
#include "systick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define ENABLE_CATAPULT_START
/* Private variables ---------------------------------------------------------*/
static float g_fLeftMotorOut = 0;
static float g_fRightMotorOut = 0;
uint32_t current_time = 0;
uint16_t g_nCatapultStartCount = 0;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ��������������
  * @param  None
  * @retval None
  */
void MotorOutput(void)
{ 
  #if 0  //�رյ�����
    g_fLeftMotorOut = 0;
    g_fRightMotorOut = 0;
  #elif 0  //ֱ�����Ƶ��� 
    g_fLeftMotorOut = g_fAngleControlOut;
    g_fRightMotorOut = g_fAngleControlOut;
  #elif 1 //ֱ��+�ٶȿ��Ƶ���
    g_fLeftMotorOut = g_fAngleControlOut - g_fSpeedControlOut;
    g_fRightMotorOut = g_fAngleControlOut - g_fSpeedControlOut;
  #elif 0 
    g_fLeftMotorOut = -g_fDirectionControlOut;
    g_fRightMotorOut = g_fDirectionControlOut;
  #else  //����ģʽ
    g_fLeftMotorOut = g_fAngleControlOut - g_fSpeedControlOut - g_fDirectionControlOut;
    g_fRightMotorOut = g_fAngleControlOut - g_fSpeedControlOut + g_fDirectionControlOut;
  #endif
  //g_fware[7] = g_fAngleControlOut - g_fSpeedControlOut;
  //g_fware[6] = g_fAngleControlOut - g_fSpeedControlOut;
  
  /*�������ΪH�ŵ�·���¾���ܲ���ͬʱ��ͨ�����ӵ��ת�ٷ�����������е��������������Ա������������ �����Կ��к�����ӣ�*/  
   if(g_fLeftMotorOut > 0 && g_fCarSpeed < 0 || g_fLeftMotorOut < 0 && g_fCarSpeed > 0)
   {
     motor_setPwm(LEFT_MOTOR, (uint16_t) 0);
   }
   if(g_fRightMotorOut > 0 && g_fCarSpeed < 0 || g_fRightMotorOut < 0 && g_fCarSpeed > 0)
   {
     motor_setPwm(RIGHT_MOTOR, (uint16_t) 0);
   }

  //���ֵ��
  if(g_fLeftMotorOut > 0)
  {
    motor_setDirection(LEFT_MOTOR, FRONT);
    g_fLeftMotorOut = g_fLeftMotorOut + LEFT_MOTOR_OUT_DEAD_ZONE; // +
  }
  else
  {
    motor_setDirection(LEFT_MOTOR, BACK);
    g_fLeftMotorOut = g_fLeftMotorOut - LEFT_MOTOR_OUT_DEAD_ZONE; // +
  }
  //���ֵ��
  if(g_fRightMotorOut > 0)
  {
    motor_setDirection(RIGHT_MOTOR, FRONT);
    g_fRightMotorOut = g_fRightMotorOut + RIGHT_MOTOR_OUT_DEAD_ZONE; // +
  }
  else
  {
    motor_setDirection(RIGHT_MOTOR, BACK);
    g_fRightMotorOut = g_fRightMotorOut - RIGHT_MOTOR_OUT_DEAD_ZONE; // +
  }
  g_fware[5] = g_fLeftMotorOut;
  g_fware[6] = g_fRightMotorOut;

  
  
  /* ���������Ŀ��ƴ��� */
  if(g_bFallFlag == 1) //��ǰ��ģ���ڵ���״̬
	{
		g_fLeftMotorOut = 0;
    g_fRightMotorOut = 0;

    #if 1 //ENABLE_CATAPULT_START
    g_nCatapultStartCount++; //ÿ��������������Լ�
    if(g_nCatapultStartCount >= 500/5 )//&& g_nCatapultStartCount < 2150/5)
    {
      g_fLeftMotorOut = 3000;
      g_fRightMotorOut = 3000;
    }
    if(g_fCarAngle > -40 && g_fCarAngle < 40)
    {
      g_bFallFlag = 0; //���������־λ
      g_nCatapultStartCount = 0;    
    }
    #else //��С������ƽ��λ�ø���������ֱ������
    if(g_fCarAngle > -15 && g_fCarAngle < 15)
    {
      g_bFallFlag = 0;
    }
    #endif
  }
    
  /* �������޷� */
  g_fLeftMotorOut = constrain(g_fLeftMotorOut, MIN_MOTOR_OUT, MAX_MOTOR_OUT);
  g_fRightMotorOut = constrain(g_fRightMotorOut, MIN_MOTOR_OUT, MAX_MOTOR_OUT);

#if 0 //1�رյ�������������
  motor_setPwm(LEFT_MOTOR, 0);
  motor_setPwm(RIGHT_MOTOR, 0);
#else //�������
  motor_setPwm(LEFT_MOTOR, (uint16_t) fabs(g_fLeftMotorOut));
  motor_setPwm(RIGHT_MOTOR, (uint16_t) fabs(g_fRightMotorOut));
#endif 
}




