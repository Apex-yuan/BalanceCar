/**
  ******************************************************************************
  * @file    motor_control.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   电机控制输出
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
  * @brief  电机控制输出函数
  * @param  None
  * @retval None
  */
void MotorOutput(void)
{ 
  #if 0  //关闭电机输出
    g_fLeftMotorOut = 0;
    g_fRightMotorOut = 0;
  #elif 0  //直立控制调试 
    g_fLeftMotorOut = g_fAngleControlOut;
    g_fRightMotorOut = g_fAngleControlOut;
  #elif 1 //直立+速度控制调试
    g_fLeftMotorOut = g_fAngleControlOut - g_fSpeedControlOut;
    g_fRightMotorOut = g_fAngleControlOut - g_fSpeedControlOut;
  #elif 0 
    g_fLeftMotorOut = -g_fDirectionControlOut;
    g_fRightMotorOut = g_fDirectionControlOut;
  #else  //正常模式
    g_fLeftMotorOut = g_fAngleControlOut - g_fSpeedControlOut - g_fDirectionControlOut;
    g_fRightMotorOut = g_fAngleControlOut - g_fSpeedControlOut + g_fDirectionControlOut;
  #endif
  //g_fware[7] = g_fAngleControlOut - g_fSpeedControlOut;
  //g_fware[6] = g_fAngleControlOut - g_fSpeedControlOut;
  
  /*电机驱动为H桥电路上下晶体管不能同时导通，增加电机转速方向更换过程中的输出清零操作，以保护电机驱动器 （测试可行后在添加）*/  
   if(g_fLeftMotorOut > 0 && g_fCarSpeed < 0 || g_fLeftMotorOut < 0 && g_fCarSpeed > 0)
   {
     motor_setPwm(LEFT_MOTOR, (uint16_t) 0);
   }
   if(g_fRightMotorOut > 0 && g_fCarSpeed < 0 || g_fRightMotorOut < 0 && g_fCarSpeed > 0)
   {
     motor_setPwm(RIGHT_MOTOR, (uint16_t) 0);
   }

  //左轮电机
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
  //右轮电机
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

  
  
  /* 跌倒后续的控制代码 */
  if(g_bFallFlag == 1) //当前车模处于跌倒状态
	{
		g_fLeftMotorOut = 0;
    g_fRightMotorOut = 0;

    #if 1 //ENABLE_CATAPULT_START
    g_nCatapultStartCount++; //每个电机控制周期自加
    if(g_nCatapultStartCount >= 500/5 )//&& g_nCatapultStartCount < 2150/5)
    {
      g_fLeftMotorOut = 3000;
      g_fRightMotorOut = 3000;
    }
    if(g_fCarAngle > -40 && g_fCarAngle < 40)
    {
      g_bFallFlag = 0; //清除跌倒标志位
      g_nCatapultStartCount = 0;    
    }
    #else //将小车扶到平衡位置附近，开启直立控制
    if(g_fCarAngle > -15 && g_fCarAngle < 15)
    {
      g_bFallFlag = 0;
    }
    #endif
  }
    
  /* 电机输出限幅 */
  g_fLeftMotorOut = constrain(g_fLeftMotorOut, MIN_MOTOR_OUT, MAX_MOTOR_OUT);
  g_fRightMotorOut = constrain(g_fRightMotorOut, MIN_MOTOR_OUT, MAX_MOTOR_OUT);

#if 0 //1关闭电机输出，测试用
  motor_setPwm(LEFT_MOTOR, 0);
  motor_setPwm(RIGHT_MOTOR, 0);
#else //正常输出
  motor_setPwm(LEFT_MOTOR, (uint16_t) fabs(g_fLeftMotorOut));
  motor_setPwm(RIGHT_MOTOR, (uint16_t) fabs(g_fRightMotorOut));
#endif 
}




