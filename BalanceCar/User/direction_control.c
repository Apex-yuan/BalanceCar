/**
  ******************************************************************************
  * @file    direction_control.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-10
  * @brief   方向控制相关代码
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "direction_control.h"
#include "mpu6050.h"
#include "protocol.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float g_fDirectionControlOut = 0;
float g_fDirectionControlNew, g_fDirectionControlOld;
uint8_t g_nDirectionControlPeriod;
uint16_t g_nDirectionControlCount;
float g_fDirectionSet;
float g_fCardirection; //角速度

float g_fDirectionControlIntegral;

float DIRECTION_P = 0.5;
float DIRECTION_I = 0.5;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void DirectionControl(void)
{
  float theta, delta_theta;
  static float last_theta;
  float fDelta; 
  float fP,fI;
  
  //通过imu角度计算当前方向（角速度）
  theta = imu_data.rpy[2];
  delta_theta = theta -  last_theta;
  g_fCardirection = delta_theta /(1000.0 / DIRECTION_CONTROL_PERIOD);  //弧度/秒
 
  //通过odom计算当前方向（角速度）
  

  //方向控制PID运算
  fDelta = DIRECTION_SET - imu_data.gyro[2] + g_fBTDirectionSet; //同时支持ROS下发和蓝牙控制
  fP = fDelta * DIRECTION_P;
  fI = fDelta * DIRECTION_I;
  
  g_fDirectionControlIntegral += fI;
  g_fDirectionControlOld = g_fDirectionControlNew;
  g_fDirectionControlNew = fP + g_fDirectionControlIntegral;
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void DirectionControlOutput(void)
{
  float fValue;
  
  fValue = g_fDirectionControlNew - g_fDirectionControlOld;
  g_fDirectionControlOut = fValue * (g_nDirectionControlPeriod + 1) / 
                           DIRECTION_CONTROL_PERIOD + g_fDirectionControlOld; 
}


