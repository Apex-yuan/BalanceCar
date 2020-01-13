/**
  ******************************************************************************
  * @file    angle_control.c 
  * @author  Apexyuan
  * @version V1.0.0
  * @date    2019-12-09
  * @brief   平衡车直立控制相关的代码
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "angle_control.h"
#include "speed_control.h"
#include "virtual_oscilloscope.h"
#include "config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float angleP = 0;
float angleI = 0;
float angleD = 0;
float g_fCarAngle;
float g_fGyroscopeAngleSpeed;
float g_fAngleControlOut;

/*车模跌倒标志位*/
bool g_bFallFlag = 0;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  直立控制算法（置于中断中运行每5ms执行一次）
  * @param  None
  * @retval None
  */
void AngleControl(void)
{
  float fValue;

  /* 获取倾角：度  角速度：度/秒 */
	g_fCarAngle = RAD2DEG(imu_data.rpy[0]); //g_fRoll;
  g_fGyroscopeAngleSpeed = RAD2DEG(imu_data.gyro[0]); //(float)g_nGyro[0];
  
  //g_fSpeedControlOut = 0;
	fValue = (g_fCarAngle - g_fSpeedControlOut) * angleP + 
           g_fGyroscopeAngleSpeed * angleD;
	g_fAngleControlOut = fValue;
  
  //跌倒检测
  if(g_fCarAngle > 50 || g_fCarAngle < (-50))
  {
    g_bFallFlag = 1;
  }

  //虚拟示波器
  g_fware[0] = g_fCarAngle;
  g_fware[1] = g_fGyroscopeAngleSpeed;
  g_fware[2] = g_fAngleControlOut;
}



